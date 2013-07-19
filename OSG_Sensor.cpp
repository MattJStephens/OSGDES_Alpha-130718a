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
#include "OSG_Event.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Tools.h"
#include "OSG_XML_Save_event_tree.h"

#include <string>
#include <sstream>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

///
// CEvent_DE_Logistics
///
bool CEvent_Sensor::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {	
		if(iter->m_identifier == "type") {	m_type = iter->m_str_value; }		
		else if(iter->m_identifier == "sub_type") {	m_sub_type = iter->m_str_value; }		
	}		
	
	return true;
}

bool CEvent_Sensor::Update(CEnvironment* p_env)
{	
	if(m_type == "mm")
	{
		if(m_sub_type == "mlb")
		{
			if(p_env->mp_input_state_type!= NULL)
			{
				/*if(p_env->mp_input_state_type->m_mouse_button== 0)
				{
					write_char_mapped_memory("mjs_innate", 0,  '1');
				}*/
			}
			else
			{
				cout <<  "enable keyboard event handler";
			}
		}		
	}	

	return true;
}

