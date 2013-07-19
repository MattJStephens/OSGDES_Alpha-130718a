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

#ifndef _INCL_OSG_XML_Save_event_tree
#define _INCL_OSG_XML_Save_event_tree

#include "OSG_Event.h"

void XML_Save_event_tree(std::string File_name,  CEventCrysalis* p_event);
void XML_Write_Event(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CEventCrysalis* p_event);

#endif