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
bool CEvent_DiscreteEventSimulation::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	return true;
}

bool CEvent_DiscreteEventSimulation::Update(CEnvironment* p_env)
{	
	if(p_env->m_active_time_type == TimeType_simulation) {		
		if(p_env->mv_discrete_event.size()> 0) {

			double DES_TIME= p_env->mv_discrete_event[0].m_time;
			p_env->update_simulation_time_beta(p_env->mv_discrete_event[0].m_time- p_env->get_elapsed_time());
			p_env->mv_discrete_event[0].m_slay = true;
		}			
		p_env->slay_marked_discrete_event();
	}	
	return true;
}



