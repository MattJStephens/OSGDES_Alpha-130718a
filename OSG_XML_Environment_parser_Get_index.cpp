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
#include "OSG_Environment.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Geometry_tools.h"

using namespace osg;
using namespace std;

std::size_t CEnvironment::PAT_index_from_name(string name)
{
	std::size_t i= 0;
	std::vector<CPositionAttitudeTransform>::iterator iter;
	for (iter = mv_PAT.begin(); iter<  mv_PAT.end(); ++iter)
	{
		if(iter->m_name == name)
		{
			return i;
		}
		i++;
	}
	return UINT_MAX;
}

std::size_t CEnvironment::PAT_size(void)
{
	return mv_PAT.size();
}


std::size_t CEnvironment::PAT_index_from_unique_ID(unsigned ID)
{
	std::size_t i= 0;
	std::vector<CPositionAttitudeTransform>::iterator iter;
	for (iter = mv_PAT.begin(); iter<  mv_PAT.end(); ++iter)
	{
		if(iter->m_unique_id == ID)
		{
			return i;
		}
		i++;
	}
	return UINT_MAX;
}
	
osg::ref_ptr<osgText::Text> CEnvironment::GetText(std::size_t index)
{
	if(index < (std::size_t)mv_text.size())
	{
		return mv_text[index].mp_text;
	}
	return NULL;
}


/*
II777777777$$7$$$777777777777777$$$ZZZZZZZZZZZ$$$$$$ZZ$$ZZZZZZOOZOOZZOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
III777777777$$$$$$$$$$$77777777$$$$$$ZZZZZZ$$Z$$7$$$$$$$Z$ZZZZZZZZZOZZOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOZOZOOOOOOOOOOOOOOOOOO
77I7IIII7777$$$$$$$$$$7777777777$$$$$$ZZZZ$ZZZ$7$$$7$$$$$$ZZZZZZZOOOZOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOZZZZOOOOOOOOOOOOOOOO
77IIIII777777$$$$$$$$7777777I77$7$$$$$$$$$$$$$$$$$$$777$$$$$ZZZOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOZOOOOOOOOOOOOOOOOOOZOOOOOOOOOOO
II7IIIII77777777$$$$777$$$$$$$$$$$$7$$$$$$$$777777777777$$$ZZZZOOOOOOOOOOOOOOOOOOOOOOOOOZOOOOOOOOOOOOOOOOOOOOOZOOZOOOOOOOOOOOOOOOOOOOOOZOOOO
IIIIIII777777777$$$$$7777$$$$$$$$$$$$$$$$$$$$77777777777$$ZZZZZZZZZOZOOOOOOOOOOOOOOOOOOZOOOOOOOOOOOOOZOOOZZOOOOOOOOOOOOOOOOOOOOOOOOOOOZOOOOO
II??I77II777777777$$777777$$$$$$.7$$$$$$$$$$$77777777$7$$$Z$ZZZZZZOOOOOOZOOOOOOOOOOOOOOOOOOOOOOOZZZZZZZOOZOZZOOZZZOZOOOOOOOOOOOOOOOOOOOOOOOO
IIIIIIIIII77I777777777777777$77=7$7$$$$$$$$$$$7777$$$$$$$$$$ZZZZZZZZZZZOOOOOOOOOOOOOOOOOOOZZZZZZZZZZZZZZZZZZOOZZZZZOZOOOOOOOOOOOOOOOZOOOOOOO
IIII?IIIII77777II77777777777777.777$$$$$$$$$$$777$$$$$$$$$$$ZZZZZZZZZZZZZZOZOOOOOOOOZZOOOZZZZZZZZZZZZZZZZZOZOOOOOZZZOOOOOZOOOOOOOOOOOOOOOOOO
IIIIIIIIIIII77I7777777III77777,.777$$$$$$$$$$777$$$$$$$ZZ$$$$$$$$ZZZZZZZZZZZZZZOOZOZZZZZZZZZZZZZZZZZZZZZZZZZZOOOOZZOZZOOOOZOOOOOOOOOOOOOOOOO
?I?IIIIIIII??IIIIIIIIIIII77I77.7777777$$$$$$$$$$$777$$$Z$$$$$$$ZZZZZZZZZZZZZZZZZZZZOZZZZZZZZZZZZZZZZZZZZZZZZZZZZOZZOOOOOOOOOOOOOOOOOOOOOOOOO
????IIIIII?IIIIIII??IIIIIII77..I77777777777$$$$$$7$$$$$$$$$$ZZ$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
????II??II??????I??IIIIIIIIII.:II77777I77777$$$7$$$$$$$$77$Z$ZZZZZZZZZZZ$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZOOOOOOOOOOOOOOOOOOZZZZZOO
????????IIII?????????IIIIIII..IIII77III777777$$$$$$$$$$$$$$$$$ZZZZZZZZZZZZZZZZZZZZZZZZZZZ$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZOZZZZZZZ
??+??????IIIII????????????II..IIIIIIIII77777$$$777$$$7$$$$$$$$$ZZZZZZZZZZZZZZZZZZZZZZZZZZZ$ZZZZ$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
?+++???++?III?????????+??II..?IIIIIIIII77777$$77$$$777$$$$$$$$$$$$ZZZZZZ$$$$ZZZZZZZZZZZZZ$$$Z$$ZZZZZ$ZZ$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
??+++=====?IIII???+????????..??II??III7I777$$$$77$$7$$777$$$$$$$$$$ZZZZZ$ZZ$$$$$ZZZZZZZZZZZZZZ$$$$$$$$$$$ZZZZZZZZ$$$$ZZZZZZZZZZZZZZZZZZZZZZZ
++??+++++++??+????+?+??+??=:???????III7777777$$$$$$$$$$$$$$$$$$$$$$$$$$ZZZZZZ$$$$ZZZZZZZZZZZZZZZZZ$$$$$$$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ$$
++++++++??++?+???+++++++?,.I?????IIIII77777777$$$$$$$$$$$$$$$$$$$$,$$$$$$$$$$$ZZZZZZZZZZZZZZZZZZZ$$$$$$$$$$$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
+++++++++++?????++,   ..+$,=~???IIIIII7777777777$$$$$$$$$$$$$$$$:~$$$$$$$$$$$$$Z$ZZZZZZZZZ$$$$$$$$$$$$$$$$$$ZZZZZZZZZZZ$ZZZZZZZ$$ZZZZZZZ$ZZZ
++++++++++???: ..?I+++?+??I.Z+=?IIIII77I7~=7777777$$$$$$$$$$$7.:$$$$$$$$$$$$$$$$$$ZZ$$$Z$$$$$$$$$$$$$$Z$$ZZZZZZZ$ZZZZZZZZZZZZZZZZZZ$Z$ZZZZZZ
++++=+==+.,I+???++++=+++???~.?IIIIIIII7II777, .777777777$$$$.:$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$7777$$$7$$$$$ZZZZZZZZZZZZZ$ZZZZ$ZZZZZ$$$$$ZZZZZ
++=+++==+++++??++++++++++??I~.IIIIIIIIII7I77777...77777777:::7$$$$,$$$$$$$$$$$$$$$$$$$$$77777$7$$$$$$7$$$$$ZZ$ZZZ$ZZZZZZZZZ$$$$$$$$ZZ$$$$$$$
~=======++++++++++++++++???+I~,~IIIIIIIIII77I77777. .777:::77777$$7.$$$$$$$$$$$$$$$$$$$$777$$$$7$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
~~==~~:~~====++++=++===++??.III~.IIII??IIIIIIIIIIIIII..I=77777777777 7$$$$$$$$$7$777777+777$7777$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
~~~==~~~======++++++++++???.?III=.III?IIIIIIIIIIIIIIII:?+:=IIIIII7777 $$7$777777777$$$$.77777$77$77$$$$$$$$$$$$$$$$77$$$$$$$$$$$$$$$$$$$$$$$
~~~~~~~~~======+++++++?????.+IIII?,I?IIIIIIIIIIIIIIII~II7IIIIIII77777I 777777777$7777$$ 7777$$$77$7$$$$$$$$$$$$$$$$77777777777777$$7$$$$77$$
~~=~====~======+=+++++????+.+??????=I????I???IIIIIIII.I.=IIIII77777777+ 7777777777$$777.777$$$$$77$7$$7$$777777I777I77777IIII777777777777777
=~=~=====~=~~====+=+++????+.+???????+????????IIIIIIII.I.~??IIII77I77II7,777777777I+~~~7.7777777777777777,777777777IIIII7IIIII77I77III7777777
====================++++??=.=???????II???IIIIIIIIIIII.I.~??IIIIIIIIIIII7+??+==+777I7777.77777777777777I.77777IIIIIIIIIIIIIIIIIIIII7777777777
~===================++++??~.=????????II????IIIIIIIII?.I.~III?IIIIIIII?:~7IZIIII7777777I.77II77I7777777,777IIIIIIIIIIIIIIIIIIIII7I77777777777
~===========++++=====+++++~.~?????????????+??IIIIIII?:I.:III????II?II,=?=:??IIIIIIIIII+~777777777777I.?IIIIIIIIIII=IIIIIIIIIII77IIIIIIIIIIII
~~~============+++++==++==:.~+????????????????IIIIII?+I.,IIII???????:=??=,??IIIIIIIIII..:77777IIIIII.+I.IIIIIII77I~IIIIII77777IIIIIIIII77I77
~~~~~~~~~~~=========+++===:.~++++++????????++???IIIII?I.,?IIIII?????.++?~,???IIIII=.=III.777IIIIIII~:III IIIIIIIII~IIIIIIIIIII~777777I77777I
~~~~~:::::~~~~~~~=====+++=:.:==+=========+++???????IIII.,IIIII??+++.????~.?IIIII.?IIIIII7+.7I7777I7:II777~,7777777:~777777777,7777777777II77
~~~~~~~~~~~~~~~~==========,.:==+++=+======++==+++??????.,?????????,???I?:.III~?IIII7III.I7II=,IIIII.7777777.:~=?77=777777777,7777I7III777777
~~~~~~~~~~~~~~~~~~=~~=====,.:=======+++++++++++++++++++.,???????????????:.IIIIIIIIIIIII.777IIII=7I7=I77777+$$77777+77777777,7777I77777777777
~~~~~~~~~~~~~========~~===,.:++==+===+++++++++++?++++++..++++++++???????,.????IIIIIIIII.IIII777777II,7777+?=7777.I7I,?+?77~777777777777777II
~~~~~~~~~~~~~~~~~=========,.:======++++++++++++++++++++..++++=++++++++++,.????????????I.II???????I???:???.?=I~+7777777+:77$77777777777777777
~~~~~~~~~~~~~~~~~=========,.,=======++++++++?++++++++++..++++++++++++++?,.????++?+?????.???I???????.I?II+II~?777777=I7777?,7777777777777I77I
::~~~~~~~=~===~~==========,.,++=+++++++++++++++++++++?+..+???????++?????,.?????????????.???????????.II?I~II~IIIIIII~IIIIII?~IIIIIIIIIIIIIIII
~~~~~~~=~==============++=,.,++++++++++++++++++++++++++..?????+++?+?????,.?????????????.???????????.?????II:?IIIIII~IIIIIIIIIIIII77IIIIIIIII
~~~~~~~===================,.,=======+++++++++++++++++++..+++++++++++??+?,.+????????????.???????I?II.IIIIIII:+IIIIII:IIIIII=I?IIIIIIIIIIIIIII
~~~~~~~~~=~===============,.,==++++++++++++++++++++++++..++??+??????????,.????????????I.IIIIIIIIIII.IIIIIII:=IIIIII,IIIIII~IIIIIIIIII??IIIII
~~~~~~~~~=================,.,===++++++++++++++++++++???..???????????????,.??????I?IIII?.????????III.IIIIIII,=IIIII?.IIIIII:IIIIIIIIIIIIIIIII
~~~~~~~~~=~===============,.,+++++++++++++++++++++++?++..+++++++++??????,.???????I?????.???????IIII.IIIIIII,~IIIII?.IIIII?,IIIIIIIIIIIIIIIII
~~~~~~~~~~================..,+++++++++++++++??++??????+..???????????????,.???????IIIII?.IIIIIIIIIII.IIIIIII,:IIIII?.IIIII?,IIIIIIIIIIIIIIIII
~~~~~~~~~~================..,===+=++++++++++??????????+..???????????????,.?IIIIIIIIIII+.IIIIIIIIIII.IIIIIII,,IIIII+.IIIII?.IIIIIIIIIIIIIIIII
~~~~~~~~~~~===============..,++++++++++++++++++???????=..???????????????..??IIIIIIIIII+.?IIIIIIIIII.IIIIIII,,IIIII+.IIIII+.IIIIIIIIIIIIIIIII
~~~~~~~~~~~=============++..,++++++++++++++???????????=..???????????????,.??IIIIIIIIII+.?IIIIIIIII?.IIIIIII,,IIIII=.IIIII+.IIIIIIIIIIIIIIIII
~~~~~~~~~~==============++..,++++++++++++?????????????~..????????????III..IIIIIIIIIIII=.+IIIIIIIII?.?IIIIII,.IIIII=.IIIII+.IIIII7IIIIIIIIIII
~~~~===============+++++++..,++++++++?????????????????~..??IIIIIIIIIIIII..IIIIIIIIIIII=.+IIIIIIIII+.?IIIIII,.IIIII~.IIIII=.IIIIIIIIIIIIIIIII
ZZZ$$ZZZ$$$$$$$Z$$$$$$$$$?..,$$$$$$$$$$$$$$$$7$$$$$$$$:..7$$$$$$$$$$$$$$..7$$$$$$$$$$$~.=$$$$$$$$$+.?$Z$$$$,.$$$Z$~.7$$$7?+7$$$$$$$$$$$$$$$$
ZZZZZZZZZZZZZZZZZZZZZZZZZ+..,$ZZZ$ZZ$Z$$Z$$$$Z$$$$$$Z$:..7$$$$$$$$ZZZ$8O..7Z$$$$$$$$ZZ~.~$$$$$$$$7?+I7$$$77II7I777777$$$$77$$$$$$$$$Z$ZZ$ZZZ
OOOOZZOZZZZZZZZZZZOOZZZZZ=..,$Z8DOOO8OOZZZZZZZZZZZZZZZ:..7ZZZZZ$ZZZ$$$$II7$7$$$$$$$$$$7$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ZZZZZZZZZZZZZZZZZZZ
OOOOZOOOOZOOOOOOZZOOOOZZZ7~:~7ZZZZZOZZZZZZZZZZ$ZZZ$ZZ$7I$$7$Z$$ZZZZZ$Z$7$$Z$$Z$$$$$$$ZZ$$$$$$Z$Z$ZZ$$ZZ$ZZ$$$$Z$ZZZZZZ$ZZZZZZZZZZZZO$ZOOOZOO
OOOOO8OOOOOOOOZZOOZZOO$ZZ$77I7$7ZZOOZZOZZZZZZOZZZZZ$$Z$$ZZZZZZZZZZZZZ$ZZ$ZZ$ZZZZZZZZZZZZZZZZZ$ZZZZZZZZZZZZZZZZZZZZZOZZZZ$$OOOOOOOOZOOOOOOOOO
OOOOO888O$OOOOOOOOOOOOZZ$ZZ$$$ZZZOOZZZZOOOOZZZZZZZZ$ZZ$$$Z$$ZZZZZZZZZZZZZ$ZZZZZZZZZZZZZOOOZOOOZZZZZZZZZZZZOZOOOZOOZOOOOOOOOOOOOOOOOOOOOOOOOO
888OOOOOOOOOOOOOOOOZOOOOZZ$$$ZZZZZZOOOOOOOOZZOZZZOZOZZZZZOZZZZZZZOZZOZOZZZZZZZZZZZOZZZZZZOOZOOZOOOZZOOZZOZOOOOOOOOOOO8OOOOOOOOOOOOOOOOOO88O8
88O8O8OOOOO8OOOOOOOOZZOOZOZOOZZZOO8OOOOOOOOOOOOZOZOOOZZZOZZZZZZZOOOZZZZZZZZZZZZZZOZZOOZOZOOOOOZZOOOOOOOOOOOOOOOOOOOOOOOOOOOOZ88888OOOO88O8OO
8888888OOOOOOOOOOOOOOZOZOOZOOZZZZZZOOOOOOOZOOZOZZZZZZOZOZOOZZZOZZOOOOOOOOZZZZZOZZZZOZOOZOOZOOOOOOOOOOOOOZOOOOOOOO8OOOOOOOOO88O8OO88O88D88888
8888888888888OOOOOOOOOOOOOOZOO8ZOOOO8O8OOOOOOZZOZZZOOZZZZZZZZOOZZZOZOOZOZZOZOOOOOOZOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOZZOOOOOZOOOOOOO88888
*/
