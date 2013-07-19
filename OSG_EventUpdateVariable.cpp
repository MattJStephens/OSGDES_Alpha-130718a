
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
// CEvent_UpdateVariable
///
bool CEvent_UpdateVariable::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {	
		if(iter->m_identifier == "type") {	m_type = iter->m_str_value; }		
		else if(iter->m_identifier == "entity") {	m_entity = iter->m_str_value; }		
		else if(iter->m_identifier == "ref_name") {	m_ref_name = iter->m_str_value; }		
		else if(iter->m_identifier == "variable_type") {	m_variable_type = iter->m_str_value; }		
		else if(iter->m_identifier == "value") {	m_value = iter->m_str_value; }		
	}		
	return true;
}

bool read_mem_map_Geode(CEnvironment* p_env)
{

	return true;
}
bool read_mem_map_Geometry(CEnvironment* p_env)
{

	return true;
}
bool read_mem_map_PAT(CEnvironment* p_env)
{

	return true;
}





bool read_mem_map_Light(CEnvironment* p_env)
{
	std::string str_index;
	if(read_string_mapped_memory("mjs_index", 0,  &str_index, 16)) {
		unsigned long unl= std::stoul(str_index, 0);						
		if(unl < p_env->mv_light.size())	{		
			std::string str_variable;
			if(read_string_mapped_memory("mjs_variable", 0,  &str_variable, 32)) {
				if(str_variable== "Diffuse") {
					std::string str_value;
					if(read_string_mapped_memory("mjs_value", 0,  &str_value, 64)) {
						std::stringstream ss_value(str_value);
						osg::Vec4 color;
						ss_value >> color.x();
						ss_value >> color.y();
						ss_value >> color.z();
						ss_value >> color.w();
						p_env->mv_light[unl].mp_light->setDiffuse(color);
					}
				}
				else if(str_variable== "Ambient") {
					std::string str_value;
					if(read_string_mapped_memory("mjs_value", 0,  &str_value, 64)) {
						std::stringstream ss_value(str_value);
						osg::Vec4 color;
						ss_value >> color.x();
						ss_value >> color.y();
						ss_value >> color.z();
						ss_value >> color.w();
						p_env->mv_light[unl].mp_light->setAmbient(color);
					}
				}
				else if(str_variable== "Specular") {
					std::string str_value;
					if(read_string_mapped_memory("mjs_value", 0,  &str_value, 64)) {
						std::stringstream ss_value(str_value);
						osg::Vec4 vec4;
						ss_value >> vec4.x();
						ss_value >> vec4.y();
						ss_value >> vec4.z();
						ss_value >> vec4.w();
						p_env->mv_light[unl].mp_light->setSpecular(vec4);
					}
				}
				else if(str_variable== "Position") {
					std::string str_value;
					if(read_string_mapped_memory("mjs_value", 0,  &str_value, 64)) {
						std::stringstream ss_value(str_value);
						osg::Vec4 vec4;
						ss_value >> vec4.x();
						ss_value >> vec4.y();
						ss_value >> vec4.z();
						ss_value >> vec4.w();
						p_env->mv_light[unl].mp_light->setPosition(vec4);
					}
				}
				else if(str_variable== "Direction") {
					std::string str_value;
					if(read_string_mapped_memory("mjs_value", 0,  &str_value, 64)) {
						std::stringstream ss_value(str_value);
						osg::Vec3 vec3;
						ss_value >> vec3.x();
						ss_value >> vec3.y();
						ss_value >> vec3.z();
						p_env->mv_light[unl].mp_light->setDirection(vec3);
					}
				}
				else if(str_variable== "ConstantAttenuation") {
					std::string str_value;
					if(read_string_mapped_memory("mjs_value", 0,  &str_value, 64)) {
						std::stringstream ss_value(str_value);
						float f;
						ss_value >> f;										
						p_env->mv_light[unl].mp_light->setConstantAttenuation(f);
					}
				}
				else if(str_variable== "LinearAttenuation") {
					std::string str_value;
					if(read_string_mapped_memory("mjs_value", 0,  &str_value, 64)) {
						std::stringstream ss_value(str_value);
						float f;
						ss_value >> f;										
						p_env->mv_light[unl].mp_light->setLinearAttenuation(f);
					}
				}
				else if(str_variable== "QuadraticAttenuation") {
					std::string str_value;
					if(read_string_mapped_memory("mjs_value", 0,  &str_value, 64)) {
						std::stringstream ss_value(str_value);
						float f;
						ss_value >> f;										
						p_env->mv_light[unl].mp_light->setQuadraticAttenuation(f);
					}
				}
				else if(str_variable== "SpotCutoff") {
					std::string str_value;
					if(read_string_mapped_memory("mjs_value", 0,  &str_value, 64)) {
						std::stringstream ss_value(str_value);
						float f;
						ss_value >> f;										
						p_env->mv_light[unl].mp_light->setSpotCutoff(f);
					}
				}
				else if(str_variable== "SpotExponent") {
					std::string str_value;
					if(read_string_mapped_memory("mjs_value", 0,  &str_value, 64)) {
						std::stringstream ss_value(str_value);
						float f;
						ss_value >> f;										
						p_env->mv_light[unl].mp_light->setSpotExponent(f);
					}
				}
				else if(str_variable== "LightNum") {
					std::string str_value;
					if(read_string_mapped_memory("mjs_value", 0,  &str_value, 64)) {
						std::stringstream ss_value(str_value);
						int n;
						ss_value >> n;										
						p_env->mv_light[unl].mp_light->setLightNum(n);
					}
				}	
			}
		}
	}	
	
	return true;
}

/*
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
	*/


//Array


bool write_mem_map_Light_details(CEnvironment* p_env, unsigned long unl)
{

	std::ostringstream oss;

	oss << "Diffuse ";
	osg::Vec4 vec4_diffuse= p_env->mv_light[unl].mp_light->getDiffuse();
	oss << vec4_diffuse.x();
	oss << " ";
	oss << vec4_diffuse.y();
	oss << " ";
	oss << vec4_diffuse.z();
	oss << " ";
	oss << vec4_diffuse.w();
	oss << " ";

	oss << "Ambient ";
	osg::Vec4 vec4_ambient= p_env->mv_light[unl].mp_light->getAmbient();
	oss << vec4_ambient.x();
	oss << " ";
	oss << vec4_ambient.y();
	oss << " ";
	oss << vec4_ambient.z();
	oss << " ";
	oss << vec4_ambient.w();
	oss << " ";

	
	oss << "Specular ";
	osg::Vec4 vec4_specular= p_env->mv_light[unl].mp_light->getSpecular();
	oss << vec4_specular.x();
	oss << " ";
	oss << vec4_specular.y();
	oss << " ";
	oss << vec4_specular.z();
	oss << " ";
	oss << vec4_specular.w();
	oss << " ";

	oss << "Position ";
	osg::Vec4 vec4_position= p_env->mv_light[unl].mp_light->getPosition();
	oss << vec4_position.x();
	oss << " ";
	oss << vec4_position.y();
	oss << " ";
	oss << vec4_position.z();
	oss << " ";
	oss << vec4_position.w();
	oss << " ";

	write_string_mapped_memory("mjs_entity_details", 0,  oss.str(), 512);			


	return true;
}



bool CEvent_UpdateVariable::Update(CEnvironment* p_env)
{
	if(m_type == "mm") {
	//	//TODO: active exe index..
		if(read_char_mapped_memory("mjs_innate", 0,  '1')) {
 			write_char_mapped_memory("mjs_innate", 0,  '0');			
			std::string str_entity;
			if(read_string_mapped_memory("mjs_entity", 0,  &str_entity, 8))	{	
				if(str_entity== "Geode") { read_mem_map_Geode(p_env); }
				else if(str_entity== "Geometry") { read_mem_map_Geometry(p_env); }	
				else if(str_entity== "Light") { read_mem_map_Light(p_env);	}	
				else if(str_entity== "PAT") { read_mem_map_PAT(p_env); }	
			}						
		}	
		else if(read_char_mapped_memory("mjs_innate", 0,  '2')) {
 			write_char_mapped_memory("mjs_innate", 0,  '0');	
			std::string str_entity;
			if(read_string_mapped_memory("mjs_entity", 0,  &str_entity, 8))	{	
				if(str_entity== "Light") { 
					std::string str_index;
					if(read_string_mapped_memory("mjs_index", 0,  &str_index, 16)) {	
						unsigned long unl= std::stoul(str_index, 0);						
						if(unl < p_env->mv_light.size())	{	
							write_mem_map_Light_details(p_env, unl);
						}
					}
				}	
			}
		}	
	}


	return false;
}