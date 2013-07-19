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
bool CEvent_UpdateTheatreOfOperations::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	//loop
	/*m_operation_index = 0;
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{				
		if(iter->m_str_value == "Real")
		{
			m_time_type = TimeType_real;
		}
		else if(iter->m_str_value == "Simulation")
		{
			m_time_type = TimeType_simulation;
		}		
	}*/
	return true;
}

bool CEvent_UpdateTheatreOfOperations::Update(CEnvironment* p_env)
{	
				
	for(p_env->m_logistics_operation_iter = p_env->mv_logistics_operation.begin(); p_env->m_logistics_operation_iter < p_env->mv_logistics_operation.end(); ++(p_env->m_logistics_operation_iter))
	{
		std::vector<CLogistics_operative>::iterator OP_iter;
		OP_iter = std::remove_if(p_env->m_logistics_operation_iter->get_Logistics_operative_vector()->begin(), p_env->m_logistics_operation_iter->get_Logistics_operative_vector()->end(), slay_Logistics_operative);
		p_env->m_logistics_operation_iter->get_Logistics_operative_vector()->erase(OP_iter, p_env->m_logistics_operation_iter->get_Logistics_operative_vector()->end());
	}

	p_env->update_theatre_of_operations(p_env->m_active_time_type);//

	return true;
}

