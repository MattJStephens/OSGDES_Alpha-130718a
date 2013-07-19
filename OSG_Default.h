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

#ifndef _OSG_Default
#define _OSG_Default

#include <string>
#include <vector>
#include <atlbase.h>
#include <msxml.h>
#include <comdef.h>

using namespace std;

class CScript
{
	private:				
		bool XML_Parse(CComPtr<IXMLDOMNode> spXMLNode);	
	public:
		CScript();
		bool XML_Parser(std::string str);
		std::vector<std::string> mv_str_environment_file_name;
		std::vector<std::string> mv_str_event_file_name;	
};

#endif