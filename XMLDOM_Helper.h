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

#ifndef _INCL_XMLDOM_Helper
#define _INCL_XMLDOM_Helper

#include <stdio.h>
#include <tchar.h>
#include <msxml6.h>

// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)

// Macro to verify memory allcation.
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)

// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)

// Helper function to create a VT_BSTR variant from a null terminated string. 
HRESULT VariantFromString(PCWSTR wszValue, VARIANT &Variant);

// Helper function to create a DOM instance. 
HRESULT CreateAndInitDOM(IXMLDOMDocument **ppDoc);

// Helper that allocates the BSTR param for the caller.
HRESULT CreateElement(IXMLDOMDocument *pXMLDom, PCWSTR wszName, IXMLDOMElement **ppElement);

// Helper function to append a child to a parent node.
HRESULT AppendChildToParent(IXMLDOMNode *pChild, IXMLDOMNode *pParent);

// Helper function to create and add a processing instruction to a document node.
HRESULT CreateAndAddPINode(IXMLDOMDocument *pDom, PCWSTR wszTarget, PCWSTR wszData);

// Helper function to create and add a comment to a document node.
HRESULT CreateAndAddCommentNode(IXMLDOMDocument *pDom, PCWSTR wszComment);

// Helper function to create and add an attribute to a parent node.
HRESULT CreateAndAddAttributeNode(IXMLDOMDocument *pDom, PCWSTR wszName, PCWSTR wszValue, IXMLDOMElement *pParent);

// Helper function to create and append a text node to a parent node.
HRESULT CreateAndAddTextNode(IXMLDOMDocument *pDom, PCWSTR wszText, IXMLDOMNode *pParent);

// Helper function to create and append a CDATA node to a parent node.
HRESULT CreateAndAddCDATANode(IXMLDOMDocument *pDom, PCWSTR wszCDATA, IXMLDOMNode *pParent);

// Helper function to create and append an element node to a parent node, and pass the newly created
// element node to caller if it wants.
HRESULT CreateAndAddElementNode(IXMLDOMDocument *pDom, PCWSTR wszName, PCWSTR wszNewline, IXMLDOMNode *pParent, IXMLDOMElement **ppElement);



#endif