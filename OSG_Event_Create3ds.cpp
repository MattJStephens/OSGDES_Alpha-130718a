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

#define NOMINMAX
#include "windows.h"

#include "OSG_DES.h"
#include "OSG_Event.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Tools.h"
#include "OSG_XML_Save_event_tree.h"
#include "OSG_XML_Save_environment_tree.h"

#include <string>

using  namespace std;
using  namespace osg;

extern CEventHandler g_event_handler;

// CEventType_CreateWave
///
bool CEvent_Create3ds::Create(CEventDesign* p_design, CEnvironment* p_env)
{

	std::vector<CParsedVariable>::iterator iter;
	/*for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {	
		if(iter->m_identifier == "PAT") {
			m_index_PAT = p_env->PAT_index_from_name(iter->m_str_value);
		}		


	}		*/


	return true;
}

bool CEvent_Create3ds::Update(CEnvironment* p_env)
{
	
	return true;
}
