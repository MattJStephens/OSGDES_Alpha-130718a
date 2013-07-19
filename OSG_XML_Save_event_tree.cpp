
#include "OSG_DES.h"
#include "OSG_XML_Save_event_tree.h"
#include "XMLDOM_Helper.h"
#include "OSG_XML_Writer.h"


void XML_Save_event_tree(std::string File_name,  CEventCrysalis* p_event)
{
	CoInitialize(NULL);

	HRESULT hr = S_OK;
    IXMLDOMDocument *pXMLDom = NULL;
    IXMLDOMElement *pRoot = NULL;
    IXMLDOMElement *pXMLDomNode = NULL; 
    IXMLDOMDocumentFragment *pDF = NULL;

    BSTR bstrXML = NULL;
    VARIANT varFileName;
    VariantInit(&varFileName);

    CHK_HR(CreateAndInitDOM(&pXMLDom));

    // Create a processing instruction element.
    CHK_HR(CreateAndAddPINode(pXMLDom, L"xml", L"version='1.0'"));

    // Create a comment element.
    CHK_HR(CreateAndAddCommentNode(pXMLDom, L"mjs OpenSceneGraph Extensible Markup Language Process Description Script (OSG XML PDS)"));   

    // Create the root element.
    CHK_HR(CreateElement(pXMLDom, L"Event", &pRoot));	
	XML_Write_Event(pXMLDom, pRoot, p_event);

	SAFE_RELEASE(pXMLDomNode);

	CHK_HR(AppendChildToParent(pRoot, pXMLDom));

    CHK_HR(pXMLDom->get_xml(&bstrXML));
    printf("Dynamically created DOM:\n%S\n", bstrXML);

	BSTR bsFileName;
	bsFileName = _bstr_t (File_name.c_str());
	//File_name
    CHK_HR(VariantFromString(bsFileName, varFileName));
    //CHK_HR(VariantFromString(L"dynamDOM.xml", varFileName));
    CHK_HR(pXMLDom->save(varFileName));
    printf("DOM saved to dynamDOM.xml\n");

CleanUp:
    SAFE_RELEASE(pXMLDom);
    SAFE_RELEASE(pRoot);
    SAFE_RELEASE(pXMLDomNode);
    SAFE_RELEASE(pDF);   
    SysFreeString(bstrXML);
    VariantClear(&varFileName);
	
	// Stop COM
	CoUninitialize();

	//return grp;

}

//EventType_AlterOrientation, 
//	EventType_AlterPosition, 
//	EventType_AlterText, 
//	EventType_BoostASIO, 
//	EventType_CreatePhysXActor, 	
//	EventType_CreateProcess, 	
//	EventType_CreateEntity, 	
//	EventType_DE_Logistics, 
//	EventType_DiscreteEventSimulation,
//	EventType_Empty, 	
//	EventType_Emitter, 
//	EventType_MoveGlobal, 	
//	EventType_Render, 
//	EventType_ReplaceDrawable,
//	EventType_Pipe, 
//	EventType_SaveEventTree, 
//	EventType_SaveEnvironmentTree, 
//	EventType_Sequencer, 	
//	EventType_Sensor,
//	EventType_SimTimeSortChildren, 
//	EventType_Source,
//	EventType_ScreenGrab,
//	EventType_TheatreOfOperations, 	
//	EventType_UpdateGraph, 
//	EventType_UpdateNxActor, 	
//	EventType_UpdatePositionParametricExpression, 		
//	EventType_UpdateSimulationTime, 	
//	EventType_UpdateText, 
//	EventType_UpdateViewer,
//	EventType_UpdateViewerAnimate, 
//	EventType_UpdateVelocity	


void XML_Write_Event_SaveEventTree(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CEventCrysalis* p_event)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	std::string expression;
	std::size_t climax_expression_index;
	CEvent_SaveEventTree* p_event_morph = dynamic_cast<CEvent_SaveEventTree*>(p_event->mp_event_nymph);	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"FileName", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_event_morph->m_file_name);		
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"ClimaxCondition", L"\n\t", pParentNode, &pSubNode));		
	climax_expression_index = p_event->mp_event_nymph->m_climax_expression_index;
	expression = p_event->mp_event_nymph->m_climax_condition_expression_list.mv_expression[climax_expression_index].get_expression();
	XMLSaveString(pXMLDom, pSubNode, expression);		
	/*if(p_event_morph->m_self_destruct)
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Climax", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "Climax");	
	}*/
	
CleanUp:	
	 SAFE_RELEASE(pSubNode);	
}

void XML_Write_Event_SaveEnvironmentTree(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CEventCrysalis* p_event)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	std::string expression;
	std::size_t climax_expression_index;
	CEvent_SaveEnvironmentTree* p_event_morph = dynamic_cast<CEvent_SaveEnvironmentTree*>(p_event->mp_event_nymph);	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"FileName", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_event_morph->m_file_name);		
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"ClimaxCondition", L"\n\t", pParentNode, &pSubNode));		
	climax_expression_index = p_event->mp_event_nymph->m_climax_expression_index;
	expression = p_event->mp_event_nymph->m_climax_condition_expression_list.mv_expression[climax_expression_index].get_expression();
	XMLSaveString(pXMLDom, pSubNode, expression);		
	/*if(p_event_morph->m_self_destruct)
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Climax", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "Climax");	
	}*/
	
CleanUp:	
	 SAFE_RELEASE(pSubNode);	
}

void XML_Write_Event(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CEventCrysalis* p_event)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	std::vector<CEventCrysalis>::iterator event_crysalis_iter;
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_event->mp_event_nymph->m_name);	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Type", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, event_type_to_string(p_event->mp_event_nymph->m_type));		
	if(p_event->mp_event_nymph->m_type == EventType_SaveEventTree) { XML_Write_Event_SaveEventTree(pXMLDom, pParentNode, p_event);	}
	else if(p_event->mp_event_nymph->m_type == EventType_SaveEnvironmentTree) { XML_Write_Event_SaveEnvironmentTree(pXMLDom, pParentNode, p_event);	}
	for(event_crysalis_iter= p_event->mv_child.begin(); event_crysalis_iter< p_event->mv_child.end(); ++event_crysalis_iter) {	
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Event", L"\n\t", pParentNode, &pSubNode));		
		XML_Write_Event(pXMLDom, pSubNode,  &(*event_crysalis_iter));		
	}		

CleanUp:	
	 SAFE_RELEASE(pSubNode);	
}


/*
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM+:,:,::::::::=7NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNO?::::,:,::+I$$I??=~::::=+?ZDNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN$+:::,,:,,,::::::=77Z$Z$ZOD8Z7+::::,::7ONMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7~::::::::::::::::::::::::::=IOO$OO+:,,::,,,,,,?MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMO~:::::::+ZZ~::,:~:~::::::::,,,::,:,:::,::,,:,:::::::=MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMZ=,::??::::~:,,::+OO8$???+~::::,:::::::::::::::,,,::::,,,:ZMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMM::,:+I=,,,:,,,,,,,788Z7I++?+::::::::::::::::::::,::::~+:,,,::ZMMMMMMMMMMMMMMMMMMMMNMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMM=:,,:I~::,:,:::,::,::~OOZI===:::::::+I=:::::::::::::::::::~,,,,:::+NMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMI~~~::~:::::::::::::,:,,,::~:::,::::::::::::::::~~~::::::~:,,,,::::,,::MMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMN::+,::,::~~=~:::+ZOOOZ+:::::::::::~???+~::::::::7NNN8OZ+::::::::::,:=+:,:?DMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMZ::+~,,:::~?~:::+OZZOOD88O:::::~~:::~$ZZZ$Z=:::::~NMNDMD8OZO+::::::::::::~:::OMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMM+::::::::::::::~$Z$$$OOZOO87::::~::::::::::::~::~:~DDDNDNDDO8O8OI:::::::::::::::$MMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMM:::::::::~~~::::7Z$$$$OOO8ZZ+:::~~~:~::::::~:~~:~~~::ZND8DDD88O8DDN=:::::::::::::,,8MMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMD::,,:,,::78ZZ:::,+$$$IIZ8OZI~:::~~:+7$$ZO$~:::~::~::::~,$8DDDDDD8DDNN?:::~:::~:::,,,,7MMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMO,:,:,,,::+88OI:~:::::::::~~:::::~:?OZ$ZOO8OO+,::+ZZ::~:::::IZ8ND88O8O=::~~~::I8N?::,,:,?MMMMMMMMMMMMMMMN
MMMMMMMMMMMMMM8,,,,,::,::$8O+:~::~:::::::::::::~~:=OOOOOD8OZZ=:,ZZ7~~+=::~::::~+I?~:::~~~~~~:~~~+~::,,::~MMMMMMMMMMMMMMN
MMMMMMMMMMMMM7,,::,:~,:::7?,:::::::::+Z$Z8O?,:~~=~:~Z8OOZOOZZ?:,=77=?Z$+~::~:::~:::~~~==~~~:~::~=~=::::,,~MMMMMMMMMMMMMN
MMMMMMMMMMMMI,:::,,,:,::::~:=7Z8O+:::I$$$ZI:~~~~=~~~~~IDMO8$$~:::::?I+:::::::~:::~~~~~:::::~~:::~+==:,::::~MMMMMMMMMMMMN
MMMMMMMMMMMD:::+:,,:,,:::::7OZOOO$~:::+I+::~~~~~~=~~~~::~?I7=:~=~~~:~~::~?7I+=~::~~:::~~~~~:::::~++=~::::,:~MMMMMMMMMMMN
MMMMMMMMMMN:::=::::~?7::::::?7ZZZ$::~::::~~:~~~:ZND8$I=~:::,:~~~~~=~=~:$OI+III?~:::::~:~=~=~~:::::++~:,::?::~MMMMMMMMMMN
MMMMMMMMMM~::=~,:?Z8O7:~::~::::::,:::~~~~:::::~~~~ZOOOZ?:::::~~~~~==~~7O8I?I7III:::~::~:~=~~~~::::::,::,:+DZ:~NMMMMMMMMN
MMMMMMMMM?,:::::$O8Z$:::I:::::::::::::~:~:::::~=~=~::::~~::::::::~~~~~8O777????I?::+~=~:~~=~~~~:::::::::~:ODD:,OMMMMMMMN
MMMMMMMMN::::::7OZOZ~:::$+:::::::::::~~:~::::~~===~=~~~=::+IZ8887::~=~$Z$$OZOZI?+::=:~~~::~~~~==~::::::::~=DD$::MMMMMMMN
MMMMMMMM~,,::::OOOZ+::~:+$::::::::+Z88OI:~:~~~=~=~~~~~~~:88O88DOOO?~~~ZO88OOOZOZI::~~~:===:~=~=??~:::::::~:$DNI:?MMMMMMN
MMMMMMM7:~,,::ZOO$~::::::::::~::+888DDDZ+:~~~~~::~=:~~~:7D88D8DNDOZ~~~IO8OOOZOZO=:~:=~:~++~=~=++?+:::::::::~DDNI,8MMMMMN
MMMMMMM,:::::+$$=:::::::~~::~~~Z8O88D88ZI:~:~++:~IONO:~:IO8D8DDDND8$:~~DOOZ8Z$$Z::~+~==~~~++~=====:::::~:,::?DNN,=MMMMMN
MMMMMMO,:::::~:::::::?O7~::~::7O888O88OO=:~~IZ7=~ODD8D~~~DND88D8DNDO:~~$8OZZZZ$7:~:7?=+:~====~~~~~::::::~::::O8N::NMMMMN
MMMMMM:::+:::::~+:::$OO8Z::~~=OOODO88O8I:~~+OZ$I~78DD88:~ZD888D88D8O:~=:ZZZOOZ$=:~~Z+=++~:~~::::::,::::~D7:,,~8D~,ZMMMMN
MMMMMO,:=+:,:::+:::+ZOOZZ::~::ZO8ODOOZ?::~~78OZ$?I8DNDDZ::O8D8DNDD8?~==~:=ZZZ7::~:=Z~=++=++=+==~:==::::7N8,,:,=Z:,7MMMMN
MMMMM,::::,::::,:::7OOZZ=:::::~7OO$?=::~~=~7OOZ$?78DMMND$::$DDD8D8O:==+=~~:::::::~=+?$7$7+===+?8NMD+~~:+8=:,:,,,,,ZMMMMN
MMMMZ:~::::::::::::?OO$=::::::::::::::~~:~:+OZ$$=ZO8MNMD8:::Z88N8$:~+=+=====~~~~~~:?7I?7$Z7~=?ODNDDI::::,,::::,,,,ZMMMMN
MMMM~:?::,::::~$=,:~Z7:::~::::::::~~:$8O~~~~$Z$==OOMMNNN8+::::~+:::=++====~=~~=~~~:7IIII7$ZI~7N8D88+~~:::::::,:,,,8MMMMN
MMM8::I::::=:,,:::,:,:::~~::::::::::+8888=~~~7~~$DDMDNNDO=::~~~~~~=~==~=~~~~~~~~=~::+?I$7I$$~+D88ZO:~~:~:=8Z=,,,,,DMMMMN
MMM+,,::::~$Z+:::::::~::~I$7+~:::,~~:7ZOO~~~~::=ZD8DDDDZ7::~=~=====~~=ZND8D8$~:~~~~::?7Z777I==8N8O=:~:::,7DO~:,:,,OMMMMN
MMM+::::::~ZZZ+,::::::~~?O8OOD$~:::~~~~::~====~~?8D8OZI::~~~~==~=~~=8D8D8D88D887~~~~::+$777I+=Z8O::~~::::,:,:,,,,,7MMMMN
MMN?::::::::7$=::::::::~=OOO88O$~::~~~~~~~~======~~~::::~~:~:~~~~~~8DDNDDDDD888O++::~~:~I$II==I7:::~~:~,:,,,:,,,,,+MMMMN
MMMN::::::~:::,:::I~::~::=ZO8OZ$::::::::~:~~~=+?+++=~~:+NNDZ+:::::88DDN8NNMDDDDO=II?:==~::::::::~~~~:::,:,:7$::,,,~MMMMN
MMMM:::::~=OO$::::Z=::7I::::~=::::::~~~?NNNO~~~=+++=~=8NDD8OO8~::?NNDDDDDNND888$I77?+:~=====~~~~~~~~::,:::INN:,,,,:MMMMN
MMMM~:::~:~$DOO:::$=::~ZO~::~~:~~$::~~INNMMNNO=~=++=+O88D8DOO8O::7NDNN8DDDDND8ZI7$7?+~:~====~::::::~:,,:::$NN::::,=NMMMN
MMMMI,:~~~:+88Z=::+~:::?8$:~~~~~8D~::=8NMNNMNOI=~==~DDOO8888O8O::+DDMN8DNDDDDOI7$Z$I?=:~===~~=~::::::,::,:?D$::~~:ZMMMMN
MMMMZ,:::7::$ZZ?::::~~~:+$,~~~=OOO:::?ODNNNDNO$II~=~OM8ZOOOOZZZ:~=8NDNDND8DDZ7$Z$7Z7I~::==~+7+~::::::::,:::I~:~D:~NMMMMN
MMMMM~:::?~::I$,::::::~~:,:~~~ZOO+:::?ODD8MNN8$77=~~=DOZOZ8ZOO$:~~8DNNNNDDO7ZZ7?7$I?+~:~~~IOOO8?:::::::ZO::,::ZO:DNMMMMN
MMMMMO::::::::::::::::::::~~~~$I~::~:=8D8DNDDN8$II:~~?Z7ZOZ$$$~:~:ZOOOOOZZOZZ$I?7$I+~::~~:88OO88?:::::::::::::8=?MMMMMMN
MMMMMMZ::::::::::::::::~:~~~~~::::~~~~$8DDNDDNNZ7I~==~:IZZZ$+:::~~+O88888OO8$??7$7~=~:~~~:8DOOO8Z,:::::~::::::+:NMMMMMMN
MMMMMMM::::::::~~:,:::::::~:~~::~~~~~~=ZDNNDMMND8I~=+=~~:::::~~~~~~DN8$$8OO8$?III~=~:~===~Z8ODO8$::::::=$?:,:::+MMMMMMMN
MMMMMMMM~::~~8O$=~~~$+:::~:~~~~:::::~~~~$DNMMNND8?~=+===~~=~~~=+===~=8Z$Z$ZZ77I===:~~====~:ODDDO=::~:?MMM?,:::~NMMMMMMMN
MMMMMMMMN::~=ZODNZ~~=$~::~:~~~~:::$=:~=~::?ONDDDO:~======~::::~=~=+==~~~?II?+=~::~~~~~~~~:~:788$::~:DNMND,,:::DMMMMMMMMN
MMMMMMMMMI:~~ZOND8O=~::~:~:~:~~=~~~~:~~~~~::~=++~~~=~~::$DDDD8I~~~===+~~~~~:~~~==~:IODND?:~::::::~:?NNMMI,,:,:DMMMMMMMMN
MMMMMMMMMM=~~?OODD8Z=~~~::::::::~~::~~7?=:~~~~~:~==~~~:=8D88DDNNZ,~==++++=====~=~:8ODNNNN$~~~:~::~:?D88~,::::$MMMMMMMMMN
MMMMMMMMMMDZ~~+8O8D88=~=~~~~===:~~~:::=ZZO?::::~=~~=~=~~7D8DDDDMNZ:~=+=+~~~~~~=~:88DD8DD87~:::::~:::~~:,::::=MMMMMMMMMMN
MMMMMMMMMMMO~:~?O8DDD8~::=~DNDDDO:~~~~~~:::~~:~~~~D$~~:::O8888DN8DI~==~~~~~~:~~~:DDNDN88$:::IZ:::::,,,::I:,~MMMMMMMMMMMN
MMMMMMMMMMMM7:::?OO8N8I~:~=D8D8D8?:~~~~~~~=~~~==~INN8=::::+$Z8888Z?~~:~~=~:~~:~~~+DMD8D7:~:~$=::::::,:=I:::8MMMMMMMMMMMN
MMMMMMMMMMMMM~:::=ZOOOO::~:IO8D8O7::~:~:::~~~~~==?NNO$::~::::::::::~~~$8DO~:~::~~~:==~:~~~:::::::~:,::~:,:ZMMMMMMMMMMMMN
MMMMMMMMMMMMMMI::::~II~::~~~~?OZ7:~~~Z+~~~~7+:~~=~ZDO+:~~:~:::::~~:~:~7O$:::~~:~=+=~~~~~::::::~::=:::::,:$MMMMMMMMMMMMMN
MMMMMMMMMMMMMMMD~::::::::~::::::~~:?OZ?:~~:~IOZ~==~+:::~::=++?I?~:~~:~:::~~~~:~===~~:::::,::::::::::~7=:+MMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMM7::::::::~::::~~~:~~::~~~~:~:~==:~~~:~III7$ZZZI::~:::::~::~+~~=~~:::,:::,::?::::::7?::OMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMD:::::~:~~:~:::::~~::=III+=::::~~:~~~=++???=,:~~:$=::::~7I~~~~:?OO?::::::::::::::::~MMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMI::::~~~~:~I:::~~::$ZOOO8Z+:~~~~::~~~:::::~~~~~~+:::~$:~~~=:ODOZ~:::::~~:::::::+MMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMM=::~~:::~~::::~::~$ZOO8Z8=~~~:~:~~~:::::::~~=~:~~~~:~~~~:~O$:,::~ZDD+::::::7MMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMM=:::~~ZO7,:::::~:~I$Z$?:~~~::~~~~~IIIII$~:~~~=~~~::::::::::=78D8+:::?:~+MMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMM7~::~~::::==:~::::::::~~~:~~~~=~+II??+::::~~:::::~=::::::$Z$7~:::~I::ZMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMM8=:::::::~+~::::~:~:~~~~~~~~~~:::::::::::7O7=:::::::::::~:~::::~~IMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMZ~:::::::::::::~7ODDNDNNDZ=~~::::::::OODDD=:::::::~::I$Z~:::~MMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7~::~~~:~::~ODDMNNNNNNN8OZ$~::::::7Z8DO::::::~~:~77=::::+MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8+:~~~::::~$DNNNMNDNNNO8$I?~::::::~~::::::::::::::::~8MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMD?~~~::::~:+7Z8OD8DOZ$I??:,::::::::::,~$OO7::::~$MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8+=~~::~~:::::~=~~~:,::::~?O8D8I::::::::7MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMDZ+:~:::::::::::::::+I7Z7=:::::+DMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM88OZ$?=~:::::::::::~:~ZDMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8O$$$I??7NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMOZ
*/
