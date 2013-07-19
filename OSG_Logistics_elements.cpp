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

#include "OSG_Logistics_elements.h"
#include "OSG_Environment.h"
#include <algorithm>
#include <functional>
#include <boost/math/distributions/normal.hpp>

extern CEnvironment g_environment;

bool slay_Logistics_operative(const CLogistics_operative &Logistics_operative)
{
	if(Logistics_operative.m_slay)
		return true;
	else return false;
}

///////////////////////////////////////////////////////////////////////////////////
// CLogistics_node
///////////////////////////////////////////////////////////////////////////////////
CLogistics_node::CLogistics_node()
{
	m_record=false;
}

///////////////////////////////////////////////////////////////////////////////////
// CLogistics_throughput
///////////////////////////////////////////////////////////////////////////////////
CLogistics_throughput::CLogistics_throughput()
{
	m_node = UINT_MAX;
	//eComponentType m_component_type;
	float m_mass= 1;
	double m_time= 0;
}

///////////////////////////////////////////////////////////////////////////////////
// CLogistics_particle
///////////////////////////////////////////////////////////////////////////////////
CLogistics_particle::CLogistics_particle()
{
	m_position.set(0,0,0);
	m_velocity.set(0,0,0);
}

void CLogistics_particle::update_position(double time)
{
	m_position += m_velocity*time;	
}

///////////////////////////////////////////////////////////////////////////////////
// CLogistics_component
///////////////////////////////////////////////////////////////////////////////////
CLogistics_component::CLogistics_component()
{
	m_mass= 1;
	m_density = 1;
	m_transport_mode = TransportMode_BeeLine;
}

///////////////////////////////////////////////////////////////////////////////////
// CLogistics_operative
///////////////////////////////////////////////////////////////////////////////////
CLogistics_operative::CLogistics_operative()
{ 
	m_slay = false;
	m_unique_id = UINT_MAX;
	m_task_duration = 0;
	m_task_elapsed_time = 0;	
	m_mass= 1;
	m_density = 1;
	m_transport_mode = TransportMode_BeeLine;
	m_state = Logistics_operativeState_in_storage;	
}

bool CLogistics_operative::set_on_route(eLogistics_operativeState op_state, eTimeType time_type, float speed)
{
	m_state = op_state;

	osg::Vec3d vecA = g_environment.get_logistics_node_position(&m_ref_start_node);
	osg::Vec3d vecB =  g_environment.get_logistics_node_position(&m_ref_destination_node);	
	osg::Vec3d vecAB = vecB - vecA;
	m_position= vecA;			
	m_destination = vecB;			
	m_distance = vecAB.length();
	vecAB.normalize();		
	m_velocity = vecAB * speed;
	
	m_task_duration = m_distance/speed; //average and constant velocity 	
	m_time_task_complete = g_environment.get_elapsed_time()+ m_task_duration;	

	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// CLogistics_operation
///////////////////////////////////////////////////////////////////////////////////
CLogistics_operation::CLogistics_operation()
{
	m_DELogistics_operative_unique_id=0;
	m_write_to_file= true;
}

CReference CLogistics_operation::add_logistics_operative(CLogistics_operative Logistics_operative) 
{ 		
	CReference ref;
	ref.m_index = mv_logistics_operative.size();
	ref.m_unique_id = m_DELogistics_operative_unique_id;	
	Logistics_operative.m_unique_id = m_DELogistics_operative_unique_id;
	Logistics_operative.m_state = Logistics_operativeState_in_storage;	
	Logistics_operative.m_slay = false;
	mv_logistics_operative.push_back(Logistics_operative);
	m_DELogistics_operative_unique_id++;

	update_throughput(&Logistics_operative, Logistics_operative.m_origin_node_index, 0);

	return ref;
}
CReference CLogistics_operation::get_reference(CLogistics_operative* p_Logistics_operative)
{
	CReference ref;
	ref.m_index= Logistics_operative_index_from_unique_ID(p_Logistics_operative->m_unique_id);
	ref.m_unique_id = p_Logistics_operative->m_unique_id;
	return ref;
}
CLogistics_operative* CLogistics_operation::get_Logistics_operative(CReference* p_ref) 
{ 
	if( p_ref->m_index<  mv_logistics_operative.size())
		return &(mv_logistics_operative[p_ref->m_index]);
	else return NULL;
}
std::size_t CLogistics_operation::Logistics_operative_index_from_name(std::string name)
{
	std::size_t i= 0;
	std::vector<CLogistics_operative>::iterator iter;
	for (iter = mv_logistics_operative.begin(); iter< mv_logistics_operative.end(); ++iter) {
		if(iter->m_name == name) {
			return i;
		}
		i++;
	}
	return UINT_MAX;
}
std::size_t CLogistics_operation::Logistics_operative_index_from_unique_ID(unsigned ID)
{
	std::size_t i= 0;
	std::vector<CLogistics_operative>::iterator iter;
	for (iter = mv_logistics_operative.begin(); iter< mv_logistics_operative.end(); ++iter) {
		if(iter->m_unique_id == ID) {
			return i;
		}
		i++;
	}
	return UINT_MAX;
}

void CLogistics_operation::update_throughput(CLogistics_operative *p_operative, std::size_t index, int type)
{	
	CLogistics_throughput throughput;
	throughput.m_component_type = g_environment.mv_Logistics_component[p_operative->m_logistics_component_index].m_component_type;
	if(type== 0) {
		throughput.m_node = p_operative->m_origin_node_index;
		g_environment.mf_throughput << "create" << ", ";
	}
	else {
		throughput.m_node = p_operative->m_ref_destination_node.m_index;
		g_environment.mf_throughput << "arrive" << ", ";
	}	
	//throughput.m_mass = g_environment.mv_Logistics_component[p_operative->m_logistics_component_index].m_mass;
	throughput.m_mass = p_operative->m_mass;
	throughput.m_time = g_environment.get_elapsed_time();	
	throughput.m_sim_time = g_environment.get_elapsed_simulation_time();
	throughput.m_real_time = g_environment.get_elapsed_real_time();
	if(index< g_environment.mv_logistics_node.size()) {
		g_environment.mv_logistics_node[index].mv_throughput.push_back(throughput);				
	}	
	
	if(g_environment.m_write_node_throughput_to_file)	{ write_throughput_to_file(&throughput); }
}


void CLogistics_operation::write_throughput_to_file(CLogistics_throughput* p_throughput)
{
		std::ostringstream oss;
		oss << "_TP_" << g_environment.mf_throughput << "_"  << p_throughput->m_node << "_" <<  p_throughput->m_component_type << ".csv";		
		g_environment.mf_throughput.open(oss.str(), std::ios_base::out | std::ios_base::app);		
		g_environment.mf_throughput <<  p_throughput->m_mass << ", " <<  p_throughput->m_sim_time << ", " <<  p_throughput->m_real_time << "\n";
		g_environment.mf_throughput.close();		
}

CLogistics_NodeState::CLogistics_NodeState()
{
	m_in_transit= false;
}

std::size_t CLogistics_operation::number_of_operatives_at_node(std::size_t node_index)
{
	std::size_t count= 0;
	std::vector<CLogistics_operative>*pv_operative = get_Logistics_operative_vector();
	std::vector<CLogistics_operative>::iterator operative_iter;
	for(operative_iter= pv_operative->begin(); operative_iter!=  pv_operative->end(); ++operative_iter) {
		if( operative_iter->m_state == Logistics_operativeState_in_storage)	{
			if( operative_iter->m_ref_rest_node.m_index == node_index)	{
				count++;
			}
		}
	}	
	return count;
}

double CLogistics_operation::mass_of_operatives_at_node(std::size_t node_index)		
{
	double mass= 0;
	std::vector<CLogistics_operative>*pv_operative = get_Logistics_operative_vector();
	std::vector<CLogistics_operative>::iterator operative_iter;
	for(operative_iter= pv_operative->begin(); operative_iter!=  pv_operative->end(); ++operative_iter) {
		if( operative_iter->m_state == Logistics_operativeState_in_storage)	{		
			if( operative_iter->m_ref_rest_node.m_index == node_index)	{
				mass+= operative_iter->m_mass;
			}
		}	
	}
	return mass;
}
std::size_t CLogistics_operation::number_of_operatives_in_transit(void)	
{
	std::size_t count= 0;
	std::vector<CLogistics_operative>*pv_operative = get_Logistics_operative_vector();
	std::vector<CLogistics_operative>::iterator operative_iter;
	for(operative_iter= pv_operative->begin(); operative_iter!=  pv_operative->end(); ++operative_iter) {		
		if(( operative_iter->m_state == Logistics_operativeState_in_transit_destroy_at_destination)||( operative_iter->m_state == Logistics_operativeState_in_transit_store_at_destination))	{
			count++;
		}
	}	
	return count;	
	return 0;
}
double CLogistics_operation::mass_of_operatives_in_transit(void)
{
	double mass= 0;
	std::vector<CLogistics_operative>*pv_operative = get_Logistics_operative_vector();
	std::vector<CLogistics_operative>::iterator operative_iter;
	for(operative_iter= pv_operative->begin(); operative_iter!=  pv_operative->end(); ++operative_iter) {		
		if(( operative_iter->m_state == Logistics_operativeState_in_transit_destroy_at_destination)||( operative_iter->m_state == Logistics_operativeState_in_transit_store_at_destination))	{
			mass+= operative_iter->m_mass;			
		}
	}	
	return mass;
}
void CLogistics_operation::write_env_state_to_file(std::string file_name)
{		
	std::vector<CLogistics_NodeState> v_node_state;
	std::vector<CLogistics_NodeState>::iterator ns_iter;

	g_environment.m_env_state_file = file_name; //"_ENV_STATE_.csv";
	g_environment.mf_env_state.open(g_environment.m_env_state_file, std::ios_base::out | std::ios_base::app);	
	
	std::size_t ln_i=0;
	if(g_environment.m_write_heading_env_state) {
		g_environment.m_write_heading_env_state=false;
		g_environment.mf_env_state << "simulation time" << ",";
		g_environment.mf_env_state << "real time" << ",";
		g_environment.mf_env_state << "in transit" << ","; 			
		for(ln_i= 0;ln_i< g_environment.mv_logistics_node.size(); ln_i++) {
			g_environment.mf_env_state << "at node" << ln_i << ",";
		}
		g_environment.mf_env_state << "\n";
	}

	ln_i=0;
	g_environment.mf_env_state << g_environment.get_elapsed_simulation_time() << ",";
	g_environment.mf_env_state << g_environment.get_elapsed_real_time() << ",";
	g_environment.mf_env_state << mass_of_operatives_in_transit() << ",";	
	for(ln_i= 0;ln_i< g_environment.mv_logistics_node.size(); ln_i++) {
		g_environment.mf_env_state << mass_of_operatives_at_node(ln_i) << ",";
	}

	g_environment.mf_env_state << "\n";
	g_environment.mf_env_state.close();		
}


bool CLogistics_operation::arrived_at_destination(CLogistics_operative *p_operative)
{
	p_operative->mp_PAT->setPosition(p_operative->m_destination);


	switch(p_operative->m_state)
	{
		case Logistics_operativeState_in_transit_store_at_destination:
		{							
			p_operative->m_state = Logistics_operativeState_in_storage;
			p_operative->m_velocity = osg::Vec3d(0,0,0);		
			p_operative->m_ref_rest_node= p_operative->m_ref_destination_node;						
			break;
		}	
		case Logistics_operativeState_in_transit_destroy_at_destination:
		{								
			p_operative->m_state = Logistics_operativeState_slay;
			p_operative->m_slay = true;
			g_environment.mv_group[g_environment.m_active_group_index].mp_group->removeChild(p_operative->mp_PAT);								
			break;
		}		
	}

	update_throughput(p_operative, p_operative->m_ref_destination_node.m_index, 1);




	if(p_operative->m_order_type == OrderType_Continuous)
	{		
		g_environment.m_active_continuous_operatives--;
		if(g_environment.m_active_continuous_operatives== 0)
		{
			if(g_environment.m_request_sim_time_mode)
			{
				g_environment.m_active_time_type = TimeType_simulation;	
				g_environment.m_request_sim_time_mode= false;
			}
		}
	}
	
	if(g_environment.m_write_env_state_to_file)
	{
		std::ostringstream oss;
		oss << "EnvState\\" << "_ES_" << g_environment.m_str_script << "_OP_" << m_name << ".csv";
		write_env_state_to_file(oss.str());
	}
	return true;
}

bool CLogistics_operation::update_operation(double delta_time)
{			
	unsigned count = 0;
	std::vector<CLogistics_operative>::iterator iter;	
	for(iter = mv_logistics_operative.begin(); iter <mv_logistics_operative.end(); ++iter)
	{			
		if(iter->m_state==Logistics_operativeState_in_storage) 	{		
			iter->mp_PAT->setPosition(iter->m_position);		
		}
		else  {			
			iter->update_position(delta_time);			
			if(g_environment.get_elapsed_time() >= iter->m_time_task_complete) 	{			
				arrived_at_destination(&(*iter));
			}
			else {
				osg::Vec3d vecA = g_environment.get_logistics_node_position(&(iter->m_ref_start_node));			
				osg::Vec3d vecAC = iter->m_position - vecA;		
				osg::Vec3d vecAB = iter->m_destination - vecA;	
				if(vecAC.length()> vecAB.length()) 	{
					iter->mp_PAT->setPosition(iter->m_destination);
				}
				else  {
					iter->mp_PAT->setPosition(iter->m_position);		
				}
			}					
		}		
	}	
	
	return true;
}

void CLogistics_operation::slay_marked_logistics_operatives(void)
{
	//std::size_t size1 = mv_logistics_operative.size();
	std::vector<CLogistics_operative>::iterator OP_iter;
	OP_iter = std::remove_if(mv_logistics_operative.begin(), mv_logistics_operative.end(), slay_Logistics_operative);
	mv_logistics_operative.erase(OP_iter, mv_logistics_operative.end());
	//std::size_t size2 = mv_logistics_operative.size();
}


///////////////////////////////////////////////////////////////////////////////////
// CLogistics_producer
///////////////////////////////////////////////////////////////////////////////////
CLogistics_producer::CLogistics_producer()
{ 
	m_producer_elapsed_time= 0; 
	m_mean= 1;
	m_standard_deviation= 0;
	m_production_type = ProductionType_Periodic;
	m_flow_rate_method = FlowRateMethod_Const;
	m_production_rate_method = ProductionRateMethod_Const;
	m_flow_expression_index = UINT_MAX;
	m_production_expression_index= UINT_MAX;
	m_infinite_supply = true;
}	

void CLogistics_producer::set_total_production_period(double period)
{
	m_total_production_period = period;
}



void CLogistics_producer::update_production_rate(void)
{
	switch(m_production_rate_method)
	{			
		case ProductionRateMethod_Bernoulli:
		{
			double m_probability;

			Mersenne_Twister mersenne_twister;
			mersenne_twister.seed(rand());
			Bernoulli_Distribution distribution(m_probability);
			Bernoulli_variate_generator  gen(mersenne_twister,distribution);
			m_total_production_period= gen();
			break;
		}
		case ProductionRateMethod_Normal:
		{
			Mersenne_Twister mersenne_twister;
			mersenne_twister.seed(rand());
			Normal_Distribution distribution(m_mean,m_standard_deviation);
			Normal_variate_generator  gen(mersenne_twister,distribution);
			m_total_production_period= gen();
			break;
		}
		case ProductionRateMethod_EXPR:
		{
			if(m_ref_prod_EXPR.m_index < g_environment.mv_EXPR.size())	{
				CEXPR * p_EXPR = &(g_environment.mv_EXPR[m_ref_prod_EXPR.m_index]);
				m_total_production_period = p_EXPR->evaluate();

				if(m_total_production_period<= 0)
					m_total_production_period = 0;
			}		
			break;
		}
	}
}

double CLogistics_producer::get_production_period(void)
{
	return m_total_production_period;
}

double CLogistics_producer::get_height_from_angle_and_volume(double angle_rad)
{
	return (3* m_total_volume * tan(angle_rad))/3.14159;
}
///////////////////////////////////////////////////////////////////////////////////
// CLogistics_demand
///////////////////////////////////////////////////////////////////////////////////
CLogistics_demand::CLogistics_demand()
{
	m_demand_elapsed_time= 0;
	m_initialise_active_time = false;
	m_mean= 1;
	m_standard_deviation= 0;
	m_demand_rate_method = DemandRateMethod_Const;
	m_order_type = OrderType_Continuous;
}

double CLogistics_demand::get_demand_period(void)
{
	return m_period;
}

void CLogistics_demand::set_demand_period(double period)
{
	m_period = period;
}


void CLogistics_demand::update_demand_period(void)
{
	switch(m_demand_type)
	{
		case DemandType_Stochastic:
		{
			switch(m_demand_rate_method)
			{			
				case DemandRateMethod_Normal:
				{			
					Mersenne_Twister mersenne_twister;
					mersenne_twister.seed(rand());
					Normal_Distribution distribution(m_mean,m_standard_deviation);
					Normal_variate_generator  gen(mersenne_twister,distribution);
					m_period= gen();
					while(m_period<= 0)	{ m_period= gen(); }
					break;
				}
			}
			break;
		}
		case DemandType_EXPR:
		{
			if(m_ref_EXPR.m_index < g_environment.mv_EXPR.size())	{
				CEXPR * p_EXPR = &(g_environment.mv_EXPR[m_ref_EXPR.m_index]);
				m_period = p_EXPR->evaluate();

				if(m_period<= 0)
					m_period = 0;
			}		
			break;
		}
	}
}

std::size_t CLogistics_node::get_PAT_index(void)
{
	return m_ref_PAT.m_index;
}
std::size_t CLogistics_node::get_geo_index(void)
{
	return m_ref_geo.m_index;

	/*switch(m_Logistics_node_type)
	{
		case(Logistics_nodeType_Node)
		{
			return m_ref_geo.m_index;
			break;
		}
	}*/
}

bool slay_order(const CLogistics_order &order)
{
	if(order.m_slay == true)
		return true;
	else return false;
}

osg::Vec3d CLogistics_node::get_position(void)
{
	osg::Vec3d vec;
	CPositionAttitudeTransform* p_PAT = g_environment.get_PAT(&m_ref_PAT);	

	if(p_PAT!= NULL)
	{
		vec = p_PAT->mp_PAT->getPosition();	
	}

	return vec;

}
void CLogistics_node::slay_flagged_orders(void)
{
	std::vector<CLogistics_order>::iterator order_iter;
	order_iter = std::remove_if(mv_order.begin(), mv_order.end(), slay_order);
	mv_order.erase(order_iter, mv_order.end());	
}





//        ...    ....................  ......................   ..               .
//                            .......,.. .......................                ..
//                      ....       ..=:...  .................                  ...
//                        ..       ...I...  ...............                    ...
//    ..                    ..........+.......  ...........                .......
//                 .....  .............=I.....     ..........                .....
//               ..,I.....................+:..     ..........           ..........
//               ....?....................+I..     ....                ...........
//                 ...$...................++...    .....              ............
//                 ....7~.................+?...    ......             ............
//                 ......$:...............+?...    ......       ...........  .....
//.                   .....~=.............~+. .   .....    .......................
//.                  .......~=............+$. .          ... ..~I~... .....,+. ...
//                   .........$7..........I7. .        .....,+IZ:.,?O?...I=... ...
//       .           ...  .....I?I........I7.............~?7,........ .... . ..  .
//       .                 .....,7+......=7$...........7?7=................       
//      ..                 ......+77....,$O..........7I:...................       
//..    ..     ..            .....?$....+Z+.........7$$.........                  
//.                ..   ..........?I$...$O..=IO$$?.,ZI,.....                      
//..         .....................,I$...Z8..=$7ZOZ.7I7......    ....              
//..         ...........+I7+?,.....+OD..ZD.ZZZN7I?OII.......    ....           .  
//           ......:~?+?I..?7?II7...II=ZO8+IO7$7$$Z+.................             
//        .....,I?$+,.........=7II=.=:I8O?O88OO?OO+....:,,............           .
//        ...+7....  ...........,8$I$7Z77ZNN$8ZOZ.,:OIIIZ$$II.........           .
//       ..IZ..................IZ$O7??OOZDZN8?IZI$Z7Z?+.....+I?+........         .
//. ......O,...    .........Z8OZZZO?O?7O$ZD$O+IZ7=~,...........I?=......          
//.....,O+.. ..    ......ZOZ8~,...7:~~O=IO877??I$,... ...........?7=....          
//...$:.......    .....77Z.......=I.,O7~?7O$?=Z?77Z....   .........O+......       
//..            .....,?$......... .....:~7,..~7?~+$O...     .........8,......     
//            .....=$7..................==+...I+..,ZZ....    ..........Z:.... .   
//           ....:IO.. ..................~?:..7+....7$...    ..........,.=IZO$.   
//        ......IO........................... I~....+7....      .............     
//        .....I............              ... 7.....??.....       .............   
//        ....7....     ....              .. ,+.....,?.....       .............   
//        ...O,.                     ..   ...??......?,......       ...........   
//       ...8....                         ...O~......?I. .......      . ......... 
//      ..,Z.....                        ....Z.......I?.........       .......... 
//     ...$....                          ....Z........$..........     ........... 
//    ,~?.... .                          ....7........7...............     ..     
//   .+.. ..                             ...,7........O...............            
//.  ...                                 ...~I........O:..............            
//. ..                                   ...I, .......I:................          
//.                                      .....    .. .:?......Matt J. Stephens
//.                                      .....    ..  .$...........2011       
//..             ....              ..                ..7......................... 
//                                                    .=~......................   
//                                                     ........................   
//                                                         ....................  .
                                                               
