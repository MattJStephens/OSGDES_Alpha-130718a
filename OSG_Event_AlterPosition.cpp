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


// CEvent_AlterPosition
///
bool CEvent_AlterPosition::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	m_occurance = Occurance_discrete;
	double x= 0;
	double y= 0;
	double z= 0;
	double Ax= 0;
	double Ay= 0;
	double Az= 0;
	double Bx= 0;
	double By= 0;
	double Bz= 0;
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {	
		if(iter->m_identifier == "PAT") {
			m_index_PAT = p_env->PAT_index_from_name(iter->m_str_value);
		}		
		if(iter->m_identifier == "Period") {
			std::istringstream iss(iter->m_str_value);
			iss >> m_period;	
		}	
		if(iter->m_identifier == "x") {
			std::istringstream iss(iter->m_str_value);
			iss >> x;
			m_end_position.x()= x;
		}
		if(iter->m_identifier == "y") {
			std::istringstream iss(iter->m_str_value);
			iss >> y;
			m_end_position.y()= y;
		}	
		if(iter->m_identifier == "z") {
			std::istringstream iss(iter->m_str_value);
			iss >> z;
			m_end_position.z()= z;
		}	
		if(iter->m_identifier == "Ax") {
			std::istringstream iss(iter->m_str_value);
			iss >> Ax;
			m_start_position.x()= Ax;
		}	
		if(iter->m_identifier == "Ay") {
			std::istringstream iss(iter->m_str_value);
			iss >> Ay;
			m_start_position.y()= Ay;
		}	
		if(iter->m_identifier == "Az") {
			std::istringstream iss(iter->m_str_value);
			iss >> Az;
			m_start_position.z()= Az;
		}	
		if(iter->m_identifier == "Bx") {
			std::istringstream iss(iter->m_str_value);
			iss >> Bx;
			m_end_position.x()= Bx;
		}	
		if(iter->m_identifier == "By") {
			std::istringstream iss(iter->m_str_value);
			iss >> By;
			m_end_position.y()= By;
		}	
		if(iter->m_identifier == "Bz") {
			std::istringstream iss(iter->m_str_value);
			iss >> Bz;
			m_end_position.z()= Bz;
		}	
	}		

	m_elapsed_time=0;
	if(m_period> 0 )
	{
		m_lerp_coeff = 1/ m_period;
	}

	return true;
}

bool CEvent_AlterPosition::Update(CEnvironment* p_env)
{
	CPositionAttitudeTransform *p_PAT = p_env->get_PAT(m_index_PAT);
	if(p_PAT!= NULL) { 

		switch(m_occurance)
		{
			case Occurance_discrete:
			{				
				p_PAT->mp_PAT->setPosition(m_end_position);	
				return true;	
				break;
			}			
			case Occurance_interpolate:
			{		
				m_elapsed_time+= p_env->get_delta_real_time();
				if(m_elapsed_time>= m_period)
				{				
					p_PAT->mp_PAT->setPosition(m_end_position);	
					return true;					
				}
				else
				{
					p_PAT->mp_PAT->setPosition(lerp_Vec3d(m_start_position, m_end_position, m_lerp_coeff* m_elapsed_time));						
					return false;
				}
				break;
			}
		}		
	}
	return true;
}
