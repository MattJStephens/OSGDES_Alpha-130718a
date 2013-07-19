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

#ifndef _INCL_OSG_Entities
#define _INCL_OSG_Entities

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <osg/Billboard>
#include <osg/Geometry>
#include <osg/io_utils>
#include <osg/Image>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/LineSegment>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Notify> 
#include <osg/Point>
#include <osg/PointSprite>
#include <osg/PolygonMode>
#include <osg/PositionAttitudeTransform>
#include <osg/Program>
//#include <osg/Producer>//deprecated

#include <osg/Sequence>
#include <osg/ShapeDrawable>
#include <osg/Shader>
#include <osg/StateSet>
#include <osg/Texture>
#include <osg/Texture2D>

#include <osgText/Font>
#include <osgText/Text>

#include <osgParticle/Particle>
#include <osgViewer/Viewer>

#include <osgDB/ReadFile>

#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>

#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator> 
#include <osgGA/FirstPersonManipulator> 
#include <osgGA/StateSetManipulator>

#include <osgAnimation/Bone>
#include <osgAnimation/Skeleton>
#include <osgAnimation/RigGeometry>
#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/UpdateMatrixTransform>
#include <osgAnimation/UpdateBone>
#include <osgAnimation/StackedTransform>
#include <osgAnimation/StackedTranslateElement>
#include <osgAnimation/StackedRotateAxisElement>

#include <osgFX/AnisotropicLighting>

#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/FireEffect>

#include <osgSim/Impostor> 
#include <osgSim/InsertImpostorsVisitor> 

#include "NxControllerManager.h"
#include "NxPhysics.h"
#include "NxPhysicsSDK.h"
//#include "MotionCallback.h"
//#include "PhysXClothCallback.h"

#include "PhysXCharacterController.h"
#include "PhysXCloth.h"
#include "PhysXCube.h"
#include "PhysXFluid.h"
#include "PhysXJoint.h"
#include "PhysXPicker.h"
#include "PhysXPlane.h"
#include "PhysXRope.h"
#include "PhysXSoftBody.h"
#include "PhysXSphere.h"
#include "PhysXTerrain.h"
#include "OSG_Logistics_elements.h"
#include "OSG_Reference.h"
#include "OSG_DES.h"
#include "XML_EXPR.h"




/////////////////////////////////////////////////////////////////////////////////////////////
// namespace
/////////////////////////////////////////////////////////////////////////////////////////////
using namespace osg;
using namespace std;
/////////////////////////////////////////////////////////////////////////////////////////////
// enum
/////////////////////////////////////////////////////////////////////////////////////////////
enum GeometryType { 
	eGeometryTypeUndefined, 
	eGeometryTypeFile,  
	eGeometryTypeBox, 
	eGeometryTypeCapsule,
	eGeometryTypeCompositeShape, 
	eGeometryTypeCone,
	eGeometryTypeCylinder,
	eGeometryTypeHeightField,
	eGeometryTypeInfinitePlane,
	eGeometryTypeKdTree, 
	eGeometryTypeSphere, 
	eGeometryTypTriangleMesh,
	eGeometryTypeCube
};
enum eSourceGeometryType { SourceGeometryType_Node, SourceGeometryType_Geode };


enum eClothType { ClothType_Free, ClothType_Attached };
enum eParsedVariableType { 
	ParsedVariableType_float, 
	ParsedVariableType_string,
	ParsedVariableType_reference, 
	ParsedVariableType_bool, 
	ParsedVariableType_unsigned, 
	ParsedVariableType_int, 
	ParsedVariableType_enum };


enum EntityCreationFlag { 
	eEntityCreationFlag_Create, 
	eEntityCreationFlag_AddCreate,  
	eEntityCreationFlag_AddOverwrite };


#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


/////////////////////////////////////////////////////////////////////////////////////////////
// class Forward
/////////////////////////////////////////////////////////////////////////////////////////////
	//class CPositionAttitudeTransform;

/////////////////////////////////////////////////////////////////////////////////////////////
// Base entities
/////////////////////////////////////////////////////////////////////////////////////////////
class CParsedVariable
{
	public:
		std::string m_str_value;
		std::string m_str_data_type;
		eParsedVariableType m_data_type;
		std::string m_identifier;		
};

class COrientation
{
	public:
		Vec3d m_axis;
		float m_angle;
};
class CRect
{ 
	public:
		double  m_left; 
		double  m_right; 
		double  m_bottom;  
		double  m_top;
};
class CWindowRect
{ 
	public:
		unsigned  m_x; 
		unsigned  m_y; 
		unsigned  m_width;  
		unsigned  m_height;
};

class CApproxArea
{
	public:
		float m_x;
		float m_y;
		float m_A;
};

class CGaussian
{
public:
	CGaussian();
	std::vector<CApproxArea> mv_approx_area;
	float m_min_x;
	float m_max_x;
	float m_sigma;
	float m_mu;
	double get_phi(double x);
	double erf(double x);
	double cumulative_distribution(double x, double mean, double standard_deviation);
};


/////////////////////////////////////////////////////////////////////////////////////////////
// Entity classes containing OSG objects
/////////////////////////////////////////////////////////////////////////////////////////////
class CEntity
{
public:		
	CEntity() { m_slay = false; m_EntityCreationFlag = eEntityCreationFlag_Create; }
	unsigned m_unique_id;
	bool m_slay;
	std::string m_name;
	EntityCreationFlag m_EntityCreationFlag;
};
class CAlphaFunc : public CEntity
{
	public:		
		osg::ref_ptr<osg::AlphaFunc> mp_alphafunc;
};
class CAnisotropicLighting : public CEntity
{
	public:		
		osg::ref_ptr<osgFX::AnisotropicLighting> mp_anisotropic_lighting;
};
class CAnimationControlPoint : public CEntity
{
	public:		
		double m_time;
		osg::AnimationPath::ControlPoint m_control_point;
};
class CAnimationPath : public CEntity
{
	public:		
		osg::ref_ptr<osg::AnimationPath> mp_animation_path;
		vector<CReference> mv_ref;
};
class CAnimationRigGeometry : public CEntity
{
	public:		
		osg::ref_ptr<osgAnimation::RigGeometry> mp_rig_geometry;
};
class CArray : public CEntity 
{
	public:	
		osg::ref_ptr<osg::Array> mp_array;
};
class CBillboard : public CEntity
{
	public:	
		osg::Vec3 m_position;
		osg::ref_ptr<osg::Billboard> mp_billboard;
		vector<CReference> mv_ref;
};
class CBoundingBox : public CEntity
{
	public:	
		osg::BoundingBox* mp_bounding_box;//no ref pointer
		vector<CReference> mv_ref;//temp?
};
class CCamera : public CEntity
{
	public:
		osg::ref_ptr<osg::Camera> mp_camera;
};
class CClearNode : public CEntity
{
	public:		
		osg::ref_ptr<osg::ClearNode> mp_clear_node;
};
class CDrawElementsUInt : public CEntity
{
	public:		
		osg::ref_ptr<osg::DrawElementsUInt> mp_DrawElementsUInt;
};
class CDrawArrays : public CEntity
{
	public:		
		osg::ref_ptr<osg::DrawArrays> mp_draw_arrays;
		vector<CReference> mv_ref;
};
class CDrawable : public CEntity
{
	public:		
		osg::ref_ptr<osg::Drawable> mp_drawable;
		vector<CReference> mv_ref;
};
class CEntityArray : public CEntity
{
	public:			
		double angle_inc;
		double angle;
		double rad;	
		double rad_inc;
		double x;
		double y;
		double z;
		double z_inc;
		std::string m_array_method;
		vector<CReference> mv_ref;
		unsigned int m_number_of_entities;
};

class CEntityPhysXCharacterController : public CEntity
{ 
	public:		
		CEntityPhysXCharacterController();
		~CEntityPhysXCharacterController();
		osg::Vec3f m_origin;
		NxControllerManager* gManager;
		UserAllocator* ualloc;
		CPhysXCharacterController* p_character_controller;
};

class CEntityPhysXCloth : public CEntity
{ 
	public:	
		CEntityPhysXCloth();
		~CEntityPhysXCloth();
		eClothType m_cloth_type;		
		osg::Vec3f m_origin;
		NxReal m_w;
		NxReal m_h; 
		NxReal m_d;
		NxReal m_thickness;
		NxReal m_bendingStiffness;
		NxReal m_stretchingStiffness;
		NxReal m_density;
		NxReal m_friction;
		NxReal m_dampingCoefficient;
		NxU32 m_solverIterations;
		bool m_attach;
		std::string m_texture_file_path;
		std::string m_primitive_type;
		bool m_tearLines;
		std::vector<NxVec3> mv_attach_verticies;
		CPhysXCloth* p_cloth;
};

class CEntityPhysXCube : public CEntity
{ 
	public:		
		CEntityPhysXCube();
		~CEntityPhysXCube();
		osg::Vec3f m_size;		
		osg::Vec3f m_position;
		CPhysXCube m_cube;
		std::string m_file_name;
		std::string m_geometry_type;
};
class CEntityPhysXFluid : public CEntity
{ 
	public:		
		CEntityPhysXFluid();
		~CEntityPhysXFluid();
		osg::Vec3f m_origin;
		osg::Vec3f m_gravity;
		osg::Vec3f mCenter;
		osg::Vec3f mRotationAxis;
		osg::Vec4 m_color;
		osg::Vec3 m_distance_attenuation;
		float m_point_size;
		float mPlanetRadius;
		float mAtmosphereRadius;
		float mGenerationRadius;		
		float mRotationSpeed;
		float mGravityPlanet;
		float mFriction;
		float mViscosity;	
		float mKernelRadiusMultiplier;
		float mRestParticlesPerMeter;
		float mMotionLimitMultiplier;
		float mPacketSizeMultiplier;
		float mStiffness;
		float mRestDensity;
		float mDamping;
		float mParticleSize;
		CPhysXFluid * p_fluid;
};
class CEntityPhysXJoint : public CEntity
{ 
	public:		
		CEntityPhysXJoint();
		~CEntityPhysXJoint();
		osg::Vec3f m_origin;
		//CPhysXJoint * p_joint;
};
class CEntityPhysXPlane : public CEntity
{ 
	public:		
		CEntityPhysXPlane();
		~CEntityPhysXPlane();
		osg::Vec3f m_origin;
		float m_planesize;
		std::string m_filename;
		CPhysXPlane m_plane;
};
class CEntityPhysXPicker : public CEntity
{ 
	public:		
		CEntityPhysXPicker();
		~CEntityPhysXPicker();		
		CPhysXPicker * p_picker;
};
class CEntityPhysXRope : public CEntity
{ 
	public:		
		CEntityPhysXRope();
		~CEntityPhysXRope();
		osg::Vec3f m_origin;
		CPhysXRope * p_rope;
};
class CEntityPhysXSphere : public CEntity
{ 
	public:		
		CEntityPhysXSphere();
		~CEntityPhysXSphere();
		float m_radius;
		osg::Vec3f m_position;
		CPhysXSphere * p_sphere;
};
class CEntityPhysXSoftBody : public CEntity
{ 
	public:		
		CEntityPhysXSoftBody();
		~CEntityPhysXSoftBody();		
		osg::Vec3f m_origin;
		CPhysXSoftBody *p_soft_body;
};
class CEntityPhysXTerrain : public CEntity
{ 
	public:		
		CEntityPhysXTerrain();
		~CEntityPhysXTerrain();
		CPhysXTerrain * p_terrain;
		osg::Vec3f m_origin;
};
class CExplosionEffect: public CEntity
{ 
	public:	
		osg::Vec3 m_position;
		float m_scale;
		float m_intensity;
		osgParticle::ExplosionEffect* mp_explosion;
};
class CExplosionDebrisEffect: public CEntity
{ 
	public:	
		osg::Vec3 m_position;
		float m_scale;
		float m_intensity;
		osgParticle::ExplosionDebrisEffect* mp_explosion_debris;
};
class CFirstPersonManipulator : public CEntity
{
	public:		
		osg::ref_ptr<osgGA::FirstPersonManipulator> mp_FPM;		
};
class CFireEffect : public CEntity
{
	public:	
		osg::Vec3 m_position;
		float m_scale;
		float m_intensity;
		osgParticle::FireEffect* mp_fire;
};
class CGeode : public CEntity
{
	public:		
		osg::ref_ptr<osg::Geode> mp_geode;
		vector<CReference> mv_ref;
};
class CGeometry : public CEntity
{
	public:		
		osg::ref_ptr<osg::Geometry> mp_geometry;
		vector<CReference> mv_ref;
};

class CGraph : public CEntity
{
	public:
		CGraph();
		std::string m_type;
		unsigned m_width;
		unsigned m_height;
		unsigned m_scale;
		unsigned m_offset_x;
		unsigned m_offset_y;
		std::vector<double> m_data;
		std::vector<std::string> m__units;
		osg::ref_ptr<osg::PositionAttitudeTransform> mp_PAT;
		osg::ref_ptr<osg::Vec3Array> mp_Vec3Array;
		unsigned long m_vec3array_count;
		osg::ref_ptr<osg::DrawArrays> mp_draw_arrays;
		osg::ref_ptr<osg::Geode> mp_geode;
		osg::ref_ptr<osg::Geometry> mp_geometry;
		osg::ref_ptr<osg::MatrixTransform> mp_matrix_transform;
		osg::ref_ptr<osg::Projection> mp_projection;
		bool m_initialised;
		bool update(std::vector<Vec3d>* p_vec);
};

class CGroup : public CEntity
{
	public:		
		osg::ref_ptr<osg::Group> mp_group;
		vector<CReference> mv_ref;
};
class CHeightField : public CEntity
{
	public:		
	    float m_size; 
        float m_scale;
        float m_z_scale;
		unsigned m_columns;
		unsigned m_rows;
		osg::ref_ptr<osg::HeightField> mp_height_field;
};
class CImage : public CEntity
{
	public:		
		osg::ref_ptr<osg::Image> mp_image;
		vector<CReference> mv_ref;
};
class CImposter : public CEntity
{
	public:		
		osg::ref_ptr<osgSim::Impostor> mp_imposter;
		vector<CReference> mv_ref;
};
class CKeySwitchMatrixManipulator : public CEntity
{
	public:		
		osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> mp_KSMM;
};
class CLight : public CEntity
{
	public:		
		osg::ref_ptr<osg::Light> mp_light;
};
class CLightSource : public CEntity
{
	public:		
		osg::ref_ptr<osg::LightSource> mp_light_source;
		vector<CReference> mv_ref;
};  
class CLineSegment : public CEntity
{
	public:		
		osg::ref_ptr<osg::LineSegment> mp_line_segment;
		//vector<CReference> mv_ref;
};  
class CMaterial : public CEntity
{
	public:		
		osg::ref_ptr<osg::Material> mp_material;
};

class CMatrix : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		osg::Matrix m_matrix;
};

class CMatrixTransform : public CEntity
{
	public:		
		osg::ref_ptr<osg::MatrixTransform> mp_matrix_transform;
		osg::Matrix m_matrix;
		vector<CReference> mv_ref;
};


class CMemoryMapClient : public CEntity
{
	public:		
		std::string m_memory_map_name;
		int m_size;
		std::vector<char> m_mem;
};

class CMemoryMapServer : public CEntity
{
	public:		
		CMemoryMapServer();
		std::string m_memory_map_name;
		std::string m_initial_str;
		int m_type;
		int m_create_option;
		int m_mode;
		int m_size;
};

class CMultipleRandomEntities: public CEntity
{
	public:		
		std::vector<CReference> mv_entity;
		CReference m_region;	
		std::string m_method;		
		osg::Vec3d m_center;
		float m_radius;
		unsigned int m_quantity;
};

class CNxBodyDesc : public CEntity
{ 
	public:	
		CNxBodyDesc() : m_NULL(false) {}

		bool m_NULL;
		vector<CReference> mv_ref;
		NxBodyDesc m_nx_body_desc;
};

//class CNxBoxShapeDesc : public CEntity
//{ 
//	public:	
//		vector<CReference> mv_ref;
//		NxBoxShapeDesc m_nx_box_shape_desc;
//};

class CPhysX_ActorDesc : public CEntity
{ 
	public:			
		CPhysX_ActorDesc () : m_shape("Box") {}
		NxActorDesc m_nx_actor_desc;		
		NxBodyDesc m_nx_body_desc;			
		std::string m_shape;
		NxSphereShapeDesc  m_nx_sphere_shape_desc;
		NxPlaneShapeDesc m_nx_plane_shape_desc;
		NxConvexShapeDesc m_nx_convex_shape_desc;
		NxTriangleMeshShapeDesc m_nx_triangle_mesh_shape_desc;
		NxCapsuleShapeDesc m_nx_capsule_shape_desc;
		NxBoxShapeDesc m_nx_box_shape_desc;		
};

class CPhysX_Actor : public CEntity
{ 
	public:	
		CPhysX_Actor() : m_applyShaderColor(false) {}		
		NxActor* mp_nx_actor;		
		bool m_applyShaderColor;
		CPhysX_ActorDesc m_PhysX_actor_desc;
		CGeode m_geode;
		CMatrixTransform m_matrix_transform;
};

class CNxCloth : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxCloth* mp_nx_cloth;
};

class CNxClothMesh : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxClothMesh* mp_nx_cloth_mesh;
};

class CNxClothMeshDesc : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxClothMeshDesc m_nx_cloth_mesh_desc;
};

class CNxClothDesc : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxClothDesc m_nx_clot_desc;
};


class CNxCompartment : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxCompartment* mp_nx_compartment;
};

class CNxFluid : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxFluid* mp_nx_fluid;
};

class CNxFluidDesc : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxFluidDesc m_nx_fluid_desc;
};

class CNxMaterial : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxMaterial* mp_nx_material;
};
class CNxMeshData : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxMeshData m_nx_mesh_data;
};

class CNxParticleData : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxParticleData m_nx_particle_data;
};

class CNxPhysicsSDK : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxPhysicsSDK* mp_nx_physics_manager;
};
class CNxPlaneShapeDesc : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxPlaneShapeDesc m_nx_plane_shape_desc;
};

class CNxScene : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxScene* mp_nx_scene;
};

class CNxSphereShapeDesc : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxSphereShapeDesc mp_nx_sphere_shape_desc;
};

class CNxTriangleMeshShapeDesc : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxTriangleMeshShapeDesc mp_nx_triangle_mesh_shape_desc;
};

class CNxU32 : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		NxU32 m_nx_u32;
};


//class NxBounds3;
//class NxBoxShape;
//class NxBoxShapeOffCenter;
//class NxPlaneShape;
//class NxSphereShape;
//class NxCapsuleShape;
//class NxCollisionSpace;
//class NxConvexShape;
//class NxTriangleMeshShape;
//class NxHeightFieldShape;
//class NxActor;
//class NxRay;
//class NxSphere;
//class NxBox;
//class NxCapsule;
//class NxWheelShape;


class CNode : public CEntity
{ 
	public:	
		vector<CReference> mv_ref;
		osg::ref_ptr<osg::Node> mp_node;
};
class CParticle : public CEntity
{ 
	public:	
		osgParticle::Particle mp_particle;
};

class CPoint : public CEntity
{
	public:		
		osg::ref_ptr<osg::Point> mp_point;
};
class CPointSprite : public CEntity
{ 
	public:		
		osg::PointSprite * mp_point_sprite;//note this is not a reference pointer
};
class CPolygonMode : public CEntity
{
	public:		
		osg::ref_ptr<osg::PolygonMode> mp_polygon_mode;
};
class CPositionAttitudeTransform : public CEntity
{
	public:		
		CPositionAttitudeTransform() { m_slay = false; }
		osg::ref_ptr<osg::PositionAttitudeTransform> mp_PAT;
		vector<CReference> mv_ref;
};

class CMultiplePAT: public CEntity
{
	public:				
		CMultiplePAT()  { m_create_logistics_nodes = false; }
		CPositionAttitudeTransform m_PAT;	
		std::string m_type;
		std::size_t m_size;
		bool m_create_logistics_nodes;
};


class CProgram : public CEntity
{
	public:		
		osg::ref_ptr<osg::Program> mp_program;
		vector<CReference> mv_ref;
};
class CProjection : public CEntity
{
	public:		
		osg::ref_ptr<osg::Projection> mp_projection;
		vector<CReference> mv_ref;
};
//class CProducer : public CEntity
//{
//	public:
//		string m_name;
//		osg::ref_ptr<osg::CProducer> mp_producer;
//		vector<CReference> mv_ref;
//};
class CQuat : public CEntity
{
	public:	
		osg::Quat mp_quat;
		vector<CReference> mv_ref;
};

class COSGSequence : public CEntity
{
	public:		
		osg::ref_ptr<osg::Sequence> mp_sequence;
};

class CSocket : public CEntity
{ 
	public:
		CSocket() { 
			m_n_socket_flags = 0; 
			m_n_socket_address_family = 0;
			m_n_socket_type = 0;
			m_n_socket_protocol = 0;
			m_n_port = 0;
		}
	public:		
		bool m_transport_layer_security;
		bool m_secure_socket_layer;
		bool m_VRFY;
		bool m_alter_IO_mode;
		
		int m_n_socket_flags;		
		int m_n_socket_address_family;		
		int m_n_socket_type;		
		int m_n_socket_protocol;
		int m_n_port;	

		std::string m_type;	
		std::string m_str_node_name;
		std::string m_str_host_name;		
		std::string m_str_service_name;
		std::string m_str_port;//service name is the port name		
		std::string m_str_from;
		std::string m_str_to;
		std::string m_str_data;			
		std::string m_str_socket_flags;
		std::string m_str_socket_address_family;
		std::string m_str_socket_type;
		std::string m_str_socket_protocol;
		
		struct addrinfo *p_addrinfo;
		WSADATA m_wsaData;
		SOCKET m_pSocket;  
		LPHOSTENT m_lpHost;
		LPSERVENT m_lpServer;
		SOCKADDR_IN m_SockAddr;  	
		vector<CReference> mv_ref;
	
};

class CSet
{ 
	public:
		unsigned long m_cardinality;
		std::string m_expression;
		float m_incr;
};

class CShader : public CEntity
{
	public:		
		osg::ref_ptr<osg::Shader> mp_shader;
		std::string m_vert_file_name;
		std::string m_frag_file_name;
};
class CShapeDrawable : public CEntity
{
	public:		
		osg::ref_ptr<osg::ShapeDrawable> mp_shape_drawable;
		vector<CReference> mv_ref;
};
class CSmokeEffect : public CEntity
{
	public:		
		osg::Vec3 m_position;
		float m_scale;
		float m_intensity;
		osgParticle::SmokeEffect* mp_smoke;
};
class CStateSet : public CEntity
{
	public:		
		osg::ref_ptr<osg::StateSet> mp_state_set;
		vector<CReference> mv_ref;
};
class CStateAttribute : public CEntity
{
	public:		
		osg::ref_ptr<osg::StateAttribute> mp_state_attribute;
};
class CText : public CEntity
{
	public:		
		osg::ref_ptr<osgText::Text> mp_text;
};
//class CTexture : public CEntity
//{
//	public:		
//		osg::ref_ptr<osg::Texture> mp_texture;
//		vector<CReference> mv_ref;
//};
class CTexture2D : public CEntity
{
	public:		
		osg::ref_ptr<osg::Texture2D> mp_texture;
		vector<CReference> mv_ref;
};
class CUByteArray : public CEntity
{
	public:		
		osg::ref_ptr<osg::UByteArray> mp_ubyte_array;
		vector<CReference> mv_ref;
};
class CUniform : public CEntity
{
	public:		
		osg::ref_ptr<osg::Uniform> mp_uniform;
		vector<CReference> mv_ref;
		std::string m_ID;
		std::string m_type;
		float m_f;
		osg::Vec3 m_vec3;
};
class CViewer : public CEntity
{
	public:
		CViewer();		
		osg::ref_ptr<osgViewer::Viewer> mp_viewer;
		vector<CReference> mv_ref;
};
class CVec3Array : public CEntity
{
	public:		
		CMDEXPR m_md_expr;
		osg::ref_ptr<osg::Vec3Array> mp_Vec3Array;
		unsigned long m_count;
};
class CVec2Array : public CEntity
{
	public:	
		osg::ref_ptr<osg::Vec2Array> mp_Vec2Array;
		unsigned long m_count;		
};
class CVec4Array : public CEntity
{
	public:		
		osg::ref_ptr<osg::Vec4Array> mp_Vec4Array;
		unsigned long m_count;		
};
class CXGMML_attribute
{
public:
	std::string m_name;
	std::string m_value;
};
class CXGMML_att
{
	public:
	std::string m_str;
	std::vector<CXGMML_attribute> mv_attribute;
};

class CXGMML_graphics
{
	public:
	std::string m_str;
	std::vector<CXGMML_attribute> mv_attribute;
};


class CXGMML_node
{
public:

	float m_x;
	float m_y;
	float m_z;
	std::string m_str;
	std::vector<CXGMML_att>mv_att;	
	CXGMML_graphics m_graphics;
};

class CXGMML_edge
{
	public:
	unsigned m_A;
	unsigned m_B;
	std::string m_str;
	std::vector<CXGMML_att>mv_att;	
	CXGMML_graphics m_graphics;
};

//typedef boost::adjacency_list < boost::listS, boost::vecS, boost::directedS, boost::no_property, boost::property < boost::edge_weight_t, int > > graph_t;
//typedef boost::graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
//typedef boost::graph_traits < graph_t >::edge_descriptor edge_descriptor;
typedef std::pair<int, int> Edge;


 typedef float Weight; 
typedef boost::property<boost::edge_weight_t, Weight> WeightProperty; 
typedef boost::property<boost::vertex_name_t, std::string> NameProperty; 
typedef boost::property<boost::vertex_index_t, int> IndexProperty; 
 
typedef boost::adjacency_list < boost::listS, boost::listS, boost::directedS, NameProperty, WeightProperty > Graph; 
 
typedef boost::graph_traits < Graph >::vertex_descriptor Vertex; 
typedef boost::graph_traits <Graph>::vertex_iterator Viter; 
 
typedef boost::property_map < Graph, boost::vertex_index_t >::type IndexMap; 
typedef boost::property_map < Graph, boost::vertex_name_t >::type NameMap; 
 
typedef boost::iterator_property_map < Vertex*, IndexMap, Vertex, Vertex& > PredecessorMap; 
typedef boost::iterator_property_map < Weight*, IndexMap, Weight, Weight& > DistanceMap;


class CXGMML_graph
{
	public:
	CXGMML_graph();
	~CXGMML_graph();
	std::vector<CXGMML_node>mv_att;	
	std::vector<CXGMML_node>mv_node;
	std::vector<CXGMML_edge>mv_edge;	
	std::vector<Edge>mv_edge_array;
	std::vector<int>mv_weight;	
	//std::vector<graph_t>mv_graph;	
	osg::Vec3d get_vec_at_node(std::string str_node);
	void create_boost_graph(void);
	Graph m_g;
	
};


/////////////////////////////
// Compound entities
///////////////////////////


#endif