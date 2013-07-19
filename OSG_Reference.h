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

#ifndef _OSG_Reference_
#define _OSG_Reference_

#include <string>

enum EntityType {
	EntityType_Undefined,
	EntityType_AlphaFunc,
	EntityType_AnimationControlPoint,
	EntityType_AnimationPath,
	EntityType_Billboard,
	EntityType_BoundingBox,	
	EntityType_ClearNode,
	EntityType_CMatrix,
	EntityType_CameraManipulator,
	EntityType_Drawable,
	EntityType_Gaussian,
	EntityType_Logistics_demand,
	EntityType_Logistics_node,	
	EntityType_Logistics_operation,	
	EntityType_Logistics_operative,	
	EntityType_Logistics_component,
	EntityType_Logistics_producer,
	EntityType_Logistics_route,
	EntityType_Logistics_transition, 	
	EntityType_Memory_map_client, 	
	EntityType_Memory_map_server, 	
	EntityType_EntityArray, 
	EntityType_DrawElementsUInt, 
	EntityType_DrawArrays, 	
	EntityType_DeviceState,
	EntityType_ExplosionEffect,
	EntityType_ExplosionDebrisEffect,
	EntityType_EXPR,
	EntityType_EXPR_vector_field,	
	EntityType_FireEffect,
	EntityType_KeyBoardEventHandler,
	EntityType_Geode,
	EntityType_Geometry, 
	EntityType_Graph, 
	EntityType_Group,
	EntityType_HeightField,
	EntityType_Image,
	EntityType_Imposter,
	EntityType_PAT, 
	EntityType_Light,
	EntityType_LightSource,
	EntityType_LineSegment,
	//EntityType_Matrix,
	EntityType_MatrixTransform,
	EntityType_Material, 
	EntityType_MultipleRandomEntities, 
	EntityType_Node, 
	
	EntityType_PhysX_Actor, 	
	EntityType_PhysX_ActorDesc,	
	EntityType_NxBodyDesc,
	EntityType_NxBoxShapeDesc,
	EntityType_NxCloth,
	EntityType_NxClothDesc,
	EntityType_NxClothMesh,
	EntityType_NxClothMeshDesc,		
	EntityType_NxFluid,		
	EntityType_NxFluidDesc,		
	EntityType_NxMeshData,	
	EntityType_NxSphereShapeDesc,
	EntityType_NxTriangleMeshShapeDesc,
	EntityType_NxScene,
	EntityType_NxWheelShapeDesc,	
	
	EntityType_OSGSequence, 
	EntityType_Particle,
	EntityType_PolygonMode,		
	EntityType_Projection,
	EntityType_Program,
	EntityType_PhysXCharacterController,
	EntityType_PhysXCloth,
	EntityType_PhysXCube,
	EntityType_PhysXFluid,
	EntityType_PhysXPicker,	
	EntityType_PhysXPlane,
	EntityType_PhysXRope,	
	EntityType_PhysXSoftBody,
	EntityType_PhysXSphere,	
	EntityType_PhysXTerrain,
	EntityType_Sequence, 
	EntityType_Set, 
	EntityType_ShapeDrawable, 
	EntityType_Shader, 
	EntityType_SmokeEffect, 
	EntityType_Socket, 
	EntityType_StateAttribute, 
	EntityType_StateSet, 		
	EntityType_Text, 
	EntityType_Texture, 
	EntityType_Texture2D, 
	EntityType_Uniform, 
	EntityType_UByteArray, 	
	EntityType_Vec2Array, 
	EntityType_Vec3Array, 
	EntityType_Vec4Array,	
	EntityType_XGMML_graph	
	
};

enum ReferenceType { 	
	ReferenceType_Undefined,
	ReferenceType_AddChild, 
	ReferenceType_AddChildArray,	
	ReferenceType_AddDrawable, 
	ReferenceType_AddEventHandler,
	ReferenceType_AddPrimitiveSet,
	ReferenceType_AddShader,
	ReferenceType_AddShape,	
	ReferenceType_AddUniform,	
	ReferenceType_Collate, 	
	ReferenceType_CreateArray, 	
	ReferenceType_Insert, 	
	ReferenceType_Optimize,	
	ReferenceType_Root,  	
	ReferenceType_SetAttribute,
	ReferenceType_SetAttributeAndModes,
	ReferenceType_SetAddress,	
	ReferenceType_SetCount,	
	ReferenceType_SetColorArray,	
	ReferenceType_SetColorBinding,	
	ReferenceType_SetCameraManipulator,
	ReferenceType_SetDataVariance,
	ReferenceType_SetImage,		
	ReferenceType_SetLight, 
	ReferenceType_SetMatrix, 
	ReferenceType_SetMode, 
	ReferenceType_SetNormalArray,
	ReferenceType_SetNormalBinding,
	ReferenceType_SetSceneData, 
	ReferenceType_SetStateSet, 
	ReferenceType_SetStateSetMode, 
	ReferenceType_SetStateSetModesOn, 
	ReferenceType_SetStateSetModesOff,	
	ReferenceType_SetTexCoordArray,	
	ReferenceType_SetTextureAttributesAndModes,	
	ReferenceType_SetUpdateCallback,
	ReferenceType_SetVertexArray,
	ReferenceType_SetZIntersect
};

enum ReferenceCreationFlag { 
	eReferenceCreationFlag_Create, 
	eReferenceCreationFlag_AddCreate,  
	eReferenceCreationFlag_AddOverwrite };

class CReference
{
	public:
		CReference() { m_slay = false; m_reference_creation_flag=eReferenceCreationFlag_Create; }
		unsigned m_unique_id;
		EntityType m_entity_type;
		ReferenceType m_ref_type;
		std::string get_entity_type_str(void);
		std::string get_ref_type_str(void);
		std::string m_ref_name;
		std::size_t m_index;
		bool m_slay;
		ReferenceCreationFlag m_reference_creation_flag;
};


//predicate
bool slay_reference(const CReference &ref);


#endif