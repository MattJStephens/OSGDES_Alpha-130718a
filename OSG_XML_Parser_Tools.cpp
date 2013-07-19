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

#include "MJS_Sockets.h"
#include "OSG_DES.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Geometry_tools.h"
#include "OSG_expression.h"

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/UFOManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/SphericalManipulator>
#include <osg/BlendFunc>
#include <osg/ShadeModel>

#include <osg/Node>
#include <cmath>

//#include "OSG_Environment.h"
//
//extern CEnvironment g_environment;

EntityType string_to_entity_type(std::string str)
{
	EntityType ET = EntityType_Undefined;	
	if(str== "AlphaFunc") ET = EntityType_AlphaFunc;	
	else if(str== "AnimationControlPoint") ET = EntityType_AnimationControlPoint;
	else if(str== "AnimationPath") ET = EntityType_AnimationPath;
	else if(str== "Billboard") ET = EntityType_Billboard;
	else if(str== "BoundingBox") ET = EntityType_BoundingBox;
	else if(str== "ClearNode") ET = EntityType_ClearNode;
	else if(str== "CameraManipulator") ET = EntityType_CameraManipulator;			
	else if(str== "CMatrix") ET = EntityType_CMatrix;			
	else if(str== "Logistics_demand") ET = EntityType_Logistics_demand;						
	else if(str== "Logistics_operation") ET = EntityType_Logistics_operation;		
	else if(str== "Logistics_operative") ET = EntityType_Logistics_operative;	
	else if(str== "Logistics_node") ET = EntityType_Logistics_node;	
	else if(str== "Logistics_component") ET = EntityType_Logistics_component;		
	else if(str== "Logistics_producer") ET = EntityType_Logistics_producer;						
	else if(str== "Logistics_transition") ET = EntityType_Logistics_transition;						
	else if(str== "DrawArrays") ET = EntityType_DrawArrays;
	else if(str== "EntityArray") ET = EntityType_EntityArray;
	else if(str== "ExplosionEffect") ET = EntityType_ExplosionEffect;
	else if(str== "ExplosionDebrisEffect") ET = EntityType_ExplosionDebrisEffect;
	else if(str== "FireEffect") ET = EntityType_FireEffect;				
	else if(str== "Geode") ET = EntityType_Geode;
	else if(str== "EXPR_vector_field") ET = EntityType_EXPR_vector_field;
	else if(str== "EXPR") ET = EntityType_EXPR;
	else if(str== "Geometry") ET = EntityType_Geometry;	
	else if(str== "Gaussian") ET = EntityType_Gaussian;		
	else if(str== "Graph") ET = EntityType_Graph;					
	else if(str== "Group") ET = EntityType_Group;					
	else if(str== "HeightField") ET = EntityType_HeightField;					
	else if(str== "Image") ET = EntityType_Image;					
	else if(str== "Imposter") ET = EntityType_Imposter;					
	else if(str== "KeyBoardEventHandler") ET = EntityType_KeyBoardEventHandler;								
	else if(str== "PAT") ET = EntityType_PAT;
	else if(str== "Light") ET = EntityType_Light;
	else if(str== "LightSource") ET = EntityType_LightSource;					
	else if(str== "Material") ET = EntityType_Material;				
	//else if(str== "Matrix") ET = EntityType_Matrix;	
	else if(str== "MatrixTransform") ET = EntityType_MatrixTransform;	
	else if(str== "MultipleRandomEntities") ET = EntityType_MultipleRandomEntities;	
	else if(str== "Node") ET = EntityType_Node;

	else if(str== "PhysX_Actor") ET = EntityType_PhysX_Actor;
	else if(str== "PhysX_ActorDesc") ET = EntityType_PhysX_ActorDesc;

	else if(str== "NxBodyDesc") ET = EntityType_NxBodyDesc;
	else if(str== "NxBoxShapeDesc") ET = EntityType_NxBoxShapeDesc;
	else if(str== "NxClothDesc") ET = EntityType_NxClothDesc;
	else if(str== "NxClothMesh") ET = EntityType_NxClothMesh;
	else if(str== "NxClothMeshDesc") ET = EntityType_NxClothMeshDesc;
	else if(str== "NxFluid") ET = EntityType_NxFluid;
	else if(str== "NxFluidDesc") ET = EntityType_NxFluidDesc;
	else if(str== "NxMeshData") ET = EntityType_NxMeshData;
	else if(str== "NxSphereShapeDesc") ET = EntityType_NxSphereShapeDesc;
	else if(str== "NxTriangleMeshShapeDesc") ET = EntityType_NxTriangleMeshShapeDesc;
	else if(str== "NxScene") ET = EntityType_NxScene;
	else if(str== "NxWheelShapeDesc") ET = EntityType_NxWheelShapeDesc;

	else if(str== "Particle") ET = EntityType_Particle;			
	else if(str== "PolygonMode") ET = EntityType_PolygonMode;					
	else if(str== "Projection") ET = EntityType_Projection;	
	else if(str== "Program") ET = EntityType_Program;					
	else if(str== "PhysXPlane") ET = EntityType_PhysXPlane;	
	else if(str== "PhysXCube") ET = EntityType_PhysXCube;	
	else if(str== "PhysXCloth") ET = EntityType_PhysXCloth;	
	else if(str== "PhysXSphere") ET = EntityType_PhysXSphere;
	else if(str== "PhysXSoftBody") ET = EntityType_PhysXSoftBody;
	else if(str== "PhysXFluid") ET = EntityType_PhysXFluid;
	else if(str== "PhysXTerrain") ET = EntityType_PhysXTerrain;				
	else if(str== "ShapeDrawable") ET = EntityType_ShapeDrawable;
	else if(str== "Shader") ET = EntityType_Shader;
	else if(str== "Set") ET = EntityType_Set;			
	else if(str== "SmokeEffect") ET = EntityType_SmokeEffect;		
	else if(str== "Socket") ET = EntityType_Socket;		
	else if(str== "StateAttribute") ET = EntityType_StateAttribute;				
	else if(str== "StateSet") ET = EntityType_StateSet;							
	else if(str== "Text") ET = EntityType_Text;
	else if(str== "Texture") ET = EntityType_Texture;
	else if(str== "Texture2D") ET = EntityType_Texture2D;				
	else if(str== "Uniform") ET = EntityType_Uniform;				
	else if(str== "Vec2Array") ET = EntityType_Vec2Array;				
	else if(str== "Vec3Array") ET = EntityType_Vec3Array;				
	else if(str== "Vec4Array") ET = EntityType_Vec4Array;	
	return ET;
}

ReferenceType string_to_reference_type(std::string str)
{
	ReferenceType RT= ReferenceType_Undefined;
	if(str== "AddChild") RT = ReferenceType_AddChild;
	else if(str== "AddChildArray") RT = ReferenceType_AddChildArray;	
	else if(str== "AddPrimitiveSet") RT = ReferenceType_AddPrimitiveSet;				
	else if(str== "AddDrawable") RT = ReferenceType_AddDrawable;
	else if(str== "AddEventHandler") RT = ReferenceType_AddEventHandler;
	else if(str== "AddShader") RT = ReferenceType_AddShader;	
	else if(str== "AddShape") RT = ReferenceType_AddShape;		
	else if(str== "AddUniform") RT = ReferenceType_AddUniform;	
	else if(str== "Collate") RT = ReferenceType_Collate;	
	else if(str== "Insert") RT = ReferenceType_Insert;		
	else if(str== "Root") RT = ReferenceType_Root;		
	else if(str== "SetAddress") RT = ReferenceType_SetAddress;			
	else if(str== "SetAttribute") RT = ReferenceType_SetAttribute;	
	else if(str== "SetAttributeAndModes") RT = ReferenceType_SetAttributeAndModes;		
	else if(str== "SetColorArray") RT = ReferenceType_SetColorArray;	
	else if(str== "SetColorBinding") RT = ReferenceType_SetColorBinding;					
	else if(str== "SetCount") RT = ReferenceType_SetCount;				
	else if(str== "SetCameraManipulator") RT = ReferenceType_SetCameraManipulator;					
	else if(str== "SetImage") RT = ReferenceType_SetImage;						
	else if(str== "SetLight") RT = ReferenceType_SetLight;		
	else if(str== "SetMatrix") RT = ReferenceType_SetMatrix;			
	else if(str== "SetStateSet") RT = ReferenceType_SetStateSet;						
	else if(str== "SetStateSetMode") RT = ReferenceType_SetStateSetMode;					
	else if(str== "SetStateSetModesOn") RT = ReferenceType_SetStateSetModesOn;					
	else if(str== "SetStateSetModesOff") RT = ReferenceType_SetStateSetModesOff;
	else if(str== "SetTexCoordArray") RT = ReferenceType_SetTexCoordArray;	
	else if(str== "SetTextureAttributesAndModes") RT = ReferenceType_SetTextureAttributesAndModes;				
	else if(str== "SetUpdateCallback") RT = ReferenceType_SetUpdateCallback;				
	else if(str== "SetVertexArray") RT = ReferenceType_SetVertexArray;	
	else if(str== "SetZIntersect") RT = ReferenceType_SetZIntersect;		
	return RT;
}

Material *createSimpleMaterial(Vec4 color)
{
	Material *material = new Material();
	material->setDiffuse(Material::FRONT,  Vec4(0.0, 0.0, 0.0, 1.0));
	material->setEmission(Material::FRONT, color);

	return material;
}

CComPtr<IXMLDOMNode> XML_NextNode(CComPtr<IXMLDOMNodeList> spXMLDOMNodeList, string* p_str_node_name, string* p_str_node_text)
{	
	CComPtr<IXMLDOMNode> spXMLNextNode;
	HRESULT hr;	
	hr = spXMLDOMNodeList->nextNode(&spXMLNextNode);	
	if ( FAILED(hr) ) return false;
	BSTR NodeName;
	spXMLNextNode->get_nodeName(&NodeName);	
	*p_str_node_name = _bstr_t (NodeName);
	BSTR text;
	spXMLNextNode->get_text(&text);		
	*p_str_node_text = _bstr_t (text);
	return spXMLNextNode;
}

void XML_Parse_Attribute(CComPtr<IXMLDOMNode> spXMLNode, Node *node)
{
	/*HRESULT hr;
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
			if(str_node_name== "RefMaterialName")
			{
				long index = material_index_from_name(str_node_text);
				if(index!= UINT_MAX)
				{
					node->getOrCreateStateSet()->setAttribute(mv_material[index].mp_material); 			
				}
			}			
		}
	}	*/
}

void XML_Parse_AlphaFunc(CComPtr<IXMLDOMNode> spXMLNode, CAlphaFunc *alphafunc)
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
			if(str_node_name== "Name")
			{
				alphafunc->m_name = str_node_text;
			}		
			else if(str_node_name== "GequalMode")
			{		
				float fl= 0.05;
				istringstream iss(str_node_text);
				iss >> fl;				
				alphafunc->mp_alphafunc->setFunction(osg::AlphaFunc::GEQUAL, fl);
			}		
		}
	}
}

void XML_Parse_AnimationControlPoint(CComPtr<IXMLDOMNode> spXMLNode, CAnimationControlPoint *animation_control_point)
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
			if(str_node_name== "Name")
			{
				animation_control_point->m_name = str_node_text;
			}		
			else if(str_node_name== "Point")
			{		
				osg::Vec3d vec;
				XML_Parse_Vec3d(spXMLNextNode, &vec);				
				animation_control_point->m_control_point.setPosition(vec);
			}		
			else if(str_node_name== "Time")
			{					
				istringstream iss(str_node_text);
				iss >> animation_control_point->m_time;	
			}		
		}
	}

}

void XML_Parse_AnimationPath(CComPtr<IXMLDOMNode> spXMLNode, CAnimationPath *animation_path)
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
			if(str_node_name== "Name")
			{
				animation_path->m_name = str_node_text;
			}		
			else if(str_node_name== "SetLoopMode")
			{						
				if(str_node_text== "LOOP")
				{
					animation_path->mp_animation_path->setLoopMode(AnimationPath::LOOP);
				}
				if(str_node_text== "NO_LOOPING")
				{
					animation_path->mp_animation_path->setLoopMode(AnimationPath::NO_LOOPING);
				}
				else if(str_node_text== "SWING")
				{
					animation_path->mp_animation_path->setLoopMode(AnimationPath::SWING);
				}				
			}	
			else if(str_node_name== "Period")
			{
				//istringstream iss(str_node_text);
				//animation_path->mp_animation_path->setTimeControlPointMap(
			}	
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				animation_path->mv_ref.push_back(ref);
			}	
		}
	}
}


void XML_Parse_BoundingBox(CComPtr<IXMLDOMNode> spXMLNode,  CBoundingBox *bounding_box)
{



}
void XML_Parse_Billboard(CComPtr<IXMLDOMNode> spXMLNode,  CBillboard *billboard)
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
			if(str_node_name== "Name")
			{
				billboard->m_name = str_node_text;
			}	
			else if(str_node_name== "Mode")
			{			
				if(str_node_text== "AXIAL_ROT")
				{
					billboard->mp_billboard->setMode(osg::Billboard::AXIAL_ROT);
				}		
			}		
			else if(str_node_name== "Position")
			{
				XML_Parse_Vec3(spXMLNextNode, &(billboard->m_position));
			}	
			else if(str_node_name== "Axis")
			{
				Vec3 axis;
				XML_Parse_Vec3(spXMLNextNode, &axis);
				billboard->mp_billboard->setAxis(axis);
			}				
			else if(str_node_name== "Normal")
			{
				Vec3 normal;
				XML_Parse_Vec3(spXMLNextNode, &normal);
				billboard->mp_billboard->setNormal(normal);
			}	
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				billboard->mv_ref.push_back(ref);
			}		
		}
	}	
}
void XML_Parse_Box(CComPtr<IXMLDOMNode> spXMLNode,  Vec3d *cen, float *x, float *y, float *z)
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
			if(str_node_name== "Center")
			{
				XML_Parse_Vec3d(spXMLNextNode, cen);								
			}
			
			else if(str_node_name== "x")
			{				
				istringstream iss(str_node_text);
				iss >> *x;
			}		
			else if(str_node_name== "y")
			{			
				istringstream iss(str_node_text);
				iss >> *y;
			}		
			else if(str_node_name== "z")
			{				
				istringstream iss(str_node_text);
				iss >> *z;
			}		
		}
	}	
}
void XML_Parse_Camera(CComPtr<IXMLDOMNode> spXMLNode, CCamera *p_camera)
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
			if(str_node_name== "Name")
			{
				p_camera->m_name = str_node_text;
			}	
					
			else if(str_node_name== "VISIBLE_MASK")
			{
				int VISIBLE_MASK = 0x01;
				//int PICKABLE_MASK = 0x02;
				p_camera->mp_camera->setCullMask(VISIBLE_MASK);
			}	
			else if(str_node_name== "CLEAR_MASK")
			{
				p_camera->mp_camera->setClearMask( GL_DEPTH_BUFFER_BIT );
			}	
			else if(str_node_name== "POST_RENDER")
			{
				p_camera->mp_camera->setRenderOrder( osg::Camera::POST_RENDER );
			}	
			else if(str_node_name== "setAllowEventFocus")
			{
				p_camera->mp_camera->setAllowEventFocus( false );
			}	
			else if(str_node_name== "setProjectionMatrix")
			{
				double left= 0;
				double right= 10;
				double bottom= 0;
				double top= 10;				
				p_camera->mp_camera->setProjectionMatrix( osg::Matrix::ortho2D(left, right, bottom, top) );
			}
			else if(str_node_name== "setGraphicsContext")
			{
				//osg::GraphicsContext gc;
			//	p_camera->mp_camera->setGraphicsContext( gc.get() );
			}
			else if(str_node_name== "setViewport")
			{
				double width= 10;
				double height= 10;				
				p_camera->mp_camera->setViewport(new osg::Viewport(0,0, width, height));
			}
	/*		
	camera->setDrawBuffer(buffer);
	camera->setReadBuffer(buffer);*/
		}		
	}
}
void XML_Parse_Capsule(CComPtr<IXMLDOMNode> spXMLNode,  Vec3d *cen, float *radius, float *height)
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
			if(str_node_name== "Center")
			{
				XML_Parse_Vec3d(spXMLNextNode, cen);								
			}
			
			else if(str_node_name== "Radius")
			{
				float shiny = 0;
				istringstream iss(str_node_text);
				iss >> *radius;
			}		
			else if(str_node_name== "Height")
			{
				float shiny = 0;
				istringstream iss(str_node_text);
				iss >> *height;
			}			
		}
	}	
}
void XML_Parse_ClearNode(CComPtr<IXMLDOMNode> spXMLNode, CClearNode *p_clear_node)
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
			if(str_node_name== "Name")
			{
				p_clear_node->m_name = str_node_text;
			}	
			else if(str_node_name== "ClearColor")
			{
				Vec4d vec;
				XML_Parse_Vec4d(spXMLNextNode, &vec);
				p_clear_node->mp_clear_node->setClearColor(vec);
				//root->addChild(backdrop);
			}	
		}
	}

}
void XML_Parse_ColorSet(CComPtr<IXMLDOMNode> spXMLNode, CColorSet *p_color_set)
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
			if(str_node_name== "Count")
			{
				istringstream iss(str_node_text);
				iss >> p_color_set->m_count;
			}		
			else if(str_node_name== "Color")
			{
				osg::Vec4 vec4;			
				XML_Parse_Vec4(spXMLNextNode, &vec4);	
				p_color_set->mv_color.push_back(vec4);
			}			
		}
	}
}
void XML_Parse_Cone(CComPtr<IXMLDOMNode> spXMLNode,  Vec3d *cen, float *radius, float *height)
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
			if(str_node_name== "Center")
			{
				XML_Parse_Vec3d(spXMLNextNode, cen);								
			}
			
			else if(str_node_name== "Radius")
			{
				float shiny = 0;
				istringstream iss(str_node_text);
				iss >> *radius;
			}		
			else if(str_node_name== "Height")
			{
				float shiny = 0;
				istringstream iss(str_node_text);
				iss >> *height;
			}			
		}
	}	
}

void XML_Parse_CMatrix(CComPtr<IXMLDOMNode> spXMLNode, CMatrix* p_matrix)
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
			if(str_node_name== "Name")
			{
				p_matrix->m_name = str_node_text;
			}	
			else if(str_node_name== "Matrix")
			{				
				XML_Parse_Matrix(spXMLNextNode, &(p_matrix->m_matrix));
			}	
		}
	}
}

void XML_Parse_CustomCamera(CComPtr<IXMLDOMNode> spXMLNode, CCustomCamera *p_camera)
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
			if(str_node_name== "VelocityCoefficient")
			{				
				istringstream iss(str_node_text);
				iss >> p_camera->m_velocity_coefficient;
			}		
			else if(str_node_name== "MouseCoefficient")
			{				
				istringstream iss(str_node_text);
				iss >> p_camera->m_mouse_coefficient;
			}		
			else if(str_node_name== "Position")
			{
				XML_Parse_Vec3d(spXMLNextNode, &(p_camera->m_position));
			}				else if(str_node_name== "UpAxis")
			{		
				if(str_node_text == "X")
				{
					p_camera->m_up_axis = NavigateUpAxisX;
				}
				else if(str_node_text == "Y")
				{
					p_camera->m_up_axis = NavigateUpAxisY;
				}
			}
		}		
	}
}
void XML_Parse_Cylinder(CComPtr<IXMLDOMNode> spXMLNode,  Vec3d *cen, float *radius, float *height)
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
			if(str_node_name== "Center")
			{
				XML_Parse_Vec3d(spXMLNextNode, cen);								
			}
			
			else if(str_node_name== "Radius")
			{
				float shiny = 0;
				istringstream iss(str_node_text);
				iss >> *radius;
			}		
			else if(str_node_name== "Height")
			{
				float shiny = 0;
				istringstream iss(str_node_text);
				iss >> *height;
			}			
		}
	}	
}
void XML_Parse_DrawElementsUInt(CComPtr<IXMLDOMNode> spXMLNode, CDrawElementsUInt *p_DEUI)
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
			if(str_node_name== "Name")
			{
				p_DEUI->m_name = str_node_text;
			}	
			else if(str_node_name== "HUDBackgroundIndices")
			{
				p_DEUI->mp_DrawElementsUInt =  new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON, 0);
				p_DEUI->mp_DrawElementsUInt->push_back(0);
				p_DEUI->mp_DrawElementsUInt->push_back(1);
				p_DEUI->mp_DrawElementsUInt->push_back(2);
				p_DEUI->mp_DrawElementsUInt->push_back(3);
			}	
		}
	}
}
void XML_Parse_DrawArrays(CComPtr<IXMLDOMNode> spXMLNode, CDrawArrays *p_draw_arrays, unsigned long* p_count)
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
			if(str_node_name== "Name")
			{
				p_draw_arrays->m_name = str_node_text;
			}	
			else if(str_node_name== "PrimitiveSet")
			{
				if(str_node_text == "POINTS")
				{
					p_draw_arrays->mp_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::POINTS); 
				}
				else if(str_node_text == "LINES")
				{
					p_draw_arrays->mp_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::LINES); //unconnected verts
				}
				else if(str_node_text == "LINE_STRIP")
				{
					p_draw_arrays->mp_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP); //connected verts
				}
				else if(str_node_text == "LINE_LOOP")
				{
					p_draw_arrays->mp_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP); 
				}
				else if(str_node_text == "TRIANGLES")
				{
					p_draw_arrays->mp_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES); 
				}
				else if(str_node_text == "TRIANGLE_STRIP")
				{
					p_draw_arrays->mp_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP); 
				}
				else if(str_node_text == "TRIANGLE_FAN")
				{
					p_draw_arrays->mp_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN); 
				}
				else if(str_node_text == "QUADS")
				{
					p_draw_arrays->mp_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::QUADS); 
				}
				else if(str_node_text == "QUAD_STRIP ")
				{
					p_draw_arrays->mp_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP); 
				}
				else if(str_node_text == "POLYGON")
				{
					p_draw_arrays->mp_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::POLYGON); 
				}
			}	
			else if(str_node_name== "SetFirst")
			{				
				if(p_draw_arrays->mp_draw_arrays!= NULL)
				{
					istringstream iss(str_node_text);
					unsigned first_index;
					iss >> first_index;
					p_draw_arrays->mp_draw_arrays->setFirst(0); 
				}
				
			}	
			else if(str_node_name== "SetCount")
			{					
				if(p_draw_arrays->mp_draw_arrays!= NULL)
				{
					if(str_node_text== "LAST")
					{													
						p_draw_arrays->mp_draw_arrays->setCount(*p_count); 
					}
					else
					{
						istringstream iss(str_node_text);
						unsigned num_verticies_in_array;
						iss >> num_verticies_in_array;
						p_draw_arrays->mp_draw_arrays->setCount(num_verticies_in_array); 
					}
				}				
			}					
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_draw_arrays->mv_ref.push_back(ref);
			}		
		}
	}
}
void XML_Parse_EntityArray(CComPtr<IXMLDOMNode> spXMLNode, CEntityArray *p_entity_array)
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
			if(str_node_name== "Name")
			{
				p_entity_array->m_name = str_node_text;
			}	
			/*else if(str_node_name== "Ref")
			{			
				CReference ref;
				XML_Parse_Reference(spXMLNextNode, &ref);	
				p_entity_array->mv_ref.push_back(ref);
			}	*/
			else if(str_node_name== "Method")
			{			
				p_entity_array->m_array_method = str_node_text;
			}			
			else if(str_node_name== "Number_of_entities")
			{
				istringstream iss(str_node_text);
				iss >> p_entity_array->m_number_of_entities;
			}		
			else if(str_node_name== "angle_inc")
			{
				istringstream iss(str_node_text);
				iss >> p_entity_array->angle_inc;
			}	
			else if(str_node_name== "angle")
			{
				istringstream iss(str_node_text);
				iss >> p_entity_array->angle;
			}	
			else if(str_node_name== "rad")
			{
				istringstream iss(str_node_text);
				iss >> p_entity_array->rad;
			}	
			else if(str_node_name== "rad_inc")
			{
				istringstream iss(str_node_text);
				iss >> p_entity_array->rad_inc;
			}	
			else if(str_node_name== "x")
			{
				istringstream iss(str_node_text);
				iss >> p_entity_array->x;
			}	
			else if(str_node_name== "y")
			{
				istringstream iss(str_node_text);
				iss >> p_entity_array->y;
			}	
			else if(str_node_name== "z")
			{
				istringstream iss(str_node_text);
				iss >> p_entity_array->z;
			}	
			else if(str_node_name== "z_inc")
			{
				istringstream iss(str_node_text);
				iss >> p_entity_array->z_inc;
			}				
		}
	}
}
void XML_Parse_ExprFunction(CComPtr<IXMLDOMNode> spXMLNode, CExprFunction *func)
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
			if(str_node_name== "Expression")
			{
				func->m_str_expression = str_node_text;
			}				
			else if(str_node_name== "ExprInterval")
			{
				CExprInterval var;
				XML_Parse_ExprInterval(spXMLNextNode, &var);
				func->mv_expr_interval.push_back(var);
			}										
		}
	}
}
void XML_Parse_EXPR(CComPtr<IXMLDOMNode> spXMLNode, CEXPR *expr)
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
			if(str_node_name== "ID")
			{					
				expr->m_ID = str_node_text;	
			}	
			else if(str_node_name== "EXPR_Variable")
			{
				CEXPR_Variable var;
				XML_Parse_EXPR_Variable(spXMLNextNode, &var);
				expr->m_variable_list.add(var);
			}		
			else if(str_node_name== "EXPR_Operation")
			{
				CEXPR_Operation function;
				XML_Parse_EXPR_Operation(spXMLNextNode, &function);
				expr->m_operation_list.add(function);
				expr->m_active_index = expr->m_operation_list.size()-1;
			}		
			
		}
	}
}
void XML_Parse_EXPR_Variable(CComPtr<IXMLDOMNode> spXMLNode, CEXPR_Variable *EXPR_variable)
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
			if(str_node_name== "ID")
			{				
				istringstream iss(str_node_text);
				iss >> EXPR_variable->m_ID;	
			}		
			else if(str_node_name== "Value")
			{				
				double f;
				istringstream iss(str_node_text);
				iss >> f;
				EXPR_variable->set(f);	
			}		
			else if(str_node_name== "Type")
			{				
				if(str_node_text == "default")
				{
					EXPR_variable->m_type = EXPR_variable_type_default;
				}
				else if(str_node_text == "time")
				{
					EXPR_variable->m_type = EXPR_variable_type_time;
				}
				else if(str_node_text == "random")
				{
					EXPR_variable->m_type = EXPR_variable_type_random;
				}
				else if(str_node_text == "normal")
				{
					EXPR_variable->m_type = EXPR_variable_type_normal;
				}		
				else if(str_node_text == "external")
				{
					EXPR_variable->m_type = EXPR_variable_type_external;
				}		
				else if(str_node_text == "node_mass")
				{
					EXPR_variable->m_type = EXPR_variable_type_node_mass;
				}		
				else
				{
					EXPR_variable->m_type = EXPR_variable_type_default;
				}
			}	
			else if(str_node_name== "Mean")
			{				
				istringstream iss(str_node_text);
				iss >> EXPR_variable->m_mean;
			}		
			else if(str_node_name== "StandardDeviation")
			{				
				istringstream iss(str_node_text);
				iss >> EXPR_variable->m_standard_deviation;
			}		
			else if(str_node_name== "Ref")
			{
				CReference ref;
				XML_Parse_Reference(spXMLNextNode, &ref);
				EXPR_variable->mv_ref.push_back(ref);
			}			
		}
	}
}
void XML_Parse_EXPR_Operand(CComPtr<IXMLDOMNode> spXMLNode, CEXPR_Operand *EXPR_Operand)
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
			if(str_node_name== "Type")
			{					
				if(str_node_text == "Constant")
				{
					EXPR_Operand->m_type = EXPR_operand_type_constant;
				}
				else if(str_node_text == "Variable")
				{
					EXPR_Operand->m_type = EXPR_operand_type_variable;
				}
				else if(str_node_text == "Operation")
				{
					EXPR_Operand->m_type = EXPR_operand_type_operation;
				}
			}	
			else if(str_node_name== "Constant")
			{			
				float constant;
				istringstream iss(str_node_text);
				iss >> constant;	
				EXPR_Operand->mv_constant.push_back(constant);
			}				
			else if(str_node_name== "VarRef")
			{				
				CEXPR_VariableReference variable_reference;
				variable_reference.m_ID = str_node_text;
				EXPR_Operand->add_variable_reference(variable_reference);
			}	
			else if(str_node_name== "OperationRef")
			{				
				CEXPR_OperationReference operation_reference;
				operation_reference.m_ID= str_node_text;
				EXPR_Operand->add_operation_reference(operation_reference);					
			}			
		}
	}
}
void XML_Parse_EXPR_Operation(CComPtr<IXMLDOMNode> spXMLNode, CEXPR_Operation *EXPR_Operation)
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
			if(str_node_name== "ID")
			{					
				EXPR_Operation->m_ID = str_node_text;	
			}	
			else if(str_node_name== "Operand")
			{			
				//Operand
				CEXPR_Operand operand;
				XML_Parse_EXPR_Operand(spXMLNextNode, &operand);
				EXPR_Operation->mv_operand.push_back(operand);				
			}			
			else if(str_node_name== "Operator")
			{			
				//Operand
				EXPR_Operation->m_operator_type = string_to_EXPR_operator_type(str_node_text);
			}			
		}
	}
}
void XML_Parse_ExprInterval(CComPtr<IXMLDOMNode> spXMLNode, CExprInterval *var)
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

			if(str_node_name== "Name")
			{
				var->m_name = str_node_text;						
			}			
			else if(str_node_name== "Start")
			{
				istringstream iss(str_node_text);
				iss >> var->m_start;			
			}				
			else if(str_node_name== "End")
			{
				istringstream iss(str_node_text);
				iss >> var->m_end;			
			}				
			else if(str_node_name== "NumPts")
			{
				istringstream iss(str_node_text);
				iss >> var->m_num_pts;				
			}								
		}
	}
}

void XML_Parse_EXPRInterval(CComPtr<IXMLDOMNode> spXMLNode, CEXPRInterval *var)
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

			if(str_node_name== "Name")
			{
				var->m_name = str_node_text;						
			}			
			else if(str_node_name== "Start")
			{
				istringstream iss(str_node_text);
				iss >> var->m_start;			
			}				
			else if(str_node_name== "End")
			{
				istringstream iss(str_node_text);
				iss >> var->m_end;			
			}				
			else if(str_node_name== "NumPts")
			{
				istringstream iss(str_node_text);
				iss >> var->m_num_pts;				
			}								
		}
	}

}
void XML_Parse_ExplosionEffect(CComPtr<IXMLDOMNode> spXMLNode, CExplosionEffect *p_explosion)
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
			if(str_node_name== "Name")
			{
				p_explosion->m_name = str_node_text;
			}	
			else if(str_node_name== "Position")
			{
				XML_Parse_Vec3(spXMLNextNode, &(p_explosion->m_position));
			}	
			else if(str_node_name== "Scale")
			{
				istringstream iss(str_node_text);
				iss >> p_explosion->m_scale;
			}	
			else if(str_node_name== "intensity")
			{
				istringstream iss(str_node_text);
				iss >> p_explosion->m_intensity;
			}	
		}
	}	

}
void XML_Parse_ExplosionDebrisEffect(CComPtr<IXMLDOMNode> spXMLNode, CExplosionDebrisEffect *p_explosion_debris)
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
			if(str_node_name== "Name")
			{
				p_explosion_debris->m_name = str_node_text;
			}	
			else if(str_node_name== "Position")
			{
				XML_Parse_Vec3(spXMLNextNode, &(p_explosion_debris->m_position));
			}	
			else if(str_node_name== "Scale")
			{
				istringstream iss(str_node_text);
				iss >> p_explosion_debris->m_scale;
			}	
			else if(str_node_name== "intensity")
			{
				istringstream iss(str_node_text);
				iss >> p_explosion_debris->m_intensity;
			}				
		}
	}	
}
void XML_Parse_FireEffect(CComPtr<IXMLDOMNode> spXMLNode, CFireEffect *p_fire)
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
			if(str_node_name== "Name")
			{
				p_fire->m_name = str_node_text;
			}	
			else if(str_node_name== "Position")
			{
				XML_Parse_Vec3(spXMLNextNode, &(p_fire->m_position));
			}	
			else if(str_node_name== "Scale")
			{
				istringstream iss(str_node_text);
				iss >> p_fire->m_scale;
			}	
			else if(str_node_name== "intensity")
			{
				istringstream iss(str_node_text);
				iss >> p_fire->m_intensity;
			}	
		}
	}	
}
void XML_Parse_FunctionOfTwoVariables(CComPtr<IXMLDOMNode> spXMLNode, CFunctionOfTwoVariables *fotv)
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
			if(str_node_name== "Expression")
			{
				fotv->m_expression = str_node_text;
			}				
			else if(str_node_name== "StartX")
			{
				istringstream iss(str_node_text);
				iss >> fotv->m_start_x;			
			}	
			else if(str_node_name== "StartY")
			{
				istringstream iss(str_node_text);
				iss >> fotv->m_start_y;			
			}		
			else if(str_node_name== "EndX")
			{
				istringstream iss(str_node_text);
				iss >> fotv->m_end_x;			
			}	
			else if(str_node_name== "EndY")
			{
				istringstream iss(str_node_text);
				iss >> fotv->m_end_y;			
			}	
			else if(str_node_name== "NumPtsX")
			{
				istringstream iss(str_node_text);
				iss >> fotv->m_num_pts_x;				
			}		
			else if(str_node_name== "NumPtsY")
			{
				istringstream iss(str_node_text);
				iss >> fotv->m_num_pts_y;			
			}		
		}
	}
}
void XML_Parse_FunctionOfOneVariable(CComPtr<IXMLDOMNode> spXMLNode, CFunctionOfOneVariable *fotv)
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
			if(str_node_name== "Expression")
			{
				fotv->m_expression = str_node_text;
			}				
			else if(str_node_name== "StartX")
			{
				istringstream iss(str_node_text);
				iss >> fotv->m_start_x;			
			}				
			else if(str_node_name== "EndX")
			{
				istringstream iss(str_node_text);
				iss >> fotv->m_end_x;			
			}				
			else if(str_node_name== "NumPtsX")
			{
				istringstream iss(str_node_text);
				iss >> fotv->m_num_pts_x;				
			}				
		}
	}
}
void XML_Parse_Geode(CComPtr<IXMLDOMNode> spXMLNode, CGeode* p_geode)
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
			if(str_node_name== "Name")
			{
				p_geode->m_name = str_node_text;
			}		
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_geode->mv_ref.push_back(ref);
			}		
			else if(str_node_name== "Sphere")
			{				
				float radius;
				Vec3d center;
				XML_Parse_Sphere(spXMLNextNode, &center, &radius);				
				p_geode->mp_geode->addDrawable(new ShapeDrawable(new Sphere(center, radius)));			
			}
			else if(str_node_name== "SetNodeMask")
			{					
				unsigned u;
				istringstream iss(str_node_text);
				iss >>u;
				p_geode->mp_geode->setNodeMask(u);
			}			
			else if(str_node_name== "Box")
			{			
				float f_x=1;
				float f_y=1;
				float f_z=1;
				Vec3d center;
				XML_Parse_Box(spXMLNextNode, &center, &f_x, &f_y, &f_z);				
				p_geode->mp_geode->addDrawable(new ShapeDrawable(new Box(center, f_x,f_y,f_z)));			
			}
			else if(str_node_name== "Capsule")
			{				
				float radius=1;
				float height=1;
				Vec3d center;
				XML_Parse_Capsule(spXMLNextNode, &center, &radius, &height);				
				p_geode->mp_geode->addDrawable(new ShapeDrawable(new Capsule(center, radius, height)));			
			}
			else if(str_node_name== "Cone")
			{				
				float radius=1;
				float height=1;
				Vec3d center;
				XML_Parse_Cone(spXMLNextNode, &center, &radius, &height);				
				p_geode->mp_geode->addDrawable(new ShapeDrawable(new Cone(center, radius, height)));			
			}
			else if(str_node_name== "Cylinder")
			{				
				float radius=1;
				float height=1;
				Vec3d center;
				XML_Parse_Cylinder(spXMLNextNode, &center, &radius, &height);				
				p_geode->mp_geode->addDrawable(new ShapeDrawable(new Cylinder(center, radius, height)));			
			}
		
			else if(str_node_name== "HeightField")
			{				
				string height_file;
				string texture_file;
				XML_Parse_HeightFieldFromFile(spXMLNextNode, &height_file, &texture_file);				

				osg::Image* heightMap = osgDB::readImageFile(height_file);
				if(heightMap==NULL) 
					throw "Unable make Height Field";
				osg::HeightField* heightField = new osg::HeightField();
				if(heightField==NULL) 
					throw "Unable make Height Field";
				heightField->allocate(heightMap->s(), heightMap->t());
				heightField->setOrigin(osg::Vec3(-heightMap->s() / 2, -heightMap->t() / 2, 0));
				heightField->setXInterval(1.0f);
				heightField->setYInterval(1.0f);
				heightField->setSkirtHeight(1.0f);
	 
				for (unsigned int r = 0; r < heightField->getNumRows(); r++) 
				{
					for (unsigned int c = 0; c < heightField->getNumColumns(); c++)
					{
							heightField->setHeight(c, r, ((*heightMap->data(c, r)) / 255.0f) * 80.0f);
					}
				}
				
				p_geode->mp_geode->addDrawable(new osg::ShapeDrawable(heightField));
				osg::Texture2D* tex = new osg::Texture2D(osgDB::readImageFile(texture_file));
				tex->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR_MIPMAP_LINEAR);
				tex->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
				tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
				tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
				p_geode->mp_geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex);

				if(p_geode->mp_geode==NULL)throw "Unable make Height Field";	
			}	
		/*	else if(str_node_name == "Optimizer")
			{
				if((loadedModel!= NULL)&&(str_node_text== "T"))
				{
					osgUtil::Optimizer optimizer;
					optimizer.optimize(loadedModel.get());			
				}
			}		*/		
			else if(str_node_name== "Cube")
			{				
				p_geode->mp_geode = createUnitCube();				
			}		
			else if(str_node_name== "HUD_Geometry")
			{				
				p_geode->mp_geode = createUnitCube();				
			}
		}
	}	
}
void XML_Parse_Geometry(CComPtr<IXMLDOMNode> spXMLNode, CGeometry *p_geometry)
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
			if(str_node_name== "Name")
			{
				p_geometry->m_name = str_node_text;
			}	
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_geometry->mv_ref.push_back(ref);
			}	
			else if(str_node_name== "SetColorBinding")
			{
				if(str_node_text== "BIND_PER_VERTEX")
				{
					if(p_geometry->mp_geometry!= NULL)
					{
						p_geometry->mp_geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX); 
					}
				}
			}		
		}
	}	
}

void XML_Parse_Graph(CComPtr<IXMLDOMNode> spXMLNode, CGraph *p_graph)
{
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);

	unsigned width= 500;
	unsigned height= 500;
	std::vector<Vec3d> v_Vec3d;

	for(long i= 0; i< listLength; i++)
	{
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{		
			if(str_node_name== "Name")
			{
				p_graph->m_name = str_node_text;
			}	
			else if(str_node_name== "Type")
			{
				p_graph->m_type = str_node_text;
			}			
			else if(str_node_name== "Width")
			{			
				istringstream iss(str_node_text);
				iss >> p_graph->m_width;		
			}	
			else if(str_node_name== "Height")
			{				
				istringstream iss(str_node_text);
				iss >> p_graph->m_height;		
			}
			else if(str_node_name== "Offset_x")
			{				
				istringstream iss(str_node_text);
				iss >>p_graph-> m_offset_x;		
			}
			else if(str_node_name== "Offset_y")
			{				
				istringstream iss(str_node_text);
				iss >> p_graph->m_offset_y;		
			}
			else if(str_node_name== "Vec3d")
			{
				Vec3d vec;
				XML_Parse_Vec3d(spXMLNextNode, &vec);	
				v_Vec3d.push_back(vec);
				
			}
		}
	}
	p_graph->update(&v_Vec3d);

	/*
	double offset_x= (double)p_graph->m_offset_x;
	double offset_y= (double)p_graph->m_offset_y;
	p_graph->m_height= height;
	p_graph->m_width= width;
	Vec3d vec;
	vec.set(offset_x, offset_y,0);
	p_graph->mp_Vec3Array->push_back(vec);

	vec.set(offset_x, offset_y+height,0);
	p_graph->mp_Vec3Array->push_back(vec);

	vec.set(offset_x, offset_y,0);
	p_graph->mp_Vec3Array->push_back(vec);
	
	vec.set(offset_x+width,offset_y,0);
	p_graph->mp_Vec3Array->push_back(vec);
	

	unsigned count = 0;
	double min_x= 0;
	double max_x= 0;
	double min_y= 0;
	double max_y= 0;
	double scale_x= 1;
	double scale_y= 1;
	double delta_x= 1;
	double delta_y= 1;
	
	//get max and min
	std::vector<Vec3d>::iterator vec_iter;
	for(vec_iter = v_Vec3d.begin(); vec_iter != v_Vec3d.end(); ++vec_iter)
	{
		if(vec_iter->x() <min_x)
		{
			min_x = vec_iter->x();
		}
		if(vec_iter->x() > max_x)
		{
			max_x = vec_iter->x();
		}
		if(vec_iter->y() <min_y)
		{
			min_y = vec_iter->y();
		}
		if(vec_iter->y() > max_y)
		{
			max_y = vec_iter->y();
		}
		count++;
	}

	delta_x = max_x - min_x;
	delta_y = max_y - min_y;

	if(max_x!= 0)
	{
		scale_x = width/delta_x;
	}

	if(max_y!= 0)
	{
		scale_y = height/delta_y;
	}

	//scale
	vec_iter = v_Vec3d.begin();
	Vec3d previous; 
	previous.x()= offset_x+ (vec_iter->x()*scale_x);
	previous.y()= offset_y+ (vec_iter->y()*scale_y);
	for(vec_iter = v_Vec3d.begin()+1; vec_iter != v_Vec3d.end(); ++vec_iter)
	{
		vec_iter->x()= offset_x+ (vec_iter->x()*scale_x);
		vec_iter->y()= offset_y+ (vec_iter->y()*scale_y);
		p_graph->mp_Vec3Array->push_back(previous);	
		p_graph->mp_Vec3Array->push_back(*vec_iter);	
		previous = *vec_iter;
	}
	
	p_graph->mp_draw_arrays->setFirst(0);
	p_graph->mp_draw_arrays->setCount((count*2)+4);

	p_graph->mp_geometry->addPrimitiveSet(p_graph->mp_draw_arrays);
	p_graph->mp_geometry->setVertexArray(p_graph->mp_Vec3Array);

	p_graph->mp_geode->addDrawable(p_graph->mp_geometry);
	//p_graph->mp_geode->getOrCreateStateSet()->setAttribute(mv_material[ref_iter->m_index].mp_material);

	p_graph->mp_matrix_transform->setMatrix(osg::Matrix::identity());		
	p_graph->mp_matrix_transform->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	p_graph->mp_matrix_transform->addChild(p_graph->mp_geode);
	p_graph->mp_projection->setMatrix(osg::Matrix::ortho2D(0,1024,0,768));
	p_graph->mp_projection->addChild(p_graph->mp_matrix_transform);

	//p_graph->mp_PAT->addChild(p_graph->mp_geode);

	*/

}
void XML_Parse_Group(CComPtr<IXMLDOMNode> spXMLNode, CGroup *p_group, std::size_t graph_node_count)
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
			if(str_node_name== "EntityCreationFlag")
			{
				if(str_node_text == "Create") {	p_group->m_EntityCreationFlag = eEntityCreationFlag_Create; }
				if(str_node_text == "AddCreate") {	p_group->m_EntityCreationFlag = eEntityCreationFlag_AddCreate; }
				if(str_node_text == "AddOverwrite") {	p_group->m_EntityCreationFlag = eEntityCreationFlag_AddOverwrite; }
			}	
			else if(str_node_name== "Name")
			{
				p_group->m_name = str_node_text; 				
			}				
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_group->mv_ref.push_back(ref);
			}		
			else if(str_node_name== "RefArray")
			{
				unsigned int n=0;
				CReference ref;				
				XML_Parse_ReferenceArray(spXMLNextNode, &ref, &n);		
				std::string name= ref.m_ref_name;
				unsigned int i= 0;
				for(i= 0; i< n; i++)
				{
					std::ostringstream oss;
					oss << name;
					oss << i;
					ref.m_ref_name = oss.str();
					oss.clear();
					p_group->mv_ref.push_back(ref);
				}
			}		
			else if(str_node_name== "GraphRefArray")
			{
				unsigned int n= graph_node_count;
				CReference ref;				
				XML_Parse_ReferenceArray(spXMLNextNode, &ref, &n);		
				std::string name= ref.m_ref_name;
				unsigned int i= 0;
				for(i= 0; i< n; i++)
				{
					std::ostringstream oss;
					oss << name;
					oss << i;
					ref.m_ref_name = oss.str();
					oss.clear();
					p_group->mv_ref.push_back(ref);
				}
			}		
		}
	}
}
void XML_Parse_HeightField(CComPtr<IXMLDOMNode> spXMLNode,  CHeightField *p_height_field)
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
			if(str_node_name== "Name")
			{
				p_height_field->m_name = str_node_text;
			}				
			else if(str_node_name== "Size")
			{
				istringstream iss(str_node_text);
				iss >> p_height_field->m_size;
			}				
			else if(str_node_name== "Scale")
			{
				istringstream iss(str_node_text);
				iss >> p_height_field->m_scale;
			}			
			else if(str_node_name== "ZScale")
			{
				istringstream iss(str_node_text);
				iss >> p_height_field->m_z_scale;
			}	
			else if(str_node_name== "Columns")
			{
				istringstream iss(str_node_text);
				iss >> p_height_field->m_columns;
			}	
			else if(str_node_name== "Rows")
			{
				istringstream iss(str_node_text);
				iss >> p_height_field->m_rows;
			}				
		}
	}
}
void XML_Parse_HeightFieldFromFile(CComPtr<IXMLDOMNode> spXMLNode,  string *height_file, string *texture_file)
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
			if(str_node_name== "HeightFile")
			{
				*height_file = 	str_node_text;						
			}			
			else if(str_node_name== "TextureFile")
			{
				*texture_file = 	str_node_text;	
			}				
		}
	}	
}
void XML_Parse_Image(CComPtr<IXMLDOMNode> spXMLNode, CImage *p_image)
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
			if(str_node_name== "Name")
			{
				p_image->m_name = str_node_text;
			}				
			else if(str_node_name== "FileName")
			{
				p_image->mp_image = osgDB::readImageFile(str_node_text.c_str());
			}				
		}
	}
}
void XML_Parse_InsertControlPoint(CComPtr<IXMLDOMNode> spXMLNode, double *time, osg::Vec3d *pt)
{


}

void XML_Parse_Imposter(CComPtr<IXMLDOMNode> spXMLNode, CImposter *p_imposter)
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
			if(str_node_name== "Name")
			{
				p_imposter->m_name = str_node_text;
			}	
			else if(str_node_name== "SetImpostorThresholdToBound")
			{
				float f;
				istringstream iss(str_node_text);
				iss >> f;		
				p_imposter->mp_imposter->setImpostorThresholdToBound(f);
			}	
			else if(str_node_name== "SetImposterThreshold")
			{
				float f;
				istringstream iss(str_node_text);
				iss >> f;		
				p_imposter->mp_imposter->setImpostorThreshold(f);
			}	
			else if(str_node_name== "SetRange")
			{
				unsigned int childNo;
				float min;
				float max;
				XML_Parse_Range(spXMLNextNode, &childNo, &min, &max);
				p_imposter->mp_imposter->setRange(childNo, min, max);
			}	
			else if(str_node_name== "SetCenter")
			{
				Vec3d vec;
				XML_Parse_Vec3d(spXMLNextNode, &vec);	
				p_imposter->mp_imposter->setCenter(vec);
			}			
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_imposter->mv_ref.push_back(ref);
			}			
		}
	}

}

void XML_Parse_Light(CComPtr<IXMLDOMNode> spXMLNode, CLight *p_light)
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
			if(str_node_name== "Ambient")
			{
				Vec4d vec;
				XML_Parse_Vec4d(spXMLNextNode, &vec);	
				p_light->mp_light->setAmbient(vec);
			}	
			
			else if(str_node_name== "Diffuse")
			{
				Vec4d vec;
				XML_Parse_Vec4d(spXMLNextNode, &vec);	
				p_light->mp_light->setDiffuse(vec);
			}
			else if(str_node_name== "Direction")
			{
				Vec3 vec;
				XML_Parse_Vec3(spXMLNextNode, &vec);	
				p_light->mp_light->setDirection(vec);
			}
			else if(str_node_name== "Name")
			{
				p_light->m_name = str_node_text;
			}
			else if(str_node_name== "SetLightNum")
			{
				int n;
				istringstream iss(str_node_text);
				iss >> n;	
				p_light->mp_light->setLightNum(n);
			}
			else if(str_node_name== "Position")
			{
				Vec4d vec;
				XML_Parse_Vec4d(spXMLNextNode, &vec);	
				p_light->mp_light->setPosition(vec);
			}	
			else if(str_node_name== "SetConstantAttenuation")
			{
				float f;
				istringstream iss(str_node_text);
				iss >> f;		
				p_light->mp_light->setConstantAttenuation(f);
			}
			else if(str_node_name== "SetLinearAttenuation")
			{
				float f;
				istringstream iss(str_node_text);
				iss >> f;		
				p_light->mp_light->setLinearAttenuation(f);
			}
			else if(str_node_name== "SetQuadraticAttenuation")
			{
				float f;
				istringstream iss(str_node_text);
				iss >> f;		
				p_light->mp_light->setQuadraticAttenuation(f);
			}
			else if(str_node_name== "Specular")
			{
				Vec4d vec;
				XML_Parse_Vec4d(spXMLNextNode, &vec);	
				p_light->mp_light->setSpecular(vec);
			}
			else if(str_node_name== "SetSpotCutoff")
			{
				float f;
				istringstream iss(str_node_text);
				iss >> f;	
				p_light->mp_light->setSpotCutoff(f);
			}
			else if(str_node_name== "SetSpotExponent")
			{
				float f;
				istringstream iss(str_node_text);
				iss >> f;	
				p_light->mp_light->setSpotExponent(f);
			}			
		}
	}	
}
void XML_Parse_Light_Source(CComPtr<IXMLDOMNode> spXMLNode, CLightSource *p_light_source)
{
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	PositionAttitudeTransform *PAT = NULL;
	for(long i= 0; i< listLength; i++)
	{
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "Name")
			{
				p_light_source->m_name = str_node_text;
			}
			if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_light_source->mv_ref.push_back(ref);
			}					
			else if(str_node_name== "SetLocalStateSetModes")
			{				
				if(str_node_text == "ON")
					p_light_source->mp_light_source->setLocalStateSetModes(StateAttribute::ON); 							
				if(str_node_text == "OFF")
					p_light_source->mp_light_source->setLocalStateSetModes(StateAttribute::OFF); 							
			}			
		}
	}	
}
void XML_Parse_Logistics_component(CComPtr<IXMLDOMNode> spXMLNode, CLogistics_component *p_component)
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
			if(str_node_name== "Name")
			{
				p_component->m_name = str_node_text;
			}	
			else if(str_node_name== "Mass")
			{				
				istringstream iss(str_node_text);
				iss >> p_component->m_mass;						
			}	
			if(str_node_name== "GeodeRef")
			{
				p_component->m_component_type = ComponentType_Geode;
				XML_Parse_Reference(spXMLNextNode, &(p_component->m_ref_component_geometry));		
			}	
			if(str_node_name== "NodeRef")
			{
				p_component->m_component_type = ComponentType_Node;
				XML_Parse_Reference(spXMLNextNode, &(p_component->m_ref_component_geometry));		
			}	
			if(str_node_name== "SubComponentRef")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);	
				p_component->mv_sub_component.push_back(ref);
			}		
			else if(str_node_name== "TransportMode")
			{				
				if(str_node_text== "BeeLine")
				{
					p_component->m_transport_mode= TransportMode_BeeLine;						
				}
				if(str_node_text== "Route")
				{
					p_component->m_transport_mode= TransportMode_Route;						
				}
			}	
			
		}
	}
}
void XML_Parse_Logistics_demand(CComPtr<IXMLDOMNode> spXMLNode, CLogistics_demand *p_demand)	
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
			if(str_node_name== "Name")
			{
				p_demand->m_name = str_node_text;
			}		
			else if(str_node_name== "Type")
			{
				p_demand->m_type = str_node_text;
			}		
			else if(str_node_name== "DemandType")
			{
				p_demand->m_str_demand_type = str_node_text;
				if(p_demand->m_str_demand_type == "Periodic")
				{
					p_demand->m_demand_type = DemandType_Periodic;
				}
				else if(p_demand->m_str_demand_type == "Stochastic")
				{
					p_demand->m_demand_type = DemandType_Stochastic;
				}				
				else if(p_demand->m_str_demand_type == "EXPR")
				{
					p_demand->m_demand_type = DemandType_EXPR;
				}				
			}				
			else if(str_node_name== "OrderType")
			{
				if(str_node_text == "DES")
				{
					p_demand->m_order_type = OrderType_DES;
				}
				else if(str_node_text == "Continuous")
				{
					p_demand->m_order_type = OrderType_Continuous;
				}	
			}	
			else if(str_node_name== "SetActiveTimeType")
			{				
				if(str_node_text == "Simulation")
				{
					p_demand->m_initialise_active_time = true;
					p_demand->m_init_active_time_type = TimeType_simulation;
				}
				else if(str_node_text == "Real")
				{
					p_demand->m_initialise_active_time = true;
					p_demand->m_init_active_time_type = TimeType_real;
				}								
			}					
			else if(str_node_name== "DemandRateMethod")
			{
				p_demand->m_str_random_distribution_type = str_node_text;
				if(p_demand->m_str_random_distribution_type == "Const")	{
					p_demand->m_demand_rate_method = DemandRateMethod_Const;
				}
				else if(p_demand->m_str_random_distribution_type == "Bernoulli") {
					p_demand->m_demand_rate_method = DemandRateMethod_Bernoulli;
				}	
				else if(p_demand->m_str_random_distribution_type == "Geometric") {
					p_demand->m_demand_rate_method = DemandRateMethod_Geometric;
				}	
				else if(p_demand->m_str_random_distribution_type == "Triangle")	{
					p_demand->m_demand_rate_method = DemandRateMethod_Triangle;
				}	
				else if(p_demand->m_str_random_distribution_type == "Exponential") {
					p_demand->m_demand_rate_method = DemandRateMethod_Exponential;
				}	
				else if(p_demand->m_str_random_distribution_type == "Normal") {
					p_demand->m_demand_rate_method = DemandRateMethod_Normal;
				}				
				else if(p_demand->m_str_random_distribution_type == "Lognormal") {
					p_demand->m_demand_rate_method = DemandRateMethod_LogNormal;
				}						
			}	
			else if(str_node_name== "StandardDeviation")
			{				
				istringstream iss(str_node_text);
				iss >> p_demand->m_standard_deviation;						
			}	
			else if(str_node_name== "Mean")
			{				
				istringstream iss(str_node_text);
				iss >> p_demand->m_mean;						
			}	
			else if(str_node_name== "ComponentRef")
			{				
				XML_Parse_Reference(spXMLNextNode, &(p_demand->m_ref_component));								
			}	
			else if(str_node_name== "NodeRef")
			{				
				XML_Parse_Reference(spXMLNextNode, &(p_demand->m_ref_node));								
			}	
			else if(str_node_name== "SourceNodeRef")
			{				
				CReference ref;
				XML_Parse_Reference(spXMLNextNode, &ref);	
				p_demand->mv_ref_source_node.push_back(ref);
			}				
			else if(str_node_name== "EXPR_REF")
			{						
				XML_Parse_Reference(spXMLNextNode, &(p_demand->m_ref_EXPR));					
			}	
			else if(str_node_name== "Period")
			{			
				float f;
				istringstream iss(str_node_text);
				iss >> f;	
				p_demand->set_demand_period(f);
			}	
			else if(str_node_name== "Logistics_operativeState")
			{				
				if(str_node_text == "transit_destroy_at_destination")
					p_demand->m_op_state = Logistics_operativeState_in_transit_destroy_at_destination;				
				else if(str_node_text == "transit_store_at_destination")
					p_demand->m_op_state = Logistics_operativeState_in_transit_store_at_destination;				
			}				
		}
	}
}
void XML_Parse_Logistics_node(CComPtr<IXMLDOMNode> spXMLNode, CLogistics_node *p_node)	
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
			if(str_node_name== "Name")
			{
				p_node->m_name = str_node_text;
			}	
			if(str_node_name== "Record")
			{
				if(str_node_text== "true")
				{
					p_node->m_record= true;
				}
			}	
			else if(str_node_name== "PATRef")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &(p_node->m_ref_PAT));								
			}	
			else if(str_node_name== "GeodeRef")
			{							
				XML_Parse_Reference(spXMLNextNode, &(p_node->m_ref_geo));	
				p_node->m_Logistics_node_type= Logistics_nodeType_Geode;
			}			
			else if(str_node_name== "NodeRef")
			{							
				XML_Parse_Reference(spXMLNextNode, &(p_node->m_ref_geo));	
				p_node->m_Logistics_node_type= Logistics_nodeType_Node;
			}		
		}
	}
}
void XML_Parse_Logistics_operation(CComPtr<IXMLDOMNode> spXMLNode, CLogistics_operation *p_operation)
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
			if(str_node_name== "Name")
			{
				p_operation->m_name = str_node_text;
			}			
		}
	}
}
void XML_Parse_Logistics_producer(CComPtr<IXMLDOMNode> spXMLNode, CLogistics_producer *p_producer)
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
			if(str_node_name== "Name")
			{
				p_producer->m_name = str_node_text;
			}		
			else if(str_node_name== "ProductionType")
			{
				p_producer->m_str_production_type = str_node_text;
				if(p_producer->m_str_production_type == "Periodic")
				{
					p_producer->m_production_type = ProductionType_Periodic;
				}
				else if(p_producer->m_str_production_type == "Synthesis")
				{
					p_producer->m_production_type = ProductionType_Synthesis;
				}	
				else if(p_producer->m_str_production_type == "Stochastic")
				{
					p_producer->m_production_type = ProductionType_Stochastic;
				}	
				else if(p_producer->m_str_production_type == "PeriodicVolume")
				{
					p_producer->m_production_type = ProductionType_PeriodicVolume;
				}	
				else if(p_producer->m_str_production_type == "EXPR")
				{
					p_producer->m_production_type = ProductionType_EXPR;
				}					
			}		
			else if(str_node_name== "ProductionRateMethod")
			{				
				p_producer->m_str_random_distribution_type = str_node_text;
				if(p_producer->m_str_random_distribution_type == "Const")	{
					p_producer->m_production_rate_method = ProductionRateMethod_Const;
				}
				else if(p_producer->m_str_random_distribution_type == "Bernoulli") {
					p_producer->m_production_rate_method = ProductionRateMethod_Bernoulli;
				}	
				else if(p_producer->m_str_random_distribution_type == "Geometric") {
					p_producer->m_production_rate_method = ProductionRateMethod_Geometric;
				}	
				else if(p_producer->m_str_random_distribution_type == "Triangle")	{
					p_producer->m_production_rate_method = ProductionRateMethod_Triangle;
				}	
				else if(p_producer->m_str_random_distribution_type == "Exponential") {
					p_producer->m_production_rate_method = ProductionRateMethod_Exponential;
				}	
				else if(p_producer->m_str_random_distribution_type == "Normal") {
					p_producer->m_production_rate_method = ProductionRateMethod_Normal;
				}				
				else if(p_producer->m_str_random_distribution_type == "Lognormal") {
					p_producer->m_production_rate_method = ProductionRateMethod_LogNormal;
				}		
				else if(p_producer->m_str_random_distribution_type == "EXPR") {
					p_producer->m_production_rate_method = ProductionRateMethod_EXPR;
				}	
				
			}	
			else if(str_node_name== "PROD_EXPR_REF")
			{						
				XML_Parse_Reference(spXMLNextNode, &(p_producer->m_ref_prod_EXPR));					
			}	
			else if(str_node_name== "FLOW_EXPR_REF")
			{						
				XML_Parse_Reference(spXMLNextNode, &(p_producer->m_ref_flow_EXPR));					
			}			
			else if(str_node_name== "StandardDeviation")
			{				
				istringstream iss(str_node_text);
				iss >> p_producer->m_standard_deviation;						
			}	
			else if(str_node_name== "Mean")
			{				
				istringstream iss(str_node_text);
				iss >> p_producer->m_mean;						
			}	
			else if(str_node_name== "ComponentRef")
			{				
				XML_Parse_Reference(spXMLNextNode, &(p_producer->m_ref_component));								
			}	
			else if(str_node_name== "NodeRef")
			{				
				XML_Parse_Reference(spXMLNextNode, &(p_producer->m_ref_node));								
			}	
			else if(str_node_name== "PeriodTotal")
			{				
				double lf;
				istringstream iss(str_node_text);
				iss >> lf;				
				p_producer->set_total_production_period(lf);						
			}	
			else if(str_node_name== "Volume")
			{				
				istringstream iss(str_node_text);
				iss >> p_producer->m_volume;						
			}	
			else if(str_node_name== "TotalVolume")
			{				
				istringstream iss(str_node_text);
				iss >> p_producer->m_total_volume;						
			}		
			else if(str_node_name== "TotalMass")
			{				
				istringstream iss(str_node_text);
				iss >> p_producer->m_total_mass;						
			}		
			else if(str_node_name== "InfiniteSupply")
			{				
				if(str_node_text == "TRUE")
					p_producer->m_infinite_supply = true;						
				if(str_node_text == "FALSE")
					p_producer->m_infinite_supply = false;						
			}					
		}
	}
}
void XML_Parse_Matrix(CComPtr<IXMLDOMNode> spXMLNode, osg::Matrix* p_matrix)
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
			if(str_node_name== "translate")
			{
				osg::Vec3d translate;
				XML_Parse_Vec3d(spXMLNextNode, &translate);					
				*p_matrix = osg::Matrix::translate(translate);
				//*p_matrix = osg::Matrix::scale(scale)*osg::Matrix::translate(translate);
			}
		}
	}	
}
void XML_Parse_MatrixTransform(CComPtr<IXMLDOMNode> spXMLNode, CMatrixTransform* p_matrix_transform)
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
			if(str_node_name== "Name")
			{
				p_matrix_transform->m_name = str_node_text;
			}		
			else if(str_node_name== "Identity")
			{
				p_matrix_transform->mp_matrix_transform->setMatrix(osg::Matrix::identity());
			}	
			else if(str_node_name== "ReferenceFrame")
			{
				if(str_node_text== "ABSOLUTE_RF")
				{
					p_matrix_transform->mp_matrix_transform->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
				}
			}
			else if(str_node_name== "Matrix")
			{
				XML_Parse_Matrix(spXMLNextNode, &(p_matrix_transform->m_matrix));	
				p_matrix_transform->mp_matrix_transform->setMatrix(p_matrix_transform->m_matrix);
			}	
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_matrix_transform->mv_ref.push_back(ref);
			}		
		}
	}	
}


void XML_Parse_MemoryMapClient(CComPtr<IXMLDOMNode> spXMLNode, CMemoryMapClient* p_memory_map_client)
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
			if(str_node_name== "Name")
			{
				p_memory_map_client->m_name = str_node_text;
			}				
			else if(str_node_name== "MemoryMapName")
			{
				p_memory_map_client->m_memory_map_name = str_node_text;
			}					
		}		
	}	
}

void XML_Parse_MemoryMapServer(CComPtr<IXMLDOMNode> spXMLNode, CMemoryMapServer* p_memory_map_server)
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
			if(str_node_name== "Name")
			{
				p_memory_map_server->m_name = str_node_text;
			}				
			else if(str_node_name== "MemoryMapName")
			{
				p_memory_map_server->m_memory_map_name = str_node_text;
			}		
			else if(str_node_name== "Size")
			{						
				istringstream iss(str_node_text);
				iss >> p_memory_map_server->m_size;
			}		
			else if(str_node_name== "InitialString")
			{						
				p_memory_map_server->m_initial_str = str_node_text;
			}					
		}		
	}	
}


void XML_Parse_Material(CComPtr<IXMLDOMNode> spXMLNode, CMaterial *p_material)
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
			if(str_node_name== "Name")
			{
				p_material->m_name = str_node_text;
			}
			else if(str_node_name== "Diffuse")
			{
				Vec4d vec;
				XML_Parse_Vec4d(spXMLNextNode, &vec);								
				p_material->mp_material->setDiffuse(Material::FRONT,  vec);
			}
			else if(str_node_name== "Specular")
			{
				Vec4d vec;
				XML_Parse_Vec4d(spXMLNextNode, &vec);								
				p_material->mp_material->setSpecular(Material::FRONT,  vec);
			}
			else if(str_node_name== "Ambient")
			{
				Vec4d vec;
				XML_Parse_Vec4d(spXMLNextNode, &vec);								
				p_material->mp_material->setAmbient(Material::FRONT,  vec);
			}		
			else if(str_node_name== "Emission")
			{
				Vec4d vec;
				XML_Parse_Vec4d(spXMLNextNode, &vec);								
				p_material->mp_material->setEmission(Material::FRONT,  vec);
			}				
			else if(str_node_name== "Shininess")
			{
				float shiny = 0;
				istringstream iss(str_node_text);
				iss >> shiny;
				p_material->mp_material->setShininess(Material::FRONT, shiny);
			}		
		}
	}	
}


void XML_Parse_MDEXPR(CComPtr<IXMLDOMNode> spXMLNode, CMDEXPR* p_md_expr)
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
			if(str_node_name== "EXPRRef")
			{
				CMDEXPRReference ref;				
				XML_Parse_MDEXPRReference(spXMLNextNode, &ref);								
				p_md_expr->mv_expr_ref.push_back(ref);
			}				
		}
	}	
}


void XML_Parse_MDEXPRReference(CComPtr<IXMLDOMNode> spXMLNode, CMDEXPRReference *ref)
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
			if(str_node_name== "Name")
			{
				ref->m_EXPR_ref_name = str_node_text;
			}	
			else if(str_node_name== "VarRef")
			{
				CMDEXPRVarRef var_ref;				
				XML_Parse_MDEXPRVarRef(spXMLNextNode, &var_ref);								
				ref->mv_EXPR_var_ref.push_back(var_ref);
			}		
			
		}
	}	
}

void XML_Parse_MDEXPRVarRef(CComPtr<IXMLDOMNode> spXMLNode, CMDEXPRVarRef *ref)
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
			if(str_node_name== "Var")
			{
				ref->m_EXPR_ref_name = str_node_text;
			}	
		}
	}	

}





void XML_Parse_MultiplePAT(CComPtr<IXMLDOMNode> spXMLNode, CMultiplePAT *p_multiple_PAT)
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
			if(str_node_name== "PositionAttitudeTransform")
			{
				CPositionAttitudeTransform PAT;
				XML_Parse_PositionAttitudeTransform(spXMLNextNode, &p_multiple_PAT->m_PAT);
			}	
			else if(str_node_name== "Type")
			{
				p_multiple_PAT->m_type = str_node_text;
			}		
			else if(str_node_name== "CreateLogisticsNodes")
			{
				if(str_node_text == "TRUE")
				{
					p_multiple_PAT->m_create_logistics_nodes = true;
				}
				if(str_node_text == "FALSE")
				{
					p_multiple_PAT->m_create_logistics_nodes = false;
				}
			}	
			
		}
	}	
}
void XML_Parse_MultipleRandomEntities(CComPtr<IXMLDOMNode> spXMLNode, CMultipleRandomEntities *p_multiple_random_entities,  CBillboard *p_billboard)
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
			if(str_node_name== "Name")
			{
				p_multiple_random_entities->m_name = str_node_text;
			}	
			else if(str_node_name== "Method")
			{
				p_multiple_random_entities->m_method = str_node_text;
			}	
			else if(str_node_name== "Radius")
			{
				istringstream iss(str_node_text);
				iss >> p_multiple_random_entities->m_radius;
			}	
			else if(str_node_name== "Quantity")
			{
				istringstream iss(str_node_text);
				iss >> p_multiple_random_entities->m_quantity;
			}	
			else if(str_node_name== "Center")
			{
				XML_Parse_Vec3d(spXMLNextNode, &(p_multiple_random_entities->m_center));	
			}	
			/*else if(str_node_name== "Vec2Array")
			{
				XML_Parse_Vec2Array(spXMLNextNode, &(p_multiple_random_entities->>m_center));	
			}	*/
			else if(str_node_name == "Billboard")
			{					
				XML_Parse_Billboard(spXMLNextNode, p_billboard);				
			}
		}
	}	
}

void XML_Parse_Node(CComPtr<IXMLDOMNode> spXMLNode,  CNode *p_node)
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
			if(str_node_name== "Name")
			{
				p_node->m_name = str_node_text;
			}				
			else if(str_node_name== "File")
			{				
				/*This includes the following geometric file formats: 3dc, 3ds, flt, geo, iv, ive, lwo, md2, obj, osg. 
					And the following image file formats: bmp, gif, jpeg, rgb, tga, tif. */

				p_node->mp_node = osgDB::readNodeFile(str_node_text);	
				if (!p_node->mp_node)
				{
					std::cout << ": No data loaded" << std::endl;
				}	
			}	
			else if(str_node_name== "Galaxy")
			{				
				int vertices;
				istringstream iss(str_node_text);
				iss >> vertices;
				p_node->mp_node = makeGalaxy(vertices);
				p_node->mp_node->setStateSet(makeStateSet(10.0f));		
			}			
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_node->mv_ref.push_back(ref);
			}				
		}
	}	
}

void XML_Parse_PhysX_Actor(CComPtr<IXMLDOMNode> spXMLNode,  CPhysX_Actor *p_PhysX_actor)
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
			if(str_node_name== "Name")
			{
				p_PhysX_actor->m_name = str_node_text;		
			}			
			else if(str_node_name== "PhysX_ActorDesc")
			{
				XML_Parse_PhysX_ActorDesc(spXMLNextNode, &(p_PhysX_actor->m_PhysX_actor_desc));				
			}	
			else if(str_node_name== "Geode")
			{				
				XML_Parse_Geode(spXMLNextNode, &(p_PhysX_actor->m_geode));				
			}	
			else if(str_node_name== "MatrixTransform")
			{				
				XML_Parse_MatrixTransform(spXMLNextNode, &(p_PhysX_actor->m_matrix_transform));				
			}	
		}
	}
}

void XML_Parse_PhysX_ActorDesc(CComPtr<IXMLDOMNode> spXMLNode,  CPhysX_ActorDesc *p_PhysX_actor_desc)
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
			if(str_node_name== "Name")
			{
				p_PhysX_actor_desc->m_name = str_node_text;		
			}	
			else if(str_node_name== "Shape")
			{				
				p_PhysX_actor_desc->m_shape = str_node_text;			
			}	
			else if(str_node_name== "Density")
			{
				istringstream iss(str_node_text);
				iss >> p_PhysX_actor_desc->m_nx_actor_desc.density;		
			}	
			else if(str_node_name== "NxGlobalPose")
			{
				XML_Parse_NxMat34(spXMLNextNode, &(p_PhysX_actor_desc->m_nx_actor_desc.globalPose));	
			}	
			else if(str_node_name== "NxBodyDesc")
			{
				if(str_node_text == "NULL")
				{
					p_PhysX_actor_desc->m_nx_actor_desc.body = NULL;
				}
				else
				{
					XML_Parse_NxBodyDesc(spXMLNextNode, &(p_PhysX_actor_desc->m_nx_body_desc));						
					p_PhysX_actor_desc->m_nx_actor_desc.body = &(p_PhysX_actor_desc->m_nx_body_desc);
				}				
			}		
			else if(str_node_name== "NxBoxShapeDesc")
			{
				XML_Parse_NxBoxShapeDesc(spXMLNextNode, &(p_PhysX_actor_desc->m_nx_box_shape_desc));				
				p_PhysX_actor_desc->m_nx_actor_desc.shapes.pushBack(&(p_PhysX_actor_desc->m_nx_box_shape_desc));
				p_PhysX_actor_desc->m_shape = "Box";
			}	
			else if(str_node_name== "NxCapsuleShapeDesc")
			{
				XML_Parse_NxCapsuleShapeDesc(spXMLNextNode, &(p_PhysX_actor_desc->m_nx_capsule_shape_desc));				
				p_PhysX_actor_desc->m_nx_actor_desc.shapes.pushBack(&(p_PhysX_actor_desc->m_nx_capsule_shape_desc));	
				p_PhysX_actor_desc->m_shape = "Capsule";
			}	
			else if(str_node_name== "NxConvexShapeDesc")
			{
				XML_Parse_NxConvexShapeDesc(spXMLNextNode, &(p_PhysX_actor_desc->m_nx_convex_shape_desc));				
				p_PhysX_actor_desc->m_nx_actor_desc.shapes.pushBack(&(p_PhysX_actor_desc->m_nx_convex_shape_desc));		
				p_PhysX_actor_desc->m_shape = "Convex";
			}	
			else if(str_node_name== "NxPlaneShapeDesc")
			{
				XML_Parse_NxPlaneShapeDesc(spXMLNextNode, &(p_PhysX_actor_desc->m_nx_plane_shape_desc));				
				p_PhysX_actor_desc->m_nx_actor_desc.shapes.pushBack(&(p_PhysX_actor_desc->m_nx_plane_shape_desc));	
				p_PhysX_actor_desc->m_shape = "Plane";
			}	
			else if(str_node_name== "NxSphereShapeDesc")
			{
				XML_Parse_NxSphereShapeDesc(spXMLNextNode, &(p_PhysX_actor_desc->m_nx_sphere_shape_desc));				
				p_PhysX_actor_desc->m_nx_actor_desc.shapes.pushBack(&(p_PhysX_actor_desc->m_nx_sphere_shape_desc));		
				p_PhysX_actor_desc->m_shape = "Sphere";
			}	
			else if(str_node_name== "NxTriangleMeshShapeDesc")
			{
				XML_Parse_NxTriangleMeshShapeDesc(spXMLNextNode, &(p_PhysX_actor_desc->m_nx_triangle_mesh_shape_desc));				
				p_PhysX_actor_desc->m_nx_actor_desc.shapes.pushBack(&(p_PhysX_actor_desc->m_nx_triangle_mesh_shape_desc));
				p_PhysX_actor_desc->m_shape = "TriangleMesh";
			}	
		}
	}
}

void XML_Parse_NxBodyDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxBodyDesc *p_nx_body_desc)
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
			if(str_node_name== "AngularDamping")
			{
				istringstream iss(str_node_text);
				iss >> p_nx_body_desc->angularDamping;		
			}	
			else if(str_node_name== "Flag")
			{				
				if(str_node_text== "NX_BF_DISABLE_GRAVITY")
				{
					p_nx_body_desc->flags = NX_BF_DISABLE_GRAVITY;	
				}
				else if(str_node_text== "NX_BF_FROZEN_POS_X")
				{
					p_nx_body_desc->flags = NX_BF_FROZEN_POS_X;	
				}
				else if(str_node_text== "NX_BF_FROZEN_POS_Y")
				{
					p_nx_body_desc->flags = NX_BF_FROZEN_POS_Y;	
				}	
				else if(str_node_text== "NX_BF_FROZEN_POS_Z")
				{
					p_nx_body_desc->flags = NX_BF_FROZEN_POS_Z;	
				}	
				else if(str_node_text== "NX_BF_FROZEN_ROT_X")
				{
					p_nx_body_desc->flags = NX_BF_FROZEN_ROT_X;	
				}		
				else if(str_node_text== "NX_BF_FROZEN_ROT_Y")
				{
					p_nx_body_desc->flags = NX_BF_FROZEN_ROT_Y;	
				}		
				else if(str_node_text== "NX_BF_FROZEN_ROT_Z")
				{
					p_nx_body_desc->flags = NX_BF_FROZEN_ROT_Z;	
				}		
				else if(str_node_text== "NX_BF_FROZEN_POS")
				{
					p_nx_body_desc->flags = NX_BF_FROZEN_POS;	
				}		
				else if(str_node_text== "NX_BF_FROZEN_ROT")
				{
					p_nx_body_desc->flags = NX_BF_FROZEN_ROT;	
				}									
				else if(str_node_text== "NX_BF_FROZEN")
				{
					p_nx_body_desc->flags = NX_BF_FROZEN;	
				}		
				else if(str_node_text== "NX_BF_KINEMATIC")
				{
					p_nx_body_desc->flags = NX_BF_KINEMATIC;	
				}			
				else if(str_node_text== "NX_BF_VISUALIZATION")
				{
					p_nx_body_desc->flags = NX_BF_VISUALIZATION;	
				}			
				else if(str_node_text== "NX_BF_DUMMY_0")
				{
					p_nx_body_desc->flags = NX_BF_DUMMY_0;	
				}			
				else if(str_node_text== "NX_BF_FILTER_SLEEP_VEL")
				{
					p_nx_body_desc->flags = NX_BF_FILTER_SLEEP_VEL;	
				}
			}		
			else if(str_node_name== "LinearVelocity")
			{	
				XML_Parse_NxVec3(spXMLNextNode, &(p_nx_body_desc->linearVelocity));								
			}	
			else if(str_node_name== "AngularVelocity")
			{	
				XML_Parse_NxVec3(spXMLNextNode, &(p_nx_body_desc->angularVelocity));								
			}				
		}
	}
}

void XML_Parse_NxBoxShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxBoxShapeDesc *p_nx_box_shape_desc)
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
			if(str_node_name== "Dimensions")
			{			
				XML_Parse_NxVec3(spXMLNextNode, &(p_nx_box_shape_desc->dimensions));	
			}				
		}
	}
}


void XML_Parse_NxCapsuleShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxCapsuleShapeDesc *p_nx_capsule_shape_desc)
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
			if(str_node_name== "radius")
			{			
				istringstream iss(str_node_text);
				iss >> p_nx_capsule_shape_desc->radius;		
			}				
		}
	}
}

void XML_Parse_NxConvexShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxConvexShapeDesc *p_nx_convex_shape_desc)
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
				
		}
	}

}

void XML_Parse_NxTriangleMeshShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxTriangleMeshShapeDesc *p_nx_triangle_mesh_shape_desc)
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
			if(str_node_name== "TriangleMesh")
			{			
				//NxTriangleMesh
				//iss >> p_nx_triangle_mesh_shape_desc->meshData;		
			}				
		}
	}


}



void XML_Parse_NxCloth(CComPtr<IXMLDOMNode> spXMLNode,  NxCloth *p_nx_cloth)
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
				
		}
	}
}

void XML_Parse_NxClothDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxClothDesc *p_nx_cloth_desc)
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
				
		}
	}
}

void XML_Parse_NxClothMesh(CComPtr<IXMLDOMNode> spXMLNode,  NxClothMesh *p_nx_cloth_mesh)
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
					
		}
	}
}

void XML_Parse_NxClothMeshDesc(CComPtr<IXMLDOMNode> spXMLNode, NxClothMeshDesc *p_nx_cloth_mesh_desc)
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
				
		}
	}
}

void XML_Parse_NxFluid(CComPtr<IXMLDOMNode> spXMLNode,  NxFluid *p_nx_fluid)
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
					
		}
	}
}

void XML_Parse_NxFluidDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxFluidDesc *p_nx_fluid_desc)
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
					
		}
	}
}

void XML_Parse_NxPlaneShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxPlaneShapeDesc *p_nx_plane_shape_desc)
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
			
		}
	}
}

void XML_Parse_NxMat34(CComPtr<IXMLDOMNode> spXMLNode,  NxMat34* p_NxMat34)
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
			if(str_node_name== "t")
			{
				XML_Parse_NxVec3(spXMLNextNode, &(p_NxMat34->t));		
			}	
		}
	}

}

void XML_Parse_NxScene(CComPtr<IXMLDOMNode> spXMLNode,  NxScene *p_nx_scene)
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
					
		}
	}
}


void XML_Parse_NxSphereShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxSphereShapeDesc *p_nx_sphere_shape_desc)
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
			if(str_node_name== "radius")
			{
				istringstream iss(str_node_text);
				iss >> p_nx_sphere_shape_desc->radius;		
			}		
		}
	}
}

void XML_Parse_NxVec3(CComPtr<IXMLDOMNode> spXMLNode, NxVec3 *vec)
{
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	float x = 0;
	float y = 0;
	float z = 0;	
	for(long i= 0; i< listLength; i++)
	{
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "x")
			{
				istringstream iss(str_node_text);
				iss >>vec->x;
			}
			else if(str_node_name== "y")
			{
				istringstream iss(str_node_text);
				iss >> vec->y;
			}
			else if(str_node_name== "z")
			{
				istringstream iss(str_node_text);
				iss >> vec->z;
			}			
		}
	}		
}

void XML_Parse_Orienation(CComPtr<IXMLDOMNode> spXMLNode, COrientation *orientation)
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
			if(str_node_name== "Axis")
			{
				XML_Parse_Vec3d(spXMLNextNode, &(orientation->m_axis));				
			}
			if(str_node_name== "Angle")
			{
				istringstream iss(str_node_text);
				iss >> orientation->m_angle;
			}
		}
	}
}
void XML_Parse_Particle(CComPtr<IXMLDOMNode> spXMLNode, CParticle* p_particle)
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
			if(str_node_name== "Name")
			{
				p_particle->m_name = str_node_text;
			}		
		}
	}	
}
void XML_Parse_PolygonMode(CComPtr<IXMLDOMNode> spXMLNode, CPolygonMode* p_pm)
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
			if(str_node_name== "Name")
			{
				p_pm->m_name= str_node_text;
			}	
			if(str_node_name== "SetMode")
			{
				p_pm->mp_polygon_mode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
			}	
		}
	}
}
void XML_Parse_PositionAttitudeTransform(CComPtr<IXMLDOMNode> spXMLNode, CPositionAttitudeTransform *p_PAT)
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
			if(str_node_name== "Name")
			{
				p_PAT->m_name= str_node_text;
			}
			if(str_node_name== "AttitudeQ")
			{
				Quat quat;
				XML_Parse_Quaternion(spXMLNextNode, &quat);				
				p_PAT->mp_PAT->setAttitude(quat);
			}
			if(str_node_name== "Orientation")
			{
				COrientation orientation;
				XML_Parse_Orienation(spXMLNextNode, &orientation);	
				osg::Quat quat;
				quat.makeRotate(orientation.m_angle, orientation.m_axis);
				p_PAT->mp_PAT->setAttitude(quat);
			}
			if(str_node_name== "Position")
			{
				Vec3d vec;
				XML_Parse_Vec3d(spXMLNextNode, &vec);				
				p_PAT->mp_PAT->setPosition(vec);
			}	
			if(str_node_name== "Scale")
			{
				Vec3d vec;
				XML_Parse_Vec3d(spXMLNextNode, &vec);				
				p_PAT->mp_PAT->setScale(vec);
			}			
			if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_PAT->mv_ref.push_back(ref);
			}		
		}
	}
}
void XML_Parse_Projection(CComPtr<IXMLDOMNode> spXMLNode, CProjection* p_projection)
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
			if(str_node_name== "Name")
			{
				p_projection->m_name = str_node_text;
			}		
			if(str_node_name== "Ortho2D")
			{				
				CRect rect;				
				XML_Parse_Rect(spXMLNextNode, &rect);								
				p_projection->mp_projection->setMatrix(osg::Matrix::ortho2D(rect.m_left,rect.m_right,rect.m_bottom,rect.m_top));
			}
			if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_projection->mv_ref.push_back(ref);
			}		
		}
	}	

}
void XML_Parse_Program(CComPtr<IXMLDOMNode> spXMLNode, CProgram* p_pm)
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
			if(str_node_name== "Name")
			{
				p_pm->m_name= str_node_text;
			}		
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_pm->mv_ref.push_back(ref);
			}	
		}
	}
}
void XML_Parse_ParametricFunction(CComPtr<IXMLDOMNode> spXMLNode, CParametricEquation *para)
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
			if(str_node_name== "Expression")
			{
				para->mv_str_expression.push_back(str_node_text);
			}	
			else if(str_node_name== "ExprInterval")
			{
				XML_Parse_ExprInterval(spXMLNextNode, &(para->m_interval));
			}	
			
		}
	}
}
void XML_Parse_PhysXSoftBody(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXSoftBody* p_physX_soft_body)
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
			if(str_node_name== "Name")
			{
				p_physX_soft_body->m_name = str_node_text;
			}				
		}
	}

}
void XML_Parse_PhysXFluid(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXFluid* p_physX_fluid)
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
			if(str_node_name== "Name")
			{
				p_physX_fluid->m_name = str_node_text;
			}		
			else if(str_node_name== "Origin")
			{
				Vec3f vec;
				XML_Parse_Vec3f(spXMLNextNode, &vec);	
				p_physX_fluid->m_origin.set(vec);
			}	
			else if(str_node_name== "Gravity")
			{
				Vec3f vec;
				XML_Parse_Vec3f(spXMLNextNode, &vec);	
				p_physX_fluid->m_gravity.set(vec);
			}		
			else if(str_node_name== "ParticleSize")
			{
				istringstream iss(str_node_text);
				iss >> p_physX_fluid->mParticleSize;
			}	
			else if(str_node_name== "GenerationRadius")
			{
				istringstream iss(str_node_text);
				iss >> p_physX_fluid->mGenerationRadius;
			}				
		}
	}
}
void XML_Parse_PhysXTerrain(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXTerrain* p_physX_terrain)
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
			if(str_node_name== "Name")
			{
				p_physX_terrain->m_name = str_node_text;
			}				
		}
	}
}
void XML_Parse_PhysXCube(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXCube* p_physX_cube)
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
			if(str_node_name== "Name")
			{
				p_physX_cube->m_name = str_node_text;
			}	
			else if(str_node_name== "Position")
			{
				Vec3f vec;
				XML_Parse_Vec3f(spXMLNextNode, &vec);	
				p_physX_cube->m_position.set(vec);
			}	
			else if(str_node_name== "Size")
			{
				Vec3f vec;
				XML_Parse_Vec3f(spXMLNextNode, &vec);	
				p_physX_cube->m_size.set(vec);
			}	
			else if(str_node_name== "FileName")
			{					
				p_physX_cube->m_file_name = str_node_text;
			}	
			else if(str_node_name== "GeometryType")
			{					
				p_physX_cube->m_geometry_type = str_node_text;
			}	
		}
	}
}
void XML_Parse_PhysXPlane(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXPlane* p_physX_plane)
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
			if(str_node_name== "Name")
			{
				p_physX_plane->m_name = str_node_text;
			}	
			if(str_node_name== "Origin")
			{
				Vec3f vec;
				XML_Parse_Vec3f(spXMLNextNode, &vec);	
				p_physX_plane->m_origin.set(vec);
			}	
			if(str_node_name== "PlaneSize")
			{
				istringstream iss(str_node_text);
				iss >> p_physX_plane->m_planesize;
			}	
			if(str_node_name== "ImageFileName")
			{
				p_physX_plane->m_filename = str_node_text;
			}	
		}
	}

}
void XML_Parse_PhysXCloth(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXCloth* p_physX_cloth)
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
			if(str_node_name== "Name")
			{
				p_physX_cloth->m_name = str_node_text;
			}	
			else if(str_node_name== "Origin")
			{
				Vec3f vec;
				XML_Parse_Vec3f(spXMLNextNode, &vec);	
				p_physX_cloth->m_origin.set(vec);
			}
			else if(str_node_name== "PrimitiveSetType")
			{
				p_physX_cloth->m_primitive_type = str_node_text;
			}
			else if(str_node_name== "h")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_h;
			}	
			else if(str_node_name== "w")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_w;
			}	
			else if(str_node_name== "d")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_d;
			}	
			else if(str_node_name== "attach")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_attach;
			}	
			else if(str_node_name== "Thickness")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_thickness;
			}	
			else if(str_node_name== "BendingStiffness")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_bendingStiffness;
			}	
			else if(str_node_name== "StretchingStiffness")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_stretchingStiffness;
			}	
			else if(str_node_name== "Density")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_density;
			}	
			else if(str_node_name== "Friction")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_friction;
			}	
			else if(str_node_name== "DampingCoefficient")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_dampingCoefficient;
			}	
			else if(str_node_name== "SolverIterations")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_solverIterations;
			}
			else if(str_node_name== "Tear")
			{					
				istringstream iss(str_node_text);
				iss >> p_physX_cloth->m_tearLines;
			}	
			else if(str_node_name== "Texture")
			{					
				p_physX_cloth->m_texture_file_path = str_node_text;
			}				
		}
	}
}
void XML_Parse_PhysXSphere(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXSphere* p_physX_sphere)
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
			if(str_node_name== "Name")
			{
				p_physX_sphere->m_name = str_node_text;
			}	
			if(str_node_name== "Position")
			{
				Vec3f vec;
				XML_Parse_Vec3f(spXMLNextNode, &vec);	
				p_physX_sphere->m_position.set(vec);
			}	
			if(str_node_name== "Radius")
			{
				istringstream iss(str_node_text);
				iss >> p_physX_sphere->m_radius;
			}	
		}
	}
}
void XML_Parse_Quaternion(CComPtr<IXMLDOMNode> spXMLNode, Quat *q)
{
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	float x = 0;
	float y = 0;
	float z = 0;
	float Angle = 0;
	for(long i= 0; i< listLength; i++)
	{
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{
			if(str_node_name== "Angle")
			{
				istringstream iss(str_node_text);
				iss >> Angle;
			}
			else if(str_node_name== "x")
			{
				istringstream iss(str_node_text);
				iss >> x;
			}
			else if(str_node_name== "y")
			{
				istringstream iss(str_node_text);
				iss >> y;
			}
			else if(str_node_name== "z")
			{
				istringstream iss(str_node_text);
				iss >> z;
			}			
		}
	}
	q->makeRotate(Angle,x,y,z);
}





void XML_Parse_Range(CComPtr<IXMLDOMNode> spXMLNode,  unsigned int * childNo, float * min, float * max)
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
			if(str_node_name== "ChildNo")
			{
				istringstream iss(str_node_text);
				iss >> (*childNo);
			}		
			else if(str_node_name== "Min")
			{				
				istringstream iss(str_node_text);
				iss >> (*min);
			}
			else if(str_node_name== "Max")
			{				
				istringstream iss(str_node_text);
				iss >> (*max);
			}			
		}
	}	

}


void XML_Parse_Reference(CComPtr<IXMLDOMNode> spXMLNode,  CReference *p_ref)
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
			if(str_node_name== "ReferenceCreationFlag")
			{
				if(str_node_text == "Create") {	p_ref->m_reference_creation_flag = eReferenceCreationFlag_Create; }
				if(str_node_text == "AddCreate") {	p_ref->m_reference_creation_flag = eReferenceCreationFlag_AddCreate; }
				if(str_node_text == "AddOverwrite") {	p_ref->m_reference_creation_flag = eReferenceCreationFlag_AddOverwrite; }
			}	
			else if(str_node_name== "RefName")
			{
				p_ref->m_ref_name = str_node_text;
			}		
			else if(str_node_name== "EntityType")
			{				
				p_ref->m_entity_type = string_to_entity_type(str_node_text);
			}
			else if(str_node_name== "RefType")
			{				
				p_ref->m_ref_type = string_to_reference_type(str_node_text);
			}			
		}
	}	
}


void XML_Parse_ReferenceArray(CComPtr<IXMLDOMNode> spXMLNode,  CReference * p_ref, unsigned int * n)
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
			if(str_node_name== "RefName")
			{
				p_ref->m_ref_name = str_node_text;
			}		
			else if(str_node_name== "EntityType")
			{				
				p_ref->m_entity_type = string_to_entity_type(str_node_text);
			}
			else if(str_node_name== "RefType")
			{				
				p_ref->m_ref_type = string_to_reference_type(str_node_text);
			}		
			else if(str_node_name== "Number_of_references")
			{				
				istringstream iss(str_node_text);
				iss >> (*n);
			}				
		}
	}	
}

void XML_Parse_Rect(CComPtr<IXMLDOMNode> spXMLNode,  CRect *p_rect)
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
			if(str_node_name== "Top")
			{
				istringstream iss(str_node_text);
				iss >> p_rect->m_top;
			}	
			if(str_node_name== "Left")
			{
				istringstream iss(str_node_text);
				iss >> p_rect->m_left;
			}		
			if(str_node_name== "Bottom")
			{
				istringstream iss(str_node_text);
				iss >> p_rect->m_bottom;
			}		
			if(str_node_name== "Right")
			{
				istringstream iss(str_node_text);
				iss >> p_rect->m_right;
			}		
		}
	}	
}
void XML_Parse_SmokeEffect(CComPtr<IXMLDOMNode> spXMLNode, CSmokeEffect *p_smoke)
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
			if(str_node_name== "Name")
			{
				p_smoke->m_name = str_node_text;
			}	
			else if(str_node_name== "Position")
			{
				XML_Parse_Vec3(spXMLNextNode, &(p_smoke->m_position));
			}	
			else if(str_node_name== "Scale")
			{
				istringstream iss(str_node_text);
				iss >> p_smoke->m_scale;
			}	
			else if(str_node_name== "intensity")
			{
				istringstream iss(str_node_text);
				iss >> p_smoke->m_intensity;
			}			
			else if(str_node_name== "SetParticleDuration")
			{
				float duration;
				istringstream iss(str_node_text);
				iss >> duration;
				p_smoke->mp_smoke->setParticleDuration(duration);
			}	
		}
	}	
}
void XML_Parse_Shader(CComPtr<IXMLDOMNode> spXMLNode,  CShader * shader)
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
			if(str_node_name== "Name")
			{
				shader->m_name= str_node_text;
			}
			else if(str_node_name== "Frag_file")
			{				
				shader->mp_shader = osg::Shader::readShaderFile (osg::Shader::FRAGMENT, str_node_text.c_str());
			}			
			else if(str_node_name== "Vert_file")
			{				
				shader->mp_shader = osg::Shader::readShaderFile (osg::Shader::VERTEX, str_node_text.c_str());
			}			
		}
	}	
}
void XML_Parse_ShapeDrawable(CComPtr<IXMLDOMNode> spXMLNode,  CShapeDrawable * sd)
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
			if(str_node_name== "Name")
			{
				sd->m_name = str_node_text;
			}		
			else if(str_node_name== "Box")
			{
				float x=1;
				float y=1;
				float z=1;
				Vec3d center;
				XML_Parse_Box(spXMLNextNode, &center, &x, &y, &z);//see geode box
				sd->mp_shape_drawable = new osg::ShapeDrawable(new osg::Box(center,x,y,z));				
			}
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				sd->mv_ref.push_back(ref);
			}	
		}
	}	
}

void XML_Parse_Socket(CComPtr<IXMLDOMNode> spXMLNode, CSocket *p_socket)
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
			if(str_node_name== "Name")
			{
				p_socket->m_name = str_node_text;
			}	
			else if(str_node_name== "Type")
			{
				p_socket->m_type = str_node_text;
			}	
			else if(str_node_name== "SocketFlags")
			{
				p_socket->m_str_socket_flags = str_node_text;
				p_socket->m_n_socket_flags= string_to_socket_flags(p_socket->m_str_socket_flags);
			}		
			else if(str_node_name== "SocketAddressFamily")
			{
				p_socket->m_str_socket_address_family = str_node_text;
				p_socket->m_n_socket_address_family= string_to_socket_address_family(p_socket->m_str_socket_address_family);
				p_socket->m_SockAddr.sin_family = p_socket->m_n_socket_address_family;
			}	
			else if(str_node_name== "SocketType")
			{
				p_socket->m_str_socket_type = str_node_text;
				p_socket->m_n_socket_type= string_to_socket_type(p_socket->m_str_socket_type);
			}	
			else if(str_node_name== "SocketProtocol")
			{
				p_socket->m_str_socket_protocol = str_node_text;
				p_socket->m_n_socket_protocol= string_to_socket_protocol(p_socket->m_str_socket_protocol);
			}				
			else if(str_node_name== "NodeName")//IP address
			{
				p_socket->m_str_node_name = str_node_text;
				if(str_node_text == "INADDR_ANY")
				{
					p_socket->m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
				}
				else if( "localhost" == str_node_text)
				{
					p_socket->m_str_host_name = str_node_text;
					p_socket->m_lpHost = gethostbyname(str_node_text.c_str());
					p_socket->m_SockAddr.sin_addr = *((LPIN_ADDR) *(p_socket->m_lpHost)->h_addr_list);
				}
				else
				{
					struct in_addr internet_address;
					internet_address.s_addr = inet_addr(str_node_text.c_str());
					p_socket->m_SockAddr.sin_addr = internet_address;
				}
				
			}			
			else if(str_node_name== "HostName")//IP address
			{
				p_socket->m_str_host_name = str_node_text;
				p_socket->m_lpHost = gethostbyname(str_node_text.c_str());
				p_socket->m_SockAddr.sin_addr = *((LPIN_ADDR) *(p_socket->m_lpHost)->h_addr_list);
			}		
			else if(str_node_name== "ServiceName")//Port
			{
				p_socket->m_str_service_name = str_node_text;
				if(str_node_text == "mail")	{
					p_socket->m_lpServer = getservbyname("mail", 0);
					if(!p_socket->m_lpServer) { p_socket->m_n_port = htons(IPPORT_SMTP); }
					else { p_socket->m_n_port = p_socket->m_lpServer->s_port; }					
					p_socket->m_SockAddr.sin_port = p_socket->m_n_port;
				}				
				else {				
					std::istringstream iss(str_node_text);
					iss >> p_socket->m_n_port;
					p_socket->m_SockAddr.sin_port = htons(p_socket->m_n_port);
				}				
			}				
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_socket->mv_ref.push_back(ref);
			}	
		}
	}	
	
	
	
	

	
	

}	


void XML_Parse_Sphere(CComPtr<IXMLDOMNode> spXMLNode,  Vec3d *cen, float * rad)
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
			if(str_node_name== "Center")
			{
				XML_Parse_Vec3d(spXMLNextNode, cen);								
			}
			
			else if(str_node_name== "Radius")
			{
				float shiny = 0;
				istringstream iss(str_node_text);
				iss >> *rad;
			}		
		}
	}	
}
void XML_Parse_StateSet(CComPtr<IXMLDOMNode> spXMLNode,  CStateSet * p_stateset)
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
			if(str_node_name== "Name")
			{
				p_stateset->m_name = str_node_text;
			}		
			else if(str_node_name== "SetMode")
			{
				if(str_node_text== "GL_LIGHTING_ON")
				{
					p_stateset->mp_state_set->setMode(GL_LIGHTING, osg::StateAttribute::ON ); 
				}
				else if(str_node_text== "GL_LIGHTING_OFF")
				{
					p_stateset->mp_state_set->setMode(GL_LIGHTING, osg::StateAttribute::OFF ); 
				}
				else if(str_node_text== "GL_BLEND_ON")
				{
					p_stateset->mp_state_set->setMode(GL_BLEND, osg::StateAttribute::ON ); 
				}
				else if(str_node_text== "GL_BLEND_OFF")
				{
					p_stateset->mp_state_set->setMode(GL_BLEND, osg::StateAttribute::OFF ); 
				}
				else if(str_node_text== "GL_LINE_SMOOTH_ON")
				{
					p_stateset->mp_state_set->setMode(GL_BLEND, osg::StateAttribute::ON ); 
				}
				else if(str_node_text== "GL_LINE_SMOOTH_OFF")
				{
					p_stateset->mp_state_set->setMode(GL_BLEND, osg::StateAttribute::OFF ); 
				}
			}	
			else if(str_node_name== "SetRenderingHint")
			{
				if(str_node_text== "TRANSPARENT_BIN")
				{
					p_stateset->mp_state_set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN); 
				}
			}
			else if(str_node_name== "BlendFunc")
			{
				if(str_node_text== "SRC_ALPHA_DST_ALPHA")
				{					 
					osg::ref_ptr<osg::BlendFunc> fn = new osg::BlendFunc(); 
					fn->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::DST_ALPHA); 
					p_stateset->mp_state_set->setAttributeAndModes(fn, osg::StateAttribute::ON);
				}
			}
			else if(str_node_name== "ShadeModel")
			{
				if(str_node_text == "FLAT")
				{
					//osg::ShadeModel* sm = dynamic_cast<osg::ShadeModel*>(p_stateset->mp_state_set->getAttribute(osg::StateAttribute::SHADEMODEL ) );         
					//sm->setMode( osg::ShadeModel::FLAT );  
					//p_stateset->mp_state_set->setMode(GL_BLEND, osg::StateAttribute::OFF ); 

					osg::ShadeModel* sm = new osg::ShadeModel();
					sm->setMode( osg::ShadeModel::FLAT );
					p_stateset->mp_state_set->setAttribute( sm );

				}
			}	
			
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_stateset->mv_ref.push_back(ref);
			}		
		}
	}
}
void XML_Parse_Text(CComPtr<IXMLDOMNode> spXMLNode, osgText::Text *p_text, string* name)
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
			if(str_node_name== "Name")
			{
				*name = str_node_text;
			}
			if(str_node_name== "CharacterSize")
			{
				float flt;
				istringstream iss(str_node_text);
				iss >> flt;
				p_text->setCharacterSize(flt);
			}
			if(str_node_name== "Font")
			{				
				p_text->setFont(str_node_text);
			}
			if(str_node_name== "InitText")
			{				
				p_text->setText(str_node_text);
			}
			if(str_node_name== "AxisAlignment")
			{
				if(str_node_text== "SCREEN")
				{
					p_text->setAxisAlignment(osgText::Text::SCREEN);
				}
			}
			if(str_node_name== "Position")
			{
				Vec3d vec;
				XML_Parse_Vec3d(spXMLNextNode, &vec);
				p_text->setPosition(vec);
			}
			if(str_node_name== "Color")
			{
				Vec4d vec;
				XML_Parse_Vec4d(spXMLNextNode, &vec);
				p_text->setColor(vec);
			}
		}
	}	
}
void XML_Parse_Uniform(CComPtr<IXMLDOMNode> spXMLNode, CUniform* p_uniform)
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
			if(str_node_name== "Name")
			{
				p_uniform->m_name = str_node_text;
			}		
			if(str_node_name== "ID")
			{
				p_uniform->m_ID = str_node_text;
			}		
			if(str_node_name== "Type")
			{
				p_uniform->m_type = str_node_text;
			}		
			if(str_node_name== "Vec3")
			{
				Vec3f vec;
				XML_Parse_Vec3(spXMLNextNode, &(p_uniform->m_vec3));							
			}	
			if(str_node_name== "Float")
			{
				istringstream iss(str_node_text);
				iss >> p_uniform->m_f;						
			}		
		}
	}	
}

//void XML_Parse_Texture(CComPtr<IXMLDOMNode> spXMLNode, CTexture *p_texture)
//{
//	HRESULT hr;
//	long listLength;
//	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
//	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
//	hr =spXMLDOMNodeList->get_length(&listLength);
//	for(long i= 0; i< listLength; i++)
//	{
//		string str_node_name;
//		string str_node_text;
//		CComPtr<IXMLDOMNode> spXMLNextNode;
//		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
//		if(spXMLNextNode!= NULL)
//		{		
//			if(str_node_name== "Name")
//			{
//				p_texture->m_name = str_node_text;
//			}		
//			else if(str_node_name== "Ref")
//			{
//				CReference ref;				
//				XML_Parse_Reference(spXMLNextNode, &ref);								
//				p_texture->mv_ref.push_back(ref);
//			}		
//		}
//	}
//}
void XML_Parse_Texture2D(CComPtr<IXMLDOMNode> spXMLNode, CTexture2D *p_texture)
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
			if(str_node_name== "Name")
			{
				p_texture->m_name = str_node_text;
			}		
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_texture->mv_ref.push_back(ref);
			}		
		}
	}
}
void XML_Parse_VarAssoc(CComPtr<IXMLDOMNode> spXMLNode, CVarAssoc *p_var_assoc)
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
			if(str_node_name== "VariableAssociationType")
			{
				if(str_node_text== "PAT_vec")
				{
					p_var_assoc->m_assoc = VariableAssoc_PAT_vec;
				}
				else if(str_node_text== "elapsed_time")
				{
					p_var_assoc->m_assoc = VariableAssoc_elapsed_time;
				}
			}			
			else if(str_node_name== "VariableAssociationSubType")
			{
				if(str_node_text== "x")
				{
					p_var_assoc->m_sub_assoc = VariableAssocSub_x;
				}
				else if(str_node_text== "y")
				{
					p_var_assoc->m_sub_assoc = VariableAssocSub_y;
				}
				else if(str_node_text== "z")
				{
					p_var_assoc->m_sub_assoc = VariableAssocSub_y;
				}
				else if(str_node_text== "Magnitude")
				{
					p_var_assoc->m_sub_assoc = VariableAssocSub_mag;
				}
			}	
			else if(str_node_name== "RefExpression")
			{
				p_var_assoc->m_ref_expr = str_node_text;
			}
			else if(str_node_name== "RefAssociated")
			{
				p_var_assoc->m_ref_assoc = str_node_text;
			}		
		}
	}
}

void XML_Parse_Variable(CComPtr<IXMLDOMNode> spXMLNode, CParsedVariable *p_variable)
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
			if(str_node_name== "Value")
			{
				p_variable->m_str_value = str_node_text;
			}		
			if(str_node_name== "DataType")
			{
				p_variable->m_str_data_type = str_node_text;
				if(str_node_text == "float")
				{
					p_variable->m_data_type = ParsedVariableType_float;
				}
				if(str_node_text == "string")
				{
					p_variable->m_data_type = ParsedVariableType_string;
				}
				if(str_node_text == "integer")
				{
					p_variable->m_data_type = ParsedVariableType_int;
				}
				
			}	
			if(str_node_name== "Identifier")
			{
				p_variable->m_identifier = str_node_text;				
			}				
		}
	}
}

void XML_Parse_Vec2Array(CComPtr<IXMLDOMNode> spXMLNode, CVec2Array *p_array)
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
			if(str_node_name== "Name")
			{
				p_array->m_name = str_node_text;
			}	
			else if(str_node_name== "Vec2")
			{
				Vec2 vec;
				XML_Parse_Vec2(spXMLNextNode, &vec);	
				p_array->mp_Vec2Array->push_back(vec);
			}
			if(str_node_name== "HUD_TexCoords")
			{
				p_array->mp_Vec2Array = new osg::Vec2Array(4);
				(*(p_array->mp_Vec2Array))[0].set(0.0f,0.0f);
				(*(p_array->mp_Vec2Array))[1].set(1.0f,0.0f);
				(*(p_array->mp_Vec2Array))[2].set(1.0f,1.0f);
				(*(p_array->mp_Vec2Array))[3].set(0.0f,1.0f);
			}		
		}
	}
}

void XML_Parse_Vec3Array(CComPtr<IXMLDOMNode> spXMLNode, CVec3Array *p_array, CXGMML_graph* p_graph)
{
	osg::Vec3d vec_A;
	osg::Vec3d vec_B;
	std::string name;
	std::string value;
	std::string A,type,B;	
	std::string A2,B2;	
	std::vector<CXGMML_edge>::iterator XGMML_edge_iter;
	std::vector<CXGMML_node>::iterator XGMML_node_iter;
	std::vector<CXGMML_attribute>::iterator XGMML_attribute_iter;

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
			int count= 0;
			Vec3d Vec;

			if(str_node_name== "Name")
			{
				p_array->m_name = str_node_text;
			}		
			else if(str_node_name== "Vec3d")
			{
				Vec3d vec;
				XML_Parse_Vec3d(spXMLNextNode, &vec);	
				p_array->mp_Vec3Array->push_back(vec);
			}
			else if(str_node_name== "Cyto")
			{		
				for(XGMML_edge_iter = p_graph->mv_edge.begin(); XGMML_edge_iter != p_graph->mv_edge.end(); ++XGMML_edge_iter)	{					
					std::string name = XGMML_edge_iter->mv_att[0].mv_attribute[2].m_name;
					std::string value = XGMML_edge_iter->mv_att[0].mv_attribute[2].m_value;								
					std::istringstream iss_v(value);
					iss_v >> A;
					iss_v >> type;
					iss_v >> B;
					A2= A;
					B2= B;
					std::istringstream iss_A(A2.erase(0, 4));
					iss_A >> XGMML_edge_iter->m_A;					
					std::istringstream iss_B(B2.erase(0, 4));
					iss_B >> XGMML_edge_iter->m_B;					
					vec_A.z()= 0;
					vec_B.z()= 0;
					
					
					for(XGMML_node_iter = p_graph->mv_node.begin();  XGMML_node_iter != p_graph->mv_node.end(); ++XGMML_node_iter)
					{
						if(XGMML_node_iter->mv_att[1].mv_attribute[2].m_value== A)
						{
							for(XGMML_attribute_iter = XGMML_node_iter->m_graphics.mv_attribute.begin(); XGMML_attribute_iter!=  XGMML_node_iter->m_graphics.mv_attribute.end(); ++XGMML_attribute_iter) {								
								if(XGMML_attribute_iter->m_name == "x")	{
									std::istringstream iss_av(XGMML_attribute_iter->m_value);
									iss_av >> vec_A.x();
								}
								if(XGMML_attribute_iter->m_name == "y")	{
									std::istringstream iss_av(XGMML_attribute_iter->m_value);
									iss_av >> vec_A.y();
								}
							}	
						}
						if(XGMML_node_iter->mv_att[1].mv_attribute[2].m_value== B)
						{
							for(XGMML_attribute_iter = XGMML_node_iter->m_graphics.mv_attribute.begin(); XGMML_attribute_iter!=  XGMML_node_iter->m_graphics.mv_attribute.end(); ++XGMML_attribute_iter) {								
								if(XGMML_attribute_iter->m_name == "x")	{
									std::istringstream iss_av(XGMML_attribute_iter->m_value);
									iss_av >> vec_B.x();
								}
								if(XGMML_attribute_iter->m_name == "y")	{
									std::istringstream iss_av(XGMML_attribute_iter->m_value);
									iss_av >> vec_B.y();
								}
							}	
						}
					}					
					p_array->mp_Vec3Array->push_back(vec_A);
					p_array->mp_Vec3Array->push_back(vec_B);				
				}				
			}
			else if(str_node_name== "Grid")
			{					
				float x1= -10;
				float x2= 10;
				float y1= -10;
				float y2= 10;
				float z= 0;
				float w= 0;
				float width= x2-x1;
				float l= 0;
				float length= y2-y1;
				float w_inc = 2;
				float l_inc = 2;			
				for(l =0 ; l<= length; l+= l_inc)
				{
					Vec.set(x1,y1+l,z);
					p_array->mp_Vec3Array->push_back(Vec);				
					Vec.set(x2,y1+l,z);
					p_array->mp_Vec3Array->push_back(Vec);					
				}	
				for(w =0 ; w<= width; w+= w_inc)
				{					
					Vec.set(x1+w,y1,z);
					p_array->mp_Vec3Array->push_back(Vec);					
					Vec.set(x1+w,y2,z);
					p_array->mp_Vec3Array->push_back(Vec);						
				}				
			}
			else if(str_node_name== "Honeycomb")
			{
				float nx = 40;
				float ny = 20;
				float i =0.5;
				Vec3d vec;
				XML_Parse_Vec3d(spXMLNextNode, &vec);	
				nx = vec.x();
				ny = vec.y();
				i = vec.z();	
				float x1 = 0;
				float x2 = i* sqrtf(3)/2;
				float x3 = i* sqrtf(3);
				float y1 = 0;
				float y2 = i* 0.5;
				float y3 = i* 1.5;
				float y4 = i* 2.0;
				float y5 = i* 3.0;
				float z = 0;				
				float w = 0;
				float l = 0;
				float w_inc = x3;
				float l_inc = y5;
				float width = nx * x3;			
				float length = ny * y5;				
				for(l =0 ; l<= length; l+= l_inc)
				{
					for(w =0 ; w<= width; w+= w_inc)
					{
						Vec3d vec1(w+x1,l+y1,z);
						Vec3d vec2(w+x2,l+y2,z);
						Vec3d vec3(w+x3,l+y1,z);
						Vec3d vec4(w+x1,l+y4,z);
						Vec3d vec5(w+x2,l+y3,z);
						Vec3d vec6(w+x3,l+y4,z);
						Vec3d vec7(w+x1,l+y5,z);
						p_array->mp_Vec3Array->push_back(vec1);	
						p_array->mp_Vec3Array->push_back(vec2);	
						p_array->mp_Vec3Array->push_back(vec2);	
						p_array->mp_Vec3Array->push_back(vec3);	
						p_array->mp_Vec3Array->push_back(vec2);	
						p_array->mp_Vec3Array->push_back(vec5);	
						p_array->mp_Vec3Array->push_back(vec4);	
						p_array->mp_Vec3Array->push_back(vec5);	
						p_array->mp_Vec3Array->push_back(vec5);	
						p_array->mp_Vec3Array->push_back(vec6);	
						p_array->mp_Vec3Array->push_back(vec4);	
						p_array->mp_Vec3Array->push_back(vec7);						
					}
				}
			}
			else if(str_node_name== "FunctionOfTwoVariables")
			{
				CFunctionOfTwoVariables fotv;
				XML_Parse_FunctionOfTwoVariables(spXMLNextNode, &fotv);
				std::vector<Vec3d> m_v3DPt;
				std::vector<Vec3d>::iterator iter;
				fotv.create(&m_v3DPt);
				for(iter = m_v3DPt.begin(); iter< m_v3DPt.end(); iter++) {
					p_array->mp_Vec3Array->push_back(*iter);
				}				
			}
			else if(str_node_name== "FunctionOfOneVariable")
			{
				CFunctionOfOneVariable fotv;
				XML_Parse_FunctionOfOneVariable(spXMLNextNode, &fotv);
				std::vector<Vec3d> m_v3DPt;
				std::vector<Vec3d>::iterator iter;
				fotv.create(&m_v3DPt);
				for(iter = m_v3DPt.begin(); iter< m_v3DPt.end(); iter++) {
					p_array->mp_Vec3Array->push_back(*iter);
				}				
			}
			else if(str_node_name== "ParametricEquation")
			{
				CParametricEquation para;
				XML_Parse_ParametricFunction(spXMLNextNode, &para);
				std::vector<Vec3d> m_v3DPt;
				std::vector<Vec3d>::iterator iter;
				para.create(&m_v3DPt);
				for(iter = m_v3DPt.begin(); iter< m_v3DPt.end(); iter++) {
					p_array->mp_Vec3Array->push_back(*iter);
				}		
			}
			else if(str_node_name== "EXPRVectorField")
			{
				CEXPRVectorField vector_field;
				XML_Parse_EXPRVectorField(spXMLNextNode, &vector_field);
				if(vector_field.m_MDEXPR.link())
				{
					std::vector<Vec3d> m_v3DPt;
					std::vector<Vec3d>::iterator iter;
					vector_field.create(&m_v3DPt,&(p_array->m_count));
					for(iter = m_v3DPt.begin(); iter< m_v3DPt.end(); iter++) {
						p_array->mp_Vec3Array->push_back(*iter);
					}		
				}
			}	
			else if(str_node_name== "VectorField")
			{
				CVectorField vector_field;
				XML_Parse_VectorField(spXMLNextNode, &vector_field);
				std::vector<Vec3d> m_v3DPt;
				std::vector<Vec3d>::iterator iter;
				vector_field.create(&m_v3DPt,&(p_array->m_count));
				for(iter = m_v3DPt.begin(); iter< m_v3DPt.end(); iter++) {
					p_array->mp_Vec3Array->push_back(*iter);
				}		
			}
			else if(str_node_name== "HUDBackgroundVertices")
			{
				p_array->mp_Vec3Array = new osg::Vec3Array;
				p_array->mp_Vec3Array->push_back( osg::Vec3( 0,    0,-1) );
				p_array->mp_Vec3Array->push_back( osg::Vec3(1024,  0,-1) );
				p_array->mp_Vec3Array->push_back( osg::Vec3(1024,200,-1) );
				p_array->mp_Vec3Array->push_back( osg::Vec3(   0,200,-1) );
			}		

				
			//CMDEXPR
		}
	}
}
void XML_Parse_Vec4Array(CComPtr<IXMLDOMNode> spXMLNode, CVec4Array *p_array)
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
			if(str_node_name== "Name")
			{
				p_array->m_name = str_node_text;
			}	
			else if(str_node_name== "Vec4")
			{
				Vec4 vec;
				XML_Parse_Vec4(spXMLNextNode, &vec);	
				p_array->mp_Vec4Array->push_back(vec);
			}
			else if(str_node_name== "HUDcolors")
			{
				p_array->mp_Vec4Array->push_back(osg::Vec4(0.8f,0.8f,0.8f,0.8f));
			}		
			else if(str_node_name== "LoopColorSet")
			{
				CColorSet color_set;
				XML_Parse_ColorSet(spXMLNextNode, &color_set);	
						
				if(color_set.mv_color.size()>=2)
				{
					bool flag = true;
					for(unsigned n= 0; n< color_set.m_count; n++)
					{
						if(flag)	
						{
							p_array->mp_Vec4Array->push_back(color_set.mv_color[0]);
							flag =false;
						}
						else
						{
							p_array->mp_Vec4Array->push_back(color_set.mv_color[1]);
							flag =true;
						}
					}
				}
			}					
		}
	}
}

void XML_Parse_Vec2(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec2 *vec)
{
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	float x = 0;
	float y = 0;
	float z = 0;		
	for(long i= 0; i< listLength; i++)
	{
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "x")
			{
				istringstream iss(str_node_text);
				iss >> x;
			}
			else if(str_node_name== "y")
			{
				istringstream iss(str_node_text);
				iss >> y;
			}			
		}
	}	
	vec->set(x,y);
}

void XML_Parse_Vec3(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec3 *vec)
{
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	float x = 0;
	float y = 0;
	float z = 0;		
	for(long i= 0; i< listLength; i++)
	{
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "x")
			{
				istringstream iss(str_node_text);
				iss >> x;
			}
			else if(str_node_name== "y")
			{
				istringstream iss(str_node_text);
				iss >> y;
			}
			else if(str_node_name== "z")
			{
				istringstream iss(str_node_text);
				iss >> z;
			}		
		}
	}	
	vec->set(x,y,z);
}
void XML_Parse_Vec4d(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec4d *vec)
{
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	float x = 0;
	float y = 0;
	float z = 0;	
	float w = 0;	
	for(long i= 0; i< listLength; i++)
	{
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "x")
			{
				istringstream iss(str_node_text);
				iss >> x;
			}
			else if(str_node_name== "y")
			{
				istringstream iss(str_node_text);
				iss >> y;
			}
			else if(str_node_name== "z")
			{
				istringstream iss(str_node_text);
				iss >> z;
			}		
			else if(str_node_name== "w")
			{
				istringstream iss(str_node_text);
				iss >> w;
			}		
		}
	}	
	vec->set(x,y,z,w);
}
void XML_Parse_Vec4(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec4 *vec)
{
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	float x = 0;
	float y = 0;
	float z = 0;	
	float w = 0;	
	for(long i= 0; i< listLength; i++)
	{
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "x")
			{
				istringstream iss(str_node_text);
				iss >> x;
			}
			else if(str_node_name== "y")
			{
				istringstream iss(str_node_text);
				iss >> y;
			}
			else if(str_node_name== "z")
			{
				istringstream iss(str_node_text);
				iss >> z;
			}		
			else if(str_node_name== "w")
			{
				istringstream iss(str_node_text);
				iss >> w;
			}		
		}
	}	
	vec->set(x,y,z,w);
}
void XML_Parse_Vec3d(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec3d *vec)
{
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	float x = 0;
	float y = 0;
	float z = 0;	
	for(long i= 0; i< listLength; i++)
	{
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "x")
			{
				istringstream iss(str_node_text);
				iss >> x;
			}
			else if(str_node_name== "y")
			{
				istringstream iss(str_node_text);
				iss >> y;
			}
			else if(str_node_name== "z")
			{
				istringstream iss(str_node_text);
				iss >> z;
			}			
		}
	}	
	vec->set(x,y,z);
}
void XML_Parse_Vec3f(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec3f *vec)
{
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr =spXMLDOMNodeList->get_length(&listLength);
	float x = 0;
	float y = 0;
	float z = 0;	
	for(long i= 0; i< listLength; i++)
	{
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "x")
			{
				istringstream iss(str_node_text);
				iss >> x;
			}
			else if(str_node_name== "y")
			{
				istringstream iss(str_node_text);
				iss >> y;
			}
			else if(str_node_name== "z")
			{
				istringstream iss(str_node_text);
				iss >> z;
			}			
		}
	}	
	vec->set(x,y,z);

}
void XML_Parse_VectorField(CComPtr<IXMLDOMNode> spXMLNode, CVectorField *vector_field)
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
			if(str_node_name== "Expression")
			{
				vector_field->mv_str_expression.push_back(str_node_text);
			}	
			else if(str_node_name== "ExprInterval")
			{
				CExprInterval var;
				XML_Parse_ExprInterval(spXMLNextNode, &var);
				vector_field->mv_expr_interval.push_back(var);
			}	
			else if(str_node_name== "Scale")
			{
				istringstream iss(str_node_text);
				iss >> vector_field->m_scale;
			}	
				
		}
	}
}
void XML_Parse_EXPRVectorField(CComPtr<IXMLDOMNode> spXMLNode, CEXPRVectorField *vector_field)
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
			if(str_node_name== "MDEXPR")
			{
				XML_Parse_MDEXPR(spXMLNextNode, &(vector_field->m_MDEXPR));
			}	
			else if(str_node_name== "ExprInterval")
			{
				CEXPRInterval var;
				XML_Parse_EXPRInterval(spXMLNextNode, &var);
				vector_field->mv_expr_interval.push_back(var);
			}	
			else if(str_node_name== "Scale")
			{
				istringstream iss(str_node_text);
				iss >> vector_field->m_scale;
			}	
			
		}
	}
}
void XML_Parse_Viewer(CComPtr<IXMLDOMNode> spXMLNode, CViewer *p_viewer)
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
			if(str_node_name== "Name")
			{
				p_viewer->m_name = str_node_text;
			}				
			else if(str_node_name== "CameraManipulator")
			{
				//TODO
				//osgGA::KeySwitchMatrixManipulator
				if(str_node_text== "TrackballManipulator")
				{
					p_viewer->mp_viewer->setCameraManipulator(new osgGA::TrackballManipulator());
				}			
				else if(str_node_text== "FlightManipulator")
				{
					p_viewer->mp_viewer->setCameraManipulator(new osgGA::FlightManipulator());
				}
				else if(str_node_text== "DriveManipulator")
				{
					p_viewer->mp_viewer->setCameraManipulator(new osgGA::DriveManipulator());
				}
				else if(str_node_text== "CPhysXTerrain")
				{
					p_viewer->mp_viewer->setCameraManipulator(new osgGA::TerrainManipulator());
				}
				else if(str_node_text== "OrbitManipulator")
				{
					p_viewer->mp_viewer->setCameraManipulator(new osgGA::OrbitManipulator());
				}
				else if(str_node_text== "FirstPersonManipulatorT")
				{
					p_viewer->mp_viewer->setCameraManipulator(new osgGA::FirstPersonManipulator(true));
				}
				else if(str_node_text== "FirstPersonManipulatorF")
				{
					p_viewer->mp_viewer->setCameraManipulator(new osgGA::FirstPersonManipulator(false));
				}
				else if(str_node_text== "FlightManipulator")
				{
					p_viewer->mp_viewer->setCameraManipulator(new osgGA::FlightManipulator());
				}					
				else if(str_node_text== "SphericalManipulator")
				{
					p_viewer->mp_viewer->setCameraManipulator(new osgGA::SphericalManipulator());
				}
				
			}		
			else if(str_node_name== "SetUpViewInWindow")
			{				
				CWindowRect window_rect;	
				window_rect.m_x = 50;
				window_rect.m_y = 50;
				window_rect.m_width = 1024;
				window_rect.m_height = 768;
				XML_Parse_WindowRect(spXMLNextNode, &window_rect);								
				p_viewer->mp_viewer->setUpViewInWindow( window_rect.m_x, window_rect.m_y, window_rect.m_width, window_rect.m_height );				
			}					
			else if(str_node_name== "Ref")
			{
				CReference ref;				
				XML_Parse_Reference(spXMLNextNode, &ref);								
				p_viewer->mv_ref.push_back(ref);
			}	
			else if(str_node_name== "LightingMode")
			{
				if(str_node_text== "SKY_LIGHT")
				{
					p_viewer->mp_viewer->getCamera()->getView()->setLightingMode(osg::View::SKY_LIGHT); 
				}
				else if(str_node_text== "HEADLIGHT")
				{
					p_viewer->mp_viewer->getCamera()->getView()->setLightingMode(osg::View::HEADLIGHT); 
				}				
			}	
			else if(str_node_name== "SetCullMask")
			{			
				unsigned u;
				istringstream iss(str_node_text);
				iss >>u;
				p_viewer->mp_viewer->getCamera()->setCullMask (u);
			}		
		}
	}	
}
void XML_Parse_WindowRect(CComPtr<IXMLDOMNode> spXMLNode, CWindowRect *p_window_rect)
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
			if(str_node_name== "x")
			{
				istringstream iss(str_node_text);
				iss >> p_window_rect->m_x;
			}
			else if(str_node_name== "y")
			{
				istringstream iss(str_node_text);
				iss >> p_window_rect->m_y;
			}
			else if(str_node_name== "width")
			{
				istringstream iss(str_node_text);
				iss >> p_window_rect->m_width;
			}
			else if(str_node_name== "height")
			{
				istringstream iss(str_node_text);
				iss >> p_window_rect->m_height;
			}
		}
	}	
}


bool XML_Parse_XGMML_Node(CComPtr<IXMLDOMNode> spXMLNode, CXGMML_node* p_node)
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
			if(str_node_name== "att")
			{							
				CXGMML_att att;								
				XML_Parse_XGMML_Att(spXMLNextNode, &att);			
				p_node->mv_att.push_back(att);
			}
			if(str_node_name== "graphics")
			{							
				CXGMML_graphics graphics;								
				XML_Parse_XGMML_Graphics(spXMLNextNode, &(p_node->m_graphics));				
			}
		}
	}
	return true;
}

bool XML_Parse_XGMML_Edge(CComPtr<IXMLDOMNode> spXMLNode, CXGMML_edge* p_edge)
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
			if(str_node_name== "att")
			{							
				CXGMML_att att;								
				XML_Parse_XGMML_Att(spXMLNextNode, &att);			
				p_edge->mv_att.push_back(att);
			}
			if(str_node_name== "graphics")
			{							
				CXGMML_graphics graphics;								
				XML_Parse_XGMML_Graphics(spXMLNextNode, &(p_edge->m_graphics));						
			}
		}
	}
	return true;
}
std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen) 
{ 
    int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL); 
 
    std::string dblstr(len, '\0'); 
    len = ::WideCharToMultiByte(CP_ACP, 0 /* no flags */, 
                                pstr, wslen /* not necessary NULL-terminated */, 
                                &dblstr[0], len, 
                                NULL, NULL /* no default char */); 
 
    return dblstr; 
} 

std::string ConvertBSTRToMBS(BSTR bstr) 
{ 
    int wslen = ::SysStringLen(bstr); 
	if(wslen== 0) {
		return "";
	}
    return ConvertWCSToMBS((wchar_t*)bstr, wslen); 
} 


bool XML_Parse_XGMML_Att(CComPtr<IXMLDOMNode> spXMLNode, CXGMML_att* p_att)
{
	HRESULT hr;	
	CComPtr<IXMLDOMNamedNodeMap> spXMLDOMNodeMap;
	long AttListLength;
	hr = spXMLNode->get_attributes(&spXMLDOMNodeMap);	
	hr = spXMLDOMNodeMap->get_length(&AttListLength);	
	CXGMML_attribute attribute;
	BSTR text;
	BSTR name;
	for(long i= 0; i< AttListLength; i++)
	{	
		CComPtr<IXMLDOMNode> spXMLDOMAttNode;
		hr = spXMLDOMNodeMap->nextNode(&spXMLDOMAttNode);
		if(spXMLDOMAttNode!= NULL)
		{			
			spXMLDOMAttNode->get_nodeName(&name);
			spXMLDOMAttNode->get_text(&text);	
			attribute.m_name = ConvertBSTRToMBS(name);
			attribute.m_value =  ConvertBSTRToMBS(text);
			p_att->mv_attribute.push_back(attribute);
		}
	}
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	long listLength;
	hr =spXMLDOMNodeList->get_length(&listLength);
	for(long i= 0; i< listLength; i++)
	{		
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "att")
			{							
				
			}
		}
	}
	return true;
}
bool XML_Parse_XGMML_Graphics(CComPtr<IXMLDOMNode> spXMLNode, CXGMML_graphics* p_graphics)
{
	HRESULT hr;	
	CComPtr<IXMLDOMNamedNodeMap> spXMLDOMNodeMap;
	long AttListLength;
	hr = spXMLNode->get_attributes(&spXMLDOMNodeMap);	
	hr = spXMLDOMNodeMap->get_length(&AttListLength);	
	CXGMML_attribute attribute;
	BSTR text;
	BSTR name;
	for(long i= 0; i< AttListLength; i++)
	{	
		CComPtr<IXMLDOMNode> spXMLDOMAttNode;
		hr = spXMLDOMNodeMap->nextNode(&spXMLDOMAttNode);
		if(spXMLDOMAttNode!= NULL)
		{			
			spXMLDOMAttNode->get_nodeName(&name);
			spXMLDOMAttNode->get_text(&text);	
			attribute.m_name = ConvertBSTRToMBS(name);
			attribute.m_value =  ConvertBSTRToMBS(text);
			p_graphics->mv_attribute.push_back(attribute);
		}
	}
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	long listLength;
	hr =spXMLDOMNodeList->get_length(&listLength);
	for(long i= 0; i< listLength; i++)
	{		
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL)
		{			
			if(str_node_name== "att")
			{							
				
			}
		}
	}
	return true;
}



//
//class ObjectManipulator : public osgGA::GUIEventHandler
//{
//	public:
//		virtual bool handle(const osgGA::GUIEventAdapter&  ea,osgGA::GUIActionAdapter&);
//		virtual void accept(osgGA::GUIEventHandlerVisitor& v)   { v.visit(*this); };
//};
//
//
//bool ObjectManipulator::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
//{
//	float fraction = 0.1f;
//	switch ( ea.getEventType() )
//	{
//		case osgGA::GUIEventAdapter::KEYDOWN: // Key Down Events
//			switch ( ea.getKey() )
//			{				
//				case 'w': //Increase Zoom Factor		
//					if(zoomFactor >=.01)
//					{
//						zoomFactor = 0.009;
//					}
//					else
//						zoomFactor= zoomFactor + 0.001;
//					terrainManipulator->setWheelZoomFactor(zoomFactor);	                                                                                                                            
//					//readFromFile();		
//					return true;
//					break;
//
//				case 's': //Decrease rate of Zooming
//		
//					if(zoomFactor <=0)
//					{
//						zoomFactor = 0.001;
//					}
//					else
//						zoomFactor= zoomFactor - 0.001;
//
//					terrainManipulator->setWheelZoomFactor(zoomFactor);
//	
//					//readFromFile();
//		
//					return true;
//					break;
//
//				case 'a': //When F1 Key Pushes
//	
//					Veye = osg::Vec3d(-16814506.402436,17059817.670479,-13739068.986011);
//					Vcentre = osg::Vec3d(-16814505.794679,17059817.045817,-13739068.495675);
//					Vup = osg::Vec3d(0.378772,-0.314671,-0.870352);
//					terrainManipulator->setHomePosition( Veye, Vcentre, Vup );
//					terrainManipulator->home(ea,aa);
//					return true;
//					break;
//
//				case 65470: //When F1 Key Pushes
//
//					char a[500] = "";	
//
//					viewer.getCamera()->getViewMatrixAsLookAt(Veye,Vcentre,Vup);
//
//					sprintf(strEyeX, "%f",Veye._v[0]);
//					sprintf(strEyeY, "%f",Veye._v[1]);
//					sprintf(strEyeZ, "%f",Veye._v[2]);
//
//					sprintf(strCentreX, "%f",Vcentre._v[0]);
//					sprintf(strCentreY, "%f",Vcentre._v[1]);
//					sprintf(strCentreZ, "%f",Vcentre._v[2]);
//
//					sprintf(strUpX, "%f",Vup._v[0]);
//					sprintf(strUpY, "%f",Vup._v[1]);
//					sprintf(strUpZ, "%f",Vup._v[2]);
//
//					strText = "Eye="+std::string(strEyeX)+"~"+std::string(strEyeY)+"~"+std::string(strEyeZ)+",Centre="+std::string(strCentreX)+"~"+std::string(strCentreY)+"~"+std::string(strCentreZ)+",Up="+std::string(strUpX)+"~"+std::string(strUpY)+"~"+std::string(strUpZ)+"\n";
//					strcat(a,"Camera Position: \n");
//					strcat(a,"       X = ");
//					strcat(a,strEyeX);
//					strcat(a,"\n       Y = ");
//					strcat(a,strEyeY);
//					strcat(a,"\n       Z = ");
//					strcat(a,strEyeZ);
//
//					strcat(a,"\nCamera Look At: \n");
//					strcat(a,"       X = ");
//					strcat(a,strCentreX);
//					strcat(a,"\n       Y = ");
//					strcat(a,strCentreY);
//					strcat(a,"\n       Z = ");
//					strcat(a,strCentreZ);
//
//					strcat(a,"\nUp Position: \n");
//					strcat(a,"       X = ");
//					strcat(a,strUpX);
//					strcat(a,"\n       Y = ");
//					strcat(a,strUpY);
//					strcat(a,"\n       Z = ");
//					strcat(a,strUpZ);
//
//					textGeode->setDrawable(0,createText(osg::Vec3(50.0f, 200.0f, 0.0f),a ,10.0f));
//					writeToFile(strText);
//					break;
//				}
//				break;
//
//
//			case osgGA::GUIEventAdapter::KEYUP: //Key Up Events
//			switch ( ea.getKey() )
//			{
//				case 65470: // When F1 key Releases
//					char a[500] = "";
//					viewer.getCamera()->getViewMatrixAsLookAt(Veye,Vcentre,Vup);
//					textGeode->setDrawable(0,createText(osg::Vec3(150.0f, 500.0f, 0.0f),a ,20.0f));
//					break;
//			}
//			break;
//		default:
//			break;
//		}
//	return false;
//  }



/*
I??????????????+????????????+????????????+??++?+++++++?+++++?+++++++++++++++++D88D8D8=?+++++++++++++++++++++++++++++++++++++++++======+=+++=
????????????????????????????+??+???????++?++++?+?+?+?+??++++++++++++++++++++++D8D88DO+++++++++++?+++++++++++++++++++++++++++++++=+=+==++++==
?????????????????????????++????????++???????++++++++++?+?+++++++++++++++++++++D88D887++++++++++++++++++++++=+++++++++++++++++++++++=+=======
??????????????????????????????++????????++?+++++++++?+++++++++++++++++++++++++DDDDDO++++++++++++++++++++++++++++==+++++++=+=+++=+=+==++=====
??????????????++????+++???????++??+++++++++++++++++++++++++++++++++++++++++++ODDD8DO++++++++++++++++++++++++++++=+=++++++====++++===========
?????????????+???+?++?+?++?++?+???++++??+++++++++++++++++++++++++++++++++++++8DDDDD+++++++++++++++++++++++++++=+===++=+=+++++===============
???????????????+???+??+??????++++++++++++++++++++++++++++++++++++++++++++++++8DDDD8=++++++++++++++++++++++++=+====+=============++====+==+==
OOOOOO8ZOOOO8O8888O$I+?????++++++++++++++++++++++++++++++++++++++++++++++++++DDDDDO++=++++++=+++++++==++=====================~====IZ8ZOZZOZZ
?????+??????+??7ZOOO8ZOZZOZOOOOO8O8888ODZI???=++++++++++++++++++++++++++++==ZDDDDD??++++++==++========+=====++==+$O8ZZZOOOOOZZ$ZOZO$?==~====
????+????++??+++++++++++++++++++++++??+7O8ZZZZOZOO8888D8D7=~+++++++=+=+==8D8Z8DDD8ODND$+==?OD7===~+O8O8OOOO$Z8ZI==~~=~~~========+===========
??+++++?+?+??+++++++++++++++++++++++++++++++++++=+++=+?$$ZZZZO88DDO8NM7??7DDDNDDDDOOD78DDNDN+NDD8D~=~~====~=~=====~=========================
??+++++?++++++++++++++++++++++++++++++++++++++++++++++=+++==++=DDDDI=IDNDDD8D8NDDDOO8DNNN8NNON~=============================================
??++++?++?++++++++++++++++++++++++++++++++++=++++++++=====++=====~Z~8DD8DDNDD8DD8DDDDD8O8DND=.,==================~~~~~~=====================
++++++++++++++++++++++++++++++++++++++++++++++++++===+++===~=ODO888$DDODD=?DZ=NDDD7N8$I?8NDND8DO8OOI~:~:~~:~~==~============================
+++++++++++++++++++++++++++++++++++++==++++++++==++=+=Z888OOO8OO?===O7==+==D8DDMNNNDZ$O?=+========~~7Z$88O$~===============================~
+++++++++++++++++++++++++++++=+++++++==+===~=+=8888888O$++=============+:=+NNNNNDDN8IOO8$Z$7================OZO8OO+~=~======================
+++++++++++++++=++=++++++++++=+++==+=~+=$O8Z8OOO+=======8====~=====~=+?OZOD8$MNDDND7$888ZO888+=======~=~==~=~======$OZZO$~=~=~==============
++++++++++++++=++++++++++=++===+===ZDDOO88?===~=====~::~=~7???O8Z$$$$$$$Z88DOO8DNNO7$D8OI888O?Z===~,7~~==~=~=+I~~====~==IZOZOO+~~=~=========
+++++=++++=+=+=+=====+=+=+~+=ZOOO8OZ===============~??OI$=ZZZZOOZOOOOOOODDDZ$$$77IIDZD8OO8O8I++$8:~OOO~~~:+ZO78N8=~=======~==~?8ZOO87=~====~
+++++++++++============$OOOOZO==================??+8OOOOOOZZ8D8OO8OOOODDDZZ$7I+O8D8D7$Z8DMN888DDDNNDDN87:ZD~=~.DNO===~~=~=~==~~~~~~~IZZZZOI~
++==+=+===========+8OOZO$=======~====~~=====~??~=DO8ZO88OOOOOOOZ$8OOOND8ZZ7O=$8NNNNNND77$OONZZ$OOO8O~~~OZ~+=~~DN88:~~~~~=~~~~~==~~~~~~=~~7ZZ
+==+===+=====7OOOZO================~~=~~~=+?==?DZO78D8DDDOOOOZ888OOOND$$77Z=Z8NNDNNNDNN7+77ZZZOOOOOZ$.OZZ:Z?888Z88$~~~~~~~~~~~~=~~~~~~~~=~~~
=======+OZOZO+================~===~=~=~=++++=O8$ZOOD888D8Z8888888OO8D7I??+=ZN8N?DNNNDNDD$77788?OOOODDN==~8~~=~NOD7~~~~~~~~~=~~~~~~~~~~~~==~~
===?$OZOI~=~============~~====~~=~=~~~~+++=DDD$$8888DD8DDDDD888O8ODZ777I?=$OO8NO8NDNDNNNO777NN8Z8Z88~~~~~=~~:D:N87~~~~~~~$$Z8O88888888O88$~~
ZZZ=+=======~=~==~==~=~~~~~~~~~~~~~~~+~++8O8DOZZDDDDDD8OOO8D88OOO87II=II+=NMNNNDDDNNNNND887$DNI878O:~~=88Z+DDZ:ND88O8888D?MDD88OOOZZZZOZ8$:~
+========~=====~=~~==~~~~~~~~~~~~~~====N8DDDO8DDDDND8OOOOO88OOZOOI=:=?=,I~8DDDN88NNNDMNNNN7$$NND8D$7~==D$:Z+=DOMD8ZZOZO8O$DD8:~~~:~=~~~~~Z~~
=====~==~=~~~=~~~=~~~~~~~~~~~~~~~~+~=+OOONDZDNNDDDD8888OZZZZ7$$I?=.:=+:I+ODDDDDDDNNNNNNNNN?$$ZDNDNZZ$O=DOM:::O=ND8$~:~~:+Z888~~~~~~~~~~~~~~~
==~===~~=~~~=~~=~~~~==~~~~~~~~~~~+~+DDDDDNDNDNNND888DOOOZOD8=$O??I+=+=7787DDND88DNDDNNNNNN?8$$I77NN7ZZDN8:Z~:~~=DO8~~~~~8Z888~~~~~~~~~:~~~~~
====~~=~~=~~=~~~~~~=~~~~~~~~~~~~=DZD$88DNNNNNDNDD8O88OOOOZ8O8DZ$7?=+I=+++7NNNNNDDDDDZDDDNN?$OZ8I7MNN7ONOZZ7~~~~~N8O~~~~~~ZO8O~~~~~~~~~~~~:~:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~7OZ88NNDDNDDDNDON88O8O8ZZZOODZZ$$7III+??$$DNNNDDDDDDN8DDNN+$ZND88NNNDD$ZD8887~~~NN8~~~~:$$88O~~~~~~~:ZO:~::~
==~~~~~~~~=~~:~~~~~~~~~~~~~~:~~=:~8NDDDNMMDND88D8888OOOOZZ8NZZ$$7$77$I7II7DNDDDDDDDDN8DDNDI$8MNNDOONN7OO88OZ$$7I8D8:~~~DZZO8O~~~~~~:~ZZ::~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~:~:=:ZDZNDDNMMNNOO88DDOZOOZOOZ8OOZZ$$777I7777$DDDDDDDDNDDNDDN8ZZNNNMD88DM+IO8OOOOZZ$7D8:~~~$$Z$8O~~:~~::$ZZ~:::~
~~~~~~~~~~~~~~~~~~:::~:::~:~~~::~O8ONNNDNDDD8ZZZ88ZOOOZ$Z$DOZZ$$$$7I7777$7DD88DDDDDDDDD8NZZDNNNM8ZO8NN78OOOZOOZZOD8I=~:=Z7$8O~~:::~~OO~:~~::
~~~~~~~~~~~~~~~~~~~~~~~~:~~:~~=~OOO8DDDDDDD88ZOOD88OOZZZOZZZZ$$$77I77$$$7$8OOOZDDDDDDNDD$+ZZDNNZOZOOMN7$OOOOOO8O8D8$$?I7Z$$IO~::~~:,OO:~~:~~
~~~~~~~~~~~~~~~~~~~~~~~~:~~~::=~O8OONNN7DND8NOOZ88DDZZZ8ZZOOZ$$$7IIZI$7?7ZOO78ZZDDDDDNDDDZZZNNNNOZOONN7$ZZZ::~OZODDOO$$ZZI777~~:::~ZO::~~~::
~~~~~~~~~~~~~~~~~~~~~~~::~~:::=~8ODDDDDDNDDD8OZOOD8N8DDD8OZZZZ$77I?ZZOZOOZOODOODDODDDNNDIZZONNNN7O$ONN$$$O::~~~~:DD,OZOOOOZZO:~~~:,=$~~:~~:~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:~=ZD8OND8DDDDD8OZ8O8OD888888ZZOZ$7$8ZZZDOZZZOODD8ODDDDDDN7ZZZDNNN8IOOODN$$7~~:::~:~ID=:~:8NDO78D8DI?ZD~~:~~:~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:~?8Z88DDDDDDDDOOZZOZZ88OO8DOOOZ8OOOZZO888D$ZN88DN+7Z$DNDNIOZZNNNNM8OOZ8NOZ87:::~~~~.8?=~:888O:=IIDZDO$Z:~~7?7I
~~~~~~~~~~~~~~~~~~~~~~~~~~~~:~~~ZOOONDDNDDDDOZZOOOZ8ZO8888OOOZZO7OOZZOOZZZDNNNNO8$ZZNNIZOZZNNNMNOO+ZNN8ZZ$$$Z$Z$$7D$7$$DZO$$$$Z7$I$O7ZZZOOZ$
~~~~~~~~~~~~~~~:~~~~~~~~:~::::::ZOZO8DDDDDD8OZOOZZOO8O8OOZOOOOO$OZOOZOZOO8NNNNNNNNZONNIZZZ,MM8DN8$O:DN+$ZZOOZO$$7$DO7$7ZZ$7?O$Z$7I?=$$ZZ7Z=:
~~~~:~~?~~~~~~~~=~:~~~:~~~~~::,~Z$ZONDNDDDNNZOOOOZOZOOOOZOOOZZOZ:8O8ZOZ=$DNNNDDO8D88D8OOD8+MNNMN8OZZDN8$Z$.=$.=$I+88I===OOI:~::,8~8=++++++~:
77$7$$$7$Z$7$$$??++=++...:~~~?::7$ZZ=8DDDNDNN8ZZOZZZOOOOOZOZZZ8ZIOZO8OOOODNNDDOD8O8O8OOZOONNNNNM8O+ONNZ$$$:::::::,DO::::OZ::::~Z::O:::::::::
+===~~~~:::~~+==~~~:~::::::::::~:ZZOOD8DDDDDN88OZOZOOOOO8OOOZOOOOOOOOZOOODDDDNDDDN$88OOOOONMNNDDD+ZODD7OZ$,:::::::88::::8O::::~:::::::::::::
::::::::::::::::::::::::::,::,:,~888D$$OO8DDNNO888OOOOOOOOOOO88OOOZOOOOO8OO$ODDND8ZOOOOOZDDD8888OOOOZ8OD8O$7=,:~:~=D,:::8O:::~~~~O~::::~::~~
::~~~~:~:::::::::::::::::,,,,,::,+ODDDOZODD8DNNO8DDNOOZOOOOOOOODDO8D8O8O8OOOO8OZO78O8OOODDODM88DN8DNNDDDDD8DZ8OO8O8DO$I~:,:::8:::Z::::::::::
::::::::::::::::,:::::::::,,,,:::,+88D8DNDDDMNNM8O8D8ZOZZZOO8OOOODD888888OO8888O88$8O8OOOZ8ZO8NNNNDN7OZOZ,::$:,,+Z~D8:::,~:::::::,::::::::::
:::::::::::::::::::::::::,,,,,,,,,:,8D8NNNNN8D88DD88DZZZZZZZ77OODD8DDD8888O888888DONOO888O8O8O888887ZZO.::::::::~::OO::::8~:~~~~:~~~~~~~===~
,,:,::::::,:,::::::,:,:::::::::::::::NO$8D8DO$OOODD8DDDZOOZZZO8D8DDDD888D888DD8D888888888NDD88888OD8OZ~~:::::::::::8O,::~OI::::::::::::::::~
::::,:,:,:,::,::::::,:::::::::,,::::O8OD88$8OZ,+Z8ONDDD88D888D8888D8DD8D8DDDDDDDD8888Z88D88888D888ODOI?,:::::::::::OO,::::::::::::::::::::::
:::::::::::::::::::::::::,,,,::,,:,,::,::,,:8DNNNDDNDDDD8D8888D8888DDDDD8DD8DDDDDDDD8888888D8888OO8DNOI7~::::::::::$O~:::~:::::::~:::::::::~
::::::::::::::::,::,::,,,,,:::,:::::::::::::,.=~DDNNDNDDDDD8DD8DDD8DDDDDD88888D8ODDO8DD888D8D8OOZZ$~,DND8I:::::::::+$,:::::~::::::::~~~~~~::
::::::::,::::::::::,::,,,,:,:,,,,:,,::::::7I7,,DNNND7:+DDDDDDDDDDDD88DDDDDDDDDDDDD888DD88888$ZZZ$=.,,:7ND87,:~:::~:,7Z::~::::::::::::~::~:~~
:~::::::::,:,:::::::,:::::::::::::,::+Z7I,:::.DDNND::::::ND.D88I=ZONDDDDDDDDDDDDDDDD888O$+,,:,:~::::::::DD8Z=IIZI~::OZ:::::~:~:~:::~:~~~:~~~
,::::,:::~::::::::::::::::::::::?Z$$:::::::,8DNNND,:::::?88~D87OIZ8DDDNNN8888$Z?,D8D888Z$Z$7+:::~:::::::,OD$$8~:::::DZ:~:::::::::::::~~~::::
::::,:::,:,:::::,,,,,,,,,::,7$OZ=7~~:,88DD8DNNNMD$ZOZZ$$7ZZZOZOODNDOZZZZZOZZZZZ$ZO8DD8$Z,::::::::::::::::,~8ZI+,::~~IO:::::::::::::::::~::::
:,:~+$Z$ZO7+~:++::,:::=?ZOZ$$7,~IO$OZODNDDNNNNND888OOZZZZODD8DDDND8ZZOOZOOZZZZZOOOZO88$ZZI:::::::::::,:::::,8OI7+:::,8?::::::::::::::::~~~::
::::~:,,::+7Z$ZZZZZZZZZZZ$ZZZOZZOOZ8DDDDDNNNDDDDDDDDDD888DDNNDDDD88OOOOOOOOOOOOOOZ7$7Z$$$7:~~::::::::::::::,,D8$Z$7?~OZ::::::::::::::::~::::
:::::::,:?$Z77$OZZZZZZZZZZZZOOZOOOOO8DNDNNDDDDDDDDDDDDDDD8888O888888D88O888OOOOOZ887ZO88Z$ZOZZZ$ZI+~~:~=~~+OOI??II7I:~O~:::::::::::::::~::::
::$=+:::::7$ZZZDDOZZOZZOZOOOOO88D8DDDDDNNDDNNDDDD888888O8OO8OO8OOOOOOOO888O8O88888$$OO8D8ZIZZOOO8~~,::~?I?ZOO7$II$,~::OO=+ZZOZ$I77$OZ$Z$ZZO$
ZZZI+Z$~IZZZZ?ONDDDDDDDDDDDDDDDDNNNNNNNDD8888O8OOOO8OOOOOOOOOOOOOOOOOOOOZZOOOOOOOOOOOZOO88O77ZZZZZI?+?I77ZOOZ::,,::::,ODOZ$888OZO8Z888ZOOOZO
ZZZZZZZZZZZZOZOOONNNDDNDDNDDDD88O8OOOO8OOOOOOOOOOOOOOZOOOOOOOOOZOZOOO$OOOOOOOZOOZOD=:~::,8DZZ$I$7I?I7$OO:,,,:=ZZ?~,~?8ONDD8888ZZ$ZZZZZZ77ZO8
ZZZZZZZOZZZZZOOOZOO88DD8OOOOOOOOOOOOOOOOOOOOOOOOOOOOOZZZZZO$=~:ZZZZ$$8ZOZOO$OOOO=:~:::,~?77?77$Z7III,,,,:,,,,,::,,:::,,~$Z8D8D8888OOOODOZZOO
Z$$$Z$$ZOZ$ZZZZZZZZZZOZZZZZZZZZZZZZZZZZZZZO$ZZZZZ$+:.,:,,,,,::::::::,::ZZ$7$$::::=$7IIII77$7$$$~,,,,,:,:,:,,,,,,:,,:,::::~::::::::::::::~::,
$Z=,::::::,:,,,,,=Z$Z$$7$$$$$7=,:,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,::,,.8ZOZ$$7$777I77$$7$$Z.,,,,,,,,,,,,,,:,:,:,:,:::::::~::::::::::::::::::
::::::,::,::,,,,,:,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,::::,:,,:,:,::,,,,,,,,,,8O$ZOZZ$ZZ77Z::,,,,,,,,,.,,,,,,,,,,,,,,::::::::::~:::::::::::::::::
:::::,,::::,,,:,,,,,,,,,,,,,,,,,:::,,,,.,,,,,,,:,,,,:,,::,,,,,,,,,,,,,:,,,:OZZZZ$:,,,,,,,,,,,,,,,.,,,,::,,,:,,:,,::,:::::::::::::::::::::::,
,,,:,,::::,:,,,,,,,,,,,,,,,,,,,,,,,,,,,:,,,,,:,,,:,,,,,.,,,,,,,,,?$$$$Z$$ZI,,:,,,,,:,,,,,,,.....,.,,,,,,,,,,,:,:,::::::::,:::::::::::::::::,
::::,:,,:,,,,,,,,,,::,,,,,,,,,,,,,:,,,:,::,:,,,,,,,,.,,,,,.,,,.,,,,,,,,,::,,:,,,,,,,,,,,,,.,....,..,,,,,,,,,,:,,,,,:,::::::,::,:::::::,::,:,
:,,:,:,,,,,,:,,,,::,,,,,,.,,::,,,:,,,,,,,,,,,,,,,,,,,,,.,,,,,,,,,,,,,,,,,:,:,,,,::,,,,,,,,,,,.,.,..,,,,,,,,,,,,,:,:,:,::::,::,:,,,::,:,:::,,
:,:,,:,,:,::,,,,,,,,,,,,,,,:,,,,,,:,,:,,,,:,,,:,,,,,,.,,,,.,,,,,,,,,,,,,,,,:::,,,,,,,:,,,,,.,,,.,.,.,,,,,,,,::::::::::::::::~::::,:,,,:::::,

*/