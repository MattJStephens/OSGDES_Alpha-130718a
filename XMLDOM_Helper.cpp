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

#include "XMLDOM_Helper.h"
#include <string>

// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)

// Macro to verify memory allcation.
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)

// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)

// Helper function to create a VT_BSTR variant from a null terminated string. 
HRESULT VariantFromString(PCWSTR wszValue, VARIANT &Variant)
{
    HRESULT hr = S_OK;
    BSTR bstr = SysAllocString(wszValue);
    CHK_ALLOC(bstr);

    V_VT(&Variant)   = VT_BSTR;
    V_BSTR(&Variant) = bstr;

CleanUp:
    return hr;
}

// Helper function to create a DOM instance. 
HRESULT CreateAndInitDOM(IXMLDOMDocument **ppDoc)
{
    HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
    if (SUCCEEDED(hr))
    {
        // these methods should not fail so don't inspect result
        (*ppDoc)->put_async(VARIANT_FALSE);  
        (*ppDoc)->put_validateOnParse(VARIANT_FALSE);
        (*ppDoc)->put_resolveExternals(VARIANT_FALSE);
        (*ppDoc)->put_preserveWhiteSpace(VARIANT_TRUE);
    }
    return hr;
}

// Helper that allocates the BSTR param for the caller.
HRESULT CreateElement(IXMLDOMDocument *pXMLDom, PCWSTR wszName, IXMLDOMElement **ppElement)
{
    HRESULT hr = S_OK;
    *ppElement = NULL;

    BSTR bstrName = SysAllocString(wszName);
    CHK_ALLOC(bstrName);
    CHK_HR(pXMLDom->createElement(bstrName, ppElement));

CleanUp:
    SysFreeString(bstrName);
    return hr;
}

// Helper function to append a child to a parent node.
HRESULT AppendChildToParent(IXMLDOMNode *pChild, IXMLDOMNode *pParent)
{
    HRESULT hr = S_OK;
    IXMLDOMNode *pChildOut = NULL;
    CHK_HR(pParent->appendChild(pChild, &pChildOut));

CleanUp:
    SAFE_RELEASE(pChildOut);
    return hr;
}

// Helper function to create and add a processing instruction to a document node.
HRESULT CreateAndAddPINode(IXMLDOMDocument *pDom, PCWSTR wszTarget, PCWSTR wszData)
{
    HRESULT hr = S_OK;
    IXMLDOMProcessingInstruction *pPI = NULL;

    BSTR bstrTarget = SysAllocString(wszTarget);
    BSTR bstrData = SysAllocString(wszData);
    CHK_ALLOC(bstrTarget && bstrData);

    CHK_HR(pDom->createProcessingInstruction(bstrTarget, bstrData, &pPI));
    CHK_HR(AppendChildToParent(pPI, pDom));

	//CHK_HR(CreateAndAddTextNode(pDom, L"\n\t", pDom));//mjs

CleanUp:
    SAFE_RELEASE(pPI);
    SysFreeString(bstrTarget);
    SysFreeString(bstrData);
    return hr;
}

// Helper function to create and add a comment to a document node.
HRESULT CreateAndAddCommentNode(IXMLDOMDocument *pDom, PCWSTR wszComment)
{
    HRESULT hr = S_OK;
    IXMLDOMComment *pComment = NULL;

    BSTR bstrComment = SysAllocString(wszComment);
    CHK_ALLOC(bstrComment);

    CHK_HR(pDom->createComment(bstrComment, &pComment));
    CHK_HR(AppendChildToParent(pComment, pDom));

CleanUp:
    SAFE_RELEASE(pComment);
    SysFreeString(bstrComment);
    return hr;
}

// Helper function to create and add an attribute to a parent node.
HRESULT CreateAndAddAttributeNode(IXMLDOMDocument *pDom, PCWSTR wszName, PCWSTR wszValue, IXMLDOMElement *pParent)
{
    HRESULT hr = S_OK;
    IXMLDOMAttribute *pAttribute = NULL;
    IXMLDOMAttribute *pAttributeOut = NULL; // Out param that is not used

    BSTR bstrName = NULL;
    VARIANT varValue;
    VariantInit(&varValue);

    bstrName = SysAllocString(wszName);
    CHK_ALLOC(bstrName);
    CHK_HR(VariantFromString(wszValue, varValue));

    CHK_HR(pDom->createAttribute(bstrName, &pAttribute));
    CHK_HR(pAttribute->put_value(varValue));
    CHK_HR(pParent->setAttributeNode(pAttribute, &pAttributeOut));

CleanUp:
    SAFE_RELEASE(pAttribute);
    SAFE_RELEASE(pAttributeOut);
    SysFreeString(bstrName);
    VariantClear(&varValue);
    return hr;
}

// Helper function to create and append a text node to a parent node.
HRESULT CreateAndAddTextNode(IXMLDOMDocument *pDom, PCWSTR wszText, IXMLDOMNode *pParent)
{
    HRESULT hr = S_OK;    
    IXMLDOMText *pText = NULL;

    BSTR bstrText = SysAllocString(wszText);
    CHK_ALLOC(bstrText);

    CHK_HR(pDom->createTextNode(bstrText, &pText));
    CHK_HR(AppendChildToParent(pText, pParent));

CleanUp:
    SAFE_RELEASE(pText);
    SysFreeString(bstrText);
    return hr;
}

// Helper function to create and append a CDATA node to a parent node.
HRESULT CreateAndAddCDATANode(IXMLDOMDocument *pDom, PCWSTR wszCDATA, IXMLDOMNode *pParent)
{
    HRESULT hr = S_OK;
    IXMLDOMCDATASection *pCDATA = NULL;

    BSTR bstrCDATA = SysAllocString(wszCDATA);
    CHK_ALLOC(bstrCDATA);

    CHK_HR(pDom->createCDATASection(bstrCDATA, &pCDATA));
    CHK_HR(AppendChildToParent(pCDATA, pParent));

CleanUp:
    SAFE_RELEASE(pCDATA);
    SysFreeString(bstrCDATA);
    return hr;
}

// Helper function to create and append an element node to a parent node, and pass the newly created
// element node to caller if it wants.
HRESULT CreateAndAddElementNode(IXMLDOMDocument *pDom, PCWSTR wszName, PCWSTR wszNewline, IXMLDOMNode *pParent, IXMLDOMElement **ppElement)
{
    HRESULT hr = S_OK;
    IXMLDOMElement* pElement = NULL;

    CHK_HR(CreateElement(pDom, wszName, &pElement));
    // Add NEWLINE+TAB for identation before this element.
    CHK_HR(CreateAndAddTextNode(pDom, wszNewline, pParent));
    // Append this element to parent.
    CHK_HR(AppendChildToParent(pElement, pParent));

CleanUp:
    if (ppElement)
        *ppElement = pElement;  // Caller is repsonsible to release this element.
    else
        SAFE_RELEASE(pElement); // Caller is not interested on this element, so release it.

    return hr;
}

void XML_Example(std::string File_name)
{

	CoInitialize(NULL);

	HRESULT hr = S_OK;
    IXMLDOMDocument *pXMLDom = NULL;
    IXMLDOMElement *pRoot = NULL;
    IXMLDOMElement *pXMLDomNode = NULL;
    IXMLDOMElement *pSubNode = NULL;
    IXMLDOMDocumentFragment *pDF = NULL;

    BSTR bstrXML = NULL;
    VARIANT varFileName;
    VariantInit(&varFileName);

    CHK_HR(CreateAndInitDOM(&pXMLDom));

    // Create a processing instruction element.
    CHK_HR(CreateAndAddPINode(pXMLDom, L"xml", L"version='1.0'"));

    // Create a comment element.
    CHK_HR(CreateAndAddCommentNode(pXMLDom, L"sample xml file created using XML DOM object."));

    // Create the root element.
    CHK_HR(CreateElement(pXMLDom, L"root", &pRoot));

    // Create an attribute for the <root> element, with name "created" and value "using dom".
    CHK_HR(CreateAndAddAttributeNode(pXMLDom, L"created", L"using dom", pRoot));

    // Next, we will create and add three nodes to the <root> element.
    // Create a <node1> to hold text content.
    CHK_HR(CreateAndAddElementNode(pXMLDom, L"node1", L"\n\t", pRoot, &pXMLDomNode));
    CHK_HR(CreateAndAddTextNode(pXMLDom, L"some character data", pXMLDomNode));
    SAFE_RELEASE(pXMLDomNode);

    // Create a <node2> to hold a CDATA section.
    CHK_HR(CreateAndAddElementNode(pXMLDom, L"node2", L"\n\t", pRoot, &pXMLDomNode));
    CHK_HR(CreateAndAddCDATANode(pXMLDom, L"<some mark-up text>", pXMLDomNode));
    SAFE_RELEASE(pXMLDomNode);

    // Create <node3> to hold a doc fragment with three sub-elements.
    CHK_HR(CreateAndAddElementNode(pXMLDom, L"node3", L"\n\t", pRoot, &pXMLDomNode));

    // Create a document fragment to hold three sub-elements.
    CHK_HR(pXMLDom->createDocumentFragment(&pDF));

    // Create 3 subnodes.
    CHK_HR(CreateAndAddElementNode(pXMLDom, L"subNode1", L"\n\t\t", pDF, NULL));
    CHK_HR(CreateAndAddElementNode(pXMLDom, L"subNode2", L"\n\t\t", pDF, NULL));
    CHK_HR(CreateAndAddElementNode(pXMLDom, L"subNode3", L"\n\t\t", pDF, NULL));
    CHK_HR(CreateAndAddTextNode(pXMLDom, L"\n\t", pDF));

    // Append pDF to <node3>.
    CHK_HR(AppendChildToParent(pDF, pXMLDomNode));
    SAFE_RELEASE(pXMLDomNode);

    // Add NEWLINE for identation before </root>.
    CHK_HR(CreateAndAddTextNode(pXMLDom, L"\n", pRoot));
    // add <root> to document
    CHK_HR(AppendChildToParent(pRoot, pXMLDom));

    CHK_HR(pXMLDom->get_xml(&bstrXML));
    printf("Dynamically created DOM:\n%S\n", bstrXML);

	//File_name
	//BSTR bsFileName;
	//bsFileName = _bstr_t (File_name.c_str());
   // CHK_HR(VariantFromString(bsFileName, varFileName));
    CHK_HR(VariantFromString(L"dynamDOM.xml", varFileName));
    CHK_HR(pXMLDom->save(varFileName));
    printf("DOM saved to dynamDOM.xml\n");

CleanUp:
    SAFE_RELEASE(pXMLDom);
    SAFE_RELEASE(pRoot);
    SAFE_RELEASE(pXMLDomNode);
    SAFE_RELEASE(pDF);
    SAFE_RELEASE(pSubNode);
    SysFreeString(bstrXML);
    VariantClear(&varFileName);

	// Stop COM
	CoUninitialize();

}

/*
..... $???,.~I$7I?=  .   .......   .    ...        ..        ..     ........................................................................
....7:::: ?...::,,.:::   .......   .    ...    ... ..     .. ...    ........................................................................
.... ....+=+++++?                  . .. ... .. ..  .. ... ..     .. ...   ..................................................................
........ $+OI7$I+.                   .. ..  .. ..     ...        .. .. .~... .... ... ......................................................
.......  :~:::::~                    .. ... .. ..     ..     ..~ .. .. ... ....... ..,.......  .............................................
=7~+==+=~~+?+7?=+++:??.=~?    ..          .        ..  ...     .?.. .+...I.~,......,......... :.............................................
. .......   ... .. ......   . ...... ..   . ..   .,.. .......  .... .....~.,.  .=... .......  ? . ..........................................
.....   .       ...                     ... .+. ...... .+~.:Z.. .I.:....:.+~...+..Z. .... ...+. . ..  .......... ........  .....  ..........
..  ... .       ...  .                 .......I .: ...D,.....~...$. ...:.,.....:.:?....... ..+..:~:.........................................
..  ... ...     ...                      :  ..:..O   =..  ,,.+=.:..  ..NO.......:.   ......~..:....   ........   ........  .....  ........  
...........     ..   . ..     .. ... ..   .... O.:...$... ...  .+......,...  ..D.... ......~.:..: ..........................................
....       ....    ... ...   ... :....O...,I.....$,..?....,.... ...  .=..7, . :..... .   ..I... ... ..... .  .....     .... ... ..... .  ...
 ...     ..       ...  ..        ....Z....I.      .M......Z...  .$....+....  .=.~.........$. ..    ...   ............    ..     ..   . .....
 ...    ...       .   .  .,. .~ ...?=.....7....   .I...I..~.....D.....=..~......Z...?.......... ..:.. ... . ,.  .....  .... ... ..... ......
         ..                ..+....O. : . .$..  ,. ..~.  8$   ......   ,.7...:..7Z....~...... ...,I...?....,..... ...=... .......  ........  
        ...     ...... .... .~.. N,.......:....,....+..........Z.................7........8.. .~.....:....++...: .?.............  ..........
 ...                 ....,:,....... .....8=.... ...::,:..D...:7~?::~ .,+.. ,.Z ...I.~......~..? ..,..~....$.: ..., .........................
 ...     ..          . ..,....:7..O..,7ZZ. ...,=. .O..=...=D~ .....  .I....:~. ....N=.   ...:Z.   ....:...Z......... .. ....   ....... .....
 ...              .  ..ZI~..  .D...,?.....  :.    .Z .O...O....=.   .,$.... I. ... .I,......=O....~: .O...NI~...~., ....,.,.................
 ...    ... ..  .......  +.......:...INN....8......+.Z....D..:+.....8? . ....D...,:..N. ....,...... 7..=,D ..,.+ .......=.................  
         ..           .: .       .I....~..~:Z ..  .O......O.. D....D..   . ....=8....., .   +.. ... ..O......~..  ~.. .+... ...    .. ....  
 .       ..     ...  .  +,:   .... .8......~8. O.I.8:....+: . .=.ZM ... ..+...M...  ..D.....N......~. ..7N..$....~....=.......  ............
....     ..     ...   . ......?.~N...:M.........:...N$ ..M ....~=D. ........:~.?..   .N....M:,.?....  ...N.D$I..  ...$.. .:.~~..  ........  
         ..      .,.,. ... .  .....$O.,...  ..:.O.  .M..==..    N..     ..    ,=: ..  O..NO...:87.~?=:,..MN.. ..     ..:....:.....        ..
    .... ..       ..+.  ..... ,$.,7...DO..  ...M..   .NNM.      N..        .. .IN.   .DI.............. ..D....   .. .....,....  .  .. ....  
                ......O,.O......   . ..N....I...M.    ..N~..  ..N......   .  ...8.....M..   ..=....,. ...N    ... .+..........?. ..  .    ..
 ........  ........,.........?Z........M....O....D.......ND. ...N...  ..... ,...O....~7. ~...+.~+.,~. ..N.    ... .~.8=:=+~,...,:..       ..
            ..  . ..=.    ...+.O8...   .N...8D....N...,...+N....N..  .: ..,~  ..Z....D .,Z. .N.........N..... ....~?.  .... ......... ... ..
.   .......    .+ . .:M..,. .....MD......M,...N. ..M.. ....:M..$N.. ............:7..D....:.I.   ... ..M$.........I......  ......   .......  
                  .  . ..Z=... ....ON+,~ZNNN..N. ..IN. .Z...N:.N$.. .Z..~..   ,..N.N..   .D:...     .N..,7ZOZ$$ON. .    ......:.. .       ..
  .....         ......D7.......,~=~:......  ZNN7.. .N..M....N8+M   .Z. O   .. .D.~MN. ....~..=. ....M.... ... ....N:.....Z?.. ....... ... ..
  ..       .....~++.+..........    ....   . ...NM7..NO ,  ..NNM,   ,...7.O+.. ...DZM .  ...O...  .NN.....     . ...+....7..    ......     ..
 .          ..     . . .,.    .. ,....+..........8NMM...   .NN,    N.,I.  .   .  .,M..   ..N...=NNN....   . ..=....:.D:=..,:~:~+,         ..
 ....   ...... .......   $    .........,:,....=N$8NNN.... .IN...,..N.I.. +....?.....M......8NMM:....DN...=D,.............,........ ...   ...
         ..    .,:....ND7ONNI....         . .M.   ..8M....DN.   ..,,Z...?,,=D. .....N~...NNM...........8N=.,:....7..$I.=O=,,~. ....   ......
  ...... ..      ..,I........,MN. ..    ... ...  ....MN..$N7.   ..O.7:.$............~N ?ND..... ... .,..........=~.........~. . ..... ......
              .:,....  ..     ..,MO.. :..   .. ..    .NN.NI.     ..D..D+..... .,  ...M=N,  .....   ....Z:~:+. ......,. ..... ,.......  .. ..
 ......     ...   .......     ....~N=...........~.....=NNN...  .....D..8 ......I,....NNN.  .....  ....,   .?..,.I:...  .....  ..  .      ...
 .                 ..  ....  .....:MNM..=O...  ...   ..NN=    . 7.. ..O7. .,..D.  ...8NN.   .  . ...IM.  ..M$....+...   ... .   ...       ..
 ...   .          ....... ..,..~NN+..ZM..O..   ..O ..  +N,   ..,.D. ...N.. ..?  .,  .ZNN.......   ..:N...N~ ........+.....:...,,. .  .   ...
.... ......        ..D...,=?ODNO.... .,N~Z..   .,N     .NZ..  ..,MI.I..8 . 8M...I....ZNN............N$.NDNNMMNNI........?..OI..,, ..........
         ..       .7,....~.....+...    $MN..   ..+.=.  .NN..     .N:....Z..~I..Z. ...ZNN.   . .?, ..NNN+..    ...+$ONND$=7....            ..
    .... ..      ............ ...      .ONO.   ...M..   NN.      .M.....N..M..N. ....ZN8......Z ....ND .......  ...~:...D..N.. .   .. ....  
                ..     ...    ..   .   .$MNM:..   .N~ ..8NI..   ...M....NNMMD.... ...ND=  ..ZD....OM...  .    ... ........:....~  .  .    ..
 ........  .....  .... ..     .. .....NNN77NNNMO=..?N...,NN...... .ND..=DD....... ..+ND....N:..=DNN.~MI  .. . ... . ........~:.....       ..
.................. ... ... ,. .. :,.8N=......:DNNNNNNNNN7NM.       .N:NN..... .....,NNNNNNMNNNNN=......DZ...I ..........      ....... ......
............    .......~,..........IN....... ......,8NNMNNNO.....  .NNO...........DNN...N................DM.... .....:=.=.................  
.....................  , ...ZNDOI..M7 ...... 7.. ........7NN...... NNM......... .NNN....8N................D..OI=?O=.........................
...................................N...   ....:=.   ......NNN.....NNN. ........ZMM8.....,.N................O....N...........................
..................     ..    ....?D=... .....,Z,...  I..  .NNO..ONNM..  ..... MNN,.....:..:: ... .. ......+$......~.........................
.................. .......   ...N...$..    ... .$...:.......NNNNNNM... .... .NNM,......+,..+ ....+$...... .I~.... ?.=.......................
.................. ...,. ... .M,........  ..?  ..M...8:...:..NNNNN ........,MNM.......:,....   .$........ .?.?,.. .=. ,.....................
..................   . .:....O...... ..I7., .=I...N..D... $...NNN?. .......NNNNNNNNND. ........DNNON,.... ..:. ..... .......................
..............  ...... ...O+IN..............?..MZ..MN,...,.7...NNN........NNNNNNNNNNNNMN~...?MN.......,... ... ...........................  
..................   . . ....N. .... ..     .... .NNNN... ..8...NNN... . :NMZ........7DNNNMNN?...  .........I.?. ...........................
.................. .......I,$. ..... ...    .. ......=N.....8  .~NNM.. ..NNN............DNN..=.=............?....: .........................
..................   ......D........ ..    ... ..Z. ...ND. .N  ..7NN7. ..NN8..:..   ......DN..................... . ........................
..................   ....    ....... ... ~+?=7. ...?.:NMNND.$.....NNN:..INN$..=..?..... ...N7.........,~:,..................................
..................   .....  ........ .. . .....,NNNNN,.. .NMN,.....NNN..ZNNZ .7.8..?.,.....ON...+,  =....... ...............................
...................... ......... .........  ~7:.... ........8NN?...NNN:.IMN8..MNNMN,..,.....NN...:Z.~=,.....................................
............   ....  ................   .. ... ......... .....MNNI.INND..MNN.N8I$....... ...7DM.N...........................................
..................    ....  .....    ...  .      .....   .......NMMZNNN..NNMM8...N..............?O87=,..:~..................................
.................. .......   ....... ...   ... ..  .........  ...=NNNNN. NNNN,N+.+M,..  .................. .................................
..................   ........... ..        .     . ..    ... ..  ..NNNNO.ZNNN....:...77,.. .................................................
............    .....................  ..........................  .NNNN..NNN.....  .... : ...................  ..........................  
...................... .......  .....  .....  ...................  .$NNN..NNNZ..............................................................
.......................................     .....  .. ... ..   .....INNN..8NNM..............................................................
..................     ..    ....... .. ..  .. ... ..     .. ..  ...?NNN..?NNN..............................................................
.................. .......   ....... ...   ... ..    ....      . ...INNN...NNN.......................... . .................................
..........,:.~,=,,........   .......,....:,.. .... ......... ... ...7NNN...NNN7................. .,., .::......................  .:. .......
.......,..,+=...~=.  .....  .....,:.....:+.=..~................. .. $NNN.. NNNN................,..,,~.,+:~..:.  ...............,..:. .......
......~~,.:.:I..,..... ..........,,...,..?.:...: ., : ...........  .ONNN.. NNNN...  ........ : ::......+.,...,..:.: ..........,$=...,.:, .  
.... ,:+,,~.~...,.   . ..... ,~+.:.,~,I=.:......=~::.,...      . ...NNNN.. NNNN............,~?:=.,:+~~........~+::.,........ .:.$.+.~IZ..  .
....::,.?..~$$.+..........  ...~IO.=..=....$.=I,..?.,::..... ... ...NMNN...NNNN.......... ...=?I.+.,.....I.~?~,.I.:~:. ....... ..?:=:I...:,.
....=..Z.8~...,,~..  .....  .,.,...+O,I...:......D..=... .     . ...MNNN...NNNN............,.,...=7,8..,,....,.D.,:... ........D$~+ ...I....
...,:......87.M.,~.  .....  ,=.....+==.D:=:,~.?.$...++...    ... ...NNNM...NNNN...........,=.....+==.N,~:,I.I.7...++.  ....... : ..I....~::.
...,...D,.+.IO..Z.. ......  .:.~.. ,.NN.~~...I.....=........ ... ..:NNNN...NNNN............:.=...,.MO.==...$.....=..,. .........N+..,,......
....,=?+.:.$.O.Z..:,.. ......,.~?8I.:..:.7,,.D..,:=.,.:. ..........DNNN?...NNNN............:.~?$+.=..,:?:..?.,::+...:...........I...$N7..~~.
.... ....DD,:8...D: ......... ....M,,.N.$.D..=?.D...,.., ........  NNNN... NNNN,................N~..:.O.D,.=+.D....... ........ D.:8Z.......
.... .....:.,.+~...   ....  ....,..:..I.O..,.,:....+.  . ....    ..NNNN....NNNN,.............,,.,:..N.O..,,=:.. ,+. ...........M......,:.   
.... ~.,.:+~I.+$.. .......    .=..=~7=,N..=8:..?,.=.. ......   .  .MNNM....NNNN:............,I..~=$I,N..:N~,.+~.+.............ND:..I~.,.. ..
......=... :.~=$ .   ..........,....,D..M.D...+...+,.    ... .. ...NNNN....NNNN=...................O..N.N..,?...~........... +...:....,, ...
...........  ..ON....................  M==87.:,=.. ...............8NNN8... NNNN+..  ............ ....8:=DZ,,.=...............+$+..7=......  
................M .... .......  .....  ..MN.......................MNNN$... NNNN?.......................8N.....................O.., .........
................N........................N?......  .. ... ..   ...NNNN~... NNNNZ ......................D?.....................N.............
................N..    ..    ....... ....N=+.. ... ..     .. .. .~NNNN.....DNNNM.......................D=7....................N.............
................N ........   ....... .. +N=7.. ..    ....      . DNNNN.....$NNNN......................:O=8....................N.............
................N.........   ....... ...OI=O.. ... ......... ....NNNNN.....=NNNN,.....................ZI=N....................N.............
...............=$..  .....  ........ ...N:=N.....................NNNNN.....,NNNN=.....................N.~M....................N.............
...............M,..... ......... ....  .N,,M.....................NNNNN......NNNNM   ..................N.,N....  ..............D~..........  

*/