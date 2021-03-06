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
#include "OSG_Default.h"
#include "OSG_XML_Parser_Tools.h"

using namespace std;

CScript::CScript()
{
	//m_str_environment_file_name= "Environment.xml";
	//m_str_event_file_name= "Event.xml";	
}

bool CScript::XML_Parse(CComPtr<IXMLDOMNode> spXMLNode)
{
	bool bA = false;
	bool bB = false;
	bool ret = false;
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	for(long i= 0; i< listLength; i++) {		
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL) {			
			if(str_node_name== "EnvironmentFileName") {							
				mv_str_environment_file_name.push_back(str_node_text);
				bA= true;
			}		
			if(str_node_name== "EventFileName")	{							
				mv_str_event_file_name.push_back(str_node_text);
				bB= true;
			}		
		}
	}
	if(bA&&bB) { return true; }
	return false;
}

bool CScript::XML_Parser(string str)
{
	bool ret= false;
	CoInitialize(NULL);
	try {		
		CComPtr<IXMLDOMDocument> spXMLDOM;
		HRESULT hr = spXMLDOM.CoCreateInstance(__uuidof(DOMDocument));
		if ( FAILED(hr) ) throw "Unable to create XML parser object";
		if ( spXMLDOM.p == NULL ) throw "Unable to create XML parser object";		
		VARIANT_BOOL bSuccess = false;
		hr = spXMLDOM->load(CComVariant(str.c_str()),&bSuccess);
		if ( FAILED(hr) ) throw "Unable to load XML document into the parser";
		if ( !bSuccess ) throw "Unable to load XML document into the parser";
		CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
		hr = spXMLDOM->get_childNodes(&spXMLDOMNodeList);
		if ( FAILED(hr) ) throw "Unable to get node list";		
		long listLength;
		hr =spXMLDOMNodeList->get_length(&listLength);
		if ( FAILED(hr) ) throw "Unable to get list length";
		for(long i= 0; i< listLength; i++)	{
			string str_node_name;
			string str_node_text;
			CComPtr<IXMLDOMNode> spXMLNextNode;
			spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
			if(spXMLNextNode!= NULL)	{
				if(str_node_name== "Script")	{
					ret = XML_Parse(spXMLNextNode);
				}
			}			
		}
		std::cout << "Script parsing  complete "  << std::endl << std::endl;		
	} 
	catch(char* lpstrErr) {
		std::cout << lpstrErr << std::endl << std::endl;
		ret = false;
	}
	catch(...) {
		std::cout << "Unknown error..." << std::endl << std::endl;
		ret = false;
	}
	CoUninitialize();
	return ret;
}


/*
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMM$$$$$OOZ$$$$$$OOOZ$77ZNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMZ$$8$$$$$$$777777777777$$$$$ZZ7$NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMM$$O$$$$777777IIII???????II7777777$$$O$$NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMZ$8$$$7777I??O8ONMNNNNNNMNNNND8O$??I7777$$$Z78MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMM$$$$$777?I8DNN$+++++++++++++++++++=?8MN8O?I77777$ZZMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMM$O$$$77I?8MNI++++++++++++++++++++++?++++++?NM8$?777$7Z7DMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMZ$$$$77?O8N7+++++++++?????????????????+++++++++=$M8ZI777$$7OMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MM$Z$$77?ONN++++++????????????IIIIII???????????++++++=OM8?777$77OMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MD$$$$7I?8N?++++++??????I77I7$77777777777I77I??????+++++++MDII77$$7NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MZ$$$77IONI+++++?????I7I777I8D8DNNNNNDNDDDDZI7777II????+++++=NDII777$7MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MO$$$77?ON+++++?????777788NDNDNDDNNNNNNNNNDNNNNND87777I?????+?++?8O?77$$7MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MD$$$77I88+++++????777IDNNDNNNNNNNNNNNMNNNNNNDOOZZZ$$$777II????++++$D7I777Z8MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MM$$$77I8O++?+????I7778NNNNNNNNMMMMMMMMMMMM8OZZZ$$$Z$$$$$$777I???++++=Z8?77$Z7MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MM$Z$77?OD++?+????777DNNNNNNNMNMMMMMMMMMMOZ$$$7777I7777$$$77$77II???++++ID$I7$$7MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMZ$$7I8N=+?+????77I8DDNNNMMMMMMMMMMMMOZZ$$777777IIIIII77777$7$I77????+++=DO?777$NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMM$$$77IN++++????77IDNNNNNNMMMMMMMMMMOZ$$7777IIIIIIIIII???I7777778I7I???++++OO?77$ZNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMN$$$7ION++++???7$IDNNNNNMMMMMMMMM8ZZ$77777IIIIII????????????II77778I7I???+?++D7I7$Z$MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMM$O$77$MI+++???77I8NNNNNMMMMMMMM8Z$$777IIIIII????????+?+++????II77777I77???+++=NOI7$Z$MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMM$$$7I8N++?+??I77DDDNNNMMMMMMM8Z$777IIII??????????????+++++++???II7777Z77??++?+=N7I7$OOMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMM$$$7IO$+++???777DNNNNMMMMMMDZ$777III????????++++++++++++++++++??II7777Z77???+?++N?77$ZOMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMN$777?M++?+???77DDDNNMMMMMMOZ$$77777IIIII?++++++++++++++++++++++??II77I7$7I???+++88?7$$$MMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMN$$$7$M++++???778NDNNMMMMNZZ$7777IIIIIIIII????+++++++++++++++++++?+II777ZZ7I??++?+88I7$Z$MMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMM$$$7IN=+++??I77DNDNNMMM8Z$77IIII???????????????+====+==++++++++++++?I777OZ7I??++++N8I7$O$MMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMM$$77?N=+????I7IDNNNMMMO$$77IIII??????++++????????+======++++++++++??II77$DI7I??+++=NI77$ZMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMM$$$7IN7+?+???7I8DMMMDZ$777IIII??????+??+++++??????++======+++++++++??I7$7O877???+++DDI7$O$MMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMM$$$7?8++++??I77DMMM8Z$77IIII?????????++++++++++?????+=======+++++++??I77$$8I77??+++=N?7$$$MMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMM7Z$7I88+?+???7IMMM8Z$77IIIII???????+++?+++++++++++???+=====++++++++???I7$$DD77???++=N877$OOMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMDO$7IDN++++??IMMM8Z$777IIIII??????++++++++++++++=+????+=====++++++????7$Z$ND77???+?+I8?7$$7MMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMM7$77?NI??+?+MMMNZ$$77IIIII???????++++++++++++=+++++???+===+++++??????I7Z8NDO7I???++=NO7$$$MMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMM8Z$7I8N+++?MMMMO$$777IIII??????????+++++++++++++++++???===+++++????III$DNNND77???+++NOI7$O$MMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMM$$7$?N=+?MMMM8Z$$77IIIII???????????+++++++++++++++????+==++++????III$MNNNND77???+++NOI7$$$MMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMOZ$7I8M+?MMMMZ$$77777IIII????????????++++++++++++++????=++++????III7MMNNDND77???+?+D8I$$Z$MMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMM$$$7?N+MMMMO$$$$77777IIIII?????+??+??+++++++++++++????+++++???II77MMMNNDDD$7???+++O8I7$$$MMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMM$$$7IDMMMMOZ$$$$$777777IIII??????++??+++++++++++++????++???III7MMMMMNNDDO77???+++D8?7$$$MMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMOZ$778MMMNOZZ$$$$$7777777IIII??????+??++++++++++++???+++??II7$MMMMMMNNDDI$7??++++N8I7$Z$MMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMM$$$7OMMMDOZZ$$$$$77777777IIII??????+??++++++++?+??II???III7DMMMMMMNNND877???++++NZI7$Z$MMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMM$7$IMMM8OZZ$7I??7777777777III????+????+++++++????II??II7ZMMMMMMMNNNNDZ$7???+++=MI7$$ODMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM$$7MMMDO$7?=:,=+?I7777777III?????????++++??????IIIII77MMMMMMMMNNNDDD7II???+++NO?7$$$MMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM$$NMMMZ$=,:III?=??I777777III?????????????????IIII77MMMMMMMMMNNNDD8777???+?++N877$Z$MMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM$$NMMO7=,=77$7I:II7777777III???????????????I777MMMMMMMMMMMNNNDD8777???++++DD?7$$O7MMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7ODMN7?=I7Z$$7~7I7777777III??????????IIII777MMMMMMMMMMMMNNDNND777???++++?N777$$$MMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM$O7MD$$Z$$$7I7777$777777IIIIIIII?IIIII7$MMMMMMMMMMMMNNNNNND8777???+++++N8I7$$ZNMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMZ$7O$$777I?7$$$$$77777IIIIIIIIIII7$NMMMMMMMMMMMNNNNDDMNDI$7I???+++++N8I7$$Z7MM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7$$77Z$$$$Z$$$$$$7777II7I77777OMNNNMNMMMNNNNNNNDNNDDD77II????+++++N8I7$$$$MM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM$$$77I$DOOZZZZ$$77777$$$ZNNNNNNNNNNNNNNNNDNNNMDDD$7777?????++++?N8I7$$$$MM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMZ$7$77?OMI=++++?????I7I77I$DDDDNNNDNNNND8DOI7$77I?????++++++NM7I7$$$$MM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7Z7777?OMO=+++++??????I777777777777777$7777I??????++?+++ZN8?77$$Z$MM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7Z$$77?7DN?+++?++??????????IIIIIIIII?????????+++++++?NO7I77$$O$MM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7Z$777I?8NM+++++++++???????????????????++++?+?++NN8?I77$$Z$MMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7$7$777I?8DN$=++++??++++++++++++++++?++++++ZNDD?I77$$$7$MMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMD7O7$777I?I8NNNI=++++++++++++++++++=+ONMOO?I77$$$$$$MMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM$$O$$7777II?IO8DNMMNNNNNNNNNMNO8Z?II777$$$$Z$MMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMO7Z$7$$7777777I??????????II77777$$$$$O$$MMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMZ77O$7$$$$$77777777$7$$$$$$$ZO$$8MMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8$77$ZOZ$$7$$$$Z8O$7$7OMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
*/
