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

#ifndef _INCL_OSG_Event
#define _INCL_OSG_Event

#include "OSG_DES.h"
#include "OSG_Environment.h"
#include "OSG_expression.h"
#include "XML_EXPR.h"
#include "OSG_time.h"
#include "NxVec3.h"
#include "NxQuat.h"

using namespace osg;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////
// Enumeration
////////////////////////////////////////////////////////////////////////////////////////////////
enum eEventType { 	
	EventType_Unknown, 
	EventType_AlterOrientation, 
	EventType_AlterPosition, 
	EventType_AlterText, 
	EventType_BoostASIO, 
	EventType_CreatePhysXActor, 	
	EventType_CreateProcess, 	
	EventType_CreateEntity, 	
	EventType_CreateWave, 	
	EventType_Create3ds, 	
	EventType_DE_Logistics, 
	EventType_DiscreteEventSimulation,
	EventType_Empty, 	
	EventType_Emitter, 	
	EventType_GetEntityDetails, 
	EventType_Innate, 
	EventType_LoadScript, 
	EventType_Math, 
	EventType_MoveGlobal, 	
	EventType_Render, 
	EventType_ReplaceDrawable,
	EventType_Pipe, 
	EventType_Python, 
	EventType_PlayWav, 
	EventType_Ruby, 
	EventType_SaveEventTree, 
	EventType_SaveEnvironmentTree, 
	EventType_Sequencer, 	
	EventType_Sensor,	
	EventType_ScreenGrab,
	EventType_SMTP,
	EventType_Socket,
	EventType_TheatreOfOperations, 	
	EventType_UpdateGraph, 
	EventType_UpdateNxActor, 	
	EventType_UpdatePositionParametricExpression, 		
	EventType_UpdateSimulationTime, 	
	EventType_UpdateText, 
	EventType_UpdateVariable,
	EventType_UpdateViewer,
	EventType_UpdateViewerAnimate, 
	EventType_UpdateVelocity
	
};



enum eSimTimeUpdateType { SimTimeUpdateType_Periodic, SimTimeUpdateType_RefList, SimTimeUpdateTypeRealTime };

enum eAlterString { AlterString_elapsed_time, 
	AlterString_elapsed_simulation_time, 
	AlterString_PAT,
	AlterString_Camera, 
	AlterString_node_throughput,
	AlterString_node_mass,
	AlterString_Socket,
	AlterString_TimeMode };

enum eSource_LotType { Source_LotType_One, Source_LotType_Random, Source_LotType_Gausian, Source_LotType_Expression };
enum eSourceType { SourceType_Geode, SourceType_Logistics_node };
enum eSourceStackMethod { SourceStackMethod_vertical,  SourceStackMethod_horizontal, SourceStackMethod_random};
enum eViewerUpdateType { ViewerUpdateType_UpdateStats, ViewerUpdateType_UpdateCamera };
enum eCameraUpdateType { CameraUpdateType_AnimationPath, CameraUpdateType_ScrollThroughControlPoints, CameraUpdateType_SetProjectionMatrix, CameraUpdateType_SetViewMatrix, CameraUpdateType_SetViewMatrixLookAt,  CameraUpdateType_FollowPath};

enum eInception { 
	Inception_Innate, 
	Inception_Date, 
	Inception_Periodic,
	Inception_Spontaneous, 
	Inception_MemMap_Conditional, 
	Inception_Conditional 
};
//enum eInception { eInception_instantaneous, eInception_periodic, eInception_conditional };
//conditions: time elapsed, sensor, semaphore, expression

enum eOccurance { Occurance_discrete, Occurance_interpolate };
enum eRecurrance { Recurrance_singular, Recurrance_perpetual };
enum eClimax { Climax_immediate, Climax_conditional };
enum eConcurrancy { Concurrancy_polling, Concurrancy_threaded };


enum eSubordination { 
	Subordination_None, 
	Subordination_Innate,
	Subordination_Simultaneous,
	Subordination_Sequential, 
	Subordination_PeriodicSimultaneous,
	Subordination_PeriodicSequential, 
};

enum eConsequence { eConsequence_none, eConsequence_create_entity, eConsequence_create_event, eConsequence_create_process, eConsequence_destroy_entity, eConsequence_destroy_event, eConsequence_destroy_process};

enum eInterpolatorType { InterpolatorType_int, InterpolatorType_float, InterpolatorType_double, InterpolatorType_vec3d };

string event_type_to_string(eEventType eT);
eEventType string_to_event_type(std::string str);

bool create_mapped_memory(std::string mem_map_name, unsigned size);
bool read_char_mapped_memory(std::string mem_map_name, unsigned index, char c);
bool read_string_mapped_memory(std::string mem_map_name, unsigned index, std::string* p_str, unsigned size);
bool write_char_mapped_memory(std::string mem_map_name, unsigned index, char c);
bool write_string_mapped_memory(std::string mem_map_name, unsigned index, std::string str, unsigned size);
/////////////////////////////////////////////////////////////////////////////////////////////////
// classes
/////////////////////////////////////////////////////////////////////////////////////////////////
class CCameraLocation
{
public:
	Vec3d m_position;
	float m_yaw;
	float m_pitch;
	float m_roll;
};

class CLogisticsDemandInfo
{
public:
	std::size_t m_Logistics_demand_index;
	std::size_t m_demand_node_index;
	std::size_t m_demand_node_PAT_index;
	osg::Vec3d m_demand_node_position;
};


/////////////////////////////////////////////////////////////////////////////////////////////////
// Event base classes
/////////////////////////////////////////////////////////////////////////////////////////////////
class CEventDesign
{
public:
	CEventDesign();
	eEventType m_type;

	eInception m_inception;
	eOccurance m_occurance;
	eRecurrance m_recurrance;
	eConcurrancy m_concurrance;
	eSubordination m_subordination;
	std::vector<eConsequence> mv_consequence;

	CEXPR m_inception_condition;
	CEXPR m_climax_condition;
	
	float m_orientation;
	float m_angular_velocity;
	float m_angular_acceleration;
	float m_mass;
	double m_elapsed_simulation_time;
	double m_elapsed_simulation_time_increment;
	double m_start_time;
	double m_end_time;
	double m_delta_time;
	double m_period;
	osg::Vec3d m_origin;
	osg::Vec3d m_position;
	osg::Vec3d m_velocity;
	osg::Vec3d m_acceleration;
	osg::Vec3d m_orientation_axis;	
	osg::Vec4d m_Qorientation;	
	std::string m_str_inception_condition;
	std::string m_str_climax_condition;
	std::string m_str_interpolator_type;
	std::string m_alter_string;
	std::string m_file_name;
	std::string m_name;
	std::string m_ref_name;	
	std::string m_str_time_type;
	std::string m_geode_name;
	std::string m_PAT_name;
	std::string m_Logistics_node_name;
	std::string m_source_lot_type;
	std::string m_source_type;
	std::string m_viewer_update_type;
	std::string m_camera_update_type;
	std::string m_str_sim_time_update_type;	
	std::vector<std::string> mv_str_expression;		
	std::vector<std::string> mv_str_climax_varibles;
	std::vector<std::string> mv_varible_association;
	std::vector<std::string> mv_response_xml;	
	std::vector<CVarAssoc> mv_assoc_var;
	std::vector<CReference> mv_ref;	
	std::vector<CParsedVariable> mv_variable;
	std::vector<CEXPR> mv_expr;

};

class CEventNymph
{

private:
	double m_local_elapsed_time;
	bool m_periodic_update;
public:
	CEventNymph();	
	string m_name;
	eEventType m_type;

	bool m_associateyVy;//deprecate
	vector<CVarAssoc> mv_assoc_var;

	CExpressionVariableList m_expression_variables;
	CExpressionList m_climax_condition_expression_list;	

	eInception m_inception;	
	eOccurance m_occurance;
	eRecurrance m_recurrance;
	eConcurrancy m_concurrance;
	eSubordination m_subordination;
	std::vector<eConsequence> mv_consequence;
	
	CEXPR m_inception_condition;
	CEXPR m_climax_condition;	
	
	std::size_t m_climax_expression_index;
	char m_mem_test;
	std::string m_mem_map_str;
	std::size_t m_mem_map_index;
	unsigned m_mem_map_address;
	vector<string> mv_response_xml;
	
	
	virtual bool Create(CEventDesign* p_design, CEnvironment* p_env)=0;
	virtual bool Update(CEnvironment* p_env)=0;	
};

class CEventCrysalis
{
	public:
		CEventCrysalis();	
		std::string m_crysalis_name;
		CEventNymph *mp_event_nymph;		
		bool m_slay;
		vector<CEventCrysalis> mv_innate;
		vector<CEventCrysalis> mv_child;
		vector<CEventCrysalis>::iterator m_event_crysalis_iter;
		bool Morph(CEventDesign* p_design, CEnvironment* p_env);
		bool LinkAssocVariables(CEventDesign* p_design, CEnvironment* p_env);
		bool Update(CEnvironment* p_env);		
		bool UpdateChild(CEnvironment* p_env, vector<CEventCrysalis>::iterator event_crysalis_iter, std::size_t index);
		bool UpdateAllChildren(CEnvironment* p_env);
		DWORD m_active_child_index;
		double m_period;		
		double m_local_elapsed_time;
		std::vector<string> mv_xml_file_name;
		std::vector<std::size_t> mv_self_destruct_index;	
		bool m_debug_info;
		void slay_marked_events(void);
};
class CEventHandler
{
	private:
		bool XML_ParseEvent(CComPtr<IXMLDOMNode> spXMLNode, CEnvironment* p_env, CEventCrysalis* p_event);
	public:
		CEventHandler();
		bool XML_EventParser(string str, CEnvironment* p_env, CEventCrysalis* p_event_crysalis);
		CEventCrysalis m_innate_events;			
		CEventCrysalis m_event;		
		bool InnateCreate(CEnvironment* p_env);
		bool Update(CEnvironment* p_env);
		bool InnateUpdate(CEnvironment* p_env);
		void insert(std::size_t index);
		void erase(std::size_t index);
		bool m_save_event_tree;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// slay_event_crysalis declaration
/////////////////////////////////////////////////////////////////////////////////////////////////
bool slay_event_crysalis(const CEventCrysalis &ec);


/////////////////////////////////////////////////////////////////////////////////////////////////
// Event polymorphs
/////////////////////////////////////////////////////////////////////////////////////////////////

class CEvent_AlterOrientation : public CEventNymph
{
public:	
	double m_lerp_coeff;
	double m_period;
	double m_elapsed_time;
	std::size_t m_index_PAT;
	osg::Quat m_start_orientation;
	osg::Quat m_end_orientation;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_AlterPosition : public CEventNymph
{
public:	
	double m_lerp_coeff;
	double m_period;
	double m_elapsed_time;
	std::size_t m_index_PAT;
	Vec3d m_start_position;
	Vec3d m_end_position;
	double m_elapsed_simulation_time;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_AlterText : public CEventNymph
{
public:
	osgText::Text *mp_text;
	eAlterString m_alter_string;
	std::size_t m_text_index;
	std::size_t m_subject_index;
	std::size_t m_logistics_node_index;
	std::size_t m_operation_index;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_BoostASIO : public CEventNymph
{
public:
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_CreatePhysXActor : public CEventNymph
{
public:
	CReference m_ref_PhysX_actor;
	CReference m_ref_Group;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_CreateWave : public CEventNymph
{
public:

	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_Create3ds : public CEventNymph
{
public:
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_DE_Logistics : public CEventNymph
{
private:	
	std::vector<CLogisticsDemandInfo>::iterator mDIIter;
	std::vector<CLogistics_node>::iterator mNodeIter;
	std::vector<CLogistics_demand>::iterator mDemandIter;
	std::vector<CLogistics_order>::iterator mOrderIter;		
	std::vector<CLogistics_operative>::iterator mLogOpIter;
	std::vector<COperativeRef>::iterator mOpRefIter;
	std::vector<CReference> ::iterator mRefIter;
	std::vector<std::size_t>::iterator mIter;
	CLogistics_demand* mp_demand;
	CLogistics_node* mp_node;
	CLogistics_producer * mp_producer;
	bool m_produce_new_operative;
public:	
	eTimeType m_time_type;	
	eComponentType m_component_type;	
	double m_local_elapsed_time;
	double m_speed;
	float m_stacking_distance;
	bool m_save_demand_info;
	std::string m_type; //real-time or simulation time
	std::string m_str_time_type;
	std::size_t m_group_index;
	std::size_t m_operation_index;
	std::size_t m_Logistics_producer_index;	
	std::size_t m_Logistics_component_index;

	std::vector<std::size_t> mv_logistics_source_node_index;	
	std::vector<std::size_t> mv_logistics_producer_index;	

	std::size_t m_producer_node_index;
	std::size_t m_producer_node_PAT_index;		
	std::size_t m_component_index;
	std::vector<std::size_t> mv_sub_component_index;
	osg::Vec3d m_last_pos;
	osg::Vec3d m_producer_node_position;	
	osg::Vec3d m_vec_AB;
	std::vector<CLogisticsDemandInfo>mv_logistics_demand_info;
	std::vector<std::size_t> mv_Logistics_component_PAT_index;

	bool Create(CEventDesign* p_design, CEnvironment* p_env);	
	bool Update(CEnvironment* p_env);

private:
	bool Link(CEventDesign* p_design, CEnvironment* p_env);	
	void update_time(CEnvironment* p_env);
	bool populate_order_list_by_demands(CEnvironment* p_env);
	bool produce_Logistics_operatives(CEnvironment* p_env);
	bool Produce_EXPR(CEnvironment* p_env);
	bool synthesis(CEnvironment* p_env);
	bool Stochastic(CEnvironment* p_env);
	bool periodic(CEnvironment* p_env);
	bool volume(CEnvironment* p_env);	
	bool explicate_orders(CEnvironment* p_env);
	bool stack_Logistics_operatives(CEnvironment* p_env);
	bool SetProducerPosition(CEventDesign* p_design, CEnvironment* p_env);
	bool SetComponentType(CEventDesign* p_design, CEnvironment* p_env);
	bool AddTransition(CEventDesign* p_design, CEnvironment* p_env);	
	bool ValidateOrder(CLogistics_order *p_order, CLogistics_operative *p_operative, std::size_t component_index, CEnvironment* p_env);	
	bool get_path(CEnvironment* p_env);
};

class CEvent_DiscreteEventSimulation : public CEventNymph
{
public:
	double m_elapsed_simulation_time_increment;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_CreateEntity : public CEventNymph
{
public:
	std::string m_entity_type;
	std::vector<std::size_t> mv_parsed_variable_index;	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_CreateProcess : public CEventNymph
{
public:
	std::string m_command_line;	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_Empty : public CEventNymph
{
public:	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_GetEntityDetails : public CEventNymph
{
public:	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_Innate : public CEventNymph
{
public:
	//double m_elapsed_simulation_time_increment;
	std::string m_str_memory_map;
	std::string m_type;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

//this CEmitterParticle must be declared before CEvent_Emitter
class CEmitterParticle
{
public:
	double m_elapsed_life_time;
	double m_life_time;

	osg::Vec3d m_position;
	osg::Vec3d m_velocity;
	osg::Vec3d m_acceleration;
	osg::ref_ptr<osg::PositionAttitudeTransform> mp_PAT;
	double m_mass;
};

class CEvent_Emitter : public CEventNymph
{
public:		
	std::string m_type;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
	std::vector<CEmitterParticle> mv_particle;	
	std::string m_node_name;
	std::string m_geode_name;
	std::string m_update_mode;	
	std::size_t m_field_index;
	std::size_t m_Vec3dArray_index;
	std::size_t m_expression_index;
	std::size_t m_EXPR_x_index;
	std::size_t m_EXPR_y_index;
	std::size_t m_EXPR_z_index;
	std::size_t m_node_index;
	std::size_t m_geode_index;
	std::size_t m_PAT_index;
	std::size_t m_group_index;
	double m_global_life_time;
	double m_period;
	double m_period_elapsed_time;
	double m_mass;
	double m_origin_randomness;
	double m_intial_velocity_randomness;
	osg::Vec3d m_origin;
	osg::Vec3d m_position;
	osg::Vec3d m_velocity;
	osg::Vec3d m_acceleration;
	CMDEXPR m_md_expr;
};

class CEvent_LoadScript : public CEventNymph
{
public:	
	std::string m_file_name;	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_Math : public CEventNymph
{
public:	
	std::string m_process;
	std::string m_WithRespectTo;
	std::vector<CEXPR> mv_expr;
	std::string m_file_name;	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_MoveGlobal : public CEventNymph
{
public:	
	eTimeType m_time_type;	
	std::size_t m_index_actor;
	double m_local_elapsed_time;	
	NxVec3 m_p0;
	NxVec3 m_position;
	NxVec3 m_v0;
	NxVec3 m_velocity;	
	NxVec3 m_acceleration;	
	NxVec3 m_orientation_axis;
	float m_theta0;
	float m_orientation;
	float m_omega0;	
	float m_angular_velocity;
	float m_alpha0;	
	float m_angular_acceleration;	
	NxQuat m_Q0;
	NxQuat m_Qorientation;	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_Python : public CEventNymph
{
public:
	std::string m_key;
	std::string m_object;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_PlayWav : public CEventNymph
{
public:
	std::string m_file;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};



class CEvent_Ruby : public CEventNymph
{
public:
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_Render : public CEventNymph
{
public:
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_ReplaceDrawable : public CEventNymph
{
public:	
	std::string m_sub_type;	
	std::vector<std::size_t> mv_parsed_variable_index;	
	std::size_t m_geode_index;	
	std::size_t m_Logistics_producer_index;	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};
class CEvent_SaveEventTree : public CEventNymph
{
public:
	string m_file_name;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};
class CEvent_SaveEnvironmentTree : public CEventNymph
{
public:
	string m_file_name;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_ScreenGrab : public CEventNymph
{
public:
	double m_period;
	std::string m_file_name;
	std::string m_file_extension;
	CReference m_ref_image;
	unsigned m_height;
	unsigned m_width;
	unsigned m_bit_color;
	unsigned m_count;	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};
class CEvent_Sensor : public CEventNymph
{
public:
	std::string m_type;
	std::string m_sub_type;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};
class CEvent_Sequencer : public CEventNymph
{
public:
	double m_period;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};


class CEvent_SMTP : public CEventNymph
{
public:
	bool m_transport_layer_security;
	bool m_secure_socket_layer;
	bool m_VRFY;
	bool m_alter_IO_mode;
	std::string m_host_id;
	int m_port;
	std::string m_str_from;
	std::string m_str_to;
	std::string m_str_data;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};


class CEvent_Socket : public CEventNymph
{
public:
	std::string m_type;
	std::size_t m_socket_index;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};


class CEvent_UpdateGraph : public CEventNymph
{
public:
	CReference m_ref_graph;
	CReference m_ref_logistics_node;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_UpdatePhysXActor : public CEventNymph
{
public:	
	eTimeType m_time_type;	
	std::size_t m_index_PAT;
	double m_local_elapsed_time;
	CReference m_ref_PhysXActor;
	Vec3d m_p0;
	Vec3d m_position;
	Vec3d m_v0;
	Vec3d m_velocity;	
	Vec3d m_acceleration;	
	Vec3d m_orientation_axis;
	float m_theta0;
	float m_orientation;
	float m_omega0;	
	float m_angular_velocity;
	float m_alpha0;	
	float m_angular_acceleration;	
	Quat m_Q0;
	Quat m_Qorientation;	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_UpdateSimulationTime : public CEventNymph
{
public:
	double m_elapsed_simulation_time_increment;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_UpdateText : public CEventNymph
{
public:
	osgText::Text *mp_text;
	eAlterString m_alter_string;
	std::size_t m_text_index;
	std::size_t m_mmc_index;	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_UpdateTheatreOfOperations : public CEventNymph
{
public:	
	std::size_t m_operation_index;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_UpdatePositionParametricExpression : public CEventNymph
{
public:	
	std::size_t  m_index_PAT;
	CLogistics_particle m_particle;
	std::size_t m_expr_x_index;
	std::size_t m_expr_y_index;
	std::size_t m_expr_z_index;
	CExpressionVariableList m_expression_variables_x;
	CExpressionVariableList m_expression_variables_y;
	CExpressionVariableList m_expression_variables_z;
	CExpressionList m_expression_list_x;	
	CExpressionList m_expression_list_y;	
	CExpressionList m_expression_list_z;	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_UpdateVariable : public CEventNymph
{
public:	
	std::string m_type;
	std::string m_entity;
	std::string m_ref_name;
	std::string m_variable_type;
	std::string m_value;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

class CEvent_UpdateViewer : public CEventNymph
{
public:
	CEvent_UpdateViewer() { m_local_elapsed_time=0; }
	bool m_animation_active;
	CReference m_ref_animation_path;
	eViewerUpdateType m_viewer_update_type;
	eCameraUpdateType m_camera_update_type;
	std::size_t m_active_viewer_index;
	std::vector<CCameraLocation> mv_location;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
	double m_local_elapsed_time;
	std::size_t m_index_location;
};

class CEvent_UpdateViewerAnimate : public CEventNymph
{
public:
	CEvent_UpdateViewerAnimate() { m_local_elapsed_time=0; }
	bool m_animation_active;
	CReference m_ref_animation_path;
	eViewerUpdateType m_viewer_update_type;
	eCameraUpdateType m_camera_update_type;
	std::size_t m_active_viewer_index;
	std::vector<CCameraLocation> mv_location;
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
	double m_local_elapsed_time;
	double m_animation_elapsed_time;
	double m_animation_period;
	std::size_t m_index_location;
	std::size_t m_index_control_point;
	osg::Vec3d A;
	osg::Vec3d B;
	std::size_t index_A;
	std::size_t index_B;
};

class CEvent_UpdateVelocity : public CEventNymph
{
public:	
	eTimeType m_time_type;	
	std::size_t m_index_PAT;
	double m_local_elapsed_time;	
	Vec3d m_p0;
	Vec3d m_position;
	Vec3d m_v0;
	Vec3d m_velocity;	
	Vec3d m_acceleration;	
	Vec3d m_orientation_axis;
	float m_theta0;
	float m_orientation;
	float m_omega0;	
	float m_angular_velocity;
	float m_alpha0;	
	float m_angular_acceleration;	
	Quat m_Q0;
	Quat m_Qorientation;	
	bool Create(CEventDesign* p_design, CEnvironment* p_env);
	bool Update(CEnvironment* p_env);
};

#endif

 