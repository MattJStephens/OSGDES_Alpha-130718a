
#include "OSG_XML.h"

// XMLNodeExerciser.cpp
//
// Locates a specific XML node in an XML document and inserts
// a new child node, with an attribute, therein.
//
#include <atlbase.h>
#include <msxml.h>
#include <iostream>

void OSG_XML()
{
	// Start COM
	CoInitialize(NULL);

	// Open the file...note this simplistic executable
	// assumes the file is named "xmldata.xml" and is 
	// in the same directory as the executable.  It
	// further assumes the file looks like this:
	//
	// <?xml version="1.0"?>
	// <xmldata>
	//    <xmlnode />
	//    <xmltext>Hello, World!</ xmltext>
	// </xmldata>
	//
	// The executable will find the node "xmlnode"
	// and insert a new node "xmlchildnode" (with the
	// attribute 'xml="fun"') if everything
	// works as expected. It then finds the node
	// "xmltext" and retrieves the text contained
	// within the node and displays that. Finally, it
	// saves the XML document into a new file named
	// "updatedxml.xml".
	try {
		// Create an instance of the parser
		CComPtr<IXMLDOMDocument> spXMLDOM;
		HRESULT hr = spXMLDOM.CoCreateInstance(__uuidof(DOMDocument));
		if ( FAILED(hr) ) throw "Unable to create XML parser object";
		if ( spXMLDOM.p == NULL ) throw "Unable to create XML parser object";

		// Load the XML document file...
		VARIANT_BOOL bSuccess = false;
		hr = spXMLDOM->load(CComVariant(L"xmldata.xml"),&bSuccess);
		if ( FAILED(hr) ) throw "Unable to load XML document into the parser";
		if ( !bSuccess ) throw "Unable to load XML document into the parser";

		CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
		hr = spXMLDOM->get_childNodes(&spXMLDOMNodeList);
		if ( FAILED(hr) ) throw "Unable to get node list";
		


		// Check for <xmldata>
		//              <xmlnode>...
		//
		// Construct search string
		//  "xmldata/xmlnode"
		CComBSTR bstrSS(L"xmldata/xmlnode");
		CComPtr<IXMLDOMNode> spXMLNode;
		hr = spXMLDOM->selectSingleNode(bstrSS,&spXMLNode);
		if ( FAILED(hr) ) throw "Unable to locate 'xmlnode' XML node";
		if ( spXMLNode.p == NULL ) throw "Unable to locate 'xmlnode' XML node";

		// The COM object "spXMLNode" now contains the XML
		// node <xmlnode>, so we create a child node at
		// this point...  The DOM creates the node itself,
		// then we place it using the XML node we located as
		// the parent.
		CComPtr<IXMLDOMNode> spXMLChildNode;
		hr = spXMLDOM->createNode(CComVariant(NODE_ELEMENT),CComBSTR("xmlchildnode"),NULL,&spXMLChildNode);
		if ( FAILED(hr) ) throw "Unable to create 'xmlchildnode' XML node";
		if ( spXMLChildNode.p == NULL ) throw "Unable to create 'xmlchildnode' XML node";

		// Place the node...if all goes well, the two nodal
		// parameters provided to appendChild will be the 
		// same node. If there is a problem, the "inserted"
		// node's pointer will be NULL.
		CComPtr<IXMLDOMNode> spInsertedNode;
		hr = spXMLNode->appendChild(spXMLChildNode,&spInsertedNode);
		if ( FAILED(hr) ) throw "Unable to move 'xmlchildnode' XML node";
		if ( spInsertedNode.p == NULL ) throw "Unable to move 'xmlchildnode' XML node";

		// Add the attribute. Note we do this through the IXMLDOMElement
		// interface, so we need to do the QI().
		CComQIPtr<IXMLDOMElement> spXMLChildElement;
		spXMLChildElement = spInsertedNode;
		if ( spXMLChildElement.p == NULL ) throw "Unable to query for 'xmlchildnode' XML element interface";

		hr = spXMLChildElement->setAttribute(CComBSTR(L"xml"),CComVariant(L"fun"));
		if ( FAILED(hr) ) throw "Unable to insert new attribute";

		// Check for <xmldata>
		//              <xmltext>...
		//
		// Construct search string
		//  "xmldata/xmltext"
		spXMLNode = NULL; // release previous node...
		bstrSS = L"xmldata/xmltext";
		hr = spXMLDOM->selectSingleNode(bstrSS,&spXMLNode);
		if ( FAILED(hr) ) throw "Unable to locate 'xmltext' XML node";
		if ( spXMLNode.p == NULL ) throw "Unable to locate 'xmltext' XML node";

		// Pull the enclosed text and display
		CComVariant varValue(VT_EMPTY);
		hr = spXMLNode->get_nodeTypedValue(&varValue);
		if ( FAILED(hr) ) throw "Unable to retrieve 'xmltext' text";

		if ( varValue.vt == VT_BSTR ) {
			// Display the results...since we're not using the
			// wide version of the STL, we need to convert the
			// BSTR to ANSI text for display...
			USES_CONVERSION;
			LPTSTR lpstrMsg = W2T(varValue.bstrVal);
			std::cout << lpstrMsg << std::endl;
		} // if
		else {
			// Some error
			throw "Unable to retrieve 'xmltext' text";
		} // else

		// If we got this far, we've inserted the new node, so
		// save the resulting XML document...
		hr = spXMLDOM->save(CComVariant("updatedxml.xml"));
		if ( FAILED(hr) ) throw "Unable to save updated XML document";

		// Write a note to the screen...
		std::cout << "Processing complete..."  << std::endl << std::endl;
	} // try
	catch(char* lpstrErr) {
		// Some error...
		std::cout << lpstrErr << std::endl << std::endl;
	} // catch
	catch(...) {
		// Unknown error...
		std::cout << "Unknown error..." << std::endl << std::endl;
	} // catch

	// Stop COM
	CoUninitialize();
}
