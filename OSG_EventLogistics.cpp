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
#include "OSG_Entities.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Tools.h"
#include "OSG_XML_Save_event_tree.h"
#include <string>
#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

//using namespace boost;

bool CEvent_DE_Logistics::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	mp_producer = NULL;
	m_Logistics_producer_index= UINT_MAX;
	m_Logistics_component_index= UINT_MAX;
	m_operation_index= UINT_MAX;	
	Link(p_design, p_env);	

	if(mp_producer== NULL) { return false; }
	SetProducerPosition(p_design, p_env);
	SetComponentType(p_design, p_env);		
	AddTransition(p_design, p_env);
	if(p_env->m_active_time_type == TimeType_simulation) {	
		p_env->add_discrete_event(p_env->get_elapsed_time(), mp_producer->get_production_period(), "PRODUCER");
	}
	m_operation_index = p_env->mv_logistics_operation.size();	
	CLogistics_operation operation;	
	std::ostringstream oss;
	oss << m_operation_index;
	operation.m_name = oss.str();
	p_env->mv_logistics_operation.push_back(operation);
	m_last_pos = m_producer_node_position;
	m_local_elapsed_time = 0;
	m_produce_new_operative = false;
	m_save_demand_info= false;

	return true;
}

bool CEvent_DE_Logistics::Update(CEnvironment* p_env)
{	
	//if linear...
	//could be a animation path
	CPositionAttitudeTransform *p_PAT = p_env->get_PAT(m_producer_node_PAT_index);
	if(p_PAT== NULL) return false;
		
	m_producer_node_position = p_PAT->mp_PAT->getPosition();	
	update_time(p_env);	
	populate_order_list_by_demands(p_env);	
	produce_Logistics_operatives(p_env);
	explicate_orders(p_env);			
	p_env->slay_flagged_orders();
	stack_Logistics_operatives(p_env);		
	
	return true;
}

void CEvent_DE_Logistics::update_time(CEnvironment* p_env)
{	
	m_local_elapsed_time+= p_env->get_delta_time();	
}

bool CEvent_DE_Logistics::populate_order_list_by_demands(CEnvironment* p_env)
{	
	//loop through demand info
	for(mDIIter = mv_logistics_demand_info.begin(); mDIIter < mv_logistics_demand_info.end(); ++mDIIter) {		
		mp_demand = &(p_env->mv_logistics_demand[mDIIter->m_Logistics_demand_index]);		
		mp_demand->m_demand_elapsed_time+= p_env->get_delta_time();
		if(mp_demand->m_demand_elapsed_time>= mp_demand->get_demand_period())	{
			mp_demand->m_demand_elapsed_time = 0;	
			mp_demand->update_demand_period();
			if(m_save_demand_info) {
					std::ostringstream oss;
					std::fstream fstr;
					oss << "_Demand_Period" << ".csv";		
					fstr.open(oss.str(), std::ios_base::out | std::ios_base::app);		
					fstr << p_env->get_elapsed_time() << ", " <<  mp_demand->get_demand_period() << "\n";
					fstr.close();		
				}
			//if using DES simulation time add state change to the state change tree
			//The next demand period is added to the current elapsed time to get the next discrete time.
			if(p_env->m_active_time_type == TimeType_simulation) {					
				p_env->add_discrete_event(p_env->get_elapsed_time(), mp_demand->get_demand_period(), mp_demand->m_name);
			}
			//create order and store at the node referended by the demand object
			CLogistics_order order;			
			order.m_ref_demand_node = mp_demand->m_ref_node;
			order.m_op_state= mp_demand->m_op_state;		
			order.m_component_index = mp_demand->m_ref_component.m_index;
			order.m_order_type= mp_demand->m_order_type;
			
			//loop through all the source nodes referenced by the demand object and store them in the order
			for(mRefIter = mp_demand->mv_ref_source_node.begin(); mRefIter != mp_demand->mv_ref_source_node.end(); ++mRefIter)
			{
				order.mv_ref_source_node.push_back(*mRefIter);
			}

			//need to add component information
			p_env->mv_logistics_node[order.m_ref_demand_node.m_index].mv_order.push_back(order);
		}
	}
	return true;
}

bool CEvent_DE_Logistics::produce_Logistics_operatives(CEnvironment* p_env)
{	
	m_produce_new_operative= false;	
	switch(mp_producer->m_production_type) {
		case ProductionType_Synthesis:	{		
			synthesis(p_env);
			break;
		}
		case ProductionType_Periodic: {		
			periodic(p_env);
			break;
		}
		case ProductionType_PeriodicVolume:	{			
			volume(p_env);		
			break;
		}
		case ProductionType_Stochastic:	{			
			Stochastic(p_env);		
			break;
		}		
		case ProductionType_EXPR:	{			
			Produce_EXPR(p_env);		
			break;
		}	

	}
	if(m_produce_new_operative) {			

		if(p_env->m_active_time_type == TimeType_simulation) {	
			p_env->add_discrete_event(p_env->get_elapsed_time(), mp_producer->get_production_period(), mp_producer->m_name);
		}


		CReference ref_Logistics_operative;
		ref_Logistics_operative.m_ref_name = "produced";

		CLogistics_operative Logistics_operative;
		Logistics_operative.m_logistics_component_index = m_Logistics_component_index;
		Logistics_operative.m_component_index = m_component_index;
		Logistics_operative.m_group_index = m_group_index;
		Logistics_operative.m_origin_node_index = mp_producer->m_ref_node.m_index;
		Logistics_operative.m_ref_rest_node.m_index = mp_producer->m_ref_node.m_index;
		if(m_Logistics_component_index< p_env->mv_Logistics_component.size())
		{
			Logistics_operative.m_transport_mode = p_env->mv_Logistics_component[m_Logistics_component_index].m_transport_mode;
		}
		else
		{
			return false;
		}
		
	
		//update flow rate from expression
		if(mp_producer->m_flow_rate_method == FlowRateMethod_EXPR)
		{
			if(mp_producer->m_ref_flow_EXPR.m_index < p_env->mv_EXPR.size())
			{
				CEXPR * p_EXPR = &(p_env->mv_EXPR[mp_producer->m_ref_flow_EXPR.m_index]);
				Logistics_operative.m_mass = p_EXPR->evaluate();
			}
		}
		else
		{
			Logistics_operative.m_mass = p_env->mv_Logistics_component[m_Logistics_component_index].m_mass;
		}

		if(mp_producer->m_infinite_supply)
		{
			switch(m_component_type) {
				case ComponentType_Node:
				{
					p_env->AddLogisticsOperative(&ref_Logistics_operative, &Logistics_operative, m_operation_index, SourceGeometryType_Node);		
					break;
				}
				case ComponentType_Geode: {
					p_env->AddLogisticsOperative(&ref_Logistics_operative, &Logistics_operative, m_operation_index, SourceGeometryType_Geode);		
					break;
				}	
			}				
		}	
		else
		{
			mp_producer->m_total_mass-= Logistics_operative.m_mass;
			if(mp_producer->m_total_mass>= 0)
			{
				switch(m_component_type) {
					case ComponentType_Node:
					{
						p_env->AddLogisticsOperative(&ref_Logistics_operative, &Logistics_operative, m_operation_index, SourceGeometryType_Node);		
						break;
					}
					case ComponentType_Geode: {
						p_env->AddLogisticsOperative(&ref_Logistics_operative, &Logistics_operative, m_operation_index, SourceGeometryType_Geode);		
						break;
					}	
				}			
			}

		}
	}
	return true;
}





bool CEvent_DE_Logistics::ValidateOrder(CLogistics_order *p_order, CLogistics_operative *p_operative, std::size_t component_index, CEnvironment* p_env)
{
	if(p_order->m_slay)
		return false;
	if(p_operative->m_state!= Logistics_operativeState_in_storage) 
		return false;
	if(p_order->m_ref_demand_node.m_index== p_operative->m_ref_rest_node.m_index)
		return false;
	if(p_order->m_component_index!= m_Logistics_component_index)		return false;
	//check that an order source node == p_operative->m_ref_rest_node.m_index

	if(p_operative->m_transport_mode == TransportMode_BeeLine)
	{
		for(mRefIter = p_order->mv_ref_source_node.begin(); mRefIter != p_order->mv_ref_source_node.end(); ++mRefIter)
		{
			if(mRefIter->m_index == p_operative->m_ref_rest_node.m_index)
			{
				return true;
			}
		}
	}
	else if(p_operative->m_transport_mode == TransportMode_Route)
	{
		for(mRefIter = p_order->mv_ref_source_node.begin(); mRefIter != p_order->mv_ref_source_node.end(); ++mRefIter)
		{
			if(mRefIter->m_index == p_operative->m_ref_rest_node.m_index)
			{
				return true;
			}
		}
	}
	
	return false;
}

bool CEvent_DE_Logistics::explicate_orders(CEnvironment* p_env)
{		
	//loop through demand nodes 	
	for(mDemandIter = p_env->mv_logistics_demand.begin(); mDemandIter !=p_env->mv_logistics_demand.end(); ++mDemandIter) {		
		mp_node =  p_env->get_logistics_node(&(mDemandIter->m_ref_node));	
		if(mp_node== NULL) { return false; }
	
		//loop through orders stored in demand node				
		for(mOrderIter = mp_node->mv_order.begin(); mOrderIter != mp_node->mv_order.end();	++mOrderIter)				
		{
			//loop through operatives 
			//TODO orders should only be fulfilled from a list of supply nodes (not any node)
			bool found_order= false;			
			for(mLogOpIter = p_env->mv_logistics_operation[m_operation_index].get_Logistics_operative_vector()->begin(); 
				mLogOpIter < p_env->mv_logistics_operation[m_operation_index].get_Logistics_operative_vector()->end(); 
		      ++mLogOpIter)
			{			
				if(!found_order)
				{
					if(ValidateOrder(&(*mOrderIter), &(*mLogOpIter), m_Logistics_component_index, p_env))
					{
						found_order =true;
						//continuous operations overides DES
						if(mOrderIter->m_order_type== OrderType_Continuous) { 							
							p_env->m_active_continuous_operatives++;
							p_env->m_active_time_type = TimeType_real;						
						}
						else { p_env->m_request_sim_time_mode = true; }
						//TODO: to specify component??


						//if(mLogOpIter->m_transport_mode == TransportMode_Route)
						//{
						//	get_path(p_env);//TODO
						//}						
						mLogOpIter->m_order_type = mDemandIter->m_order_type;
						mLogOpIter->m_ref_start_node = mLogOpIter->m_ref_rest_node;
						mLogOpIter->m_ref_destination_node = mOrderIter->m_ref_demand_node;
						mLogOpIter->set_on_route(mOrderIter->m_op_state, m_time_type, m_speed);
						mOrderIter->m_slay = true;		
					}
				}				
			}				
		}
	}

	return true;
}	

bool CEvent_DE_Logistics::stack_Logistics_operatives(CEnvironment* p_env)
{	
	for(mNodeIter = p_env->mv_logistics_node.begin(); mNodeIter < p_env->mv_logistics_node.end(); ++mNodeIter) {		
		float found_in_storage = 0;		
		for(mLogOpIter = p_env->mv_logistics_operation[m_operation_index].get_Logistics_operative_vector()->begin(); 
			mLogOpIter < p_env->mv_logistics_operation[m_operation_index].get_Logistics_operative_vector()->end(); 
			++mLogOpIter)	{	
			if((mLogOpIter->m_state == Logistics_operativeState_in_storage)
				&&(std::distance(p_env->mv_logistics_node.begin(), mNodeIter)== mLogOpIter->m_ref_rest_node.m_index))	{			
				Vec3d vec = mNodeIter->get_position();
				vec.z()+= (m_stacking_distance* found_in_storage);	
				mLogOpIter->m_position = vec; 	
				found_in_storage+= 1;
			}
		}		
	}
	return true;
}

bool CEvent_DE_Logistics::AddTransition(CEventDesign* p_design, CEnvironment* p_env)
{	
	for(mDIIter = mv_logistics_demand_info.begin(); mDIIter < mv_logistics_demand_info.end(); ++mDIIter){
		if(mDIIter->m_Logistics_demand_index== UINT_MAX) return false;
		CLogistics_demand* mp_demand;
		mp_demand = &(p_env->mv_logistics_demand[mDIIter->m_Logistics_demand_index]);
		mDIIter->m_demand_node_index=  mp_demand->m_ref_node.m_index;
		if(mDIIter->m_demand_node_index== UINT_MAX) return false;
		mDIIter->m_demand_node_PAT_index= p_env->mv_logistics_node[mDIIter->m_demand_node_index].m_ref_PAT.m_index;
		if(mDIIter->m_demand_node_PAT_index== UINT_MAX) return false;	
		mp_demand->m_demand_elapsed_time = 0;		
		double elapsed_time = p_env->get_elapsed_time();
		double period = mp_demand->get_demand_period();
		std::string name = mp_demand->m_name;			
		p_env->add_discrete_event(elapsed_time, period, name);
	}	
	return true;
}

bool CEvent_DE_Logistics::SetProducerPosition(CEventDesign* p_design, CEnvironment* p_env)
{
	if(m_Logistics_producer_index!= UINT_MAX) {
		mp_producer->m_producer_elapsed_time = 0;		
		m_producer_node_index= mp_producer->m_ref_node.m_index;		
		if(m_producer_node_index!=UINT_MAX)	{
			m_producer_node_PAT_index= p_env->mv_logistics_node[m_producer_node_index].m_ref_PAT.m_index;
			CPositionAttitudeTransform *p_producer_node_PAT = p_env->get_PAT(m_producer_node_PAT_index);
			if(p_producer_node_PAT!= NULL) 
			{
				m_producer_node_position = p_producer_node_PAT->mp_PAT->getPosition();	
			}
		}	
	}
	return true;
}

bool CEvent_DE_Logistics::SetComponentType(CEventDesign* p_design, CEnvironment* p_env)
{
	if(m_Logistics_component_index!= UINT_MAX) 	{
		switch(p_env->mv_Logistics_component[m_Logistics_component_index].m_component_type)	{
			case ComponentType_Node: {
				m_component_type = ComponentType_Node;
				m_component_index= p_env->mv_Logistics_component[m_Logistics_component_index].m_ref_component_geometry.m_index;
				break;
			}
			case ComponentType_Geode: {
				m_component_type = ComponentType_Geode;
				m_component_index= p_env->mv_Logistics_component[m_Logistics_component_index].m_ref_component_geometry.m_index;
				break;
			}			
		}	
	}	
	return true;
}

bool CEvent_DE_Logistics::Link(CEventDesign* p_design, CEnvironment* p_env)
{
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{
		if(iter->m_identifier == "Logistics_producer")
		{
			m_Logistics_producer_index = p_env->index_from_name(iter->m_str_value, p_env->mv_Logistics_producer);
			mp_producer = &(p_env->mv_Logistics_producer[m_Logistics_producer_index]);
			mp_producer->update_production_rate();

			//link flow rate expression index
			if(mp_producer->m_flow_rate_method == FlowRateMethod_EXPR) {			
				std::vector<CEXPR>::iterator EXPR_iter;
				for (EXPR_iter =  p_env->mv_EXPR.begin(); EXPR_iter<  p_env->mv_EXPR.end(); ++EXPR_iter) {
					if(EXPR_iter->m_ID == mp_producer->m_ref_flow_EXPR.m_ref_name) {
						mp_producer->m_ref_flow_EXPR.m_index = std::distance(p_env->mv_EXPR.begin(), EXPR_iter);
					}			
				}					
			}
			//link production rate method expression index
			if(mp_producer->m_production_rate_method == ProductionRateMethod_EXPR) {			
				std::vector<CEXPR>::iterator EXPR_iter;
				for (EXPR_iter =  p_env->mv_EXPR.begin(); EXPR_iter<  p_env->mv_EXPR.end(); ++EXPR_iter) {
					if(EXPR_iter->m_ID == mp_producer->m_ref_prod_EXPR.m_ref_name) {
						mp_producer->m_ref_prod_EXPR.m_index = std::distance(p_env->mv_EXPR.begin(), EXPR_iter);
					}			
				}					
			}
		}
		else if(iter->m_identifier == "Logistics_demand")
		{
			CLogisticsDemandInfo demand_info;
			demand_info.m_Logistics_demand_index = p_env->index_from_name(iter->m_str_value, p_env->mv_logistics_demand);			
			mv_logistics_demand_info.push_back(demand_info);
			mp_demand = &(p_env->mv_logistics_demand[demand_info.m_Logistics_demand_index]);
			if(mp_demand->m_initialise_active_time) { p_env->m_active_time_type = mp_demand->m_init_active_time_type; }


				//link production rate method expression index
		/*	if(mp_demand->m_demand_type == DemandType_EXPR) {			
				std::vector<CEXPR>::iterator EXPR_iter;
				for (EXPR_iter =  p_env->mv_EXPR.begin(); EXPR_iter<  p_env->mv_EXPR.end(); ++EXPR_iter) {
					if(EXPR_iter->m_ID == mp_demand->m_ref_EXPR.m_ref_name) {
						mp_demand->m_ref_EXPR.m_index = std::distance(p_env->mv_EXPR.begin(), EXPR_iter);
					}			
				}					
			}*/
			mp_demand->update_demand_period();
		}
		else if(iter->m_identifier == "Logistics_source_node")
		{
			std::size_t index = p_env->index_from_name(iter->m_str_value, p_env->mv_logistics_node);
			if(index!= UINT_MAX)
			{
				mv_logistics_source_node_index.push_back(index);
			}			
		}
		else if(iter->m_identifier == "Logistics_component")
		{
			m_Logistics_component_index = p_env->index_from_name(iter->m_str_value, p_env->mv_Logistics_component);

			std::vector<CReference>::iterator ref_iter;
			for(ref_iter = p_env->mv_Logistics_component[m_Logistics_component_index].mv_sub_component.begin(); 
				ref_iter < p_env->mv_Logistics_component[m_Logistics_component_index].mv_sub_component.end(); 
				++ref_iter)
			{
				ref_iter->m_index = p_env->index_from_name(ref_iter->m_ref_name, p_env->mv_Logistics_component);
				mv_sub_component_index.push_back(ref_iter->m_index);
			}
		}
		else if(iter->m_identifier == "TimeType")
		{
			m_str_time_type = iter->m_str_value;
			if(m_str_time_type == "Real")
			{				m_time_type = TimeType_real;				
			}
			else if(m_str_time_type == "Simulation")
			{
				m_time_type = TimeType_simulation;
			}	
		}
		else if(iter->m_identifier == "Speed")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_speed;
		}	
		else if(iter->m_identifier == "StackingDistance")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_stacking_distance;
		}			
		if(iter->m_identifier == "Group")
		{
			m_group_index = p_env->index_from_name(iter->m_str_value, p_env->mv_group);
		}		
		if(iter->m_identifier == "SaveDemandInfo")
		{
			if(iter->m_str_value == "TRUE")
				m_save_demand_info = true;
			else
				m_save_demand_info = false;
		}	
		
	}
	return true;
}

bool CEvent_DE_Logistics::periodic(CEnvironment* p_env)
{
	mp_producer->m_producer_elapsed_time+= p_env->get_delta_time();
	if(mp_producer->m_producer_elapsed_time>= mp_producer->get_production_period())
	{
		mp_producer->m_producer_elapsed_time = 0;	
		m_produce_new_operative=true;	
	}	
	return true;
}

bool CEvent_DE_Logistics::Stochastic(CEnvironment* p_env)
{
	mp_producer->m_producer_elapsed_time+= p_env->get_delta_time();
	if(mp_producer->m_producer_elapsed_time>= mp_producer->get_production_period())
	{
		mp_producer->update_production_rate();
		mp_producer->m_producer_elapsed_time = 0;	
		m_produce_new_operative=true;	
	}	
	return true;
}

bool CEvent_DE_Logistics::Produce_EXPR(CEnvironment* p_env)
{
	mp_producer->m_producer_elapsed_time+= p_env->get_delta_time();
	if(mp_producer->m_producer_elapsed_time>= mp_producer->get_production_period())
	{
		mp_producer->update_production_rate();
		mp_producer->m_producer_elapsed_time = 0;	

		if(mp_producer->get_production_period() > 0) {
			m_produce_new_operative=true;	
		}
	}	
	return true;
}

bool CEvent_DE_Logistics::volume(CEnvironment* p_env)
{
	mp_producer->m_producer_elapsed_time+= p_env->get_delta_time();
	if(mp_producer->m_producer_elapsed_time>= mp_producer->get_production_period()) {
		mp_producer->m_producer_elapsed_time = 0;	
		if(mp_producer->m_total_volume>= mp_producer->m_volume)	{
			mp_producer->m_total_volume-= mp_producer->m_volume;
			m_produce_new_operative=true;	
		}				
	}	
	return true;
}

bool CEvent_DE_Logistics::synthesis(CEnvironment* p_env)
{	
	std::string operative_component_name;
	std::string sub_component_name;
	std::string rest_node_name;
	std::vector<COperativeRef> v_operatives_found;
	std::vector<std::size_t> v_indicies_found;
	//Loop through Logistics operations
	for(p_env->m_logistics_operation_iter = p_env->mv_logistics_operation.begin(); p_env->m_logistics_operation_iter < p_env->mv_logistics_operation.end(); ++(p_env->m_logistics_operation_iter))
	{			
		//Loop through Logistics operatives
		for(mLogOpIter = p_env->m_logistics_operation_iter->get_Logistics_operative_vector()->begin(); 
			mLogOpIter < p_env->m_logistics_operation_iter->get_Logistics_operative_vector()->end(); 
			++mLogOpIter)
		{						
			//Loop sub component refrerences
			for(mIter = mv_sub_component_index.begin(); mIter < mv_sub_component_index.end(); ++mIter)
			{
				bool found_index_allready=false;
				bool found_index=false;

				//if operative component index is equal to sub component refernence
				// and the component is in storage then we have found one

				std::size_t producer_logistics_node_index = p_env->mv_Logistics_producer[m_Logistics_producer_index].m_ref_node.m_index;
				if((mLogOpIter->m_logistics_component_index == (*mIter))&&(mLogOpIter->m_state== Logistics_operativeState_in_storage)
					&&(producer_logistics_node_index == mLogOpIter->m_ref_rest_node.m_index))	{


					operative_component_name = p_env->mv_Logistics_component[mLogOpIter->m_logistics_component_index].m_name;
					sub_component_name = p_env->mv_Logistics_component[*mIter].m_name;
					rest_node_name = p_env->mv_Logistics_producer[m_Logistics_producer_index].m_name;

					found_index = true;
					std::vector<std::size_t>::iterator i2_iter;//component index needed
					for(i2_iter = v_indicies_found.begin(); 
						i2_iter < v_indicies_found.end(); 
						++i2_iter)	{
						if(*mIter == *i2_iter)
						{
							found_index_allready= true;
						}
					}							
				}
				if(found_index&&!found_index_allready)	{
					v_indicies_found.push_back(*mIter);
					COperativeRef op_ref;
					op_ref.m_operation_index = std::distance(p_env->mv_logistics_operation.begin(), p_env->m_logistics_operation_iter);
					op_ref.m_operative_index = std::distance(p_env->m_logistics_operation_iter->get_Logistics_operative_vector()->begin(), mLogOpIter);
					v_operatives_found.push_back(op_ref);
				}
			}
		}
	}			
		
	if(v_indicies_found.size() == mv_sub_component_index.size())
	{
		m_produce_new_operative=true; //this assumes no construction time so needs to be improved
		//need to set component state to under_construction
		//Logistics_operativeState_under_construction
				
		//slay the sub component operatives	
		for(mOpRefIter = v_operatives_found.begin(); mOpRefIter < v_operatives_found.end(); ++mOpRefIter)
		{
			std::vector<CLogistics_operative>* p_op_vec;
			p_op_vec = p_env->mv_logistics_operation[mOpRefIter->m_operation_index].get_Logistics_operative_vector();
			(*p_op_vec)[mOpRefIter->m_operative_index].m_slay= true;
			(*p_op_vec)[mOpRefIter->m_operative_index].m_state= Logistics_operativeState_slay;
		}
	}

	return true;
}


//typedef adjacency_list < listS, vecS, directedS, no_property, property < edge_weight_t, int > > graph_t;
//  typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
//  typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
//  typedef std::pair<int, int> Edge;*/
//
//  const int num_nodes = 5;
//  enum nodes { A, B, C, D, E };
//  char name[] = "ABCDE";
//  Edge edge_array[] = { Edge(A, C), Edge(B, B), Edge(B, D), Edge(B, E), Edge(C, B), Edge(C, D), Edge(D, E), Edge(E, A), Edge(E, B) };
//  int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };
//  int num_arcs = sizeof(edge_array) / sizeof(Edge);
//
//  graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
//  property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
//
//  std::vector<vertex_descriptor> p(num_vertices(g));
//  std::vector<int> d(num_vertices(g));
//  vertex_descriptor s = vertex(A, g);
//
//
//  dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
//
//  std::cout << "distances and parents:" << std::endl;
//  graph_traits < graph_t >::vertex_iterator vi, vend;
//  for (boost::tuples::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
//    std::cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
//    std::cout << "parent(" << name[*vi] << ") = " << name[p[*vi]] << std::
//      endl;
//  }
//  std::cout << std::endl;
//
//  std::ofstream dot_file("figs/dijkstra-eg.dot");
//
//  dot_file << "digraph D {\n"
//    << "  rankdir=LR\n"
//    << "  size=\"4,3\"\n"
//    << "  ratio=\"fill\"\n"
//    << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";
//
//  graph_traits < graph_t >::edge_iterator ei, ei_end;
//  for (boost::tuples::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
//    graph_traits < graph_t >::edge_descriptor e = *ei;
//    graph_traits < graph_t >::vertex_descriptor
//      u = source(e, g), v = target(e, g);
//    dot_file << name[u] << " -> " << name[v]
//      << "[label=\"" << get(weightmap, e) << "\"";
//    if (p[v] == u)
//      dot_file << ", color=\"black\"";
//    else
//      dot_file << ", color=\"grey\"";
//    dot_file << "]";
//  }
//  dot_file << "}";
  ///return EXIT_SUCCESS;

bool CEvent_DE_Logistics::get_path(CEnvironment* p_env)
{

	//std::size_t graph_index= 0;
	//graph_t* p_g = &(p_env->mv_xgmml_graph[graph_index].mv_graph[0]);
	////boost::property_map<graph_t, boost::edge_weight_t>::type weightmap = boost::get(boost::edge_weight, *p_g);

 //   std::vector<vertex_descriptor> p(boost::num_vertices(*p_g));
	//std::vector<int> d(boost::num_vertices(*p_g));
	//int node_A= 0;
 //   vertex_descriptor s = boost::vertex(node_A, *p_g);	
	//
	//
	//boost::dijkstra_shortest_paths((*p_g), s, boost::predecessor_map(&(p[0])).distance_map(&(d[0])));



 // std::cout << "distances and parents:" << std::endl;
 // boost::graph_traits < graph_t >::vertex_iterator vi, vend;
 // for (boost::tuples::tie(vi, vend) = boost::vertices((*p_g)); vi != vend; ++vi) {
 //   std::cout << "distance(" << *vi << ") = " << d[*vi] << ", ";
 //   std::cout << "parent(" << *vi << ") = " << p[*vi] << std::
 //     endl;
 // }
 // std::cout << std::endl;



 //  // Backtrack from end node, storing distances
 //int target =  boost::vertex(B, g);
 //std::vector<int> nodes;
 //std::vector<float> distance;
 //do{
 //    nodes.push_back(target);
 //    distance.push_back(d[target]);
 //    target = p[target];
 //}while(target != s);


  //std::ofstream dot_file("figs/dijkstra-eg.dot");

  //dot_file << "digraph D {\n"
  //  << "  rankdir=LR\n"
  //  << "  size=\"4,3\"\n"
  //  << "  ratio=\"fill\"\n"
  //  << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

  //graph_traits < graph_t >::edge_iterator ei, ei_end;
  //for (boost::tuples::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
  //  graph_traits < graph_t >::edge_descriptor e = *ei;
  //  graph_traits < graph_t >::vertex_descriptor
  //    u = source(e, g), v = target(e, g);
  //  dot_file << name[u] << " -> " << name[v]
  //    << "[label=\"" << get(weightmap, e) << "\"";
  //  if (p[v] == u)
  //    dot_file << ", color=\"black\"";
  //  else
  //    dot_file << ", color=\"grey\"";
  //  dot_file << "]";
  //}
  //dot_file << "}";
  /////return EXIT_SUCCESS;

  return true;
}

/*                                                                                               
                                                                                                     
    DDDD        8DDD                D.    D.   DD                                                   
   .DDZD      .8DMDD               DD    DD.  .DD                                                   
   .DD.DN     .DDDDD      DDD     .DD ...DD.  .DD  :N: .       DD.    ...      .   ,                
   .DD.8D,.   8D.DDD  .DDDDDDDN   DDDDD DDDDD .DDDDNNDND.   DDDDNNDD  8D    DDD    DD.              
   .DD  D8. ..DD.DDD  8D...   DD   DD    DD.  .DDD.  .8D   DD. .  .DN  DN  .DDD    DD               
   .DD   D. .NN  DDD      . .DDD   DD    DD.  .DD..   DD .,DNDDDDDDDD..DD  .D.DD  8N                
   .DD  .DD  DD. DDD   8DDDDD,DD   DD    DD.  .DD     DD  .DN8DDDDDDD  .DN DN.,D .ND                
   .DD   ,D.DD.. DDD  DD    .,DD   DD    DD.  .DD     DD ..DD.       .  DD.D. .DDDD                 
   .DD    DNDD   DDD  DN..  .DDD   DN    DD   .DD     DD   NDN.. .DDN   .DND  .D8DD.                
   .DD    .D8.   DDD   NNDDDD.DD.   DDD. .DDD..DD     DD     DDDDD8.     MDD   .DD.                 
                        ....        .. .   . .                 ..                                   
          . .                                                                                        
          DD                                                                                        
          DD                                                                                        
          DD    ...DDO .   .   .DN  . :NO..   .  ONO     ...,DD                                     
          DD  ..DDDDDNDD.  DD.DDNDD8DDDDDND... NNNDDDD .  DNDDDDDN                                  
          DD  .DD     8ND  DDD   .DD8.  ,DD  .D8.    DD   DD..  D                                   
          DD    .. .DDDDD  DD     DD     DD .DDNDDDDDNDD..DND8N                                     
     .    DD  .DDNDDD DDD  DD.    DD     DD .DN.        .    DDDNN                                  
   8D8   .DD  ,DD   ..NDD  DD.    DD     DD  DD8.     .  .D     ,DD                                 
   .ND. .DDD  .DD   .NDDD  DD.    DD     DD   DDN ..,DD.  DD.   DDD                                 
     DDD8D      DDDDD .8D  DD.    DD    .DD.  .,DDDDDD     DDDDDN                                   
                 

       ..DD .       .                             ..                                                
     8NDDDDDDD8    8D                            .DD                                                
    .8N.    .D8.   DD                            .DD                                                
    DDD     ..DD. D8DND     8DDD     ND .8D8     .DD 8DD8       8DD8     .DD  8DN       DDDD        
    .DDDN         DDDDD  .DDD.,DDN  .D8ND8.DD8 . .DND8.DDN    NDD, NDD,.  DDDN .DDD   NND  DD8      
    .  DDDDDD8     DD    D8      D8 .DDD.    NN. .DN .   DD .DN .    NN   DD    .DD   DD.  .  ..    
        .  .DDDD   DD   DDNDDDDDNDD..DD      DD  .DN    .DD  DDDDDDDDDDD  D8     DD.. 8DDNN8        
    DD      . DD   DD   DD...........DD     .DD  .DD    .DD  DD ........  DD     DD.   .,DDDD8      
   .DDO.      DD   DD   .DD     .D ..DDD     DD. .DD    .DD .ND      :O   DD     DD..,8D  . .DD     
     DD8D  .DDD.   DD.  .8D8 ..8DN  .DDD8  DDD.  .DD    .DD   DD8, .DDD   DD     DD.. DD8  .DDD.    
    .. DDDDDN  .    DDD  . DDDDD.   .DD 8DDD .   .DD    .DD     NDDDD ..  DD     DD.  ..DDDDD  .    
                    .       .  .    .DD  .                                                          
                                    .DD                                                             
                                     DD                                                             
                                                                                                    
                                                                                                    
  */

