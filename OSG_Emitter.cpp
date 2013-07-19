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


//this CEmitterParticle must be declared before CEvent_Emitter
//class CEmitterParticle
//{
//public:
//	double m_elapsed_life_time;
//	double m_life_time;
//
//	osg::Vec3d m_position;
//	osg::Vec3d m_velocity;
//	osg::Vec3d m_acceleration;
//	osg::ref_ptr<osg::PositionAttitudeTransform> mp_PAT;
//	double m_mass;
//};

bool CEvent_Emitter::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	m_origin_randomness= 0;
	m_intial_velocity_randomness = 0;
	m_period_elapsed_time=0;
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{
		if(iter->m_identifier == "Period")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_period;					
		}		
		if(iter->m_identifier == "Mass")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_mass;					
		}		
		if(iter->m_identifier == "OriginRandomness")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_origin_randomness;					
		}		
		if(iter->m_identifier == "InitialVelocityRandomness")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_intial_velocity_randomness;					
		}		
		if(iter->m_identifier == "Group")
		{
			m_group_index = p_env->index_from_name(iter->m_str_value, p_env->mv_group);			
		}		
		if(iter->m_identifier == "Type")
		{
			m_type = iter->m_str_value;
		}		
		if(iter->m_identifier == "GeodeName")
		{
			m_geode_index = p_env->index_from_name(iter->m_str_value, p_env->mv_geode);	
		}	
		if(iter->m_identifier == "NodeName")
		{
			m_node_index =p_env->index_from_name(iter->m_str_value, p_env->mv_node);
		}				
		if(iter->m_identifier == "Vec3dArrayName")
		{
			m_Vec3dArray_index = p_env->index_from_name(iter->m_str_value, p_env->mv_Vec3Array);
		}	
		if(iter->m_identifier == "UpdateMode")
		{
			m_update_mode = iter->m_str_value;
		}			
		if(iter->m_identifier == "EXPR_X_ref")
		{
			std::vector<CEXPR>::iterator EXPR_iter;
			for (EXPR_iter =  p_env->mv_EXPR.begin(); EXPR_iter<  p_env->mv_EXPR.end(); ++EXPR_iter) {
				if(EXPR_iter->m_ID == iter->m_str_value) {
					m_EXPR_x_index = std::distance(p_env->mv_EXPR.begin(), EXPR_iter);
				}			
			}	
		}	
		if(iter->m_identifier == "EXPR_Y_ref")
		{
			//m_EXPR_y_index =p_env->index_from_name(iter->m_str_value, p_env->mv_EXPR);
			std::vector<CEXPR>::iterator EXPR_iter;
			for (EXPR_iter =  p_env->mv_EXPR.begin(); EXPR_iter<  p_env->mv_EXPR.end(); ++EXPR_iter) {
				if(EXPR_iter->m_ID == iter->m_str_value) {
					m_EXPR_y_index = std::distance(p_env->mv_EXPR.begin(), EXPR_iter);
				}			
			}	
		}	
		if(iter->m_identifier == "EXPR_Z_ref")
		{
			//m_EXPR_z_index =p_env->index_from_name(iter->m_str_value, p_env->mv_EXPR);
			std::vector<CEXPR>::iterator EXPR_iter;
			for (EXPR_iter =  p_env->mv_EXPR.begin(); EXPR_iter<  p_env->mv_EXPR.end(); ++EXPR_iter) {
				if(EXPR_iter->m_ID == iter->m_str_value) {
					m_EXPR_z_index = std::distance(p_env->mv_EXPR.begin(), EXPR_iter);
				}			
			}	
		}	
		
	}	


	m_origin  = p_design->m_origin;
	m_velocity  = p_design->m_velocity;
	m_acceleration  = p_design->m_acceleration;
	m_position = p_design->m_position;


	//link to vector field....
	
	return true;
}



double evaluate_EXPR(CEnvironment* p_env, std::size_t m_EXPR_index, std::string var_name, double lf_in)
{
	double lf_out= DBL_MAX;	
	if(m_EXPR_index < p_env->mv_EXPR.size()) {

		p_env->mv_EXPR[m_EXPR_index].m_variable_list.set_with_str(var_name, lf_in);
		lf_out =  p_env->mv_EXPR[m_EXPR_index].evaluate();
		
	}	
	return lf_out;
}


bool CEvent_Emitter::Update(CEnvironment* p_env)
{	 	
	m_period_elapsed_time+= p_env->get_delta_time();

	if(m_period_elapsed_time> m_period)
	{
		m_period_elapsed_time= 0;

		//create new particle
		CEmitterParticle particle;		
		std::size_t particle_index = mv_particle.size();		
		mv_particle.push_back(particle);
		
		mv_particle[particle_index].m_position = m_origin;

		if(m_origin_randomness> 0)
		{
			mv_particle[particle_index].m_position.x()+= (-m_origin_randomness/2)+ random_float(m_origin_randomness);
			mv_particle[particle_index].m_position.y()+= (-m_origin_randomness/2)+ random_float(m_origin_randomness);
			mv_particle[particle_index].m_position.z()+= (-m_origin_randomness/2)+ random_float(m_origin_randomness);
		}
		

		mv_particle[particle_index].m_velocity = m_velocity;

		if(m_intial_velocity_randomness> 0)
		{
			mv_particle[particle_index].m_velocity.x()+= (-m_intial_velocity_randomness/2)+ random_float(m_intial_velocity_randomness);
			mv_particle[particle_index].m_velocity.y()+= (-m_intial_velocity_randomness/2)+ random_float(m_intial_velocity_randomness);
			mv_particle[particle_index].m_velocity.z()+= (-m_intial_velocity_randomness/2)+ random_float(m_intial_velocity_randomness);
		}
		
		mv_particle[particle_index].mp_PAT = new osg::PositionAttitudeTransform();	
		mv_particle[particle_index].mp_PAT->setPosition(particle.m_position);

		if(m_type== "Geode")
		{
			if(m_geode_index< p_env->mv_geode.size())
			{
				mv_particle[particle_index].mp_PAT->addChild(p_env->mv_geode[m_geode_index].mp_geode);
			}
		}
		else if(m_type== "Node")
		{
			if(m_geode_index< p_env->mv_geode.size())
			{
				mv_particle[particle_index].mp_PAT->addChild(p_env->mv_node[m_node_index].mp_node);
			}
		}

		if(m_group_index < p_env->mv_group.size())
		{
			p_env->mv_group[m_group_index].mp_group->addChild(mv_particle[particle_index].mp_PAT);
		}	

		//add child
		

	}

	//update position of existing particles
	std::vector<CEmitterParticle>::iterator particle_iter;
	for(particle_iter = mv_particle.begin(); particle_iter != mv_particle.end(); ++particle_iter)
	{

		//alter velocity with vector field
		if(m_update_mode == "EXPR")
		{
			double ForceX = evaluate_EXPR(p_env, m_EXPR_x_index, "x", particle_iter->m_position.x());
			double ForceY = evaluate_EXPR(p_env, m_EXPR_y_index, "y", particle_iter->m_position.y());
			double ForceZ = evaluate_EXPR(p_env, m_EXPR_z_index, "z", particle_iter->m_position.z());

			double AccelerationX = ForceX/ m_mass;
			double AccelerationY = ForceY/ m_mass;
			double AccelerationZ = ForceZ/ m_mass;
			particle_iter->m_velocity.x()+= AccelerationX;
			particle_iter->m_velocity.y()+= AccelerationY;
			particle_iter->m_velocity.z()+= AccelerationZ;
		}


		particle_iter->m_position = particle_iter->m_position + (particle_iter->m_velocity*p_env->get_delta_time());
		particle_iter->mp_PAT->setPosition(particle_iter->m_position);
	}

	return true;
}
