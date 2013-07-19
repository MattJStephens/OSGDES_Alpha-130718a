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

#include "OSG_Environment.h"
#include "OSG_Tools.h"
#include <algorithm>
#include <functional>
#include <sstream>

//#include "LogFile.h"
//extern CLogFile g_log_file;

CEnvironment::CEnvironment()
{
	m_uniqueLightNumber = 0;
	m_LIGHTS = 3;
	m_elapsed_real_time= 0;
	m_previous_real_time = 0;
	m_delta_real_time = 0;
	m_elapsed_simulation_time= 0;
	m_delta_simulation_time= 0;
	m_elapsed_simulation_time_increment = 10;
	m_active_viewer_index = 0;
	m_Render = true;
	m_throughput_file = "_throughput.txt";
	m_request_real_time_mode = false;
	m_request_sim_time_mode = false;
	m_active_time_type = TimeType_real;
	m_write_node_throughput_to_file = false;
	m_quit_on_max_time = false;
	m_write_env_state_to_file = false;
	m_max_time=1;
	m_write_heading_env_state=true;
	m_active_continuous_operatives = 0;
}
CEnvironment::~CEnvironment()
{
}

bool CEnvironment::PAT_set_position(CReference *p_ref, const osg::Vec3d* p_vec)
{
	CPositionAttitudeTransform* p_pat = get_PAT(p_ref);
	if(p_pat!= NULL)  {
		p_pat->mp_PAT->setPosition(*p_vec);
	}
	else  {		
		p_ref->m_index = PAT_index_from_unique_ID(p_ref->m_unique_id);		
		p_pat = get_PAT(p_ref);
		if(p_pat!= NULL)  {
			p_pat->mp_PAT->setPosition(*p_vec);
		}
		else  {
			std::size_t s = mv_PAT.size();
			std::size_t index = p_ref->m_index;
			std::size_t ID = p_ref->m_unique_id;
			p_ref->m_slay = true;
			return false;
		}
	}
	return true;
}
bool CEnvironment::PAT_set_attitude(CReference *p_ref, const osg::Quat* p_quat)
{
	CPositionAttitudeTransform* p_pat = get_PAT(p_ref);
	if(p_pat!= NULL) 
		p_pat->mp_PAT->setAttitude(*p_quat);
	else 
		return false;
	return true;
}

bool CEnvironment::link_logistics_operative(CReference* p_ref_Logistics_operative, std::size_t operation_index, std::size_t logistics_component_index, 
	std::size_t component_index, std::size_t group_index, eSourceGeometryType SourceGeometryType)
{
	CLogistics_operative* p_Logistics_operative = mv_logistics_operation[operation_index].get_Logistics_operative(p_ref_Logistics_operative);
	if(p_Logistics_operative!= NULL) {
		p_Logistics_operative->mp_PAT = new osg::PositionAttitudeTransform();	
		switch(SourceGeometryType) 	{
			case SourceGeometryType_Node: {			
				p_Logistics_operative->mp_PAT->addChild(mv_node[component_index].mp_node);			
				break;
			}
			case SourceGeometryType_Geode:	{		
				p_Logistics_operative->mp_PAT->addChild(mv_geode[component_index].mp_geode);
				break;
			}
		}	
		p_Logistics_operative->m_logistics_component_index= logistics_component_index;//redundant?
		mv_group[group_index].mp_group->addChild(p_Logistics_operative->mp_PAT);	
	}

	return true;

}
		
bool CEnvironment::AddLogisticsOperative(CReference* p_ref_Logistics_operative, CLogistics_operative * p_logistics_operative, 
			std::size_t operation_index, eSourceGeometryType SourceGeometryType)
{
	if(operation_index>= mv_logistics_operation.size())
		return false;
	
	*p_ref_Logistics_operative = mv_logistics_operation[operation_index].add_logistics_operative(*p_logistics_operative);	
	link_logistics_operative(p_ref_Logistics_operative, operation_index, p_logistics_operative->m_logistics_component_index, p_logistics_operative->m_component_index, 
		p_logistics_operative->m_group_index, SourceGeometryType);

	return true;
}

bool slay_PAT(const CPositionAttitudeTransform &PAT)
{
	if(PAT.m_slay == true)
		return true;
	else return false;
}


void CEnvironment::init_time()
{	
	m_elapsed_real_time = mv_viewer[m_active_viewer_index].mp_viewer->elapsedTime();	
	m_previous_real_time= m_elapsed_real_time;	
	m_delta_real_time = 0;
	
	m_elapsed_simulation_time= 0;
	m_previous_simulation_time= 0;
	m_delta_simulation_time= 0;
}


void  CEnvironment::update_simulation_time()
{
	m_elapsed_simulation_time+= m_elapsed_simulation_time_increment;
	m_delta_simulation_time= m_elapsed_simulation_time - m_previous_simulation_time;
	m_previous_simulation_time= m_elapsed_simulation_time;
}


void CEnvironment::update_simulation_time_beta(double DeltaT)
{
	m_elapsed_simulation_time+= DeltaT;
	m_delta_simulation_time= m_elapsed_simulation_time - m_previous_simulation_time;
	m_previous_simulation_time= m_elapsed_simulation_time;
}
bool CEnvironment::update_time()
{
	m_elapsed_real_time = mv_viewer[m_active_viewer_index].mp_viewer->elapsedTime();	
	m_delta_real_time =  m_elapsed_real_time- m_previous_real_time; 
	m_previous_real_time=  m_elapsed_real_time;	
	if(m_quit_on_max_time)
	{
		if(m_elapsed_real_time> m_max_time)
			return false;
	}
	return true;
}


double CEnvironment::get_delta_time(void)
{
	switch(m_active_time_type)
	{
		case TimeType_simulation:
		{
			return get_delta_simulation_time();
			break;
		}
		case TimeType_real:
		{
			return get_delta_real_time();
			break;
		}			
	}
	return get_delta_real_time();
}

double CEnvironment::get_elapsed_time(void)
{	
	switch(m_active_time_type)
	{
		case TimeType_simulation:
		{
			return get_elapsed_simulation_time();
			break;
		}
		case TimeType_real:
		{
			return get_elapsed_real_time();
			break;
		}			
	}
	return get_elapsed_real_time();

}

osg::Vec3d CEnvironment::get_logistics_node_position(CReference* p_ref)
{
	osg::Vec3d vec;
	CLogistics_node * p_Logistics_node = get_logistics_node(p_ref);
	if(p_Logistics_node!= NULL)
	{
		vec = p_Logistics_node->get_position();
	}
	return vec;
}

bool CEnvironment::update_operation(std::size_t operation_index, eTimeType time_type)
{		
	return mv_logistics_operation[operation_index].update_operation(get_delta_time());	
}

bool CEnvironment::update_theatre_of_operations(eTimeType time_type)
{	
	for(m_logistics_operation_iter = mv_logistics_operation.begin(); m_logistics_operation_iter < mv_logistics_operation.end(); ++m_logistics_operation_iter)
	{	
		m_logistics_operation_iter->update_operation(get_delta_time());	
	}
	return true;
}

bool slay_ACE(const CActiveContinuousEvent &ACE)
{
	if(ACE.m_slay == true)
		return true;
	else return false;
}

void CEnvironment::slay_marked_ACE(void)
{
	/*std::vector<CActiveContinuousEvent>::iterator ACE_iter;
	ACE_iter = std::remove_if(mv_ACE.begin(),mv_ACE.end(), slay_ACE);
	mv_ACE.erase(ACE_iter, mv_ACE.end());*/
}

void CEnvironment::slay_marked_PAT(void)
{
	std::vector<CPositionAttitudeTransform>::iterator PAT_end;
	PAT_end = std::remove_if(mv_PAT.begin(),mv_PAT.end(), slay_PAT);
	mv_PAT.erase(PAT_end, mv_PAT.end());
}

void CEnvironment::slay_marked_logistics_operatives(std::size_t index)
{
	mv_logistics_operation[index].slay_marked_logistics_operatives();
}

void CEnvironment::slay_flagged_orders(void)
{
	std::vector<CLogistics_node>::iterator node_iter;
	for(node_iter = mv_logistics_node.begin(); node_iter != mv_logistics_node.end(); ++node_iter)				
	{
		node_iter->slay_flagged_orders();
	}	
}

bool CEnvironment::add_discrete_event(double elapsed_time, double period, std::string description)
{
	CDiscreteEvent DE;
	DE.m_time = elapsed_time + period;	
	DE.m_description = description;
	if(mv_discrete_event.size() == 0) 	{				
		mv_discrete_event.push_back(DE);	
		return true;
	}	
	else {	
		for(m_DE_iter = mv_discrete_event.begin(); m_DE_iter < mv_discrete_event.end(); ++m_DE_iter) {			
			if(DE.m_time == m_DE_iter->m_time) { return true; }
			else if(DE.m_time < m_DE_iter->m_time) {				
				mv_discrete_event.insert(m_DE_iter, DE);
				return true;
			}				
		}		
		mv_discrete_event.push_back(DE);	
		return true;		
	}
	return false;
}


bool slay_discrete_event(const CDiscreteEvent &DE)
{
	if(DE.m_slay == true)
		return true;
	else return false;
}

void  CEnvironment::slay_marked_discrete_event(void)
{	
	m_DE_iter = std::remove_if(mv_discrete_event.begin(), mv_discrete_event.end(), slay_discrete_event);
	mv_discrete_event.erase(m_DE_iter, mv_discrete_event.end());
}
