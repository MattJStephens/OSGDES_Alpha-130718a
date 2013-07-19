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


///
// CEvent_AlterOrientation
///
bool CEvent_AlterOrientation::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	m_occurance = Occurance_discrete;

	double Qx= 0;
	double Qy= 0;
	double Qz= 0;
	double Qw= 0;
	double QAx= 0;
	double QAy= 0;
	double QAz= 0;
	double QAw= 0;
	double QBx= 0;
	double QBy= 0;
	double QBz= 0;
	double QBw= 0;
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {	
		if(iter->m_identifier == "PAT") {
			m_index_PAT = p_env->PAT_index_from_name(iter->m_str_value);
		}			
		if(iter->m_identifier == "Period") {
			std::istringstream iss(iter->m_str_value);
			iss >> m_period;	
		}	
	
		if(iter->m_identifier == "Qx") {
			std::istringstream iss(iter->m_str_value);
			iss >> Qx;
			m_end_orientation.x() = Qx;
		}	
		if(iter->m_identifier == "Qy") {
			std::istringstream iss(iter->m_str_value);
			iss >> Qy;
			m_end_orientation.y() = Qy;
		}	
		if(iter->m_identifier == "Qz") {
			std::istringstream iss(iter->m_str_value);
			iss >> Qz;
			m_end_orientation.z() = Qz;
		}	
		if(iter->m_identifier == "Qw") {
			std::istringstream iss(iter->m_str_value);
			iss >> Qw;
			m_end_orientation.w() = Qw;
		}	
		
		if(iter->m_identifier == "QAx") {
			std::istringstream iss(iter->m_str_value);
			iss >> QAx;
			m_start_orientation.x() = QAx;
		}	
		if(iter->m_identifier == "QAy") {
			std::istringstream iss(iter->m_str_value);
			iss >> QAy;
			m_start_orientation.y() = QAy;
		}	
		if(iter->m_identifier == "QAz") {
			std::istringstream iss(iter->m_str_value);
			iss >> QAz;
			m_start_orientation.z() = QAz;
		}	
		if(iter->m_identifier == "QAw") {
			std::istringstream iss(iter->m_str_value);
			iss >> QAw;
			m_start_orientation.w() = QAw;
		}	
		
		if(iter->m_identifier == "QBx") {
			std::istringstream iss(iter->m_str_value);
			iss >> QBx;
			m_end_orientation.x() = QBx;
		}	
		if(iter->m_identifier == "QBy") {
			std::istringstream iss(iter->m_str_value);
			iss >> QBy;
			m_end_orientation.y() = QBy;
		}	
		if(iter->m_identifier == "QBz") {
			std::istringstream iss(iter->m_str_value);
			iss >> QBz;
			m_end_orientation.z() = QBz;
		}	
		if(iter->m_identifier == "QBw") {
			std::istringstream iss(iter->m_str_value);
			iss >> QBw;
			m_end_orientation.w() = QBw;
		}	


	}		
	
	//m_end_orientation.set(Qx, Qy, Qz, Qw);
	
	m_elapsed_time=0;
	if(m_period> 0 )
	{
		m_lerp_coeff = 1/ m_period;
	}

	return true;
}

bool CEvent_AlterOrientation::Update(CEnvironment* p_env)
{
	CPositionAttitudeTransform *p_PAT = p_env->get_PAT(m_index_PAT);
	if(p_PAT!= NULL) { 

		switch(m_occurance)
		{
			case Occurance_discrete:
			{				
				p_PAT->mp_PAT->setAttitude(m_end_orientation);	
				return true;	
				break;
			}			
			case Occurance_interpolate:
			{		
				m_elapsed_time+= p_env->get_delta_real_time();
				if(m_elapsed_time>= m_period)
				{				
					p_PAT->mp_PAT->setAttitude(m_end_orientation);	
					return true;					
				}
				else
				{
					p_PAT->mp_PAT->setAttitude(lerp_Quat(m_start_orientation, m_end_orientation, m_lerp_coeff* m_elapsed_time));						
					return false;
				}
				break;
			}
		}		
	}	

	return true;
}
