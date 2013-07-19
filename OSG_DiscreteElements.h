///////////////////////////////
//
// Matt Jim. Stephens 
// Geelong, Australia
// 2012
//
// Discrete elements
/////////////////////// 
#ifndef _INCL_OSG_DiscreteElements
#define _INCL_OSG_DiscreteElements



#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>

#include "OSG_Reference.h"
#include "OSG_time.h"

enum eLogistics_operativeType { Logistics_operativeType_Node, Logistics_operativeType_Group, Logistics_operativeType_Demand, Logistics_operativeType_Source, Logistics_operativeType_Component, Logistics_operativeType_Geometry };
enum eLogistics_operativeUpdateType { Logistics_operativeUpdateType_Instantaneous, Logistics_operativeUpdateType_Linear,  Logistics_operativeUpdateType_Circular, Logistics_operativeUpdateType_Parametric, Logistics_operativeUpdateType_Custom };
enum eDemandType { DemandType_Periodic, DemandType_Random  };
enum eProductionType { ProductionType_Periodic, ProductionType_Random  };
enum eRandomDistributionType { RandomDistributionType_Linear, RandomDistributionType_Gaussian  };

enum eLogistics_nodeType { Logistics_nodeType_Node, Logistics_nodeType_Group, Logistics_nodeType_Geode };
enum eLogistics_operativeState { 
	
	Logistics_operativeState_under_construction,
	Logistics_operativeState_in_storage,
	Logistics_operativeState_in_transit_destroy_at_destination,
	Logistics_operativeState_in_transit_store_at_destination,
	Logistics_operativeState_at_destination,
	Logistics_operativeState_slay
};

enum eComponentType { ComponentType_Node, ComponentType_Group, ComponentType_Geode };

class CLogistics_entity
{
public:		
	CLogistics_entity() { m_slay = false; }
	unsigned m_unique_id;
	bool m_slay;
	std::string m_name;
	CTimeFootprint m_foot_print;
};

class CLogistics_order : public CLogistics_entity
{
public:		
	long m_units;
	CReference m_ref_demand_node;
	eLogistics_operativeState m_op_state;	
};

class CLogistics_node : public CLogistics_entity
{
public:	
	eLogistics_nodeType m_Logistics_node_type;
	CReference m_ref_PAT;	
	CReference m_ref_geo;	
	std::vector<CLogistics_order> mv_order;
	std::size_t get_PAT_index(void);
	std::size_t get_geo_index(void);
	//std::vector<CReference> mv_ref_Logistics_operatives_in_storage;
	osg::Vec3d get_position(void);
	void slay_flagged_storage(void);
	void slay_flagged_orders(void);	
};

class CLogistics_particle : public CLogistics_entity
{
	public:
		CLogistics_particle();		
		double m_mass;
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
		void update_position(double time);
};

class CLogistics_operative : public CLogistics_particle
{
public:	
	CLogistics_operative();
	eLogistics_operativeType m_Logistics_operative_type;	
	eLogistics_operativeUpdateType m_update_type;
	eLogistics_operativeState m_state;
	osg::ref_ptr<osg::PositionAttitudeTransform> mp_PAT;
	CReference m_ref_geode;
	CReference m_ref_Logistics_operative_type;
	CReference m_ref_rest_node;	
	CReference m_ref_start_node;
	CReference m_ref_destination_node;
	osg::Vec3d m_destination;		
	double m_distance;
	double m_time_task_complete;
	double m_task_duration;
	double m_task_elapsed_time;
	bool set_on_route(eLogistics_operativeState op_state, eTimeType time_type, float speed);
};

bool slay_Logistics_operative(const CLogistics_operative &Logistics_operative);

class CLogistics_operation : public CLogistics_entity
{
public:
	CLogistics_operation();	
	double m_start_time;
public:		
	std::size_t Logistics_operative_index_from_name(std::string name);		
	std::size_t Logistics_operative_index_from_unique_ID(unsigned ID);	
	//bool set_on_route(CReference* p_ref_start_node, CReference* p_ref_destination_node, CReference* p_ref_Logistics_operative, eLogistics_operativeState op_state, float speed, double elapsed_time);
	CReference add_Logistics_operative(CLogistics_operative Logistics_operative);
	CLogistics_operative* get_Logistics_operative(CReference* p_ref);
	CReference get_reference(CLogistics_operative* p_Logistics_operative);
	bool update_operation(double delta_time);
	void slay_marked_Logistics_operatives(void);
	eTimeType m_time_type;	
	std::vector<CLogistics_operative>* get_Logistics_operative_vector(void)
	{
		return &mv_Logistics_operative;
	}
private:
	std::vector<CLogistics_operative> mv_Logistics_operative;
	unsigned m_DELogistics_operative_unique_id;

};

class CLogistics_product : public CLogistics_entity//where an Logistics_operative is an instance of a component
{
public:	
	std::string m_description;
	eComponentType m_component_type;
	CReference m_ref_component_geometry;
	std::vector<CReference> mv_sub_component;//components required to build this Logistics_operative
};

bool slay_order(const CLogistics_order &order);

class CLogistics_producer : public CLogistics_entity 
{
public:
	CLogistics_producer() { m_local_elapsed_time= 0; }	
	
	CReference m_ref_node;		
	CReference m_ref_component;		
	std::string m_type;
	std::string m_str_random_distribution_type;
	eRandomDistributionType m_random_distribution_type;
	eProductionType m_production_type;
	std::string m_str_production_type;
	double m_rand_min;
	double m_rand_max;
	double m_design_period;
	double m_construction_period;
	double m_total_production_period;
	eTimeType m_time_type;	
	double m_local_elapsed_time;	
};

//note also sub components can drive demand..
class CLogistics_demand : public CLogistics_entity
{
public:
	CLogistics_demand() { m_local_elapsed_time= 0; }
	
	CReference m_ref_component;
	CReference m_ref_node;
	std::string m_type;	
	std::string m_str_demand_type;
	eDemandType m_demand_type;
	std::string m_str_random_distribution_type;
	eRandomDistributionType m_random_distribution_type;
	double m_rand_min;
	double m_rand_max;
	std::vector<CReference> mv_ref_source_node;
	double m_time;
	double m_period;
	eTimeType m_time_type;	
	double m_local_elapsed_time;
	eLogistics_operativeState m_op_state;
};


class CLogistics_state_change : public CLogistics_entity
{
public:		
	std::string m_description;
	CReference m_ref_Logistics_operative;
};

class CLogistics_state_change_list : public CLogistics_entity
{
public:		
	double m_time;
	std::vector<CLogistics_state_change> mv_state_change_list;
};
#endif