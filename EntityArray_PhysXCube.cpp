

/* CPhysXCube actor - rigid body */
#include "PhysXCube.h"
#include "LogFile.h"

extern CLogFile g_log_file;



CPhysXCube::CPhysXCube(NxScene* gScene, osg::Vec3f size, osg::Vec3f position, std::string m_geometry_type, std::string m_file_name) : _scene(gScene), 
	_size(size), _position(position), _applyShaderColor(true)
{
	osg::Geode* _geode = this->init( _scene, _size, _position, m_geometry_type, m_file_name);
	addChild(_geode);
	setMatrix(osg::Matrix::scale(_size)*osg::Matrix::translate(position));

	_applyShaderColor = false;
	if (_applyShaderColor)
	{
		setUpdateCallback(new MotionCallback(_actor, this->getOrCreateStateSet()));	
	}
	else
	{
		setUpdateCallback(new MotionCallback(_actor));	
	}
	

	// for debugging only
	//setName("cube");
	//_actor->setName(this->getName().c_str());

}	


// create cube
osg::Geode* CPhysXCube::init(NxScene* scene, osg::Vec3f size, osg::Vec3f position, std::string m_geometry_type, std::string m_file_name)
{
	//osg::Geode* _geo_cube = (osg::Geode*)osgDB::readNodeFile("mj_truncated_cube.3DS");
	osg::Geode* _geo_cube = (osg::Geode*)osgDB::readNodeFile(m_file_name.c_str());
	if (_geo_cube)
	{
		NxBodyDesc BodyDesc;
		BodyDesc.angularDamping	= 0.0f;		
		NxBoxShapeDesc BoxDesc;
		BoxDesc.dimensions = NxVec3(_size.x(), _size.y(), _size.z());		
		NxActorDesc ActorDesc;
		ActorDesc.shapes.pushBack(&BoxDesc);
		ActorDesc.body			= &BodyDesc;
		ActorDesc.density		= 10.0f;
		ActorDesc.globalPose.t  = NxVec3(position.x(), position.z(), position.y());
		NxActor* newActor = scene->createActor(ActorDesc);
		newActor->userData = (void*)0;
		_actor = newActor;		
	}
	else
	{
		g_log_file.write("cannot find cube.osg");
	}

	return _geo_cube;
}

osg::Group* createTower(NxScene* scene, osg::Vec3f origin, int n)
{
	osg::Group* group = new osg::Group;

	const float size = 1.0f;  // box size
	const float dy   = 0.02f; // spacing
	
	osg::Vec3f tmppos(0.0f, 0.0f , 0.0f*size);

	osg::Vec3f position = origin + tmppos;
	for (int i=0; i<n; i++)
	{
		position.z() += 2.0f*size + dy;
		CPhysXCube* cube = new CPhysXCube(scene, osg::Vec3f(size, size, size), position, "cube.3ds", "3ds" );
		group->addChild(cube);		
	}

	return group;
}

osg::Group* createTowers(NxScene* scene, osg::Vec3f origin, float dx, float dy, int nx, int ny)
{
	osg::Group* group = new osg::Group;
	osg::Vec3f position = origin;

	for (int i=0; i<nx; i++)
	{
		position.x() = origin.x() + dx*i;
		for(int j=0; j<ny; j++)
		{
			position.y() = origin.y() + dy*j;
			group->addChild( createTower( scene, position, 10) );
		}
	}
	return group;
}

osg::Group* createWall(NxScene* scene, osg::Vec3f origin, float size, int nx, int ny)
{
	osg::Group* group = new osg::Group;
	osg::Vec3f position = origin;

	for (int i=0; i<nx; i++)
	{
		position.x() = origin.x() + size * i;
		group->addChild( createTower( scene, position, ny) );		
	}

	return group;
}

osg::Group* createTriangle(NxScene* scene, osg::Vec3f origin, float size, int n)
{
	osg::Group* group = new osg::Group;

	osg::Vec3f p1 = origin;
	osg::Vec3f p2 = origin;

	for (int i=0; i<n; i++)
	{
		p1.x() = origin.x() + size*i;
		p2.x() = origin.x() - size*i;

		group->addChild( createTower(scene, p1, n-i));
		if (i>0)
		{
			group->addChild( createTower(scene, p2, n-i));
		}

	}

	return group;
}

/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~77=~~~~~~~+:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~7IO=~~~~~~I=~~~Z:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~I??=~~+==I=+7$8,~==~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=I7~~O???:=$77?=$O+$O$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~II?I7?+?===+IIZ$+$D==~~~+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=+?8+I$I?~I+~II8OOO8Z+Z88~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=77+ZI777?=888$ZZDZ+OD~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~I8Z$II8?+?Z$Z=ODDDI?~~~~~~~~~+?~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:~=?=~~ID$I7?7O~=~Z8+$DDZ?=~=~~~I77Z8N?~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~?Z7Z$$$$7O??DOOZII78+~,87ODZZI++=I77$I?$~$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$O8D8DND$I+N$D?I7Z?+.7ONDOO7?I7$ON8OND8:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:ZN$7ZNDDO$Z?Z?7ZNZD7I+7OO8N?ION~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:~~+++=~NDMOODZ$Z7NO$Z8DZ77$D8D7?O7~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~?877ZO$7+=?~$N7OZIO7O8$O8$IIODNMZZD$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~I?8O$ODN8787IZI$DD77O77NNN77$ZZZ?8$:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~8ONNNNN$7$78O=ZO7$Z?O77OZOO8NNOII++=:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~?DDNNM8OZZ77$IZ$D877IODOZON8D7?I=I7+???I+==~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:~:~~~8ON8DNMDO7O87IIZZ$?$$O8NNNOI?+??+7O$IZ$78Z=~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:??=?ZDNNDIOMZ8D7DO$7$I77=O8NMN8II77DDNIZ8OZ+~::~~~~~~~:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~?DD8DDIDN87ZZZDDD8ZZ?7IOD8?NDN?+=IZ8ND$$ND+=~+I~:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:7NODNDNOO7DZ?IOZZODDN?$OM$7ZNNDM?7$$~+7+78D:~::~~~~~~~~~:~~~~~~~:~~~~~~~~~~~~~~~~~~~:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:~==~~:$MDMZD8DOI7+8OMND7$ZNMOI8Z8M$$=$+=7O$?~~~=~::~~~::::::::::::~:::~~~::~~~:~~~~~~~~:
~~~~~~~~~~~~~~~~:::::::~~~~~~~~~~~~~~~~~::~::::~~~:~=:$O++~~~IONNN8D??OD88$IOZ7ONOMNMNI$+$I$OO?++?7$777:~::::::::::::::::::::::::::~:~::::~:
~~~~~::::::::::::::::::::::::::::::::~~:::::~~~:::::I$8NO$I+~~O8$D88O$N8$MI87?ZDDDNNNZ$Z$Z8NOD?88D8ZN:~:::~:::::::::::::::::::::::::::::::::
~~::::::::::::::::::::::::::::::::::::::~:~~~:~~:~::~~D8DDDO7ZODDNZ8Z$I7DM8D7I$OMN$O88I$ZDNM$ZZDND?~~~::::::::::::::::::::::::::::::::::::::
~:::::::::::~:::::::::::::::::~:::~~~:~?7++?=~+=~~::I$DDDDNNNZ8M8NO8DI77NMM$O$DMNO$8NO7IDNN$$88NOZ~~~::::~:::::::::::::::::::::::~::::::::::
~::~:::::::::::::::::::::::::::::::::~~~Z8Z$I+7:7I:~:$DMNMN88MNN8MDON8IION7O7$NNN$DDMOZ8MMOI$D=:::::::::::::::::::::::::::::::::::::::::::::
~:::::::::::::::::::::::::::::::::::::~:=O8O87II7I?:~=IO$O$I8NNNZDNZ8$77DN7Z$7N8Z8ODDII7MN7$::::::::::::::::::::::::::::::::::::::::::::::::
:::::::::::::::::::::::::::::::::::::::::~7ZONNZ$$I++?:$DOZZ7ID7D$$ZO$$7D$ZO$IZ7ONDDO?Z7ON7::::::::::::::::~:~::::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::~::IO8ZM77?IIONNNNDZI8NZNZZZZZ7DZ877ZDOM8NOD7ZM=::::~:::::~~:~+~+~?=::::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::~~I+$NDNOZ$$ND8DDZI7ONMDOZO$7Z8O8DON8NZ88OO??I?~~~~+==~=~+~+?ZI?O$$=::::::::::::::::::::::::::::
:::::::::::::::::::::::::::::::::::::::::~~~=7=~=NONN8DMNONDNOO7$ZOZ$8OZ$78$Z8NNND8DN$I$IZZ=Z$7DD7=7+??I+ZNND~~:::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::~~Z8ZD7ZO=I=~=IMNN88NNMDOIZ+OM8OD$D$O$NI$O7$ZNZ??+OI=$O8N8ZO7Z$IZ???7=OD:~::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::??II+8ON$Z??=?+$NMMDDMNNDZZOO?77OO8O8$OODD877O?II77IO8ZDND8?7??+IOZ7$88:::::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::888$==II+$+87$I?O8NMMMMO$88Z$D8NNDOZ88DDDNO8$I?$787$ONNNNNZ$OZODZDDNO~~:::::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::D88O8I8OI7$78$ZZ$=NDMMM$O$MD7NMNNN78DZNDNN8O$$++=+ODOMDNDNO78N8NDMMD::::::::::::::::::::::::::::::::::
:::::::::::::::::::::::::::::::::::~~+~8DDDD8DOZ$$I$ODOO7ZI+MNZDO8Z8DDNNDOODZZZ888ZZ?I77IZONNNN78$O7NNMND:::::::::::::::::::::::::::::::::::
:::::::::::::::::::::::::::::::~=O7OZ7ZD7NMN8ODZZ7I?INDDOZN$7ND8$NZNMNNMNNM888OND8$7?$$NOMNNNN=78DOZMM?:::::::::::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::DN$D$8N8MMND8DZ8$I+IO7MNMM8ZNZODDMNMMMNN$NNNN8D8D7IO$D8NMN7=7$DON8~:::::::::::::::::::::::::::::::::::::::
:::::::::::::::::::::::::::::::::::$D?Z$88DNN8N8OO$8$7Z7~+$I$I+?+ZNMMMMMMMN$8DDM8887N8O?MMNNZZOON87:::::::::::::::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::IZOD88DMNZDZOO$Z7O$OI87D8O$ZOMMMMMM8$$7M8NDZZ?7NOONNZ?8NOO?,~=~=+=+=~=::::::::::::::::::::::::::::::::
:::::::::::::::::::::::::::::::::::::::::::+7NDDD$DO88D8O$ZZDMD8ZI$8NMNMM78ZNDMNMO$$Z8DMN8II88I$=II++?+ZZ$IZ$~::::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::~DNNZ8878ZO8ONN8D8OI8MMMOD8$ZNNDOIZ$$MMZ7??+I?Z?I77?=+$NZ~==+?OZO8::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::NZZDM8OD888OM8ND8$ZNDDO?ODNNMND$8$N$Z$88DZ8I7$7$$7NDI~+=?O?D8DI:::::::::::::::::::::::::::
:::::::::::::::::::::::::::::::::::::::::::::::::~:=78NONMD8DMNN88ODMMD$ZDNZ8DMND$77$$I8888DO$7D7NDNN=~?D7DZOM8+::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::==ODDNIMDOONNMDMNMN8N88N8DNNDDOZ7?~=,,:=~:77?7IDDN?77$M8MDNI:~::::::::::::::::::::::::::::
::::::::::::::::::::::::::::I8ZOO87I+:::::::::::?ZOD8$OZ$MDNDNMMMMM?7?=INMDNMMM7I$?$7+++~?7ZM$MM8MN78ZMOMO7$$:~:::::::::::::::::::::::::::::
:::::::::::::::::::::::::::::ONMMOZOI?+==7::=::+DDDNDMD8$$NM7NNNMO7NZ7D$?MNNMNDZO8ZO?77?Z7?I$$+7M8~$OM8IOOO???OZ::::::::::::::::::::::::::::
:::::::::::::::::::::::::::::::IDNND8D$7++$=7+=$ODDD8DDN88DM88NO$NDDNDD$ZNMMND$8ZD8IN$ZO$7NDN7$I$?$MNMI$I+DODOO:::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::DMNMN8O?8OI7I8ZNNDNONOZZ$ZDD$ODN8ND8O$OZMMMO$$7$ZO8N8Z$8DNDDDZ$M8Z$:7DZ8N8NO::::::::::::::::::::::::::::::
:::::::::::::::::::::::::::::::,+~::,?D8DZOOD8DZDNDNN$NOOZNO88NDNMMMD$77O8MD+7+$$$D7N7NZNDMZDNZO$ZZOOZ$NNNMD::::::::::::::::::::::::::::::::
::::::::::::::::::::::::::7?7?++I==~:=~::$8NNNDNMMMNNNNNO777O8ODMD$=I?D8Z$$$=:??7OZDOON8DNMDN87$7$OZMNMNM?::::::::::::::::::::::::::::::::::
:::::::::::::::::::::::::::~OOO77I7+~=~::::::,:$DMMZMMDDOOONON8D?$ODOI+D$8:~7$+?OODNDDD8N$NN8IIZ$MMMMMN:::::::::::::::::::::::::::::::::::::
::::::::::::::::::::::::::::~IZ$7OZI7$?=~:I~:::~~+ZZNNMN88DD8NNN7NNNZ7I7$DI=$Z$OZN8DM8M8MMI$?++~MMMM$:::::::++==+I~:::::::::::::::::::::::::
:::::::::::::::::::::::::::::=$$ZDDZOZZI?=Z7~$I$7+=$,=I8NOMNMMDO8DD8$Z$IINMODZIZNNM$MMMM8ZD$8D$7D8+?==::~:==$$8O8D~:::::::::::::::::::::::::
::::::::::::::::::::::::::::::=$DNZODZ8OZ?Z?:~++$ZI=:$+7Z8DI8NMODNN8DOO$N887I?8DNNND88NNZ8DMMD$$:?Z7ZDIII7$ZO$8MD:::::::::::::::::::::::::::
::::::::::::::::::::::::::::::,,:,$$MZNZ877OZO7$IZ$DIMMDNZNNDDOZNDNDOZOOZII8$8NMMMNNMMNOD8ZNNI??Z8DOD$I7O8N7NMNII:::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::,~DI$DDZZ$IIOOZZNMMIDNNDNNZDD$8Z8ZIZ?DNONMMMDMNNO88MDD??7?DMNND?OM8NMMN+::::::::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::,OINN$OO7$8N88ZNNN$OO8$ZZNDN8O8O77NMMNMMDMDZZ8ZNZN7IZDODDNMND7ZMMM7,::::::::::::::::::::::::::::::::
::::::,,,::::::::::,:,:::::::=:+7~+=+=:=~7Z$N888O$O8MMO7Z$87?Z7?Z7DDNZ8DD7MNNMMMNZ$7Z$N+$8NZD8DDMDN87778~::=~:~::,,::::::::::::::::::::::::,
:,,:,,,,,,,,,,,,,,:::,::,,::ZDD8NZO7D+=?~7NDO+$ZD$IOND88$NDDD8$8ZOD8DZ$$NOZO8M77DIOO?8OZZDZDNNNNMNDD?+~??+I==III?7~::,:::::::::::::::::::::,
:,::,,,,,,,,,,,,,,,,::,I$?$+O8ZMM8DMZOD8NO88N78ZZ88OODDZOOZOO?OZ$NDDD8NDON$M~,:DDNOZ7$ODDDNMDM8ZMI==~+==$$$$$I7O7$7?7$I::::::,,,,::::::::::,
:,,:,,,,,,,,,,,,,,,,,,?8DDNNO8I7~ONMDNMMMDD+DD88DZ8DD8$DZZ$ZZ?$IIONMNMNNND::,~+?ZNNN?88ONNMO88O?I$7Z77?777?$7O$Z78D8+:,:,,,,,,,,,,,,,,,,,,:,
:,,,,,,,,,,,,,,,,,,,,:::,DNNDONO7=+I7+ZZNMNMNODDDNDD8DDO$DZN8IZ7ODZNNMNMND$DDZ7D$OZ8Z8OZZN8$N87O8Z8NOO$O$ZOI$NNO8M,:::::::::,,,,,,,:,,,,,,:,
,,,,,,,,,,,,,,,,,,,,,,,:,::~888NZDZ?77I?==:==I7NNMMNNDMODD8N8OODMMMMMNNNND:IZII7N88NMMDM8877Z$DD$O8NZN77$OONNDNO:::,,,,::,,,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,,:,,:,,,,~,:~?=+?+I=O$$O?Z??M87I~ONNMMNMOZ8DDMMMMNONMN8MZO8=~O8N7D$NNMN8$OM8$OD$$NN8O$IZ88D8NNZ,:?I?IIII?=,,,,,,,,,,,,,,,,,,:,
,,,,,,,,,,,,,,,,,,88$8D=?:~$+$7$$+Z?ZDZOZI7MD7O?$DMDMMMDMON77MNDMMMOIMM8O:=7DONMMMMDMZ$8O$ON88NDN?78ONMZ=~=:::???7$OOO?::,,,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,,,,,::NNNO8Z7$?DDZ$IO?O7O$Z$II$NN8NDNMMMNDNMM8NNNMMD7D?N?8DN,,?ONNNMNOO$Z7N8NZMMNN$78,~?=+I?=Z??OO88DM,:,,,,,,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,,,,,,,,,ZONNOI8NO8778$$Z8$OIO$I7$Z+OZOMMMNMMNN$7MID=IMMM8NNM,.=~~ZMDND8NNZ8DNMNN+8Z$$77+??$77N88DNM?,,,:,,,,,,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,,,,,,,,,,,ZDNNNMODDZD$$DOO7O87Z$Z$?8OODDNOIDDNNN7?OZ$?8NM~D~:=,87INMM7OMDDMMNO7I$$O$ZOI$ZIIDZMOD,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,,,,,,,,,:,,,:,,ZZ$O7I+OINZO$$78$7$ZN88NNNNOM$+Z?IZD8DN8M7$Z+DD,?I7NNDNDNMMD87I8O788ZZ8NDD8Z:,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,?I:=$7ZO8OO$88ZOZ8NZMOOD=$?+:,$NDN$,OZ?DO=:=7NNNDM8MD8$ZZZZOO88N8NDN+~,,,,,,,,,,,,,,,,:,,,,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,~=8=7OZODZ8ZO$$ZONDNMONDNNN87777NNM8$MZ8NNN8ON8NMM8ZIZ$$NDN$DDDNND,,,,,,,,,,,,,:::~~I?+?=,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,+IDDO88O8DDO$$D77ONZ8D7=,,,~:,,+$N?IM+DNDMNMO?$I88ZODOMNNDNNNZ=:,,,,,:,::=?=~=+7?=$I$$8,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,+,+I88$OOOZN8Z87Z$$DO88=~:,,,,,8$D:?I+=~MO7~88ODN7DDNNDMMMNOZ~,~+:==IIII=$OO?7ZIZ88Z8,,,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:~7ZN8Z8DDZZ$$Z$7=+7+77:,~,:~+$N=,+7N7,:~,I8ND8DO8NDDZO?Z$+=7+7II$?O$8$DNDID8DN8,,:,,,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,~~$$::,,~,,,:,,,:,,,,,,,,,,I$$?7IZNZOD$ZI$,7$~?~,~DNO8DNNN$M7,~+,=ZNDMDNDNND8IOII7??=D$O$8?D8DDNDNZNO,,,:,=??+I~,,,,,,,,,,,,,,,
,,,,,,,,,,,,I$$ZZZO:=$=$I7+OI?I~~~=+~,,,?I$O=:ZO$NNDZ$8O$$$O:,,~I8MMNDM~+?+:+:+8Z7$NNIIO$I88+78OZ$77?IIZDNNNN8Z,::~,=+?$?$ZOO,,,,,,,,,,,,,,,
,,,,,,,,,,,,~ODN8Z++7ODDOD8OD8$Z$IO87IIZN$M8+$O8NDOONMD87$7=$$N=:DNMMN7?7:,,~?ZD$Z,.,,.Z$OD8DDD7II+8ZO7NZDMDD~~?I?I?77ND88::,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,:$NNNND788NOD8NZONNMDND$$NNDNNDMNNNO87OMMNO$$+II?:,,,,IDI?:I~+:IM?,,,~~?+~+ZDNNNDOD8IZ$NNNNNNNI7ONID+DIM8888=,,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,78N8ND8OONDND8NNZ8NN?8MMMMMNNMMMNMMNII,.=+?~OOZ$?,,,,ZD?=+M?8NO7=:::~~$OZ78NDNZ$MZZIZONDMM8D?7$O$8ONDZ8DD7,:,,,,,,,,,,,,,,,,,,
,,,,,,,,,,,,,,,,,:+ZDNDNDMN$7NNDZODDZZMD8$?=,,,,:D=:,,:I8O$$ZII:+8,NO+:=8MMNMND?87ODNDOD$DN$DINZ8N8NNM88O7IDD8ND$+II~?+~~:,~::~::,,,,,,,,,,,
,,,,,,,,,,,,,,,,,,,~:O$DM7I?III7OOODNMNOMMDN:~,~877Z:+,$ZZOO8D$7~?=8=+M=MDZZ?+ONNOIOO8ODDZ$Z7MMNMND78O~,,.,,,,,==?+=?I+=~=~==~::~,,,,,,,,,,,
,,,,,,,,,,,,,,,,,,,,,,,78D7O$II7IMDDMDMMNNNO8Z8O$$=~,,?I88DO$ZZ$I78MD7??8Z8OIDNMMZD7D8N87$OZNNO8$~Z=I=~:,,,,,,=???7+??++~+?~??~=~,~~.,,,,,,,
,,,,,,,,,,,,,,,,,,,,,,,,,DNDOO7I7$I$ZZD$78$MNZ8N8N?~=+I=7$O8O?7$7I~$$====?II$ONMMDN$8$N$77?=I88$===+=,~...,,,:+I$7I77+I?7?+=~I~===+::,::,,,,
,,,,,,,,,,,,,,,,,,,,,,,,,Z77OOZZZI$77$Z77Z$8MMO87NNNO?IIOZO=,?~+?$?$$~+=+I$NNNOZMZDOO7$$7?+=$ZDOI~::.~...~=~:+I77$$?$Z$$$O7=+II$+~==?=~~~:,,
,,,,,,,,,,,,,,,,,,,,,,,,,,=7O88$$8$?$ONN8Z$DDOO8ODMNDD7D8ZODO7=~~~,MM===DNDNMNNND8IIIZ8$I?INZ+77$+IIOI+I$$8O$$$788OOZO$Z+???==?I?=+$+?~~+~,,
,,,,,,,,,,,,,,,,,,,,,.,,,,+77?N$$O$Z?I$=$++??~+~=::?..:,+.,...,ON8NMZDD=ODONNNDN87I=I+$7?I8$D8N8MZO???7I7DZ$$8O7IO8Z$?~?+~+++$II7=+==?I=~=.,
,,,,,,,,,,,,,,,.:,,,,,,,,NNNN7ZDNOD?$OZ$ZOZ?I+I?I+O8+I+.,~DD:.:,ZDNNO$~7NZZIMNZN8$=~=IZZ$8NDZZMNODDNN7DOZ?ZZ$IOIZ?I??IO7?I=??+?+=~I+I+:=,:.,
,,,,,,,,...,:OD$8O?I~~=::DN8MM7MO7DO8D?7O$O88N7DO++:?I88.,:,,+:+DMD8IN+77ZN~+=+~??~I7Z$O$=I$ZOMO+8DM8D8D8Z$7ZZI$OZZOZ7???==7=++??7?==~+??=:~
,...........,IDD8DZD$II?I.I$DND8$DOMO8NMNON8DDD8$D8$?ZO+,~~~:=~~I?MZZ7M7I??I7+,+?I==Z=Z7$I=+OM~~I8MMN88ZZ$O$IO8O8O+$7$7I~~+=$+IIIZI,=:=+7~=+
...........,....NNO7N$NN+8MNMMN$D8NNN8DDNMMNDDMO8DOOZ78OOI=?$~ONDOO$+N:=$O7??I==+IO8NZ77OZ7$7=+?O?OODO8NOODN8D$D?7?7I$ZZ??=+~=$?7===7$+?~+++
..................,$NNOOMMMOMOD$D8ZD$$DMM8DDNDN8O8Z8ZZ$7Z7?+~?D:~O$NN?7==??~::~::$=$N78$ZO$$7??=$Z7?$ODDDMNMMO?.I$ZZ$ZZ?IZ?$7Z~II=:=++++?I?+
.....................,..:7$NDNNMNMDNNND$DDM?7I$OI7ZZO$8OZ$$ZOI:::MNNN:~OI$Z$I7I8INNMMN7ODZ$DZ7ZDZD8D$MDMND?$?ZZ7Z?I==IZ7I7?+~I??+~+=~$?I=~=~
........~..,.................,.=I..:.,~:.?+ZNI77=7$IOO8D77+NOO+8$ONZI+I$ZZI7$O78DMMMZ7OZ7NDZZDO8II$?7$8=OO?$7OOO7$II=$$$=7???+I$+$II7+??+:~?
.....?Z$II~:,,,..................,,........,:O88O7?7I8$NMDZ78DDDNMMD7=$OZ$$+$8MNNMD7$$Z?+$IZO?7ZZDIOII??7??II$$O?ZO$?77+?+~I7=I77+ZZ=7I:?+:~
.....8DDD7Z$Z==:,.....~:....,..:I7O=,....+I7IZD88D$7??77$==7DNO=7ONDO+OZZ$ZO?$MMDD$I$$DDNN$INOO8NND8N8$OIOI?$$7$ZZ?7777?$Z$ZI??7$Z$O~=~~==~,
.......$8NMOI8Z?~I~:~?7?~~?+N=.,ZDD?I..,.:,OZ=$MDDZ8$?$Z$~IDNNNNNMMI?7NM8OII+=I?Z$IMZZZ8ZDDONND8NZMMNO$$Z$ZZ$IZ=?I7$ZI$$I7IO7$?:Z?OI?++=:~,,
.........OON888O88+D87$Z7I=Z8O.,$NZ$++.,...,:7N8$ZIOOZZO$$IZZNDOMMNDIN~?7+~:I+7I7$$$=IZ88DDDNMMDMMNNOZ8DNN$8$ZIII7Z77OZ$$7Z?7==I~?++=+=:~:~,
............~88DNN8NDOZ$$$$I8N8IDDNO8I=:....::?Z8DNOI$8DZ7?=++$DDOO$Z$$:,=~+:..78NOD7OZND$ODIMNNNN877Z$MZ78D$$7OZ8O8Z87OZZ7$7?I?==+?+I+===::
...::,,...,.......ZMMN7ZO8OZDI8INMN+N+?888OI++8Z8NN8ZOI7$7+7,?IO$DMDD8..?=77I+++DNMNND8MDDMDMDN8ZD$I87?III+IOOI?$ZZOI+$7OZZ78ZZZO?I7~+~~::=:
.,?+I7ZI~::.........+$=D8DN8MNIZZZI=~?.~===..~:8DNN$DD8ODZZI++I~MMMNDZDO?ZODIII?$NNM=ZND8?8DD8$$MI==7?7I7I?+8OIZ7?ZII$O7$8DDNNO$7I$?++~=,=~=
..D8NODOO7+:++=~,,...++ZZ7NNO8DZI?$D7Z$$OO$?.O$DM$NNND$O?I77+Z=8NMDM$8Z8$?II+=IIIZONODZND+DDDDM$??IO$$ZOOZO?7OZ$$O$8O$8ZNNN8IZO$OZ$++=,:~=~~
...8DDNNDO++I7?,OI?~+77I78=NNN+~?++D7O7Z$?O$+DDDNMMDMNO7?DZ?:DMN8DNNNMDOI$ODDDOO7ONZ7NMN$.IO$OZ$?D$7NNDNNN?OO$$DZZNOM$OMDIIZO$$?Z==+++,~~~:~
.,.==78NN8ZZ$D8DM8NOO88NDOMDMMND8OZ$7$ODOO$$D$NMNNMN78N8???NZ~:ZMMMMMZ?$7D8NNDO88OZ:O?+OI8DD$$IOZ8Z8M8ONDZ8OZONID8NO7I?=7Z$ZO8DI+?+=====+...
=.:~~=,.,:.:ZNNNM8O8ZZ$OONONNNNDD8Z8Z$I+II?I=$DM8Z$Z78MN++=~?~?IMNNMN?8IDM8NDDOMOO==78NNO$+$IDNZD8NNONN7I~:+,?$7$?+$?=$ZZDOOZ7IZ7$~+?++=:...
,~,..,..=~=:.,..~,:~I,~+~$I7DZNNDDO$$8I$Z$777I7DNN887ZO7$$$III+?MMM$?:+:D$N$NDM8MNN8NNNO87ZODDDNMN8Z8+$?$I877$I+III$7I$?ZND=I?77?=7+?=~==:..
7=++I??II+=~,=::..,,.~:~?ONMN7$ZNZ8NO88$8OOZZZ7?8N8O8DO7NND88N8:=$MNN8I,888IZDNMDNNOO7=+$NMNNNDNNDNNZ8DODND8IZ$II$7$+~?+I$$O??$I?77?I=~:::..
I+$ZI77I7??+,=,~=+...+~.:7DMNODN+7NO7OZ8DZZZZ$$DD$+$8O$+OID$:NZNMMMMI+,~.+NNNNNDMDN+7$ZODMMMMDNMNDOID$$NMNMNDD8ONO$8Z7OI7I=ZI7$$$$7??+~.....
7?+I8Z8Z$Z7??+.+~,...~I.?+?IZNMMN87DZO888D788DZOIDD$IOI?8MOO:~,?NNN$III7O$8NDD$DNMZ$OMONND$MNNNM7??$$?,+7$?8$O$87+?$ZZ8?IO+Z$Z7O7$7I?+:.....
...=:~=ZDOIZIIII=+::7IZ=+DD8ODDMMMDZ8OD=D7NZO$87Z7?O$?IO7+???+,M8$87$8$+8MNNNNND88ZII8N8ZMDNDOM$I7:=:,..:..:++=I+ZIO?O7?+?IZ$Z7ZZ+I7=~......
~=+I?I===778ODN8DO8ZN8DDNMD8MMMMNM8NMDNNNOI8ODZZII?OOOIOND8ZZ7=MMNDIN8MDNMMDDNNN$8O88OMMMNDN?OOII$~,...:+I,.=:,~$I8ZIIOZ+$7I?7I77=?=:.......
7Z7?$?I+7I?~$Z++DODNO8DNM8MNDNN8N8NDN$DDNOON8D8$7O$$87O$D8$8$7DDMNOMM+$DNN8MDZNDNONNNNDNND7:?:7=.+.?...~I~.~::=Z++I+++IZODI7~?++::~:~:.=,:?~
D8$7Z$$=?=7.+=:+,ZZ?~~.:.,+$8D878NDM8MMONO+...,$MMN8Z?$7D888$+=MMNDN?::+I.DDNMDD8ND88ZZ$77~...?+.?.~........,?+?,..7+IZ?+$ZO$?$I,:~=~.=::$Z$
~8OOO$Z$Z7IZ,+7I~+7=~+I$O?DZ$$Z$ZMNN8N$=8ZO~.~Z8DD8+=?DONOO7OO$MMOI7Z8II7:...:.?$OOI=I=?....77++.?.=,........,,:,~,+~$7+Z?I$I$7+=+=:,,=~+~==
::,O8DID$$IDZ=Z7~$==D?$ZN$ZDZDZ788:+$O8DDDO8O~.. . ...:DDND8IIONN+=D7I~I7+~?:::,?7O$I7.?:,.?$77?:?~::.,~:::?+.:,~::==+$+Z7?=:?++==~=:.?I$7~~
,,,:,8NO8I7D$Z7$O$NI8D8I$IMNNOZNNDN$+8ND?+=.... .  .,.$$$I7NMNOMNDOINIDI=:=:?:~~~==::::7=~~~?I~$++~:::,,:~,:+II?,:=?=IO?IIII.+~~:.,,.~+?+==:
,::::::,+8D$~77ZO8$NDODNO?8ODO8ND8NO==.......   . ...I7IIIZZ+~ZN8ZION+~~~~~~=~:~~~~~~:~+~:~~~~7+$I.::::~,~~?7=$?====$7?I+I,~~~~,Z~,:7++O=+~7
::::::::,:::?==7~Z~IZO8$?Z$D77$7~~=+~::+.. ..:, ....77?II+?++7MN8$DNN:~~~?77~~~7:~~~:~~=:::::::~777I,:~:=??OZ~~+$?Z$+ZZO7?:+.+I.,+I+7I=+:==7
:==,.,=:.,:~I7Z8~...+.,...IZOI:..:?+?++I=.   ..  . .III7I==+IIMNO8I~O=~~~7?~~~:~?~~:~~~?~:::~~:=~Z77$~=+7+??=?77$IZ+OO$77+~I~:.,,,~+===+,~~:
.,,,...~.,,:..,,,,.....~~~+IO++,,,,,,,,,.+..........,~=+I7?+++ONZ=7778+$7?+~~+O?~N~~~:+I~~~~:::+?+=?77I7I$7+I8$OO8$ZZ??8I,,?+.:~.:+.~~==~=~:
..........=.,,+:,=............,...... .... .........~=~:+7???=NDI7D8O$II7??=~===OOD::~==I?77II=++I~:OZI7IIZ$?DDID$O7M87=?:I+I.:....,..77++~:
88DD88Z,D7D$88N8......~~~~~~~~==~=~~77I==:.,,......,,I++??+II+ND8NMDNO?I?I?:,...?$I~7~~=~~~~~~~+,?=Z$$77?IIZ$8$77$$88$7=+:=~+,7I?....,..?~~?
788ZZO$~88DDMDMN$,....+::=~:=7?~~~~:8Z?==,.~==~:::~~~~I+=+:II7N8DNNZ8OZII$778DNDDD?$8+I?Z?++==:?.+,~OO8$?I?NMZ$MND7OD8$,+=++I.DOO+7...  ..,,
?ZZD$8$+OZ88NOND$:....:. ~~,:=:..::,N$+~~..O78$IDD$DD7I$II+I?IMDDO==777?O88Z8D8$NO?IZ~?+OD88OO7I.$,78DO$Z?+NN87DD8?$NNO:?=,++.887$:.. ....,~
*/