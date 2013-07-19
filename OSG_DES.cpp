//////////////////////////////////////////////////////////////////////
//	MJim - Alpha
//	Stephens Atomics 
//	Copyright (C) 2013  Matthew James Stephens, B.Eng- Robotics(Hons)
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	<http://www.gnu.org/licenses/>   
//////////////////////////////////////////////////////////////////////

#include "OSG_DES.h"

#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/Registry>

#include "OSG_Environment.h"
#include "OSG_Event.h"
#include "OSG_Default.h"
#include "OSG_Physx_2_8.h"
#include "registry.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "mjs_wave.h"
#include "Framework.h"

using namespace osg;
using namespace std;

osg::Quat quat;
float x= 1;
float y= 0;
float z= 1;
float angle = 0;

char m_OS_date_buffer[9];
char m_OS_time_buffer[9];
struct __timeb64 timebuffer;

CEnvironment g_environment;
CEventHandler g_event_handler;
CScript g_script;
CScript g_innate_script;
COSG_PhysX g_PhysX;
CRegistry registry;
bool g_physx_active;


int main( int argc, char **argv )
{		
	osg::ArgumentParser arguments(&argc,argv);

	//create_wave();//temp

	std::string startup_wav = "wave-440-env-01.wav";

	ALuint      uiBuffer;
	ALuint      uiSource;  
	ALint       iState;
	ALFWInit();
	if (!ALFWInitOpenAL_default())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
	}
	alGenBuffers( 1, &uiBuffer );
	if (!ALFWLoadWaveToBuffer(startup_wav.c_str(), uiBuffer))
	{
		ALFWprintf("Failed to load %s\n", startup_wav.c_str());
	}
	alGenSources( 1, &uiSource );
	alSourcei( uiSource, AL_BUFFER, uiBuffer );	
    alSourcePlay( uiSource );
	ALFWprintf("Playing Source ");		
	do
	{
		Sleep(100);
		ALFWprintf(".");
		alGetSourcei( uiSource, AL_SOURCE_STATE, &iState);
	} while (iState == AL_PLAYING);	 

	std::ostringstream oss;
	_strdate(m_OS_date_buffer);
	_strtime(m_OS_time_buffer);
	_ftime64( &timebuffer );			    
	oss << m_OS_date_buffer;
	oss << "MDY, ";
	oss << m_OS_time_buffer;
	oss << ".";
	oss << timebuffer.millitm;				
	oss << "hms.ms: ";			

	g_physx_active= true;
	srand((unsigned)time(0)); //intitialise random seed		

	int argument_count = argc;
	std::vector<std::string> mv_string;
	int i=0;
	for (i=0;i< argument_count; i++) { mv_string.push_back(argv[i]); }	
	std::string str_script;
	std::string str_node_script;
	str_script = get_default_script();
	str_node_script= get_node_script();
	//g_log_file.create(true, true,  "___Log.txt");		
	//g_log_file.write(oss.str());

	char *buffer;	
	buffer= _getcwd(NULL,0);
	size_t size= strlen(buffer);	
	std::string cwd = buffer;
	std::ostringstream oss_cwd;
	oss_cwd << cwd;		
	std::ostringstream oss_shader_path;
	oss_shader_path << cwd;
	oss_shader_path << "\\Shaders";	
	std::ostringstream oss_media_path;
	oss_media_path << cwd;
	oss_media_path << "\\Media";			
	std::ostringstream oss_images_path;
	oss_images_path << cwd;
	oss_images_path << "\\Images";			
	osgDB::FilePathList pathList = osgDB::getDataFilePathList();
	pathList.push_back(oss_cwd.str());	
	pathList.push_back(oss_shader_path.str());	
	pathList.push_back(oss_media_path.str());
	pathList.push_back(oss_images_path.str());	
	osgDB::setDataFilePathList(pathList);
	if(argument_count>1) { str_script = mv_string[1]; }	
	cout << "script: " << str_script << "\n";
	g_environment.m_str_script = str_script;
	size_t str_index = str_script.find_last_of(".");
	g_environment.m_str_script.erase(str_index, std::string::npos);		
	g_PhysX.InitNx();	
	g_environment.mp_scene= g_PhysX.gScene;
	if(g_script.XML_Parser(str_script)) { cout << "script parsed\n"; }
	else { cout << "missing environment or event file \n"; } 
	if(str_node_script!= "none") { g_environment.XGMML_Graph_Parser(str_node_script); }	
	std::vector<std::string>::iterator vec_str_iter;
	for(vec_str_iter = g_script.mv_str_environment_file_name.begin(); vec_str_iter != g_script.mv_str_environment_file_name.end(); ++vec_str_iter) {
		if(!g_environment.XML_EnvironmentParser(*vec_str_iter))	{ cout <<  "failed to create environment"; return 0; }
	}		
	//todo: store number of active OSG_DES exe's
	//innate memory maps
	create_mapped_memory("mjs_innate", 8);
	create_mapped_memory("mjs_entity", 8);
	create_mapped_memory("mjs_index", 16);
	create_mapped_memory("mjs_variable", 32);
	create_mapped_memory("mjs_value", 64);
	create_mapped_memory("mjs_entity_details", 512);

	g_environment.LinkReferences();
	for(vec_str_iter = g_script.mv_str_event_file_name.begin(); vec_str_iter != g_script.mv_str_event_file_name.end(); ++vec_str_iter) {
		if(!g_event_handler.XML_EventParser(*vec_str_iter, &g_environment, &(g_event_handler.m_event)))	{ cout <<  "failed to parse events"; return 0; }
	}		

	//add innate events
	g_event_handler.InnateCreate(&g_environment);

	eTimeType m_type= TimeType_real;
	if(g_environment.mv_viewer.size()== 0)	{ cout <<  "No viewer defined in script"; }
	else {
		g_environment.mv_viewer[g_environment.m_active_viewer_index].mp_viewer->setSceneData(g_environment.mv_group[g_environment.m_active_group_index].mp_group);		
		g_environment.mv_viewer[g_environment.m_active_viewer_index].mp_viewer->realize();
		g_environment.init_time();	
		while (!g_environment.mv_viewer[g_environment.m_active_viewer_index].mp_viewer->done())  {			
			if(g_environment.update_time())	{
				g_event_handler.InnateUpdate(&g_environment);
				g_event_handler.Update(&g_environment);				
				if(g_environment.m_Render) {				
					if(g_physx_active) { g_PhysX.gScene->simulate( 1.0f/60.0f ); }
					//g_PhysX.gScene->simulate( g_environment.get_delta_time() );
					g_environment.mv_viewer[g_environment.m_active_viewer_index].mp_viewer->frame();	
					if(g_physx_active) {
						g_PhysX.gScene->flushStream();
						g_PhysX.gScene->fetchResults( NX_RIGID_BODY_FINISHED, true );
					}
				}
			}
			else goto end;
		}
	}

end:
	/*m_PhysXTest.destroy();*/
	g_PhysX.ReleaseNx();
	//g_log_file.close();

	// Clean up by deleting Source(s) and Buffer(s)
	alSourceStop(uiSource);
    alDeleteSources(1, &uiSource);
	alDeleteBuffers(1, &uiBuffer);

	ALFWShutdownOpenAL();

	ALFWShutdown2();
}

std::string get_default_script()
{
	std::string str_default= "undefined";
	if(!registry.CreateKey(CRegistry::localMachine, OSG_DES_KEY)) {	return "Defaults.xml"; }
	if(!registry.GetValue("default_script" , &str_default))	{		
		if(!registry.SetValue("default_script", "Defaults.xml")) { return "Defaults.xml"; }
		return "Defaults.xml";
	}	
	return str_default;	
}

std::string get_node_script()
{
	std::string str= "undefined";
	if(!registry.CreateKey(CRegistry::localMachine, OSG_DES_KEY)) {	return "none"; }
	if(!registry.GetValue("node_script" , &str))	{		
		if(!registry.SetValue("node_script", "none")) {	return "none";	}
		return "none";
	}	
	return str;	
}








/*
$$$$$$$$$$$$$$777$I??+I??+~::+I$$7I=~~~~~:,,,,,:=++I7$$$77$7$$77$$$7$77$7777777$77$7$777777777777777$$7777$77$777$7$7$7$7$$$$$$$$$$$$$$$ZZZZ
$7I$$$$$$$$$$$$$777IIIIII==~:~I=++?=???+=?+==========++++=~:~777777$7777$7$7777777$777$77$7$77777777$$$7$77$$$$$$$$$$$7777777$$$$$$$$$$$ZZZZ
7II77$$$$$$$$$$$$$$III$7I77==:,,~+==++=~=~=??+++===~=+==~+=====~~,~??????III77777$77$7$77$7$$$$77$$77$7$$$$$$$7$$$$$$$$77$7777$$$$$$$$$$ZZZZ
$7$$$$$$$$$$$$$$$$7$$777II====~::,::~+=~:::::~=~====~~~~::~~~~~~~~~~~::,~======++?I7777777$7$7$$77$$$7$$$$$$$$77$$77$$7777777$77$$$$$$$$ZZZZ
Z$$$$$$$$$$$$$$$$77$7$?+II??+=====~:::::::::::,,,::~~::,,,,:::::::::::::::::::~~~==++?II7777$7$$$7$7$$$$$$$$$$$$7$$$7$$$7$$$$$$$$$$$$$$$ZZZZ
$$$$$$$$$$$$$$$$77777IIIIII???+?++++==~~~::::::,,,,,,,,,,,,,,,:,,:,,:::,:,,,,,,,::,:~~==++?77$$$7$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ZZZZ
$$$$$$$$$$$7$$$$77777IIIIIIII?I???++++====~~:::::,,,,,,,,,,,,,:7?=7?:,,,,,,,,,,,,:,,:::::::+I77$$$$$$$$$$$$$$777$$$$$$$$$$$$$$$$$$$$$$$$$ZZZ
$$$$$$$$$$$$$$$7I7IIIIIIIIIIII????+++++++====~~~:::::,,,,,,,,::~?ZI+===,,,,,,,::::::::::::::~~::=+?II777777777IIII7777$77777$$$$$$$$$$$$$ZZZ
$$$$$$$$$$$$$$$77IIII??I??II???+??++++++==+=====~:::::::,,:~==+++??$?7?=+,,,,,:::::::::::::::~~~==~===++++?+?????III?IIIII7777$$$$$$$$$$$$ZZ
$$$$$$$$$$$7$777IIIIIII???????????++?+?+++++++===~~~~:::::~?IIIII$OO$77$O7+~::,:,:::::::::::::::~~~=====+++?+??????IIIIII777$$$$$$$$$$$$$$$Z
$$$$$$$$$$$7777777II7IIII??????????+?+?++++?+??++====~~~~~Z$Z$$$ZZO8$7Z$Z+?II:::::::::::::::::::::~~=++==+++???????IIIII77777$$$$$$$$$$$$$$Z
$$$$$$77$$$7777777777IIIIII??I???????+??++?+++++++++=====~88OOOO8OOOO?77ZIZIZ7,:::::::::,,,,::::::::~~=++++++???IIIIIII7777$$$$$$$$$$$$$$$Z$
$$$$$$$$$77777777777I7IIIII??II?I???+??????+???+++++++++++8O$8888O887O$7$ZOZ?+?I+::::,,,,,,,,,,,,,,,,::~~=+?+++?IIIII77777$$$$$$$$$$$$$$$$$$
$$$$7777$$$777777777777IIIII?II?III??????????++++++?+?+++$88O8OOOZOOO$ZI$$ZZO7I7+$:,,,,,,,,,,,,,,,,,,:,:::~===++??III777777$7$7$$$$$$$$$$$$$
$$$$7$7777777777777777I7III7IIIIIII????????????+????++???ZO8O8888$O7?+Z+OZZZ77$$++8,,,,,,,,,,,,,,,,,,,,,,::::~~~=+++III7777$7$$$$$$$$$$$$$$$
$$$$7777777777777777777I77777I7IIIII??????????????????OOOZI7OZOZZO$8$8ZZZOZIIZ$O$++O,,,,,,,,,,,,,,,,,,,,,,,,,,,,::~~~==+?I777$$$$$$$$$$$$$$$
$$777777777777777777777777I77I7I7II?I??????????I??+O$OO88888$8$OOO8$7OOZOOO$7I$IO??+O,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,::~~=++II777$$7$$$$$$$
$77777777777777777777777777I7III7IIIIII???????II??OOOOZ$IZOOZ7ZOO8OO$OOOOOOZ$Z$8$Z+$$$,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:::::~=+I77$$$$$$
777$77777777777777777777777777II7I7IIIII?I????????OO$ZZZO8OO8?O8$OIOOOO8OOOOZZ$OOOZ7?I,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:,::::::::::~~~::~+?
77777777777777777777777777777777IIIIIIIIIII????+++OO88OZ8Z87ZOO8?IO8OOOZOZO8OZZZO$ZZ7I$,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:,,,:::::::~~~=~~====
7777777$777777777777777777777777777IIII?II???I?+?Z88O8OOO7ZOZOO?IIZOOZZZOZOZZ8O7ZZOZ8Z$O.,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:::::::::~~~~~===++++
7777777777777777777777777777777777777II?II???????O8$7ZO$8I$8$ZI=IO$8OZOO$ZZZZOZZZ$$8ZZZZ+,,,,,,,,,,,,,,,,,,,,,,,,,,,,::::::::::~~~~~====++++
7777777777777777777777777777777777777IIII?III?I?$ZZZO7ZI7=88O?=I$$OOOZZZ$$$Z8ZZZZ7ZOOZO8Z.,,,,,,,,,,,,,,,,,,:,,:::::::::::::~~~~~~~==~=++?+?
77777777777777777777777777777777777777IIII?????8O$O$ZZOZOO8=I$7+ZOOZZZO$$$Z$ZO$77OZZZOZZO,,,,,,,,,,,:,:::::::::::::::::~:~~~~~~~==++==++++++
77777777777777777777777777777777777I7IIIII?I???8Z?IZ+ZO8?8$Z$$+7ZZ88$$OO$ZZZZZ$Z$$ZZZOZZO::::::::::::::::::::::~~~~~~~~~~~~~~====+++?????++?
I?????II???I?II7777777777777777777777I7III????I7$=+7IZZOZ$II~7OI78OZZZ$OZZ$ZZZ$$$$O$Z$ZZO::~~~:~~~~~~:~~~~~~=~~~=======++++=+++++????+?????I
I??++++????IIII77777777777777777777777IIII???7?===I?$=I$=$?I8OZ8ZOOOO7O$$Z$ZZZ$Z7$$Z$ZO8Z8$==~~~~~~~~~~=~===~~=+==::~+???+++?+?????????II?I7
++++==+++??????II777777777777777777777I7IIIIOI=+=+$IZ~I7+$$OZOOO+O8O8877Z$O$ZO$$$$$Z$7$ZOIOOOZ++=~~~::,,,,,,::::::,,,,:~???????????????III77
==+++++++==+===++??I777777777777777777777I?7+I$:~~OOO?77$OZZZ?8O$OZZD88I7Z$$$$8Z$ZZ$$O$ZOOOOZZO?:,,,,,,,,:,,,,,,,,,::,,::???????????IIIII777
????++++======+++????I777777777777777777Z$$+7=~==?O77ZI7$O+~?8O88OO$88O8+?I7$$$OZ$$ZO$O$O$IOOZZZ8.,,,,,,,,:,,,,,,,,,::::::=???????IIII777777
+++++++++=++++++++???IIIII77777777777O8Z?O?~I+?IIO$$+$I$,~ZOOO8OOZ7Z7$ZO$+I7$7$7Z$$ZZZ$ZOO??Z7ZZZZ,,,,,,,:::,,,,,,:,:::::::~:+?+??II77777777
==++++++=++====+++++????????I777777ZO$OZ+7=:~:?=Z=?:++$8Z8OOO8O$77$$7?888$=+777Z7Z$O$ZOOOOZ???ZOOZOZD?::I?:,,,,,::::::::::~~~~::=+I777777777
++++======+++++==++++++????IIIIII$OZOIO~?=?=IOI+~$ZZZZZ$O88OO8IIIZ?$7+O8Z8I+I?I7$$7OZZOOZZO7I??Z7ZIOOOO8O8OZ,,::,::::~~~~~=====~~~=I77777777
+++++++++?????????????IIIIIIIIIOOZ$=O7$?=Z=O$~?ZZOOZ+ZZIOZO8??$ZZ88??++88ZZ7+++I77Z$OZZOOOZI?Z???I$7$888888+?I:::::::::::~~~~=~===~=+I777777
???????????III??????IZDDDIII$OOOZ$ZO?$7$O7~Z+Z8Z$OI$Z8OO$O88$O?DZZ8OO8?IOZ8$Z+=+I77$OOZZ$OZ7?I+????I7Z88OO88Z+7I,::::,,,:::~~~==?II7I?777777
IIIIII???I???II?I?+?ZDNNNDDZO+OZZOZ$OOO?=I$7Z+Z~ZZOOZO$OO88?O8O$88O8888$$7Z8O$+~+I7$8ZOOOOOZI?I+7????I8OO88888+++I=,,,,,:,,::::~?+III7777777
IIIII?????++++??+++?DD8DNNNNNDDN$7OI?I?7ZZIZZ~Z78OOZZZ8O888O?ZOOZ88OOO88O?$I7III+=I7Z$OOOOZZ7$I?+I77OZIZ7O88888O++?++~,,,,,,,:::=+?II7777777
7III?I??II?I?I???+8NDNNDNNDNNDDNNNN87$ZO$O$OOOOZOOOOO$8O888$==88OZ8888O8888I+II=O?=?I$Z$$ZZZ8OO$I??$I77O8OO88OO8O+++++??=::::,:::~=III777777
7IIIIIIIIIIIIII?ZDD8NNDDNDDDDDDD8NDDDDDOOZ8$OZZZO888O88888O8I+?OZOZO8O8O8O8Z7=++?$7++?$77$77OZZ$$$87ZZO8O888OOOOZOI++++?I+?=:::::::~+II77777
77IIIIIIIIIIIIIINDDNNNNNNDDNNDDNDD8NNNDDONDD~=8O88O8OOOOOIZO7O8OI7Z$O8$8OO888I7+?$IZ7==77Z$I777Z$IOZZ$O$O88O88OO88O8???+?$I++++::::~=?I77777
77IIIIIIIIIIII7ZN8D$OODNDZDDDNNDDDNDDNNDDDNND8O$O8O8O8888=~?IOOO8=IIO88O8OZO88Z+$I7I7O==7OZ7I77Z$7$Z7OZOOOZOOZOOOOOOOOI?+????++=?+=:::+I778M
DOOOZI7IIIIIII$OZ$$DDDDDZZDDDZDNDDDD8DNNNDDDDDDDD8888O8888+=~IOZ$O+=OZO8Z$8$88O88I?+=$8?=$88$ZO$O77777$ZOOZO8OOOO8O8OOOZI????7??+++==~:=IDMM
NNDD8D88D8OZ$ZZ$ZZZDD8888OONDZD88DDDDNDNDDDDDDDDDDND88OO8O8ODO8Z=$7?=+IIOO88ZOI$88$=++$O?~+ZOO$OZZ$77I7I$I77OO8OOOOOZO8O8O$??+?Z?+$===N?OMMM
888D888DD8DDZ88888888DDDD8NDD8DDODDDDNDNNDNDDDNNDDNDDD8Z$OO888888?O$O===IIII77III7$O7=IIZ7~=77ZOZZ7III7II7II77OOOOOOOOOOZZZ8I??IMM+8+IMMMMMM
DDNNDDD88DDDDDDDNNNNNDNNDDDDD$DDOZ8N8OODODDDNNNNDDD8DDDD8O:I88O8OO888O?~===+=+++=+II=~I+?+7==7777I7IIIIIIIII77$OO8OOOOOOOOOONZ$MMMDMMMMMMMMM
NDMNDNDDNNNNDD8NNN8DNNNNNNN88O$ZZZZZ$$Z$Z$ZODDD8DNNDNDDDZDD7:,7O888O888O====+:::::~?=::::+=7=~=IIIIIIII?7IIIIII77777Z$77$DMMMMMMMMMMMMMMMMMM
MNNNDNDNDNDNNDDND8DDDNNNNNNDNNNNNNZZOZZ$ZZZ$N8DDZDDDDDDDDDDD88$=:O8O8DD8O8Z=++7I?=:,,,,.,::?+I=~~I77II?++??I????ID777IINDMMMMMMMMMMMMMMMMMMM
MMMNNNMMNNDMMNNNNDD88888DDD88D88DNNNDO$ZZZZZ$$Z$7ZDDDDDDDDDDNDNNNNDNDNDDOODD8Z??7777II??I?I?~::,:~+~~=====++=7IIDNMMNNMMMMMMMMMMMMMMMMMMMMMM
MMMMMNDNMMNNDNNMNDNDDDDNDD8888888DD8DDNDDDDDDDDDDDDDDDDNNNNNDNDNDDNNNNNNNNNNNN88O7III777IIII7IIIII~::,:::::~OZ=N8MMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMNNMNNMMMDMNNMNMMMMNDDD8OO88DD8DDDDDDD8DDN8ONDDDDDDDDDDNNNNDNNNNDDNNNNNNNNNNDDD8DD8O7III$777I77II7I?::?MNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMNNMMMMMMMNNNNMMMNMMDDDO888DDDDDDDD88D8O8D8DDD8DDDDDDDDDDDDDDDDDDNNNNNNNNNNDNNDDD88ODDDDDOI7777$$7MNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMNNMMDMMMMMNMMD8$88DD88DO8DDDDDDDDDDDNDDDNDDDDDDDNDDNNNDDDDNNNNNNNNNDDDDDDDDDD88D8888DNMNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMNMMMMMMMMMMMNMMNMMMMNNMMMMMMD8D8DDNND8DDDDDDDDDNNNNDNDDNNDNDDDDDDDDDNDDNNNNNNNDDDDDDDDND8DD8D8DDMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMNNNMMMMMMMDMNMMNNNDNNDD8DDDNNDNNDNNNMNNNNNNDDNDDDD8DDD8DD8DDDDDDNNDDDNDNNNNDDDNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMNMMMMMNNMMMMMMMMMMMNMMMMNMNDDDDDDNDDDNNNNNNNMMNMNNDD8DDDDDDDDDDDDDDDNDDNNDNDNDDDDMNMNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMNNMMMMMNNMMMMMMNMMMMMMMNN8NNNNNNDNNNNMMNNNNNNDMNNNNDDD888888NMMDDDDDDD8DNMMMMMMMNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMNNMMMMMMNMNMMNMMNMMMMMMMMDNDNNNDDNNNNMNMNNNNNNNDDDNDDNDNDDMMMNMMNNMNDNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMNNNNMMMMMMMMMMMMMNNMMMMMMMNNMMDDDNNDNMM8DNNDNNDDNNNDND88MMMMNNMMMMNMNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMNMMMMMMMMNNMMMMMMNMMMMMMMNMMMMMMMMMNNDNNNNNDNNDMNDO8NDNNNN8ONNMMMMNMNNMMMNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
*/
