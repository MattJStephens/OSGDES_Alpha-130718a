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

////////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////
#define NOMINMAX
#include "MJS_Sockets.h"

#include "OSG_DES.h"
#include "OSG_Environment.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Geometry_tools.h"

#include <limits>
#include <osgUtil/Optimizer>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/Registry>
//#include <osg/ShadeModel>
#include "OSG_Physx_2_8.h"
#include "ObjMesh.h"
#include "OSG_Tools.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <cstring>

using namespace osg;
using namespace std;
//using namespace boost::interprocess;

extern class COSG_PhysX g_PhysX;
extern bool g_physx_active;

void CEnvironment::set_PAT_position(unsigned* p_index, Vec3d* p_vec)
{
	if(*p_index< mv_PAT.size())
	{
		mv_PAT[*p_index].mp_PAT->setPosition(*p_vec);
	}
}

void CEnvironment::set_PAT_orientation(unsigned* p_index, osg::Quat* p_quat)
{
	if(*p_index< mv_PAT.size())
	{
		mv_PAT[*p_index].mp_PAT->setAttitude(*p_quat);
	}
}

void CEnvironment::set_PAT_orientation(unsigned* p_index, Vec3d* p_axis, float* p_angle)
{
	if(*p_index< mv_PAT.size())
	{
		Quat quat;
		quat.makeRotate(*p_angle, *p_axis);
		mv_PAT[*p_index].mp_PAT->setAttitude(quat);
	}			
}

bool CEnvironment::XGMML_Graph_Parser(string str)
{
	CoInitialize(NULL);
	try {		
		CComPtr<IXMLDOMDocument> spXMLDOM;
		HRESULT hr = spXMLDOM.CoCreateInstance(__uuidof(DOMDocument));
		if ( FAILED(hr) ) {
				throw "Unable to create XGMML parser object";
				return false;
		}
		if ( spXMLDOM.p == NULL ) {
			throw "Unable to create XGMML parser object";
			return false;
		}

		VARIANT_BOOL bSuccess = false;
		hr = spXMLDOM->load(CComVariant(str.c_str()),&bSuccess);
		if ( FAILED(hr) ) throw "Unable to load XGMML document into the parser";
		if ( !bSuccess ) throw "Unable to load XGMML document into the parser";

		CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
		hr = spXMLDOM->get_childNodes(&spXMLDOMNodeList);
		if ( FAILED(hr) ) throw "Unable to get XGMML node list";		

		long listLength;
		hr =spXMLDOMNodeList->get_length(&listLength);
		if ( FAILED(hr) ) throw "Unable to get XGMML list length";

		for(long i= 0; i< listLength; i++)	{
			string str_node_name;
			string str_node_text;
			CComPtr<IXMLDOMNode> spXMLNextNode;
			spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
			if(spXMLNextNode!= NULL)
			{
				if(str_node_name== "graph")
				{
					std::size_t graph_index= 0;
					CXGMML_graph graph;
					XML_Parse_XGMML_Graph(spXMLNextNode, &graph);
					graph_index= mv_xgmml_graph.size();
					mv_xgmml_graph.push_back(graph);
					mv_xgmml_graph[graph_index].create_boost_graph();
				}
			}			
		}
		
		std::cout << "Graph_Parser complete "  << str << " " << std::endl << std::endl;
	} 
	catch(char* lpstrErr) {	
		std::cout << lpstrErr << std::endl << std::endl;
		return false;
	} 
	catch(...) {		
		std::cout << "Unknown error..." << str << " " << std::endl << std::endl;
		return false;
	}

	
	CoUninitialize();
	
	return true;

}



bool CEnvironment::XML_Parse_XGMML_Graph(CComPtr<IXMLDOMNode> spXMLNode, CXGMML_graph* p_graph)
{
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	for(long i= 0; i< listLength; i++)
	{		
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "node")
			{							
				CXGMML_node node;
				XML_Parse_XGMML_Node(spXMLNextNode, &node);			
				p_graph->mv_node.push_back(node);
			}
			if(str_node_name== "edge")
			{							
				CXGMML_edge edge;
				XML_Parse_XGMML_Edge(spXMLNextNode, &edge);			
				p_graph->mv_edge.push_back(edge);
			}
		}
	}
	return true;
}


bool CEnvironment::XML_EnvironmentParser(string str)
{
	bool ret = false;
	CoInitialize(NULL);
	try {		
		CComPtr<IXMLDOMDocument> spXMLDOM;
		HRESULT hr = spXMLDOM.CoCreateInstance(__uuidof(DOMDocument));
		if ( FAILED(hr) ) {
				throw "Unable to create Environment XML parser object";
				return false;
		}
		if ( spXMLDOM.p == NULL ) {
			throw "Unable to create Environment XML parser object";
			return false;
		}

		VARIANT_BOOL bSuccess = false;
		hr = spXMLDOM->load(CComVariant(str.c_str()),&bSuccess);
		if ( FAILED(hr) ) throw "Unable to load Environment XML document into the parser";
		if ( !bSuccess ) throw "Unable to load Environment XML document into the parser";

		CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
		hr = spXMLDOM->get_childNodes(&spXMLDOMNodeList);
		if ( FAILED(hr) ) throw "Unable to get Environment node list";		

		long listLength;
		hr =spXMLDOMNodeList->get_length(&listLength);
		if ( FAILED(hr) ) throw "Unable to get Environment list length";

		for(long i= 0; i< listLength; i++)	{
			string str_node_name;
			string str_node_text;
			CComPtr<IXMLDOMNode> spXMLNextNode;
			spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
			if(spXMLNextNode!= NULL)
			{
				if(str_node_name== "Environment")
				{
					ret = XML_Parse_Environment(spXMLNextNode);
				}
			}			
		}
		
		std::cout << "Environment Parser complete "  << str << " " << std::endl << std::endl;
	} 
	catch(char* lpstrErr) {	
		std::cout << lpstrErr << std::endl << std::endl;
		return false;
	} 
	catch(...) {		
		std::cout << "Unknown error..." << str << " " << std::endl << std::endl;
		return false;
	}

	
	CoUninitialize();
	
	return ret;
}


bool tempz(std::size_t init_string_length2, std::size_t default_size2,std::string init_string2)
{
	if(init_string_length2== 0&&default_size2==0&&init_string2==""){	return true; }
	return false;
}

bool CEnvironment::XML_Parse_Environment(CComPtr<IXMLDOMNode> spXMLNode)
{
	

	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	for(long i= 0; i< listLength; i++)
	{		
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "ActiveViewerIndex")
			{							
				istringstream iss(str_node_text);
				iss >> m_active_viewer_index;
			}
			else if(str_node_name== "ActiveGroupIndex")
			{							
				istringstream iss(str_node_text);
				iss >> m_active_group_index;
			}			
			else if(str_node_name== "AlphaFunc")
			{
				CAlphaFunc alphafunc;				
				alphafunc.mp_alphafunc = new osg::AlphaFunc();
				XML_Parse_AlphaFunc(spXMLNextNode, &alphafunc);			
				mv_alphafunc.push_back(alphafunc);
			}	
			else if(str_node_name== "AnimationControlPoint")
			{
				CAnimationControlPoint animation_control_point;				
				XML_Parse_AnimationControlPoint(spXMLNextNode, &animation_control_point);			
				mv_animation_control_point.push_back(animation_control_point);
			}	
			else if(str_node_name== "AnimationPath")
			{
				CAnimationPath animation_path;				
				animation_path.mp_animation_path = new osg::AnimationPath();
				XML_Parse_AnimationPath(spXMLNextNode, &animation_path);			
				mv_animation_path.push_back(animation_path);
			}				
			else if(str_node_name== "BackgroundColor")//temp
			{
				XML_Parse_Vec4d(spXMLNextNode, &m_background_color);				
			}
			else if(str_node_name== "Billboard")
			{
				CBillboard billboard;
				billboard.mp_billboard = new osg::Billboard();
				XML_Parse_Billboard(spXMLNextNode, &billboard);			
				mv_billboard.push_back(billboard);
			}
			else if(str_node_name== "BoundingBox")
			{
				CBoundingBox bounding_box;
				bounding_box.mp_bounding_box = new osg::BoundingBox();
				XML_Parse_BoundingBox(spXMLNextNode, &bounding_box);				
				mv_bounding_box.push_back(bounding_box);
			}		
			else if(str_node_name== "ClearNode")
			{				
				CClearNode clear_node;
				clear_node.mp_clear_node = new osg::ClearNode();
				XML_Parse_ClearNode(spXMLNextNode, &clear_node);				
				mv_clear_node.push_back(clear_node);
			}			
			else if(str_node_name== "Camera")
			{				
				CCamera camera;
				camera.mp_camera = new osg::Camera();
				XML_Parse_Camera(spXMLNextNode, &camera);				
				mv_camera.push_back(camera);
			}
			else if(str_node_name== "CustomCamera")
			{				
				XML_Parse_CustomCamera(spXMLNextNode, &m_custom_camera);				
			}
			else if(str_node_name== "CMatrix")
			{
				CMatrix matrix;			
				XML_Parse_CMatrix(spXMLNextNode, &matrix);				
				mv_matrix.push_back(matrix);
			}	
			else if(str_node_name== "DrawElementsUInt")
			{
				CDrawElementsUInt DEUI;
				XML_Parse_DrawElementsUInt(spXMLNextNode, &DEUI);				
				mv_draw_elements_UInt.push_back(DEUI);
			}
			else if(str_node_name== "DrawArrays")
			{
				CDrawArrays draw_arrays;//need to initialise in parser
				XML_Parse_DrawArrays(spXMLNextNode, &draw_arrays, &m_last_count);	
				if(draw_arrays.mp_draw_arrays!= NULL)
				{
					mv_draw_arrays.push_back(draw_arrays);
				}
			}
			else if(str_node_name== "EntityArray")
			{
				CEntityArray EA;				
				XML_Parse_EntityArray(spXMLNextNode, &EA);
				if(EA.m_array_method == "spiral1")
				{
					std::string name= EA.m_name;
					unsigned int i=0;
					/*double angle_inc = (3.14159/180) * 5;
					double angle=0;
					double rad = 100;	
					double rad_inc= 0;
					double x=0;
					double y=0;
					double z= 100;
					double z_inc= 5;*/
					for(i=0; i< EA.m_number_of_entities; i++)
					{
						CEntityPhysXCube physX_cube;
						std::ostringstream oss;
						oss << name;
						oss << i;
						physX_cube.m_name= oss.str();
						oss.clear();											
						EA.x = EA.rad * cos(EA.angle);
						EA.y = EA.rad * sin(EA.angle);
						EA.angle += EA.angle_inc;
						EA.z += EA.z_inc;
						physX_cube.m_size.set(1,1,1);
						physX_cube.m_file_name = "cube.osg";						
						physX_cube.m_position.set(EA.x,EA.y,EA.z);
						physX_cube.m_cube.init(mp_scene, physX_cube.m_size, physX_cube.m_position, physX_cube.m_geometry_type, physX_cube.m_file_name);
						mv_physX_cube.push_back(physX_cube);				

					}
				}
				mv_entity_array.push_back(EA);
			}				
			else if(str_node_name== "Logistics_component")
			{
				CLogistics_component component;				
				XML_Parse_Logistics_component(spXMLNextNode, &component);				
				mv_Logistics_component.push_back(component);
			}		
			else if(str_node_name== "Logistics_demand")
			{
				CLogistics_demand demand;				
				XML_Parse_Logistics_demand(spXMLNextNode, &demand);				
				mv_logistics_demand.push_back(demand);
			}		
			else if(str_node_name== "Logistics_node")
			{
				CLogistics_node Logistics_node;				
				XML_Parse_Logistics_node(spXMLNextNode, &Logistics_node);				
				mv_logistics_node.push_back(Logistics_node);
			}		
			else if(str_node_name== "Logistics_operation")
			{
				CLogistics_operation operation;				
				XML_Parse_Logistics_operation(spXMLNextNode, &operation);				
				mv_logistics_operation.push_back(operation);
			}		
			else if(str_node_name== "Logistics_producer")
			{
				CLogistics_producer producer;				
				XML_Parse_Logistics_producer(spXMLNextNode, &producer);				
				mv_Logistics_producer.push_back(producer);
			}				
			if(str_node_name== "MaxTime")
			{							
				istringstream iss(str_node_text);
				iss >> m_max_time;
			}
			else if(str_node_name== "EXPR")
			{
				std::size_t expr_index= mv_EXPR.size();
				CEXPR expr;
				XML_Parse_EXPR(spXMLNextNode, &expr);	
				mv_EXPR.push_back(expr);					
				if(!mv_EXPR[expr_index].m_operation_list.link_operation_references(&(mv_EXPR[expr_index])))
				{
					return false;
				}									
			}			
			else if(str_node_name== "Explosion")
			{
				CExplosionEffect effect;
				XML_Parse_ExplosionEffect(spXMLNextNode, &effect);	
				effect.mp_explosion = new osgParticle::ExplosionEffect(effect.m_position, effect.m_scale, effect.m_intensity);				
				mv_explosion_effect.push_back(effect);
			}
			else if(str_node_name== "ExplosionDebris")
			{
				CExplosionDebrisEffect effect;
				XML_Parse_ExplosionDebrisEffect(spXMLNextNode, &effect);	
				effect.mp_explosion_debris = new osgParticle::ExplosionDebrisEffect(effect.m_position, effect.m_scale, effect.m_intensity);					
				mv_explosion_debris_effect.push_back(effect);
			}
			else if(str_node_name== "Fire")
			{
				CFireEffect effect;
				XML_Parse_FireEffect(spXMLNextNode, &effect);	
				effect.mp_fire = new osgParticle::FireEffect(effect.m_position, effect.m_scale, effect.m_intensity);		
				mv_fire_effect.push_back(effect);
			}			
			else if(str_node_name== "FilePath")
			{			
				osgDB::FilePathList pathList = osgDB::getDataFilePathList();	
				std::size_t path_list_size = pathList.size();
				std::size_t n = 0;
				bool found_str = false;
				for(n=0; n< path_list_size; n++)
				{
					if(	str_node_text == pathList[n]) { found_str= true; }
				}
				if(!found_str) { pathList.push_back(str_node_text); }


				osgDB::setDataFilePathList(pathList);
				//osg::ref_ptr<osgDB::ReaderWriter::Options> local_options = options ? static_cast<osgDB::ReaderWriter::Options*>(options->clone(osg::CopyOp::SHALLOW_COPY)) : new osgDB::ReaderWriter::Options;
				//local_options->getDatabasePathList().push_front(dirname);
			}
			else if(str_node_name== "Geode")
			{
				CGeode geode;
				geode.mp_geode = new osg::Geode();
				XML_Parse_Geode(spXMLNextNode, &geode);				
				mv_geode.push_back(geode);
			}
			else if(str_node_name== "Geometry")
			{
				CGeometry geometry;
				geometry.mp_geometry = new osg::Geometry();
				XML_Parse_Geometry(spXMLNextNode, &geometry);				
				mv_geometry.push_back(geometry);
			}	
			else if(str_node_name== "Graph")
			{
				CGraph graph;				
				graph.mp_PAT = new osg::PositionAttitudeTransform();
				graph.mp_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::LINES);
				graph.mp_Vec3Array = new osg::Vec3Array();
				graph.mp_geometry = new osg::Geometry();
				graph.mp_geode = new osg::Geode();
				graph.mp_projection = new osg::Projection();
				graph.mp_matrix_transform = new osg::MatrixTransform();
				XML_Parse_Graph(spXMLNextNode, &graph);				
				mv_graph.push_back(graph);
			}	
			else if(str_node_name== "Group")
			{
				std::size_t graph_node_count= 0;

				if(mv_xgmml_graph.size()> 0)
				{
					graph_node_count = mv_xgmml_graph[0].mv_node.size();
				}
				CGroup group;
				group.mp_group = new osg::Group();
				XML_Parse_Group(spXMLNextNode, &group, graph_node_count);	

				switch(group.m_EntityCreationFlag)
				{
					case eEntityCreationFlag_Create: { mv_group.push_back(group); break;}
					case eEntityCreationFlag_AddCreate:	{
						std::vector<CGroup>::iterator group_iter;
						std::size_t group_index;
						bool group_exists = false;
						for(group_iter = mv_group.begin(); group_iter != mv_group.end(); ++group_iter) { 
							if(group_iter->m_name== group.m_name) { 
								group_exists = true; 
								group_index = std::distance(mv_group.begin(), group_iter); 
							}	
						}
						if(group_exists)
						{							
							//if group exists then add all the found references to the existing group
							std::vector<CReference>::iterator ref_iter;
							for(ref_iter = group.mv_ref.begin(); ref_iter != group.mv_ref.end(); ++ref_iter) { 
								//TODO: make sure that the existing references are renamed and properly linked by the linker...
								mv_group[group_index].mv_ref.push_back(*ref_iter);
							}						
						}
						break;
					}
					case eEntityCreationFlag_AddOverwrite:
					{
						std::vector<CGroup>::iterator group_iter;
						bool group_exists = false;
						for(group_iter = mv_group.begin(); group_iter != mv_group.end(); ++group_iter)
						{
							if(group_iter->m_name== group.m_name){ group_exists = true; }
						}
						if(group_exists)
						{
							//overwrite existing group

						}
						break;
					}
				}			

			
				
			}		
			else if(str_node_name== "HeightField")
			{
				CHeightField height_field;
				height_field.mp_height_field = new osg::HeightField();
				XML_Parse_HeightField(spXMLNextNode, &height_field);	
				height_field.mp_height_field->allocate(height_field.m_columns,height_field.m_rows);
				height_field.mp_height_field->setXInterval(height_field.m_scale);
				height_field.mp_height_field->setYInterval(height_field.m_scale);

				float height = 1;
				for(unsigned int r=0;r<height_field.m_rows;++r)
				{
					for(unsigned int c=0;c<height_field.m_columns;++c)
					{
						height_field.mp_height_field->setHeight(c,r,height_field.m_z_scale*height);
					}
				}
				mv_height_field.push_back(height_field);
			}					
			else if(str_node_name== "Image")
			{
				CImage image;
				image.mp_image = new osg::Image();
				XML_Parse_Image(spXMLNextNode, &image);				
				mv_image.push_back(image);
			}		
			else if(str_node_name== "Imposter")
			{
				CImposter imposter;
				imposter.mp_imposter = new osgSim::Impostor();
				XML_Parse_Imposter(spXMLNextNode, &imposter);	

				//const osg::BoundingSphere& bs = model->getBound();

				mv_imposter.push_back(imposter);
			}		
			else if(str_node_name== "KeyBoardEventHandler")
			{			
				mp_input_state_type = new CInputDeviceStateType;
				mp_keyboard_event_handler = new CKeyboardEventHandler(mp_input_state_type);
				// tankPAT->setUpdateCallback(new updateTankPosCallback(tIDevState));
			}
			else if(str_node_name== "Light")
			{				
				CLight light;
				light.mp_light = new osg::Light();
				light.mp_light->setLightNum(m_uniqueLightNumber++);
				XML_Parse_Light(spXMLNextNode, &light);				
				mv_light.push_back(light);
			}
			else if(str_node_name== "LightSource")
			{				
				CLightSource light_source;
				light_source.mp_light_source = new osg::LightSource();
				XML_Parse_Light_Source(spXMLNextNode, &light_source);				
				mv_light_source.push_back(light_source);
			}				
			else if(str_node_name== "Material")
			{
				CMaterial material;
				material.mp_material = new osg::Material();
				XML_Parse_Material(spXMLNextNode, &material);				
				mv_material.push_back(material);
			}			
			else if(str_node_name== "MatrixTransform")
			{
				CMatrixTransform matrix_transform;
				matrix_transform.mp_matrix_transform = new osg::MatrixTransform();
				XML_Parse_MatrixTransform(spXMLNextNode, &matrix_transform);				
				mv_matrix_transform.push_back(matrix_transform);
			}
			else if(str_node_name== "MemoryMapClient")
			{				
				CMemoryMapClient memory_map_client;
				std::size_t index= mv_memory_map_client.size();
				XML_Parse_MemoryMapClient(spXMLNextNode, &memory_map_client);				
				mv_memory_map_client.push_back(memory_map_client);

				//Open already created shared memory object.
				boost::interprocess::shared_memory_object shm (boost::interprocess::open_only, mv_memory_map_client[index].m_memory_map_name.c_str(), boost::interprocess::read_only);

				//Map the whole shared memory in this process
				boost::interprocess::mapped_region region(shm, boost::interprocess::read_only);

				//Check that memory was initialized to 1
				char *mem = static_cast<char*>(region.get_address());
				for(std::size_t i = 0; i < region.get_size(); ++i)
				{					
					mv_memory_map_client[index].m_mem.push_back(*mem);
					*mem++;
				}

			}
			else if(str_node_name== "MemoryMapServer")
			{				
				CMemoryMapServer memory_map_server;
				std::size_t index= mv_memory_map_server.size();
				XML_Parse_MemoryMapServer(spXMLNextNode, &memory_map_server);
				mv_memory_map_server.push_back(memory_map_server);	
				boost::interprocess::shared_memory_object::remove(mv_memory_map_server[index].m_memory_map_name.c_str()); 
				boost::interprocess::shared_memory_object shm(boost::interprocess::create_only, mv_memory_map_server[index].m_memory_map_name.c_str(), boost::interprocess::read_write);			

				if(memory_map_server.m_initial_str.length()==0)
				{
					shm.truncate(mv_memory_map_server[index].m_size);	
					boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);
					std::memset(region.get_address(), 0, region.get_size());
				}
				else
				{
					shm.truncate(memory_map_server.m_initial_str.length());	
					boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);					
					std::memcpy(region.get_address(), memory_map_server.m_initial_str.c_str(), memory_map_server.m_initial_str.length());			
				}			

			}
			else if(str_node_name== "Node")
			{
				CNode node;
				node.mp_node = new osg::Node();
				XML_Parse_Node(spXMLNextNode, &node);				
				mv_node.push_back(node);
			}			
			else if(str_node_name== "PhysX_Actor")
			{
				if(g_physx_active)
				{
					CPhysX_Actor PhysX_actor;
					PhysX_actor.m_geode.mp_geode = new osg::Geode;
					PhysX_actor.m_matrix_transform.mp_matrix_transform = new osg::MatrixTransform;
					XML_Parse_PhysX_Actor(spXMLNextNode, &PhysX_actor);	
					osg::Vec3f position;
					position.x() = PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.globalPose.t.x;
					position.y() = PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.globalPose.t.y;
					position.z() = PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.globalPose.t.z;
					osg::Matrix mat = osg::Matrix::translate(position);
					PhysX_actor.m_matrix_transform.mp_matrix_transform->setMatrix(mat); 			
					if(PhysX_actor.m_PhysX_actor_desc.m_shape == "Box") {
						unsigned int num_drawables = PhysX_actor.m_geode.mp_geode->getNumDrawables();
						if(num_drawables== 0) {
							PhysX_actor.m_geode.mp_geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0)
							, PhysX_actor.m_PhysX_actor_desc.m_nx_box_shape_desc.dimensions.x * 2
							, PhysX_actor.m_PhysX_actor_desc.m_nx_box_shape_desc.dimensions.z * 2
							, PhysX_actor.m_PhysX_actor_desc.m_nx_box_shape_desc.dimensions.y * 2)));	
						}					
					}
					if(PhysX_actor.m_PhysX_actor_desc.m_shape == "Sphere") {
						unsigned int num_drawables = PhysX_actor.m_geode.mp_geode->getNumDrawables();
						if(num_drawables== 0) {
							osg::Vec3 center = osg::Vec3(0,0,0);
							float radius = PhysX_actor.m_PhysX_actor_desc.m_nx_sphere_shape_desc.radius;
							PhysX_actor.m_geode.mp_geode->addDrawable(new ShapeDrawable(new Sphere(center, radius)));	
						}					
					}
					osg::ref_ptr<osg::Material> material= new osg::Material();
					material->setDiffuse ( osg::Material::FRONT, osg::Vec4(random_float(1),random_float(1), random_float(1), 1.0f) );	     
					PhysX_actor.m_geode.mp_geode->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );

					PhysX_actor.mp_nx_actor = mp_scene->createActor(PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc);
					if(PhysX_actor.m_matrix_transform.mp_matrix_transform!= NULL) {
						if (PhysX_actor.m_applyShaderColor) {
							PhysX_actor.m_matrix_transform.mp_matrix_transform->setUpdateCallback(new MotionCallback(PhysX_actor.mp_nx_actor
								, PhysX_actor.m_matrix_transform.mp_matrix_transform->getOrCreateStateSet()));	
						}
						else  {
							PhysX_actor.m_matrix_transform.mp_matrix_transform->setUpdateCallback(new MotionCallback(PhysX_actor.mp_nx_actor));
						}	
						if(PhysX_actor.m_geode.mp_geode!= NULL)	{
							PhysX_actor.m_matrix_transform.mp_matrix_transform->addChild(PhysX_actor.m_geode.mp_geode);
						}
					}
					mv_PhysX_actor.push_back(PhysX_actor);
				}
			}	
			else if(str_node_name== "MultiplePAT")
			{	
				std::vector<CXGMML_graph>::iterator XGMML_graph_iter;				
				std::vector<CXGMML_node>::iterator XGMML_node_iter;
				std::vector<CXGMML_attribute>::iterator XGMML_attribute_iter;
				osg::Vec3d vec_att;

				double x= 0;
				double y= 0;
				double z= 0;
				
				CMultiplePAT MPAT;
				MPAT.m_PAT.mp_PAT = new osg::PositionAttitudeTransform();
				XML_Parse_MultiplePAT(spXMLNextNode, &MPAT);
				std::size_t MPAT_index= mv_multiple_PAT.size();
				mv_multiple_PAT.push_back(MPAT);
				
				CXGMML_graph* p_graph;
				CXGMML_node* p_node;
				CXGMML_attribute* p_attribute;
				vec_att.set(0,0,0);				
				std::size_t PAT_index= 0;
				//std::size_t graph_size= mv_xgmml_graph.size() ;
				//std::size_t xgmml_graphics;

				unsigned count = 0;
				for(XGMML_graph_iter = mv_xgmml_graph.begin(); XGMML_graph_iter != mv_xgmml_graph.end(); ++XGMML_graph_iter) {	
					p_graph = &(*XGMML_graph_iter);										
					for(XGMML_node_iter = p_graph->mv_node.begin(); XGMML_node_iter != p_graph->mv_node.end(); ++XGMML_node_iter)	{
						p_node =  &(*XGMML_node_iter);
						CPositionAttitudeTransform PAT;						
						PAT = MPAT.m_PAT;
						PAT_index = mv_PAT.size();
						mv_PAT.push_back(PAT);
						mv_PAT[PAT_index].mp_PAT = new osg::PositionAttitudeTransform();
						mv_PAT[PAT_index].mp_PAT->setScale(MPAT.m_PAT.mp_PAT->getScale());
						//xgmml_graphics= XGMML_node_iter->m_graphics.mv_attribute.size() ;
						for(XGMML_attribute_iter = p_node->m_graphics.mv_attribute.begin(); XGMML_attribute_iter != p_node->m_graphics.mv_attribute.end(); ++XGMML_attribute_iter) {
							p_attribute = &(*XGMML_attribute_iter);
							if(XGMML_attribute_iter->m_name == "x")	{
								std::istringstream iss(XGMML_attribute_iter->m_value);
								iss >> x;
								vec_att.x() = x;
							}
							if(XGMML_attribute_iter->m_name == "y")	{
								std::istringstream iss(XGMML_attribute_iter->m_value);
								iss >> y;
								vec_att.y() = y;
							}
							if(XGMML_attribute_iter->m_name == "z")	{
								std::istringstream iss(XGMML_attribute_iter->m_value);
								iss >> z;
								vec_att.z() = z;
							}							
						}					
						mv_PAT[PAT_index].mp_PAT->setPosition(vec_att);
						//mv_PAT[PAT_index].mp_PAT->setScale(
						std::ostringstream oss;
						oss << PAT.m_name;
						oss << count;
						mv_PAT[PAT_index].m_name = oss.str();

						if(MPAT.m_create_logistics_nodes){
							std::ostringstream oss2;
							oss2 << "LN_";
							oss2 << oss.str();
							CLogistics_node Logistics_node;		
							Logistics_node.m_name = oss2.str();
							Logistics_node.m_ref_PAT.m_ref_name  = oss.str();
							Logistics_node.m_ref_PAT.m_index  = PAT_index;
							mv_logistics_node.push_back(Logistics_node);
						}

						count++;						
					}
				}
			}
			else if(str_node_name== "MultipleRandomEntities")
			{				
				CMultipleRandomEntities MRE;
				CBillboard base_billboard;
				base_billboard.mp_billboard = new osg::Billboard();
				XML_Parse_MultipleRandomEntities(spXMLNextNode, &MRE,  &base_billboard);	
				//TODO: if("entity== Billboard...")
				if(MRE.m_method =="Circle") {
					unsigned int i = 0;
					for(i= 0; i< MRE.m_quantity; i++) {
						CBillboard billboard;
						billboard.mp_billboard = new osg::Billboard();
						billboard = base_billboard;
						CReference ref;	
						ref.m_entity_type = EntityType_Billboard;
						ref.m_index = mv_billboard.size();
						std::ostringstream oss;
						oss << "MRE_" << mv_multiple_random_entities.size() << "_" << MRE.mv_entity.size();
						billboard.m_name = oss.str();
						ref.m_ref_name = oss.str();
						float rad_angle = degree_to_radian(random_float(360));
						float radius = random_float(MRE.m_radius);
						float x = radius * cos(rad_angle);
						float y = radius * sin(rad_angle);					
						billboard.m_position.set(MRE.m_center.x()+ x, MRE.m_center.y()+ y, 10);							
						mv_billboard.push_back(billboard);
						MRE.mv_entity.push_back(ref);					
					}
					if(MRE.m_method == "Region")
					{
											/*
						#define MIN(x,y) (x < y ? x : y)
					#define MAX(x,y) (x > y ? x : y)
					#define INSIDE 0
					#define OUTSIDE 1

					typedef struct {
					   double x,y;
					} Point;

					int InsidePolygon(Point *polygon,int N,Point p)
					{
					  int counter = 0;
					  int i;
					  double xinters;
					  Point p1,p2;

					  p1 = polygon[0];
					  for (i=1;i<=N;i++) {
						p2 = polygon[i % N];
						if (p.y > MIN(p1.y,p2.y)) {
						  if (p.y <= MAX(p1.y,p2.y)) {
							if (p.x <= MAX(p1.x,p2.x)) {
							  if (p1.y != p2.y) {
								xinters = (p.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
								if (p1.x == p2.x || p.x <= xinters)
								  counter++;
							  }
							}
						  }
						}
						p1 = p2;
					  }

					  if (counter % 2 == 0)
						return(OUTSIDE);
					  else
						return(INSIDE);
					}

						*/
					}
				}			
				mv_multiple_random_entities.push_back(MRE);
			}	
			
			else if(str_node_name== "PhysXCloth")
			{
				CEntityPhysXCloth physX_cloth;
				XML_Parse_PhysXCloth(spXMLNextNode, &physX_cloth);					
				if(physX_cloth.m_primitive_type == "Points")
				{
					physX_cloth.p_cloth = new CPhysXCloth(mp_scene,
						physX_cloth.m_origin,
						physX_cloth.m_w,
						physX_cloth.m_h,
						physX_cloth.m_d,
						physX_cloth.m_thickness,
						physX_cloth.m_bendingStiffness,
						physX_cloth.m_stretchingStiffness,
						physX_cloth.m_density,
						physX_cloth.m_friction,
						physX_cloth.m_dampingCoefficient,
						physX_cloth.m_solverIterations,
						physX_cloth.m_texture_file_path,
						physX_cloth.m_tearLines,
						PrimitiveSetType_Points,
						physX_cloth.m_attach);					
				}
				else if (physX_cloth.m_primitive_type == "Faces")
				{
					physX_cloth.p_cloth = new CPhysXCloth(mp_scene,
						physX_cloth.m_origin,
						physX_cloth.m_w,
						physX_cloth.m_h,
						physX_cloth.m_d,
						physX_cloth.m_thickness,
						physX_cloth.m_bendingStiffness,
						physX_cloth.m_stretchingStiffness,
						physX_cloth.m_density,
						physX_cloth.m_friction,
						physX_cloth.m_dampingCoefficient,
						physX_cloth.m_solverIterations,
						physX_cloth.m_texture_file_path,
						physX_cloth.m_tearLines,
						PrimitiveSetType_Triangles,
						physX_cloth.m_attach);
				}
				mv_physX_cloth.push_back(physX_cloth);				
			}
			else if(str_node_name== "PhysXCube")
			{
				CEntityPhysXCube physX_cube;
				XML_Parse_PhysXCube(spXMLNextNode, &physX_cube);
				physX_cube.m_cube.init(mp_scene, physX_cube.m_size, physX_cube.m_position, physX_cube.m_geometry_type, physX_cube.m_file_name);
				mv_physX_cube.push_back(physX_cube);				
			}			
		
			else if(str_node_name== "PhysXFluid")
			{
				CEntityPhysXFluid physX_fluid;
				XML_Parse_PhysXFluid(spXMLNextNode, &physX_fluid);			
				std::size_t index = mv_physX_fluid.size();
				mv_physX_fluid.push_back(physX_fluid);				
				mv_physX_fluid[index].p_fluid = new CPhysXFluid(mp_scene);
				mv_physX_fluid[index].p_fluid->m_origin = physX_fluid.m_origin;
				mv_physX_fluid[index].p_fluid->m_gravity = physX_fluid.m_gravity;
				mv_physX_fluid[index].p_fluid->mGenerationRadius=  physX_fluid.mGenerationRadius;
				mv_physX_fluid[index].p_fluid->mParticleSize=  physX_fluid.mParticleSize;
				mv_physX_fluid[index].p_fluid->initialize();				
			}
			else if(str_node_name== "PhysXPlane")
			{
				CEntityPhysXPlane physX_plane;
				XML_Parse_PhysXPlane(spXMLNextNode, &physX_plane);
				physX_plane.m_plane.init(mp_scene, physX_plane.m_origin, physX_plane.m_filename, physX_plane.m_planesize);
				mv_physX_plane.push_back(physX_plane);				
			}
			else if(str_node_name== "PhysXSoftBody")
			{
				CEntityPhysXSoftBody physX_soft_body;
				XML_Parse_PhysXSoftBody(spXMLNextNode, &physX_soft_body);		

				NxSoftBodyDesc softBodyDesc;
				softBodyDesc.globalPose.t = NxVec3(0.0, 100.0, 0.0);
				softBodyDesc.particleRadius = 0.5f;//0.3
				softBodyDesc.volumeStiffness = 0.3f;
				softBodyDesc.stretchingStiffness = 0.3f;
				softBodyDesc.friction = 1.0f;
				softBodyDesc.solverIterations = 7;//5

			/*	char *fileName = "sphere";	

				char tetFileName[256], objFileName[256], s[256];
				sprintf(tetFileName, "../models/%s.tet", fileName);
				sprintf(objFileName, "../models/%s.obj", fileName);
*/
				ObjMesh *objMesh = new ObjMesh();
				objMesh->loadFromObjFile("sphere.obj");
				//objMesh->L

				//std::string dirname = "../models/";			

				physX_soft_body.p_soft_body = new CPhysXSoftBody(mp_scene, softBodyDesc, "sphere.tet", objMesh);
				mv_physX_soft_body.push_back(physX_soft_body);				
			}			
			else if(str_node_name== "PhysXSphere")
			{
				CEntityPhysXSphere physX_sphere;
				XML_Parse_PhysXSphere(spXMLNextNode, &physX_sphere);
				physX_sphere.p_sphere = new CPhysXSphere(mp_scene, physX_sphere.m_radius, physX_sphere.m_position);
				mv_physX_sphere.push_back(physX_sphere);				
			}
			else if(str_node_name== "PhysXTerrain")
			{
				CEntityPhysXTerrain physX_terrain;
				XML_Parse_PhysXTerrain(spXMLNextNode, &physX_terrain);
				physX_terrain.p_terrain = new CPhysXTerrain(g_PhysX.gPhysicsSDK, mp_scene, physX_terrain.m_origin);
				mv_physX_terrain.push_back(physX_terrain);				
			}
			else if(str_node_name== "PolygonMode")
			{
				CPolygonMode polygon_mode;
				polygon_mode.mp_polygon_mode = new osg::PolygonMode();
				XML_Parse_PolygonMode(spXMLNextNode, &polygon_mode);				
				mv_polygon_mode.push_back(polygon_mode);				
			}			
			else if(str_node_name== "PositionAttitudeTransform")
			{
				CPositionAttitudeTransform PAT;
				PAT.mp_PAT = new osg::PositionAttitudeTransform();
				XML_Parse_PositionAttitudeTransform(spXMLNextNode, &PAT);	
				std::size_t index;
				add_PAT(PAT, &index);
			}
			else if(str_node_name== "Projection")
			{
				CProjection projection;
				projection.mp_projection = new osg::Projection();
				XML_Parse_Projection(spXMLNextNode, &projection);				
				mv_projection.push_back(projection);
			}
			else if(str_node_name== "Program")
			{
				CProgram prgm;
				prgm.mp_program = new osg::Program();
				XML_Parse_Program(spXMLNextNode, &prgm);				
				mv_program.push_back(prgm);
			}
			else if(str_node_name== "Quat")
			{
				CQuat quat;
				//quat.mp_quat = new osg::Quat();
				//XML_Parse_Quaternion(spXMLNextNode, &quat.mp_quat);				
				//mv_quat.push_back(quat);
			}			
			else if(str_node_name== "QuitOnMaxTime")
			{
				if(str_node_text== "TRUE")
					m_quit_on_max_time = true;
				else if(str_node_text== "FALSE")
					m_quit_on_max_time = false;
			}						
			else if(str_node_name== "Ref")
			{
				CReference ref;
				XML_Parse_Reference(spXMLNextNode, &ref);				
				mv_ref.push_back(ref);
			}	
			else if(str_node_name== "Render")
			{
				if(str_node_text== "TRUE")
					m_Render = true;
				else if(str_node_text== "FALSE")
					m_Render = false;
			}	
			else if(str_node_name== "Shader")
			{
				CShader shader;
				XML_Parse_Shader(spXMLNextNode, &shader);				
				mv_shader.push_back(shader);
			}		
			else if(str_node_name== "ShapeDrawable")
			{
				CShapeDrawable sd;
				XML_Parse_ShapeDrawable(spXMLNextNode, &sd);				
				mv_shape_drawable.push_back(sd);
			}				
			else if(str_node_name== "Smoke")
			{
				CSmokeEffect effect;
				XML_Parse_SmokeEffect(spXMLNextNode, &effect);	
				effect.mp_smoke = new osgParticle::SmokeEffect(effect.m_position, effect.m_scale, effect.m_intensity);	
				mv_smoke_effect.push_back(effect);
			}	


			//Server
			//
			//
			//1.Initialize Winsock.
			//2.Create a socket.
			//3.Bind the socket.
			//4.Listen on the socket for a client.
			//5.Accept a connection from a client.
			//6.Receive and send data.
			//7.Disconnect.
			//
			//Client
			//
			//
			//1.Initialize Winsock.
			//2.Create a socket.
			//3.Connect to the server.
			//4.Send and receive data.
			//5.Disconnect.
			else if(str_node_name== "Socket")
			{
				CSocket socket;
				XML_Parse_Socket(spXMLNextNode, &socket);			
				if(initialize_Winsock(&(socket.m_wsaData))) {
					if(create_socket(socket.m_pSocket,
						socket.p_addrinfo, 
						socket.m_n_socket_flags, 
						socket.m_n_socket_address_family, 
						socket.m_n_socket_type, 
						socket.m_n_socket_protocol, 
						socket.m_str_node_name,
						socket.m_str_service_name))
					{					
						if(socket.m_type== "Client") {							
							if(connect_socket(socket.m_pSocket, socket.m_SockAddr))
							{
								mv_socket.push_back(socket);
							}							
						}
						else if(socket.m_type== "Server") {							
							if(bind_socket(socket.m_pSocket, (SOCKADDR *) &(socket.m_SockAddr), sizeof (socket.m_SockAddr)))
							{
								mv_socket.push_back(socket);
							}
						}		
						else { mv_socket.push_back(socket); }
					}
				}						
			}		
			
			else if(str_node_name== "StateSet")
			{
				CStateSet stateset;
				stateset.mp_state_set = new osg::StateSet();
				XML_Parse_StateSet(spXMLNextNode, &stateset);	
				mv_stateset.push_back(stateset);
			}	
			else if(str_node_name== "Text")
			{
				CText text;
				text.mp_text = new osgText::Text();
				XML_Parse_Text(spXMLNextNode, text.mp_text, &(text.m_name));	
				text.mp_text->setDataVariance(osg::Object::DYNAMIC);//or use a callback
				mv_text.push_back(text);
			}
			/*else if(str_node_name== "Texture")
			{
				CTexture texture;
				texture.mp_texture = new osg::Texture;
				XML_Parse_Texture(spXMLNextNode, &texture);	
				mv_text.push_back(texture);
			}*/
			else if(str_node_name== "Texture2D")
			{
				CTexture2D texture;
				texture.mp_texture = new osg::Texture2D;
				XML_Parse_Texture2D(spXMLNextNode, &texture);	
				mv_texture2D.push_back(texture);
			}
			else if(str_node_name== "Uniform")
			{
				CUniform uniform;				
				XML_Parse_Uniform(spXMLNextNode, &uniform);	
				if(uniform.m_type== "Vec3")
				{
					uniform.mp_uniform = new osg::Uniform(uniform.m_ID.c_str(),uniform.m_vec3);
				}
				else if(uniform.m_type== "Float")
				{
					uniform.mp_uniform = new osg::Uniform(uniform.m_ID.c_str(),uniform.m_f);
				}
				mv_uniform.push_back(uniform);
			}
			else if(str_node_name== "Variable")
			{
				CParsedVariable variable;
				XML_Parse_Variable(spXMLNextNode, &variable);	
				mv_variable.push_back(variable);
			}			
			else if(str_node_name== "Vec2Array")
			{
				CVec2Array arr;
				arr.mp_Vec2Array = new osg::Vec2Array;
				XML_Parse_Vec2Array(spXMLNextNode, &arr);				
				mv_Vec2Array.push_back(arr);
			}
			else if(str_node_name== "Vec3Array")
			{
				CXGMML_graph* p_graph= NULL;
				if(mv_xgmml_graph.size()> 0)
				{
					p_graph = &(mv_xgmml_graph[0]);
				}
				CVec3Array arr;
				arr.mp_Vec3Array = new osg::Vec3Array;
				XML_Parse_Vec3Array(spXMLNextNode, &arr, p_graph);	
				
				arr.m_count = arr.mp_Vec3Array->size();
				m_last_count = arr.mp_Vec3Array->size();
				
				mv_Vec3Array.push_back(arr);
			}
			else if(str_node_name== "Vec4Array")
			{
				CVec4Array arr;
				arr.mp_Vec4Array = new osg::Vec4Array;
				XML_Parse_Vec4Array(spXMLNextNode, &arr);				
				mv_Vec4Array.push_back(arr);
			}
			else if(str_node_name== "Viewer")
			{
				CViewer viewer;
				viewer.mp_viewer = new osgViewer::Viewer;
				XML_Parse_Viewer(spXMLNextNode, &viewer);				
				mv_viewer.push_back(viewer);
			}		
			else if(str_node_name== "VirtualFileSystem")
			{					
				//Virtual File System Manager -> initialize();
				//Virtual File System Manager -> mount( "my archive file name, e.g. a zip file", "/" ); // this mounts the archive into root path "/"
				//VFSStream vfsstream = Virtual File System Manager -> getStream( "/mesh/cow.osg" ); // this file is actually in the zip file archive
				//if ( vfsstream.valid() )
				//{
				//	osg::ref_ptr< osg::Node > node = osgDB::readNodeFile( vfsstream );
				//	...
				//}
			}
			else if(str_node_name== "WriteNodeThroughputToFile")
			{
				if(str_node_text== "TRUE")
					m_write_node_throughput_to_file = true;
				else if(str_node_text== "FALSE")
					m_write_node_throughput_to_file = false;
			}	
			else if(str_node_name== "WriteEnvStateToFile")
			{
				if(str_node_text== "TRUE")
					m_write_env_state_to_file = true;
				else if(str_node_text== "FALSE")
					m_write_env_state_to_file = false;
			}	
			
		}
	}

	return true;
}



/*
       .......................................................................................................             .   .       ..  .
     ..............................................................................................................    ... ... ..          .
   . ...........................=~~~:,,::::,,,,...................................................................... .......               
    ............................ ,I~:~::::::,,:~.............................................................................  . .. . ......
   . ........................ ...7.?~~~~~:::,:.~,...................................................................................  ......
   .............................,~$.?~~~~~:::...,, .................. ...=$+~~~~~+~+. ............................................   ... ...
   ............................ =:~7=?=~~~~~~....,...... ...............77+Z$O8NI+~8$.???$$=+=I.............................................
  ..............................,.,~7,I+=~=~,,,...:.....................:$?7??$O+II?77I77$$I7I+:~... .... ..................................
 ...........,~,..+=... ........~,,,:=I=I++==,,,,...=...................II?$$Z==:III7I7:I,....I?=~==. ................ ..................... 
 .........?+I7~::ZZ$?I....... .~~:~?:=I?7?+~:::,,,,,+................ :7$??D?D==7$7$7I7...+,::7$II?................,.:+=.:~~=...............
 .......,+$$I?+$:?$$D7=$......IZ??++=++D8$?=~:::,,,,:..................II8+ZII8?$+$7???.,I?,?I+$Z:~.............+77I~+=~?87?+IZ+............
........:DZ~?+?+?I+?Z77+~:+++$$$88ONO+??DO7+~:::::::..................I$=7+7I7$$7OII++I+??I................ ..?I+~:::~?Z7I=8DO+I=~..........
.......:87~~+II++?:$+ON$???I+?+~:~:~7I7DMNZI?+====~:..................7I?+IZZ7II$IDI+Z$O$7$?~~............?7?~:::~::=O7O8ZI?I$............ .
......,$7=:7IZ7+=?=~77+?7$DINO8=DDIOD7IO$DO?,,,~:~~..................,$787O7Z+?I$OMDZ$??DZ7?$I~...=I=.++I=:::~~:=ZDNZ=$I87=~+?..............
......:$?~$$$~+I=7ZI8$?~:$==7OI+78+ZN=ZZD:=8I~,?:,:................,$$$IOI8Z7Z=ZD77+:+$I87O?++,::::,Z7+?$~~~+77ZI?Z+7OO8O$~~+?..............
.....+77ZO:O8D8I$+I8?$::7?I=I$8?7OIIZ=:OOZ7$ZI~Z~+7+$I,+,.,........+.OZIIZ$7+$DN8ZI$?+:?ZOI8NZOI=N77I7?7ZDO$7$O7ZZ$$8D$O??~=?I++===,........
.....I?$=~O+??I$OD8D=?=?~78~=OZII+ZI=+~$Z+~:,:+$=::II+7O==Z7I?ZI7Z?ZOZ7I?O7ID8N?OZZ7~+DI~8+?D8Z7+7787N?:Z=?O7$Z7Z78$??O$O7I+??====~~=.......
.....??=OII7I~~=+?I8I77=:IZ?~Z7$+=$?:$?7N:+.,,+?=OO$:7+OD7$O??Z$O$I7I=+7~~=7?++++?,:~DIOIIIDII=~IZ7$$ZIO$$O8$Z7$I$$7O$D$I+ZNN8+7I7=.........
....~++II$~?I?=~88D?ZI~=+~7$++?=?:I=~7~7DMM7??I=Z$O7++:?:,O:=$:?~:+???=8?++OI?$8+7Z$$NN77DZ?,=8877$=Z78I?7=+Z7+77IIZI?OZZ7IN$+NOI7O.........
....=++Z+II$$$$=+Z~+??~8$++I7II+Z:I$=$ZZDN7=77I:~=+7Z=:+:=?~==,:I~I:~~++:=+?~+?~I8ZIZ8NZ$$Z7I??8Z+I$?ZO?++$78777I7+Z$=+Z78?DZ7D?+77.........
....~++I=77+++?$$::,$I:$I$Z+87=:$:IZ+ZZONI~Z+~7?II?$Z+:+:?=$~~+.~?=?=?+I~?+I?=7+=Z=O$$$8?O?+Z7++~7ZO?II=I?Z7Z7Z777+877+8$Z+8N7D8Z+7=........
....=?IZ+,~:,,=~I:,~++?I$ZZ?+==:$77IO$7+=:7+7+~~~~8ZD$Z=$:$ZZ$ZZZ78ZOOZ$Z8ZZOZ78O7+DZO?+?I??7?~+?7OO?$7?I=II7+$$I7ID$$?87I?Z7?~:,+O$?$:.....
....?+$8?,:,,:I~I:~++8I7Z$I8=~OZI8ZZ?8$=777ZO87I$7OO$=++7?OI?I~+~+???=~7:+++=7?==7+Z=+,,,,~=?~=IZ8IOZ+O?7M8NNDDDI$NN8DNDDDD8:+:,:IZ$,,,.....
....=:~=?::~~=$?$+?7I8+=?=I+==~:=$Z+~??=DOIO7??=:?77877=ZI7II?I=???==?=?=II??I?I?7$D8ZIII7O$ZNOO~Z7D:?O7I=7ZZ?ZZDNZDND8M8$N8=I=++ZDD7??.....
....,::=~==I?=+?I+$87=+O$DOZ8O?~ZD78$+N78NZ7==77I??+:O?=D+$Z7~D88OOOOZ8Z8O888O8D8DNZ?$?I???+I7+ZOIOM+7DN$O77OIO7D8+DD7=MO8+Z8ODDOOD88$,.....
....,~=I+$$$::~O=I=7N$$?N8ZON$77Z8I8OZZ$OOI7I$8Z78NII=8$I=7IZI8$7ZIII7ZZD7I7$OO$8?8~~=OZZ=I?:=7ZD$7D~?O$OI8MDZ87Z$=8Z+?M8NI77$ZN$O$Z,. .....
.....~+?O$:=~=?$77+?8Z$8NZ8M8ZONODODZODZ$OO:,,:+I+++$ZZ7ZIII7$7$$I$Z7=?$ZII7$OODZ?I:,:877~+I??+7=Z8=DO8??$?+Z?O8788MND8MNM$O8Z?ZOOI?........
... .:?$7Z$:I$O$::?~,ZNO8NIOOZ?8D8IOZ?OOINI:~==?+?8ND8DZ$7OOO$D?Z~8OZ==N7?Z7=I$I=DZ7?=:,I?O?OMZO7+?+++?=I78D?8$$$IOI7INZ$OM==~~:O+?+++......
.....=I=7$I$7OZI88=:I7Z8ZNZ$D$IZO+=I=:=O???77=?,,:=::???O$O$ZDI7Z$?7ZMII78?ZNN8$8O$Z77I7$O8$7,~II$OZ$DZZ+I?:~:=$ZN$O$$?Z$++O~::,=?====......
.....,?$=ZD~::Z8I~+I$??:7N8OZZ$$$7III?Z$,DNN8NNO$77$++?+..............7N78..$DNZZ8$II+7OIOZOO??7+O?==$7?I$ZI+~~~::::+7$IOZN:NZ7,~OOZZ...... 
......+Z$=I$7ZZZ7IO+?++7IZ==I~~~=~==?ZZ:=I?==~~:::::,:................?NZ,..$88+$8?OZ$Z$ZO7IZ77$?+++ZZ++IZI77777?=~~~::~::+??7:,,,I+......  
......,:ZO?++777??:$:+~7I??I7I7OZZZO7O~:,.,,.,,.,,:=,...............+7IOIO.$Z=O$NDNDDD8?DIIIO+7II7ZZ8OZ?$Z7?++II$777I+=~:?Z$Z8$?7I$~......  
. ... .::OOI?IIII:O=7=8ZOO88O888NDOD7~+?++?II7$$ZO7~.............. .~I8?$7+7O,$ODDD8Z7OZZ78O7$OZ?~7O$ZII7ZOOO$I++?I777$77???I7=$7=I.......  
........~II$?I?=,DO77ZZ,.........?+=$$ZZOZZOOO888M$I7...............=OO~8~+,8Z$777D8$I:?+.,I?I7IIIZ$I?II...=7ZOOZ7?==???+?++=~~~+=.... . .  
  ........~,,~+?O$DNN=.............IN=$+7IINZ87+$OI+I. .............. ?7=8?..$~O7..II,?+:..~IZ~++?7$$~I~.... .=7ZOOZ$I??ZO==IOZOZ....... .  
..  ... ...........  ...............:Z~=8ZN$8?+,,?=I.. .................I$I$.,O$8?:=Z$:~=.:I$I8Z~:IOZZI.. ......O$7$OIZZ=I8Z8OZ. .......... 
...     ...................................Z?DI$II~......................$$I7I.,:=Z?O+I?:$??Z7777$$I77=...................~:............   .
      .. ...  ...............................$+$?O+.......................7Z?8,:IO$O7Z?I=7+$77777$:III................................      
     .. .          ........................ .  ............................I$+$IIZ?D8I+7.7$$ZI++$8ZI7?............. ................. .     
 . . ..      ..  ..    .................................................... I=???~.Z7+~II.+$$I+?I,....................................     .
 .       . .     ..        .................................................~7=+$$$~7==:+?+.....I....................................       
 ..                           .................................................II+:7O$7?,..........................................         
                          ... .      ........... ...............................:Z$+=................................................       
         .               ... ..       .  ........................................7:??$............................................ ..       
                          ..          .   .......................................,,,... ...........................................         
                                    ... ..    ...................................................................................           
         ..                      ...   ...     ...  ..............................................................................          
                                 ....    ..    ...     ..........................................................................           
*/


