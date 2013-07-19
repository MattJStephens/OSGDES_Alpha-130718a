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
// CEvent_UpdateVelocity
///

bool CEvent_UpdatePhysXActor::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	m_index_PAT = p_env->PAT_index_from_name(p_design->m_ref_name);	
	m_p0  = p_design->m_position;
	CPositionAttitudeTransform *p_PAT = p_env->get_PAT(m_index_PAT);
	if(p_PAT!= NULL) 
		p_PAT->mp_PAT->setPosition(m_p0);	

	m_velocity  = p_design->m_velocity;
	m_acceleration  = p_design->m_acceleration;
	m_orientation_axis = p_design->m_orientation_axis;	
	m_theta0 = p_design->m_orientation;
	m_orientation = m_theta0;	
	m_omega0 = p_design->m_angular_velocity;
	m_angular_velocity = m_omega0;
	m_alpha0 = p_design->m_angular_acceleration;
	m_angular_acceleration = m_alpha0;	
	
	m_Q0.makeRotate(osg::DegreesToRadians(m_theta0), m_orientation_axis);
	m_Qorientation.makeRotate(osg::DegreesToRadians(m_theta0), m_orientation_axis);
	
	if(p_design->m_str_time_type == "Real")
	{
		m_time_type = TimeType_real;
	}
	else
	{
		m_time_type = TimeType_simulation;
	}		
	m_local_elapsed_time=0;	
	return true;
}
bool CEvent_UpdatePhysXActor::Update(CEnvironment* p_env)
{	 
	std::vector<CVarAssoc>::iterator iter;
	for(iter = mv_assoc_var.begin(); iter != mv_assoc_var.end(); ++iter)
	{
		switch(iter->m_assoc)
		{
			case VariableAssoc_PAT_vec:
			{
				Vec3d vec3d;
				CPositionAttitudeTransform *p_PAT = p_env->get_PAT(iter->m_assoc_index);
				if(p_PAT!= NULL) 
					vec3d = p_PAT->mp_PAT->getPosition();
				else
				{
					bool bad=true;
				}

				switch(iter->m_sub_assoc)
				{
					case VariableAssocSub_x:
					{
						m_expression_variables.mv_variable[iter->m_expr_index].m_value = (double)vec3d.x();
						break;
					}
					case VariableAssocSub_y:
					{
						m_expression_variables.mv_variable[iter->m_expr_index].m_value =  (double)vec3d.y();
						break;
					}
					case VariableAssocSub_z:
					{
						m_expression_variables.mv_variable[iter->m_expr_index].m_value =  (double)vec3d.z();
						break;
					}
					case VariableAssocSub_mag:
					{
						m_expression_variables.mv_variable[iter->m_expr_index].m_value =  (double)vec3d.length();
						break;
					}
				}
				break;
			}
		}
	}	
	
	m_local_elapsed_time+= p_env->get_delta_time();

	CPositionAttitudeTransform *p_PAT = p_env->get_PAT(m_index_PAT);
	if(p_PAT!= NULL) 
	{		
		m_position = m_p0 + (m_velocity * m_local_elapsed_time);
		p_PAT->mp_PAT->setPosition(m_position);
		m_orientation = m_theta0 + (m_angular_velocity * m_local_elapsed_time);
		m_Qorientation.makeRotate(osg::DegreesToRadians(m_orientation), m_orientation_axis);
		p_PAT->mp_PAT->setAttitude(m_Qorientation);
	}

	return true;
}



