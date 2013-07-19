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

#ifndef _INCL_OSG_Logistics_elements
#define _INCL_OSG_Logistics_elements

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>

#include "OSG_Reference.h"
#include "OSG_time.h"
#include "OSG_BOOST_random.h"

enum eLogistics_operativeType { Logistics_operativeType_Node, Logistics_operativeType_Group, Logistics_operativeType_Demand, Logistics_operativeType_Source, Logistics_operativeType_Component, Logistics_operativeType_Geometry };
enum eLogistics_operativeUpdateType { Logistics_operativeUpdateType_Instantaneous, Logistics_operativeUpdateType_Linear,  Logistics_operativeUpdateType_Circular, Logistics_operativeUpdateType_Parametric, Logistics_operativeUpdateType_Custom };
enum eDemandType { DemandType_Periodic, DemandType_Stochastic, DemandType_EXPR  };
enum eProductionType { ProductionType_Periodic, ProductionType_Synthesis, ProductionType_Stochastic, ProductionType_PeriodicVolume, ProductionType_EXPR  };

enum eProductionRateMethod { ProductionRateMethod_Const, ProductionRateMethod_EXPR, ProductionRateMethod_Bernoulli, 
	ProductionRateMethod_Geometric, ProductionRateMethod_Triangle, ProductionRateMethod_Exponential, ProductionRateMethod_Normal, ProductionRateMethod_LogNormal };

enum eDemandRateMethod { DemandRateMethod_Const, DemandRateMethod_EXPR, DemandRateMethod_Bernoulli, 
	DemandRateMethod_Geometric, DemandRateMethod_Triangle, DemandRateMethod_Exponential, DemandRateMethod_Normal, DemandRateMethod_LogNormal };

enum eFlowRateMethod { FlowRateMethod_Const, FlowRateMethod_EXPR, FlowRateMethod_Bernoulli, 
	FlowRateMethod_Geometric, FlowRateMethod_Triangle, FlowRateMethod_Exponential, FlowRateMethod_Normal, FlowRateMethod_LogNormal };

enum eLogistics_nodeType { Logistics_nodeType_Node, Logistics_nodeType_Group, Logistics_nodeType_Geode };
enum eLogistics_operativeState { 
	
	Logistics_operativeState_under_construction,
	Logistics_operativeState_in_storage,
	Logistics_operativeState_in_transit_destroy_at_destination,
	Logistics_operativeState_in_transit_store_at_destination,
	Logistics_operativeState_slay
};

enum eComponentType { ComponentType_Node, ComponentType_Group, ComponentType_Geode };

enum eOrderType { OrderType_DES, OrderType_Continuous};

enum eTransportMode { TransportMode_BeeLine, TransportMode_Route };

class CLogistics_order;//class forward
class CLogistics_operative;//class forward

bool slay_order(const CLogistics_order &order);
bool slay_Logistics_operative(const CLogistics_operative &Logistics_operative);


///////////////////////////////
//CLogistics_entity
///////////////////////////////
class CLogistics_entity
{
public:		
	CLogistics_entity() { m_slay = false; }
	unsigned m_unique_id;
	bool m_slay;
	std::string m_name;	
};

///////////////////////////////
//COperativeRef
///////////////////////////////
class COperativeRef
{
public:
	std::size_t m_operation_index;
	std::size_t m_operative_index;
};

///////////////////////////////
//CLogistics_order
///////////////////////////////
class CLogistics_order : public CLogistics_entity
{
public:		
	long m_units;
	CReference m_ref_demand_node;
	std::vector<CReference> mv_ref_source_node;
	eLogistics_operativeState m_op_state;
	eOrderType m_order_type;
	std::size_t m_component_index;
	std::vector<std::size_t> mv_component_index;
};

///////////////////////////////
//CLogistics_NodeState
///////////////////////////////
class CLogistics_NodeState
{
public:
	CLogistics_NodeState();
	bool m_in_transit;
	std::size_t m_logistics_node_index;
	std::size_t m_operation_node_index;
	std::size_t m_operative_count;
	//std::vector<std::size_t> mv_operative_index;
};

///////////////////////////////
//CLogistics_throughput
///////////////////////////////
class CLogistics_throughput
{
public:	
	CLogistics_throughput();
	unsigned int m_node;
	eComponentType m_component_type;
	float m_mass;
	double m_time;
	double m_sim_time;
	double m_real_time;
	int time_type;	
};

///////////////////////////////
//CLogistics_node
///////////////////////////////
class CLogistics_node : public CLogistics_entity
{
public:	
	CLogistics_node();
	std::size_t get_PAT_index(void);
	std::size_t get_geo_index(void);
	osg::Vec3d get_position(void);	
	void slay_flagged_orders(void);	

	eLogistics_nodeType m_Logistics_node_type;
	CReference m_ref_PAT;	
	CReference m_ref_geo;	
	bool m_record;	
	std::vector<CLogistics_throughput> mv_throughput;
	std::vector<CLogistics_order> mv_order;	
};

///////////////////////////////
//CLogistics_route
///////////////////////////////
class CLogistics_route : public CLogistics_entity
{
public:		
	CReference m_ref_LNodeA;	
	CReference m_ref_LNodeB;	
};

///////////////////////////////
//CLogistics_particle
///////////////////////////////
class CLogistics_particle : public CLogistics_entity
{
	public:
		CLogistics_particle();	
		void update_position(double time);

		float m_mass;
		float m_density;
		osg::Vec3d m_position;
		osg::Vec3d m_velocity;
		osg::Vec3d m_acceleration;
		osg::Vec3d m_jerk;		
		osg::Quat m_Qorientation;
		osg::Quat m_Qangular_velocity;
		osg::Quat m_Qangular_acceleration;	
		osg::Vec3d m_angular_axis;
		float m_angular_speed;	
		float m_angular_acceleration;
		float m_angular_jerk;				
};

///////////////////////////////
//CLogistics_operative
///////////////////////////////
class CLogistics_operative : public CLogistics_particle
{
public:	
	CLogistics_operative();

	eLogistics_operativeType m_Logistics_operative_type;	
	eLogistics_operativeUpdateType m_log_op_update_type;
	eLogistics_operativeState m_state;
	eOrderType m_order_type;
	osg::ref_ptr<osg::PositionAttitudeTransform> mp_PAT;
	CReference m_ref_geode;
	CReference m_ref_Logistics_operative_type;
	CReference m_ref_rest_node;	
	CReference m_ref_start_node;
	CReference m_ref_destination_node;
	std::size_t m_logistics_component_index;
	std::size_t m_component_index;
	std::size_t m_origin_node_index;
	std::size_t m_group_index;
	osg::Vec3d m_destination;
	double m_distance;
	double m_time_task_complete;
	double m_task_duration;
	double m_task_elapsed_time;
	eTransportMode m_transport_mode;
	bool set_on_route(eLogistics_operativeState op_state, eTimeType time_type, float speed);
};

///////////////////////////////
//CLogistics_operation
///////////////////////////////
class CLogistics_operation : public CLogistics_entity
{
public:
	CLogistics_operation();	
	double m_start_time;
	bool m_write_to_file;
public:		
	std::size_t Logistics_operative_index_from_name(std::string name);		
	std::size_t Logistics_operative_index_from_unique_ID(unsigned ID);	
	bool arrived_at_destination(CLogistics_operative *p_operative);
	void update_throughput(CLogistics_operative *p_operative, std::size_t index, int type);
	CReference add_logistics_operative(CLogistics_operative Logistics_operative);
	CLogistics_operative* get_Logistics_operative(CReference* p_ref);
	CReference get_reference(CLogistics_operative* p_Logistics_operative);
	bool update_operation(double delta_time);
	void slay_marked_logistics_operatives(void);	
	void write_throughput_to_file(CLogistics_throughput* p_throughput);
	void write_env_state_to_file(std::string file_name);
	std::size_t number_of_operatives_at_node(std::size_t node_index);		
	double mass_of_operatives_at_node(std::size_t node_index);		
	std::size_t number_of_operatives_in_transit(void);		
	double mass_of_operatives_in_transit(void);		
	std::vector<CLogistics_operative>* get_Logistics_operative_vector(void)
	{
		return &mv_logistics_operative;
	}
private:
	std::vector<CLogistics_operative> mv_logistics_operative;
	unsigned m_DELogistics_operative_unique_id;

};

///////////////////////////////
//CLogistics_component
///////////////////////////////
class CLogistics_component : public CLogistics_entity//where an Logistics_operative is an instance of a component
{
public:	
	CLogistics_component();
	std::string m_description;
	eComponentType m_component_type;
	eTransportMode m_transport_mode;
	CReference m_ref_component_geometry;
	std::vector<CReference> mv_sub_component;
	float m_mass;
	float m_density;
};

///////////////////////////////
//CLogistics_producer
///////////////////////////////
class CLogistics_producer : public CLogistics_entity 
{
private:
	double m_total_production_period;	
public:
	CLogistics_producer(); 

	eProductionType m_production_type;

	eProductionRateMethod m_production_rate_method;
	eFlowRateMethod m_flow_rate_method;
	
	CReference m_ref_node;		
	CReference m_ref_component;		
	CReference m_ref_prod_EXPR;
	CReference m_ref_flow_EXPR;

	std::string m_str_production_type;
	std::size_t m_flow_expression_index;
	std::size_t m_production_expression_index;
	std::string m_str_random_distribution_type;
	
	double m_rand_min;
	double m_rand_max;
	double m_design_period;
	double m_construction_period;	
	double m_producer_elapsed_time;	
	double m_volume;//TODO: deprecate - use mass instead
	double m_total_volume;//TODO: deprecate - use mass instead
	double m_mean;
	double m_standard_deviation;	
	double m_total_mass;
	bool m_infinite_supply;
	
	double get_height_from_angle_and_volume(double angle_rad);
	void set_total_production_period(double period);	
	double get_production_period(void);	
	void update_production_rate(void);
};

//note also sub components can drive demand..
class CLogistics_demand : public CLogistics_entity
{
private:
	double m_period;
public:
	CLogistics_demand();	
	CReference m_ref_component;
	CReference m_ref_node;
	CReference m_ref_EXPR;

	eLogistics_operativeState m_op_state;
	eOrderType m_order_type;
	eTimeType m_init_active_time_type;
	eDemandType m_demand_type;
	eDemandRateMethod m_demand_rate_method;

	std::string m_type;	
	std::string m_str_demand_type;	
	std::string m_str_random_distribution_type;
	std::vector<CReference> mv_ref_source_node;
	std::size_t m_expression_index;

	bool m_initialise_active_time;
	double m_rand_min;
	double m_rand_max;	
	double m_time;
	double m_demand_elapsed_time;	
	double m_mean;
	double m_standard_deviation;	

	double get_demand_period(void);
	void set_demand_period(double period);
	void update_demand_period(void);
};

class CDiscreteEvent
{
public:
	CDiscreteEvent() { m_slay = false; }
	bool m_slay;
	std::string m_description;
	double m_time;
};



#endif