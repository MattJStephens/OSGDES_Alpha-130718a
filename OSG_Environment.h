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

#ifndef _INCL_OSG_CEnvironment
#define _INCL_OSG_CEnvironment

#include "OSG_DES.h"
#include <msxml.h>
#include <comdef.h>
#include <fstream>
#include <atlbase.h>
#include "OSG_Entities.h"
#include "KeyBoardEventHandler.h"
#include "OSG_Camera.h"
#include "OSG_time.h"

//class forwards
class CEventHandler;


class CActiveContinuousEvent
{
public:
	CActiveContinuousEvent() {m_slay=false; }
	bool m_slay;
	std::size_t m_operation_index;
	std::size_t m_operative_index;
};//temp?


///
//CEnvironment
///
class CEnvironment
{
	
private://private member variables	
		
	unsigned m_PAT_unique_id;
	std::vector<CPositionAttitudeTransform> mv_PAT;
	Mersenne_Twister  m_mt;
	bool m_quit_on_max_time;
	double m_max_time;
	double m_elapsed_real_time;
	double m_elapsed_simulation_time;
	double m_elapsed_simulation_time_increment;
	double m_previous_real_time;
	double m_delta_real_time;
	double m_previous_simulation_time;
	double m_delta_simulation_time;	

public: //public member variables

	
	eTimeType m_active_time_type;
	bool m_request_real_time_mode;
	bool m_request_sim_time_mode;		
	bool m_write_node_throughput_to_file;
	bool m_write_env_state_to_file;
	bool m_write_heading_env_state;
	bool m_Render;		
	unsigned long m_last_count;
	std::ofstream mf_throughput;	
	std::ofstream mf_env_state;
	std::string m_throughput_file;
	std::string m_env_state_file;
	std::string m_str_script;
	std::size_t m_active_viewer_index;
	std::size_t m_active_group_index;		
	CCustomCamera m_custom_camera;
	Vec4d m_background_color;		
	StateSet *m_lightStateSet;			
	CInputDeviceStateType* mp_input_state_type;
	CKeyboardEventHandler* mp_keyboard_event_handler;
	NxScene* mp_scene;	
	
	//std::vector<CActiveContinuousEvent> mv_ACE;		
	unsigned m_active_continuous_operatives;

	std::vector<CArray> mv_array;			
	std::vector<CAlphaFunc> mv_alphafunc;		
	std::vector<CAnimationControlPoint> mv_animation_control_point;
	std::vector<CAnimationPath> mv_animation_path;
	std::vector<CAnisotropicLighting> mv_anisotropic_lighting;
	std::vector<CBillboard> mv_billboard;
	std::vector<CBoundingBox> mv_bounding_box;
	std::vector<CClearNode> mv_clear_node;		
	std::vector<CCamera> mv_camera;		
	std::vector<CLogistics_demand> mv_logistics_demand;
	std::vector<CLogistics_node> mv_logistics_node;		
	std::vector<CLogistics_operation> mv_logistics_operation;
	std::vector<CLogistics_operation>::iterator m_logistics_operation_iter;
	std::vector<CLogistics_producer> mv_Logistics_producer;
	std::vector<CLogistics_component> mv_Logistics_component;
	std::vector<CLogistics_route> mv_Logistics_route;
	std::vector<CDiscreteEvent> mv_discrete_event;
	std::vector<CDiscreteEvent>::iterator m_DE_iter;		
	std::vector<CDrawElementsUInt> mv_draw_elements_UInt;		
	std::vector<CDrawArrays> mv_draw_arrays;		
	std::vector<CDrawable> mv_drawable;				
	std::vector<CEntityArray> mv_entity_array;	
	std::vector<CEntityPhysXCube> mv_physX_cube;	
	std::vector<CEntityPhysXPlane> mv_physX_plane;			
	std::vector<CEntityPhysXCloth> mv_physX_cloth;			
	std::vector<CEntityPhysXSphere> mv_physX_sphere;
	std::vector<CEntityPhysXSoftBody> mv_physX_soft_body;
	std::vector<CEntityPhysXFluid> mv_physX_fluid;
	std::vector<CEntityPhysXTerrain> mv_physX_terrain;		
	std::vector<CExplosionEffect> mv_explosion_effect;
	std::vector<CExplosionDebrisEffect> mv_explosion_debris_effect;
	std::vector<CEXPR> mv_EXPR;
	std::vector<CEXPRVectorField> mv_EXPR_vector_field;
	std::vector<CFirstPersonManipulator> mv_FPM;	
	std::vector<CFireEffect> mv_fire_effect;			
	std::vector<CGeode> mv_geode;
	std::vector<CGeometry> mv_geometry;
	std::vector<CGaussian> mv_gaussian;
	std::vector<CGroup> mv_group;
	std::vector<CGraph> mv_graph;
	std::vector<CHeightField> mv_height_field;			
	std::vector<CImage> mv_image;	
	std::vector<CImposter> mv_imposter;			
	std::vector<CKeySwitchMatrixManipulator> mv_KSMM;				
	std::vector<CLight> mv_light;
	std::vector<CLightSource> mv_light_source;
	std::vector<CLineSegment> mv_line_segment;
	std::vector<CMaterial> mv_material;		
	std::vector<CMatrix> mv_matrix;	
	std::vector<CMatrixTransform> mv_matrix_transform;		
	std::vector<CMemoryMapClient> mv_memory_map_client;	
	std::vector<CMemoryMapServer> mv_innate_memory_map_server;	
	std::vector<CMemoryMapServer> mv_memory_map_server;	
	std::vector<CMultipleRandomEntities> mv_multiple_random_entities;
	std::vector<CMultiplePAT> mv_multiple_PAT;		
	std::vector<CNode> mv_node;		
	std::vector<CPhysX_Actor> mv_PhysX_actor;	
	std::vector<CPhysX_ActorDesc> mv_PhysX_actor_desc;		
	std::vector<COSGSequence> mv_OSGequence;	
	std::vector<CSocket> mv_socket;		
	std::vector<CParticle> mv_particle;
	std::vector<CParsedVariable> mv_variable;
	std::vector<CPointSprite> mv_point_sprite;		
	std::vector<CPolygonMode> mv_polygon_mode;				
	std::vector<CProgram> mv_program;
	std::vector<CProjection> mv_projection;
	//std::vector<CProducer> mv_Logistics_producer; //deprecated			
	std::vector<CQuat> mv_quat;	
	std::vector<CReference> mv_ref;
	std::vector<CSequence> mv_sequence;		
	std::vector<CSet> mv_set;		
	std::vector<CShader> mv_shader;
	std::vector<CShapeDrawable> mv_shape_drawable;		
	std::vector<CStateSet> mv_stateset;
	std::vector<CStateAttribute> mv_state_attribute;
	std::vector<CSmokeEffect> mv_smoke_effect;		
	std::vector<CText> mv_text;		
	//std::vector<CTexture> mv_texture; //base class		
	std::vector<CTexture2D> mv_texture2D;	
	std::vector<CUniform> mv_uniform;
	std::vector<CUByteArray> mv_ubyte_array;
	std::vector<CVec2Array> mv_Vec2Array;		
	std::vector<CVec3Array> mv_Vec3Array;		
	std::vector<CVec4Array> mv_Vec4Array;		
	std::vector<CViewer> mv_viewer;		
	std::vector<CXGMML_graph> mv_xgmml_graph;	

	//cached random frequencies
	std::vector<double> mv_frequency;

private://private functions			
	bool XML_Parse_Environment(CComPtr<IXMLDOMNode> spXMLNode);		
	bool XML_Parse_XGMML_Graph(CComPtr<IXMLDOMNode> spXMLNode, CXGMML_graph* p_graph);		
	int m_uniqueLightNumber;
	int m_LIGHTS;		

public://public functions
	CEnvironment();
	~CEnvironment();
	void init_time();
	void update_simulation_time();
	void update_simulation_time_beta(double DeltaT);
	void set_simulation_time_increment(double incr) { m_elapsed_simulation_time_increment = incr; }
	void LinkReferences(void);	
	void set_PAT_position(unsigned* p_index, Vec3d* p_vec);
	void set_PAT_orientation(unsigned* p_index, osg::Quat* p_quat);
	void set_PAT_orientation(unsigned* p_index, Vec3d* p_axis, float* p_angle);	
	void slay_marked_discrete_event(void);
	void slay_marked_PAT(void);		
	void slay_marked_logistics_operatives(std::size_t index);
	void slay_flagged_orders(void);		
	void slay_marked_ACE(void);
	bool update_operation(std::size_t operation_index, eTimeType time_type);
	bool update_theatre_of_operations(eTimeType time_type);
	bool update_time();	
	bool XML_EnvironmentParser(string str);				
	bool XGMML_Graph_Parser(string str);	
	bool add_discrete_event(double elapsed_time, double period, std::string description);
	bool PAT_set_position(CReference *p_ref, const osg::Vec3d* p_vec);
	bool PAT_set_attitude(CReference *p_ref, const osg::Quat* p_quat);	
	double get_elapsed_real_time() { return m_elapsed_real_time; }
	double get_elapsed_simulation_time() { return m_elapsed_simulation_time ; }
	double get_delta_real_time() { return m_delta_real_time ; }
	double get_delta_simulation_time() { return m_delta_simulation_time ; }
	double get_delta_time(void);
	double get_elapsed_time(void);					
	osg::ref_ptr<osgText::Text> GetText(std::size_t index);			
	std::size_t PAT_index_from_unique_ID(unsigned ID);		
	std::size_t PAT_index_from_name(std::string name);		
	std::size_t PAT_size(void);		
		
	bool AddLogisticsOperative(CReference* p_ref_Logistics_operative, CLogistics_operative * p_logistics_operative, 
		std::size_t operation_index, eSourceGeometryType SourceGeometryType);
		
	bool link_logistics_operative(CReference* p_ref_Logistics_operative, std::size_t operation_index, 
		std::size_t logistics_component_index, std::size_t component_index, std::size_t group_index, eSourceGeometryType SourceGeometryType);		

		
	unsigned add_PAT(CPositionAttitudeTransform PAT, std::size_t *index) { 			
		*index = mv_PAT.size();
		PAT.m_unique_id = m_PAT_unique_id;
		mv_PAT.push_back(PAT);
		m_PAT_unique_id++;
		return m_PAT_unique_id;
	}	
	CLogistics_node* get_logistics_node(CReference* p_ref) 
	{ 
		if( p_ref->m_index<  mv_logistics_node.size())
			return &(mv_logistics_node[p_ref->m_index]);
		else return NULL;
	}
	osg::Vec3d get_logistics_node_position(CReference* p_ref);

	CImage* get_Image(CReference* p_ref) 
	{ 
		if( p_ref->m_index<  mv_image.size())
			return &(mv_image[p_ref->m_index]);
		else return NULL;
	}

	CPositionAttitudeTransform* get_PAT(std::size_t index) 
	{ 
		if( index<  mv_PAT.size())
			return &(mv_PAT[index]);
		else return NULL;
	}

	CPositionAttitudeTransform* get_PAT(CReference* p_ref) 
	{ 
		if( p_ref->m_index<  mv_PAT.size())
			return &(mv_PAT[p_ref->m_index]);
		else return NULL;
	}
		
	osg::Vec3d get_PAT_position(std::size_t index)
	{
		if(index< mv_PAT.size())
			return mv_PAT[index].mp_PAT->getPosition();			
		osg::Vec3d vec;
		return vec;			
	}		

	template <class T> std::size_t index_from_name(string name, const T & vec)
	{		
		typename T::const_iterator iter;
		for (iter = vec.begin(); iter< vec.end(); ++iter)
		{
			if(iter->m_name == name)
			{
				return std::distance(vec.begin(), iter);
			}			
		}
		return UINT_MAX;
	};

	template <class T> void update_reference(CReference * p_reference, const T & vec)
	{
		p_reference->m_index= index_from_name(p_reference->m_ref_name, vec);
	};

	template <class T_entity, class T_vector> T_entity* get_entity(CReference* p_ref, const T_vector & vec) 
	{ 
		typename T_entity * p_entity = NULL;
		if( p_ref->m_index<  vec.size())
		{
			p_entity = &(vec[p_ref->m_index]);
		}
		return p_entity;
	}		
};

#endif