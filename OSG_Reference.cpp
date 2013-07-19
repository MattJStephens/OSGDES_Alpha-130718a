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

#include "OSG_Reference.h"

///-///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///- Predicate - slay_reference
///-///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool slay_reference(const CReference &ref)
{
	if(ref.m_slay == true)
		return true;
	else return false;
}

std::string CReference::get_entity_type_str(void)
{
	std::string str;

	switch(m_entity_type)
	{
		case EntityType_Undefined: { return "Undefined"; break;	}	
		case EntityType_AlphaFunc: { return "AlphaFunc"; break;	}
		case EntityType_AnimationControlPoint: { return "AnimationControlPoint"; break;	}
		case EntityType_AnimationPath: { return "AnimationPath"; break;	}
		case EntityType_Billboard: { return "Billboard"; break;	}
		case EntityType_BoundingBox: { return "BoundingBox"; break;	}
		case EntityType_ClearNode: { return "ClearNode"; break;	}
		case EntityType_CMatrix: { return "CMatrix"; break;	}
		case EntityType_CameraManipulator: { return "CameraManipulator"; break;	}
		case EntityType_Drawable: { return "Drawable"; break;	}
		case EntityType_Gaussian: { return "Gaussian"; break;	}
		case EntityType_Logistics_demand: { return "Logistics_demand"; break;	}
		case EntityType_Logistics_node: { return "Logistics_node"; break;	}	
		case EntityType_Logistics_operation: { return "Logistics_operation"; break;	}	
		case EntityType_Logistics_operative: { return "Logistics_operative"; break;	}	
		case EntityType_Logistics_component: { return "Logistics_component"; break;	}
		case EntityType_Logistics_producer: { return "Logistics_producer"; break;	}
		case EntityType_Logistics_route: { return "Logistics_route"; break;	}
		case EntityType_Memory_map_client: { return "Memory_map_client"; break;	} 	
		case EntityType_Memory_map_server: { return "Memory_map_server"; break;	} 	
		case EntityType_EntityArray: { return "EntityArray"; break;	} 	
		case EntityType_DrawElementsUInt: { return "DrawElementsUInt"; break;	} 	
		case EntityType_DrawArrays: { return "DrawArrays"; break;	} 	
		case EntityType_DeviceState: { return "DeviceState"; break;	} 	
		case EntityType_ExplosionEffect: { return "ExplosionEffect"; break;	} 	
		case EntityType_ExplosionDebrisEffect: { return "ExplosionDebrisEffect"; break;	} 	
		case EntityType_EXPR: { return "EXPR"; break;	} 	
		case EntityType_EXPR_vector_field: { return "_EXPR_vector_field"; break;	} 	
		case EntityType_FireEffect: { return "FireEffect"; break;	} 	
		case EntityType_KeyBoardEventHandler: { return "KeyBoardEventHandler"; break;	} 	
		case EntityType_Geode: { return "Geode"; break;	} 	
		case EntityType_Geometry: { return "Geometry"; break;	} 	
		case EntityType_Group: { return "Group"; break;	} 	
		case EntityType_HeightField: { return "HeightField"; break;	} 	
		case EntityType_Image: { return "Image"; break;	} 	
		case EntityType_Imposter: { return "Imposter"; break;	} 	
		case EntityType_PAT: { return "PAT"; break;	} 	
		case EntityType_Light: { return "Light"; break;	} 	
		case EntityType_LightSource: { return "LightSource"; break;	} 	
		case EntityType_LineSegment: { return "LineSegment"; break;	} 	
		case EntityType_MatrixTransform: { return "MatrixTransform"; break;	} 	
		case EntityType_Material: { return "Material"; break;	} 	
		case EntityType_MultipleRandomEntities: { return "MultipleRandomEntities"; break;	} 	
		case EntityType_Node: { return "Node"; break;	} 	
		case EntityType_OSGSequence: { return "OSGSequence"; break;	} 	
		case EntityType_PhysX_Actor: { return "PhysX_Actor"; break;	} 	
		case EntityType_PhysX_ActorDesc: { return "PhysX_ActorDesc"; break;	} 	
		case EntityType_NxBodyDesc: { return "NxBodyDesc"; break;	} 	
		case EntityType_NxBoxShapeDesc: { return "NxBoxShapeDesc"; break;	} 	
		case EntityType_NxCloth: { return "NxCloth"; break;	} 	
		case EntityType_NxClothDesc: { return "NxClothDesc"; break;	} 	
		case EntityType_NxClothMesh: { return "NxClothMesh"; break;	} 	
		case EntityType_NxClothMeshDesc: { return "NxClothMeshDesc"; break;	} 	
		case EntityType_NxFluidDesc: { return "NxFluidDesc"; break;	} 	
		case EntityType_NxMeshData: { return "NxMeshData"; break;	} 	
		case EntityType_NxSphereShapeDesc: { return "NxSphereShapeDesc"; break;	} 	
		case EntityType_NxTriangleMeshShapeDesc: { return "NxTriangleMeshShapeDesc"; break;	} 	
		case EntityType_NxScene: { return "NxScene"; break;	} 	
		case EntityType_NxWheelShapeDesc: { return "NxWheelShapeDesc"; break;	} 	
		case EntityType_Particle: { return "Particle"; break;	} 	
		case EntityType_PolygonMode: { return "PolygonMode"; break;	} 	
		case EntityType_Projection: { return "Projection"; break;	} 	
		case EntityType_Program: { return "Program"; break;	} 	
		case EntityType_PhysXCharacterController: { return "PhysXCharacterController"; break;	} 	
		case EntityType_PhysXCloth: { return "PhysXCloth"; break;	} 	
		case EntityType_PhysXCube: { return "PhysXCube"; break;	} 	
		case EntityType_PhysXFluid: { return "PhysXFluid"; break;	} 	
		case EntityType_PhysXPicker: { return "PhysXPicker"; break;	} 	
		case EntityType_PhysXPlane: { return "PhysXPlane"; break;	} 	
		case EntityType_PhysXRope: { return "PhysXRope"; break;	} 	
		case EntityType_PhysXSoftBody: { return "PhysXSoftBody"; break;	} 
		case EntityType_PhysXSphere: { return "PhysXSphere"; break;	} 	
		case EntityType_PhysXTerrain: { return "PhysXTerrain"; break;	} 	
		case EntityType_Sequence: { return "Sequence"; break;	} 	
		case EntityType_Set: { return "Set"; break;	} 	
		case EntityType_ShapeDrawable: { return "ShapeDrawable"; break;	} 	
		case EntityType_Shader: { return "Shader"; break;	} 			
		case EntityType_SmokeEffect: { return "SmokeEffect"; break;	} 	
		case EntityType_StateAttribute: { return "StateAttribute"; break;	} 	
		case EntityType_StateSet: { return "StateSet"; break;	} 	
		case EntityType_Text: { return "Text"; break;	} 	
		case EntityType_Texture: { return "Texture"; break;	} 	
		case EntityType_Texture2D: { return "Texture2D"; break;	} 	
		case EntityType_Uniform: { return "Uniform"; break;	} 	
 		case EntityType_UByteArray: { return "UByteArray"; break;	} 	
		case EntityType_Vec3Array: { return "Vec3Array"; break;	} 	
		case EntityType_Vec4Array: { return "Vec4Array"; break;	} 	
		case EntityType_XGMML_graph: { return "XGMML_graph"; break; } 	
	}
	return "undefined";
}

std::string CReference::get_ref_type_str(void)
{
	std::string str;
	switch(m_ref_type)
	{
		case ReferenceType_Undefined: { return "Undefined"; break;	}
		case ReferenceType_AddChild: { return "AddChild"; break;	}
		case ReferenceType_AddChildArray: { return "AddChildArray"; break;	}
		case ReferenceType_AddDrawable: { return "AddDrawable"; break;	}
		case ReferenceType_AddEventHandler: { return "AddEventHandler"; break;	}
		case ReferenceType_AddPrimitiveSet: { return "AddPrimitiveSet"; break;	}
		case ReferenceType_AddShader: { return "AddShader"; break;	}
		case ReferenceType_AddShape: { return "AddShape"; break;	}
		case ReferenceType_AddUniform: { return "AddUniform"; break;	}
		case ReferenceType_Collate: { return "Collate"; break;	}
		case ReferenceType_CreateArray: { return "CreateArray"; break;	}
		case ReferenceType_Insert: { return "Insert"; break;	}
		case ReferenceType_Optimize: { return "Optimize"; break;	}
		case ReferenceType_Root: { return "Root"; break;	}
		case ReferenceType_SetAttribute: { return "SetAttribute"; break;	}
		case ReferenceType_SetAttributeAndModes: { return "SetAttributeAndModes"; break;	}
		case ReferenceType_SetAddress: { return "SetAddress"; break;	}
		case ReferenceType_SetCount: { return "SetCount"; break;	}
		case ReferenceType_SetColorArray: { return "SetColorArray"; break;	}
		case ReferenceType_SetColorBinding: { return "SetColorBinding"; break;	}
		case ReferenceType_SetCameraManipulator: { return "SetCameraManipulator"; break;	}
		case ReferenceType_SetDataVariance: { return "SetDataVariance"; break;	}
		case ReferenceType_SetImage: { return "SetImage"; break;	}
		case ReferenceType_SetLight: { return "SetLight"; break;	}
		case ReferenceType_SetMatrix: { return "SetMatrix"; break;	}		
		case ReferenceType_SetMode: { return "SetMode"; break;	}
		case ReferenceType_SetNormalArray: { return "SetNormalArray"; break;	}
		case ReferenceType_SetNormalBinding: { return "SetNormalBinding"; break;	}
		case ReferenceType_SetSceneData: { return "SetSceneData"; break;	}
		case ReferenceType_SetStateSet: { return "SetStateSet"; break;	}
		case ReferenceType_SetStateSetMode: { return "SetStateSetMode"; break;	}
		case ReferenceType_SetStateSetModesOn: { return "SetStateSetModesOn"; break;	}
		case ReferenceType_SetStateSetModesOff: { return "SetStateSetModesOff"; break;	}
		case ReferenceType_SetTexCoordArray: { return "SetTexCoordArray"; break;	}
		case ReferenceType_SetTextureAttributesAndModes: { return "SetTextureAttributesAndModes"; break;	}
		case ReferenceType_SetUpdateCallback: { return "SetUpdateCallback"; break;	}
		case ReferenceType_SetVertexArray: { return "SetVertexArray"; break;	}
		case ReferenceType_SetZIntersect: { return "SetZIntersect"; break;	}
	}
	return  "undefined";
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
//.                                      .....    ..  .$...........2011-2012       
//..             ....              ..                ..7......................... 
//                                                    .=~......................   
//                                                     ........................   
//                                                         ....................  .