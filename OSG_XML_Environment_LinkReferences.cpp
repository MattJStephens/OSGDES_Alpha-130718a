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

#include "OSG_DES.h"
#include "OSG_Environment.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Geometry_tools.h"
#include <limits>
#include <osgUtil/IntersectVisitor>

using namespace osg;
using namespace std;

///-///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///-CEnvironment
///-///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEnvironment::LinkReferences(void)
{
	//CAnimationPath
	std::vector<CAnimationPath>::iterator animation_path_iter;
	for(animation_path_iter = mv_animation_path.begin(); animation_path_iter< mv_animation_path.end(); ++animation_path_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = animation_path_iter->mv_ref.begin(); ref_iter< animation_path_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_AnimationControlPoint:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_animation_control_point);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_Insert:
							{								
								animation_path_iter->mp_animation_path->insert(mv_animation_control_point[ref_iter->m_index].m_time, mv_animation_control_point[ref_iter->m_index].m_control_point);
								break;
							}
						}
					}
					break;
				}					
			}
		}
	}	

	//CBillboard
	std::vector<CBillboard>::iterator billboard_iter;
	for(billboard_iter = mv_billboard.begin(); billboard_iter< mv_billboard.end(); ++billboard_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = billboard_iter->mv_ref.begin(); ref_iter< billboard_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_Geometry:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_geometry);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddDrawable:
							{
								//osg::Drawable * drawable = mv_geometry[ref_iter->m_index].mp_geometry;
								//billboard_iter->mp_billboard->addDrawable(drawable, billboard_iter->m_position);
								billboard_iter->mp_billboard->addDrawable(mv_geometry[ref_iter->m_index].mp_geometry, billboard_iter->m_position);
								break;
							}
						}
					}
					break;
				}		
				case EntityType_Node:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_node);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetZIntersect:
							{
								osg::LineSegment* line_segment = new osg::LineSegment();
								line_segment->set(osg::Vec3(billboard_iter->m_position.x(), billboard_iter->m_position.y(), 99999) ,
									osg::Vec3(billboard_iter->m_position.x(), billboard_iter->m_position.y(),-99999) );								  
								osgUtil::IntersectVisitor intersect_visitor;
								intersect_visitor.addLineSegment(line_segment);
								mv_node[ref_iter->m_index].mp_node->accept(intersect_visitor);
								osgUtil::IntersectVisitor::HitList IV_hit_list;
								IV_hit_list = intersect_visitor.getHitList(line_segment);
								if ( !IV_hit_list.empty() )	{								 
									osg::Vec3 intersection_pt = IV_hit_list.front().getWorldIntersectPoint();
									billboard_iter->m_position = IV_hit_list.front().getWorldIntersectPoint();
								}							
								break;
							}
						}
					}
					break;
				}			
			}
		}
	}	

	//DrawArrays
	std::vector<CDrawArrays>::iterator DrawArraysIter;
	for(DrawArraysIter = mv_draw_arrays.begin(); DrawArraysIter < mv_draw_arrays.end(); ++DrawArraysIter)
	{
		std::vector<CReference>::iterator iter_ref;	
		for(iter_ref = DrawArraysIter->mv_ref.begin(); iter_ref < DrawArraysIter->mv_ref.end(); ++iter_ref)
		{
			update_reference(&(*iter_ref), mv_Vec3Array);
			DrawArraysIter->mp_draw_arrays->setCount(mv_Vec3Array[iter_ref->m_index].m_count);
		}
	}

	//EXPR
	std::vector<CEXPR>::iterator EXPRIter;
	for(EXPRIter = mv_EXPR.begin(); EXPRIter < mv_EXPR.end(); ++EXPRIter) {	
		std::vector<CEXPR_Variable>::iterator var_iter;
		for(var_iter = EXPRIter->m_variable_list.mv_variable.begin(); var_iter !=  EXPRIter->m_variable_list.mv_variable.end(); ++var_iter)	{
			std::vector<CReference>::iterator ref_iter;
			for(ref_iter = var_iter->mv_ref.begin(); ref_iter !=  var_iter->mv_ref.end(); ++ref_iter) {
				switch(ref_iter->m_entity_type)	{
					case EntityType_Logistics_node:	{
						ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_logistics_node);						
						break;
					}	
				}
			}
		}
	}


	//CGeode
	std::vector<CGeode>::iterator geode_iter;
	for(geode_iter = mv_geode.begin(); geode_iter< mv_geode.end(); ++geode_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = geode_iter->mv_ref.begin(); ref_iter< geode_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_Geometry:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_geometry);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddDrawable:
							{
								geode_iter->mp_geode->addDrawable(mv_geometry[ref_iter->m_index].mp_geometry);
								break;
							}
						}
					}
					break;
				}	
				case EntityType_HeightField:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_height_field);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddDrawable:
							{
								geode_iter->mp_geode->addDrawable(new osg::ShapeDrawable(mv_height_field[ref_iter->m_index].mp_height_field));
								break;
							}
						}
					}
					break;
				}		
				case EntityType_ShapeDrawable:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_shape_drawable);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddDrawable:
							{
								geode_iter->mp_geode->addDrawable(mv_shape_drawable[ref_iter->m_index].mp_shape_drawable);
								break;
							}
						}
					}
					break;
				}				
				case EntityType_Text:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_text);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddDrawable:
							{
								geode_iter->mp_geode->addDrawable(mv_text[ref_iter->m_index].mp_text);
								break;
							}
						}
					}
					break;
				}				
				case EntityType_Material:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_material);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetAttribute:
							{
								geode_iter->mp_geode->getOrCreateStateSet()->setAttribute(mv_material[ref_iter->m_index].mp_material);
								break;
							}
						}
					}
					break;
				}
				case EntityType_StateSet:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_stateset);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetStateSet:
							{
								geode_iter->mp_geode->setStateSet(mv_stateset[ref_iter->m_index].mp_state_set);
								break;
							}
						}
					}
					break;
				}
			}
		}
	}	
	//CGeometry
	std::string debug;
	std::vector<CGeometry>::iterator geometry_iter;
	for(geometry_iter = mv_geometry.begin(); geometry_iter< mv_geometry.end(); ++geometry_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = geometry_iter->mv_ref.begin(); ref_iter< geometry_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_DrawArrays:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_draw_arrays);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddPrimitiveSet:
							{
								geometry_iter->mp_geometry->addPrimitiveSet(mv_draw_arrays[ref_iter->m_index].mp_draw_arrays);
								break;
							}
						}
					}
					break;
				}	
				case EntityType_StateSet:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_stateset);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetStateSet:
							{
								geometry_iter->mp_geometry->setStateSet(mv_stateset[ref_iter->m_index].mp_state_set);
								break;
							}
						}
					}
					break;
				}
				case EntityType_Vec3Array:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_Vec3Array);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetVertexArray:
							{
								geometry_iter->mp_geometry->setVertexArray(mv_Vec3Array[ref_iter->m_index].mp_Vec3Array);
								break;
							}
						}
					}
					break;
				}		
				case EntityType_Vec4Array:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_Vec4Array);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetColorArray:
							{
								geometry_iter->mp_geometry->setColorArray(mv_Vec4Array[ref_iter->m_index].mp_Vec4Array);
								geometry_iter->mp_geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

								break;
							}
						}
					}
					break;
				}	
				case EntityType_Vec2Array:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_Vec2Array);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetTexCoordArray:
							{
								geometry_iter->mp_geometry->setTexCoordArray(0, mv_Vec2Array[ref_iter->m_index].mp_Vec2Array);
								break;
							}
						}
					}
					break;
				}	

				
			}
		}
	}	
	

	//Graph
	std::vector<CGraph>::iterator GraphIter;
	for(GraphIter = mv_graph.begin(); GraphIter < mv_graph.end(); ++GraphIter)
	{
		//
	}


	///
	//CGroup
	///
	std::vector<CGroup>::iterator group_iter;
	for(group_iter = mv_group.begin(); group_iter< mv_group.end(); ++group_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = group_iter->mv_ref.begin(); ref_iter< group_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				/*case  EntityType_EntityArray:
				{
					
					
					break;
				}*/
				case EntityType_Billboard:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_billboard);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_billboard[ref_iter->m_index].mp_billboard);
								break;
							}
						}
					}
					break;
				}

				case EntityType_ClearNode:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_clear_node);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_clear_node[ref_iter->m_index].mp_clear_node);
								break;
							}
						}
					}
					break;
				}
				
				case EntityType_ExplosionEffect:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_explosion_effect);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_explosion_effect[ref_iter->m_index].mp_explosion);
								break;
							}
						}
					}
					break;
				}
				case EntityType_ExplosionDebrisEffect:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_explosion_debris_effect);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_explosion_debris_effect[ref_iter->m_index].mp_explosion_debris);
								break;
							}
						}
					}
					break;
				}
				case EntityType_FireEffect:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_fire_effect);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_fire_effect[ref_iter->m_index].mp_fire);
								break;
							}
						}
					}
					break;
				}
				case EntityType_Graph:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_graph);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								//if graph HUD else..
								//group_iter->mp_group->addChild(mv_graph[ref_iter->m_index].mp_PAT);
								group_iter->mp_group->addChild(mv_graph[ref_iter->m_index].mp_projection);
								break;
							}
						}
					}
					break;
				}

				case EntityType_Geode:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_geode);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_geode[ref_iter->m_index].mp_geode);
								break;
							}
						}
					}
					break;
				}	

				case EntityType_Group:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_group);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_group[ref_iter->m_index].mp_group);
								break;
							}
						}
					}
					break;
				}								
				
				case EntityType_Imposter:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_imposter);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_imposter[ref_iter->m_index].mp_imposter);
								break;
							}
						}
					}
					break;
				}	
					
				case EntityType_LightSource:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_light_source);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_light_source[ref_iter->m_index].mp_light_source);
								break;
							}
						}
					}
					break;
				}	

				case EntityType_MatrixTransform:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_matrix_transform);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_matrix_transform[ref_iter->m_index].mp_matrix_transform);
								break;
							}
						}
					}
					break;
				}
				case EntityType_MultipleRandomEntities:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_multiple_random_entities);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								std::vector<CReference>::iterator MRE_ref_iter;
								for(MRE_ref_iter= mv_multiple_random_entities[ref_iter->m_index].mv_entity.begin(); MRE_ref_iter!= mv_multiple_random_entities[ref_iter->m_index].mv_entity.end(); ++MRE_ref_iter)
								{									
									group_iter->mp_group->addChild(mv_billboard[MRE_ref_iter->m_index].mp_billboard);
								}
								break;
							}
						}
					}
					break;
				}

				case EntityType_Node:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_node);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_node[ref_iter->m_index].mp_node);
								break;
							}
						}
					}
					break;
				}

				case EntityType_PAT:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_PAT);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								//debug = ref_iter->m_ref_name;
								group_iter->mp_group->addChild(mv_PAT[ref_iter->m_index].mp_PAT);
								break;
							}
						}
					}
					break;
				}						
				case EntityType_PhysX_Actor:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_PhysX_actor);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_PhysX_actor[ref_iter->m_index].m_matrix_transform.mp_matrix_transform);
								break;
							}
						}
					}
					break;
				}

				case EntityType_PhysXCloth:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_physX_cloth);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_physX_cloth[ref_iter->m_index].p_cloth);
								break;
							}
						}
					}
					break;
				}
				case EntityType_PhysXCube:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_physX_cube);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_physX_cube[ref_iter->m_index].m_cube.mp_matrix_transform);
								break;
							}
						}
					}
					break;
				}				
				case EntityType_PhysXFluid:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_physX_fluid);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_physX_fluid[ref_iter->m_index].p_fluid);
								break;
							}
						}
					}
					break;
				}
				case EntityType_PhysXPlane:
				{								
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_physX_plane);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_physX_plane[ref_iter->m_index].m_plane.mp_matrix_transform);
								break;
							}
						}
					}
					break;
				}
				case EntityType_PhysXSphere:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_physX_sphere);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_physX_sphere[ref_iter->m_index].p_sphere);
								break;
							}
						}
					}
					break;
				}
				case EntityType_PhysXTerrain:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_physX_terrain);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_physX_terrain[ref_iter->m_index].p_terrain);
								break;
							}
						}
					}
					break;
				}
				case EntityType_PhysXSoftBody:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_physX_soft_body);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_physX_soft_body[ref_iter->m_index].p_soft_body);
								break;
							}
						}
					}
					break;
				}
				case EntityType_Projection:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_projection);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_projection[ref_iter->m_index].mp_projection);
								break;
							}
						}
					}
					break;
				}	
				case EntityType_SmokeEffect:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_smoke_effect);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								group_iter->mp_group->addChild(mv_smoke_effect[ref_iter->m_index].mp_smoke);
								break;
							}
						}
					}
					break;
				}
			}
		}
	}

	//CImposter
	std::vector<CImposter>::iterator ImposterIter;
	for(ImposterIter = mv_imposter.begin(); ImposterIter < mv_imposter.end(); ++ImposterIter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = ImposterIter->mv_ref.begin(); ref_iter< ImposterIter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_Group:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_group);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								ImposterIter->mp_imposter->addChild(mv_group[ref_iter->m_index].mp_group);
								break;
							}
						}
					}
					break;
				}		
				case EntityType_Node:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_node);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								ImposterIter->mp_imposter->addChild(mv_node[ref_iter->m_index].mp_node);
								break;
							}
						}
					}
					break;
				}			
			}
		}
	}


	//CLogistics_component
	std::vector<CLogistics_component>::iterator Logistics_componentIter;
	for(Logistics_componentIter = mv_Logistics_component.begin(); Logistics_componentIter < mv_Logistics_component.end();++Logistics_componentIter)
	{
		switch(Logistics_componentIter->m_component_type)
		{
			case ComponentType_Geode:
			{
				update_reference(&(Logistics_componentIter->m_ref_component_geometry), mv_geode);
				break;
			}
			case ComponentType_Node:
			{
				update_reference(&(Logistics_componentIter->m_ref_component_geometry), mv_node);
				break;
			}
		}
	}

	//CLogistics_node
	std::vector<CLogistics_node>::iterator Logistics_nodeIter;
	for(Logistics_nodeIter = mv_logistics_node.begin(); Logistics_nodeIter < mv_logistics_node.end();++Logistics_nodeIter)
	{
		update_reference(&(Logistics_nodeIter->m_ref_PAT), mv_PAT);
		
		switch(Logistics_nodeIter->m_Logistics_node_type)
		{
			case Logistics_nodeType_Geode:
			{
				update_reference(&(Logistics_nodeIter->m_ref_geo), mv_geode);
				break;
			}
			case Logistics_nodeType_Node:
			{
				update_reference(&(Logistics_nodeIter->m_ref_geo), mv_node);
				break;
			}
		}
	}

	//CLogistics_producer
	std::vector<CLogistics_producer>::iterator Logistics_producerIter;
	for(Logistics_producerIter = mv_Logistics_producer.begin(); Logistics_producerIter < mv_Logistics_producer.end(); ++Logistics_producerIter)
	{
		update_reference(&(Logistics_producerIter->m_ref_node), mv_logistics_node);
		update_reference(&(Logistics_producerIter->m_ref_component), mv_Logistics_component);
	}

	//CLogistics_demand
	std::vector<CLogistics_demand>::iterator Logistics_demandIter;
	for(Logistics_demandIter = mv_logistics_demand.begin(); Logistics_demandIter < mv_logistics_demand.end(); ++Logistics_demandIter)
	{	
		update_reference(&(Logistics_demandIter->m_ref_node), mv_logistics_node);
		update_reference(&(Logistics_demandIter->m_ref_component), mv_Logistics_component);
		std::vector<CReference>::iterator iter_ref;	
		for(iter_ref = Logistics_demandIter->mv_ref_source_node.begin(); iter_ref < Logistics_demandIter->mv_ref_source_node.end(); ++iter_ref)
		{
			update_reference(&(*iter_ref), mv_logistics_node);
		}

		std::vector<CEXPR>::iterator EXPR_iter;
		for (EXPR_iter =  mv_EXPR.begin(); EXPR_iter<  mv_EXPR.end(); ++EXPR_iter) {
			if(EXPR_iter->m_ID == Logistics_demandIter->m_ref_EXPR.m_ref_name) {
				Logistics_demandIter->m_ref_EXPR.m_index = std::distance(mv_EXPR.begin(), EXPR_iter);
			}			
		}	
	}	

	//CMatrixTransform
	std::vector<CMatrixTransform>::iterator matrix_transform_iter;
	for(matrix_transform_iter = mv_matrix_transform.begin(); matrix_transform_iter< mv_matrix_transform.end(); ++matrix_transform_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = matrix_transform_iter->mv_ref.begin(); ref_iter< matrix_transform_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_Geode:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_geode);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								matrix_transform_iter->mp_matrix_transform->addChild(mv_geode[ref_iter->m_index].mp_geode);
								break;
							}
						}
					}
					break;
				}	
				/*case EntityType_Geometry:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_geometry);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								matrix_transform_iter->mp_matrix_transform->addChild(mv_geometry[ref_iter->m_index].mp_geometry->g);
								break;
							}
						}
					}
					break;
				}	*/
				case EntityType_CMatrix:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_matrix);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetMatrix:
							{
								matrix_transform_iter->mp_matrix_transform->setMatrix(mv_matrix[ref_iter->m_index].m_matrix);
								break;
							}
						}
					}
					break;
				}		
				case EntityType_PhysX_Actor:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_PhysX_actor);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetUpdateCallback:
							{
								matrix_transform_iter->mp_matrix_transform->setUpdateCallback(new MotionCallback(mv_PhysX_actor[ref_iter->m_index].mp_nx_actor));
								break;
							}
						}
					}
					break;
				}			
			}
		}
	}	
	

	//CPAT
	std::vector<CPositionAttitudeTransform>::iterator PAT_iter;
	for(PAT_iter = mv_PAT.begin(); PAT_iter< mv_PAT.end(); ++PAT_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = PAT_iter->mv_ref.begin(); ref_iter< PAT_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{

				case EntityType_AnimationPath:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_animation_path);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetUpdateCallback:
							{
								PAT_iter->mp_PAT->setUpdateCallback(new osg::AnimationPathCallback(mv_animation_path[ref_iter->m_index].mp_animation_path));
								//PAT_iter->mp_PAT->setUpdateCallback(new osg::AnimationPathCallback(mv_animation_path[ref_iter->m_index].mp_animation_path,0.0,1.0));
								break;
							}
						}
					}
					break;
				}			
				case EntityType_Node:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_node);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								debug = ref_iter->m_ref_name;
								PAT_iter->mp_PAT->addChild(mv_node[ref_iter->m_index].mp_node);
								break;
							}
						}
					}
					break;
				}			
				case EntityType_PAT:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_PAT);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								PAT_iter->mp_PAT->addChild(mv_PAT[ref_iter->m_index].mp_PAT);
								break;
							}
						}
					}
					break;
				}			
				case EntityType_Geode:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_geode);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								PAT_iter->mp_PAT->addChild(mv_geode[ref_iter->m_index].mp_geode);
								break;
							}
						}
					}
					break;
				}		
				
				case EntityType_Group:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_group);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								mv_group[ref_iter->m_index].mp_group->addChild(PAT_iter->mp_PAT);
								break;
							}
						}
					}
					break;
				}		

				case EntityType_LightSource:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_light_source);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								PAT_iter->mp_PAT->addChild(mv_light_source[ref_iter->m_index].mp_light_source);
								break;
							}
						}
					}
					break;
				}				
			}
		}
	}
	//CProjection
	std::vector<CProjection>::iterator projection_iter;
	for(projection_iter = mv_projection.begin(); projection_iter< mv_projection.end(); ++projection_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = projection_iter->mv_ref.begin(); ref_iter< projection_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_MatrixTransform:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_matrix_transform);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddChild:
							{
								projection_iter->mp_projection->addChild(mv_matrix_transform[ref_iter->m_index].mp_matrix_transform);
								break;
							}
						}
					}
					break;
				}			
			}
		}
	}	
	//CProgram
	std::vector<CProgram>::iterator program_iter;
	for(program_iter = mv_program.begin(); program_iter< mv_program.end(); ++program_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = program_iter->mv_ref.begin(); ref_iter< program_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_Shader:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_shader);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddShader:
							{
								program_iter->mp_program->addShader(mv_shader[ref_iter->m_index].mp_shader);
								break;
							}
						}
					}
					break;
				}			
			}
		}
	}	

	//CLightSource
	std::vector<CLightSource>::iterator light_source_iter;
	for(light_source_iter = mv_light_source.begin(); light_source_iter< mv_light_source.end(); ++light_source_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = light_source_iter->mv_ref.begin(); ref_iter< light_source_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_Light:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_light);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetLight:
							{
								light_source_iter->mp_light_source->setLight(mv_light[ref_iter->m_index].mp_light); 
								break;
							}
						}
					}
					break;
				}
				case EntityType_Group:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_group);
					if(ref_iter->m_index!= UINT_MAX)
					{
						StateSet *m_lightStateSet = mv_group[ref_iter->m_index].mp_group->getOrCreateStateSet();
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetStateSetModesOn:
							{
								light_source_iter->mp_light_source->setStateSetModes(*m_lightStateSet, StateAttribute::ON); 	
								break;
							}
							case ReferenceType_SetStateSetModesOff:
							{
								light_source_iter->mp_light_source->setStateSetModes(*m_lightStateSet, StateAttribute::OFF);
								break;
							}						
						}
					}
					break;
				}			
			}
		}
	}	
	//CNode
	std::vector<CNode>::iterator node_iter;
	for(node_iter = mv_node.begin(); node_iter< mv_node.end(); ++node_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = node_iter->mv_ref.begin(); ref_iter< node_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_Material:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_material);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetAttribute:
							{
								node_iter->mp_node->getOrCreateStateSet()->setAttribute(mv_material[ref_iter->m_index].mp_material, osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
								break;
							}
						}
					}
					break;
				}
			}
		}
	}	
	
	//CTexture2D
	std::vector<CTexture2D>::iterator texture_iter;
	for(texture_iter = mv_texture2D.begin(); texture_iter< mv_texture2D.end(); ++texture_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = texture_iter->mv_ref.begin(); ref_iter< texture_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_Image:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_image);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetImage:
							{
								texture_iter->mp_texture->setImage(mv_image[ref_iter->m_index].mp_image);
								break;
							}
						}
					}
					break;
				}			
			}
		}
	}	
	//CStateSet 
	std::vector<CStateSet>::iterator stateset_iter;
	for(stateset_iter = mv_stateset.begin(); stateset_iter< mv_stateset.end(); ++stateset_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = stateset_iter->mv_ref.begin(); ref_iter< stateset_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_AlphaFunc:
				{			
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_alphafunc);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetAttributeAndModes:
							{							
								stateset_iter->mp_state_set->setAttributeAndModes(mv_alphafunc[ref_iter->m_index].mp_alphafunc, osg::StateAttribute::ON);
								break;
							}
						}
					}
					break;
				}		
				case EntityType_Uniform:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_uniform);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_AddUniform:
							{
								stateset_iter->mp_state_set->addUniform(mv_uniform[ref_iter->m_index].mp_uniform);
								break;
							}
						}
					}
					break;
				}
				case EntityType_Geode:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_geode);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetStateSet://could just be set state set
							{
								mv_geode[ref_iter->m_index].mp_geode->setStateSet(&(*stateset_iter->mp_state_set));
								break;
							}
						}
					}
					break;
				}	
				case EntityType_Geometry:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_geometry);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetStateSet://could just be set state set
							{
								mv_geometry[ref_iter->m_index].mp_geometry->setStateSet(&(*stateset_iter->mp_state_set));
								break;
							}
						}
					}
					break;
				}	


				case EntityType_PolygonMode:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_polygon_mode);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetAttribute:
							{
								stateset_iter->mp_state_set->setAttribute(mv_polygon_mode[ref_iter->m_index].mp_polygon_mode);
								break;
							}
						}
					}
					break;
				}
				case EntityType_Program:
				{
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_program);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetAttribute:
							{
								stateset_iter->mp_state_set->setAttribute(mv_program[ref_iter->m_index].mp_program.get());
								break;
							}
						}
					}
					break;
				}
				case EntityType_Node:
				{				
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_node);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{
							case ReferenceType_SetStateSet://could just be set state set
							{
								mv_node[ref_iter->m_index].mp_node->setStateSet(&(*stateset_iter->mp_state_set));
								break;
							}
						}
					}
					break;
				}	
				case EntityType_Texture2D:
				{			
					ref_iter->m_index = index_from_name(ref_iter->m_ref_name, mv_texture2D);
					if(ref_iter->m_index!= UINT_MAX)
					{
						switch(ref_iter->m_ref_type)
						{							
							case ReferenceType_SetTextureAttributesAndModes:
							{
								std::size_t texture_size = mv_texture2D.size();
								stateset_iter->mp_state_set->setTextureAttributeAndModes(0, mv_texture2D[ref_iter->m_index].mp_texture, osg::StateAttribute::ON);
								break;
							}
						}
					}
					break;
				}		
						
			}
		}
	}	
	//CViewer
	std::vector<CViewer>::iterator viewer_iter;
	for(viewer_iter = mv_viewer.begin(); viewer_iter< mv_viewer.end(); ++viewer_iter)
	{
		std::vector<CReference>::iterator ref_iter;
		for(ref_iter = viewer_iter->mv_ref.begin(); ref_iter< viewer_iter->mv_ref.end(); ++ref_iter)
		{
			switch(ref_iter->m_entity_type)
			{
				case EntityType_KeyBoardEventHandler:
				{					
					switch(ref_iter->m_ref_type)
					{
						case ReferenceType_AddEventHandler:
						{
							viewer_iter->mp_viewer->addEventHandler(mp_keyboard_event_handler);
							break;
						}
					}				
					break;
				}					
			}
		}
	}	
}







/*
                                                                                                                                            
                                                                                                                                            
                                                                                                                                            
                                                                                                                                            
                                                                                                                     . .  .                 
                                                                                                                     .Z87..                 
                                                                                                             ...:NNND?7?+7:,.               
                                                                                                    .Z77$?$D?$I+=~?~++===~~~,..             
                                                                                                ..,NNDN$7D?8MNNM+=~~~~::,,,,,..             
                                                                         ......   .  ........:..N+OI+?DN8NINNNN++~,,.....                   
                                                                         .77I?.     I+??I$MZ,7O+8Z+I+III7DIMNN?+~,.....                     
                                                                     .,??II?$:~8.7N7=+:I8:ZII?$$77=~+?8Z7Z$INM?=:..                         
                                                           ..~..7+,..7+,~I++?+=7OI=7ZNZ+=~~$ZONOII=~=+?$I7$$7IZZZOZOOOZ87                   
                                                           .++$$I8II?7NM88=NNM?++$M7$I8Z$Z7$DNDO$I?7$?MNNNNNNNNNO7I7Z$Z?++~,.               
                              ..                          ,.Z?.:N77=~=7D77,Z$$D$...$?ZZNDZ7MMM$O7OMNNNNNMND8OZZ$$OI.?I7=?+Z=:..             
                     ........=$7...                    ..~+88ODI?OID=:~IIZ~Z?NNZ...$OOOMO78$8MNNMNNNOOZZ$III?????????7$N8M7+~,.  .  .       
                     .~7ZZ????+I=$,.                   ...?+~=??ZOZ~77?7I7O??7$OZO?$ONONI~NMNDDO8ZZ$I?+====++???I7$DI$DMZ++=:,. .?$7 .      
           ...:$$Z7?7?+I+?MMDMDD$?=~,.                 ...ZD$OZMDNI8=MDM7?$87$8$$N7NI=IN$=$ZMM78?===~~~=~=+II7Z8D77OND+?+~:,7INIO==?~,..    
        ..7ZIII?7?=8MNMNNDD88887I?+=:.               . .DOO$+$+IIOM+MZ$I+=,OZ$8I7IO8I?+IN$$77:.7~~~=?=?I7ZOMN=7$?DD+++=~8=D~7+?$=:?+=:..    
       ..I?7?I7MMMMDO$7I?$$$$$$7+I?+:...        ....$?=7IININ7NM$?.877~+?=MN=$:IO?MI7IIZ.=NN=?+DZZ7$ODMI$$ZZ7INDN+D7=7$+$++$~+IOOI=+=,.     
       7?I?$7IIDMM8$IIIII$$$77=MNN~???.. . .. ...I$$~MO77Z~=88$OD78MM++ZI8O~ZI=I?INI~=IIO=$I$$ZZNZ$OOD888O...8:M??+Z$?~=Z??OON?NZ~?=:...    
       I7?+???I?MMO$II??+$77$NI=~~7$:$7N.:I$$787=?IOZ=7~=7++Z7MN?=MMO+NM$OI$N=7+III+$77ZZ7?DN$O8DDMNMD~=O?...$==$+??7Z8?NNINO7ZD??=:,. .    
     ..II$+I$II+MMZI??+??ZZ$$?7NNM7$ZII7III?7?7=$.I:=.IIDDM+$?ZZ?Z$MM$7D=OMO,M$MI+Z?I7Z87OOZ$OO8~?7N7=7ZZ?7=Z?7Z8ZOZ$NOMZO7OM$++=~:,..      
     ,$I?ZZ7ZMMD+ODZZ$$$Z888DII7ZDI+8++:?+=7~=:ZD~INM8?M?8+?=IMD?~?MZ~+N,Z8I87NO$Z77ZZ$IOONI$7$O8=7Z77ID7IZDZDNMM8DI7DDZ$I+++=~:,,...       
     .+III+7MMMMMMMNMO:MNMDMO$M??M7?=$?:I=~+,DM?IO?MI$IMOMN~?=~=NOZD77IM$I$=7=~I7:7?DZ=77ZNI++++ZDNI?Z$OM$DNN78I77$MO??++==~:,,....         
     ..$OI7OZZZOO88DNMMMMMMM87MM~7~==Z$==8~MM$~DIM?M?M?I?7M7D?M$+$D~NMZZ7=Z:I=M?~8$OM$ZI+I8ZO8O$OZ?NM+D88DDZ$ZMDDI++=~~:,,,,...             
     ..=I8$$$7$$$$$$$$$ZOO8MMMMM7IZ7$IID87I=+?I??M+:+M+7NNI?7D?8D$ZO7Z=OZ$?++?I7??....I$$.?MO?$?I78$?8??I++$++?7Z?+~:,,........             
     ..7+7?ZDOOOZZO$$7777I$I$Z8N$O77$7~7$,ZN7I??I.7.+IIIO8$OD~ZOII8$I+IIIMD8ZI:8O: ,..N~II77$?$I?=I8I$?+~~:::::O??=:,...                    
     .7ZII$IIIIIIIII?III77?I78$D+M$.7IZ7,.I77III8,Z,N$+D?~Z+ZN?M~++8M?ZI7Z7O8DIMZ7II=I$8OI,NIN+7==$+++=~,,.....I=++:,..                     
      ..Z77++++++++++7+?IOIZ~=Z$O+=,~ZMN?=I$$M8Z:+I=~Z$O?I==:IM8Z7?8ZOMOONNO87+$II?7DMO$?7?O??I=+?+=~::,,.   ..?=+=:,..                     
       .IIZZ===++?+++I?+$?$IM~.~7M$+ZI8IO7Z$,+=O+7IZ=ZNO7?I7$II7$MMZ?N?877O7,D8:~+M$O,$$+++=+=+=I?+~,..... ...$I++=:..                      
       .=77$==~~=+??+I8D.?OM=I$8O8Z~I7ZZO8=ZZ77II?N=O$$D8M8+8M8$DM878O7O7ZO+8ZOD+O?$Z$?$=:O=,.=++I=:..     .:I?=+$I?,..                     
       ,77$$==~~~::~~$+=+M7ZID+$IMOO7$N877ODZNI?7I8?7?7ZDOIZM++7$~N87+=7I,MI?$IO8OI7I+?I=+8Z=++I?++:..     ..,~~++++~:.                     
       .,I$OO~~~~~~::~+77OO$MMMMDM~8MM7IO$=N$M$D=~77II8?+++?===,=??$OMZ877NI?N++==~====~=7~+77?~77+:..      ...,::::,,..                    
        .,77Z~~::::~=~=IO?$Z8Z7?7M:?O8M~+7+D7?7Z+=77$I?+~:::,,,,:~~D7$7?$+?+=:=~,:::::,,,,:~I7=O??=~,. .       ......                       
        ..~IIN:~=~~~~:~+II?=IZN7D?MDO7$O$+ID7,7?M?OD$+7+~,........,,:~:~~~:=:,.,............,:~++=~~,...                                    
         .:8OO:~=~:~=????=+I7ZIONMN$7777$=ZN+$+=ONMN8$$+:,.        ...,:,.,,:,.              ..,:,,,..                                      
         ..:I$Z==~+=+=+=+??ZIZ8OMMNM$$7I~7Z+7+=:78NI77$=:..         ...........              .  .....                                       
         ...~78O~======+IZZ7$NMMMMMNZMZIZD?D+=:,IZO++I7+:,               .                                                                  
           ..O77D==~=+IIO?78?MMMMM8OZO$DOZ=+=~,.+$Z??II+:,                                                                                  
           ..$$$$M++?I$?I$NMMMMNO7O$OOOZZZZI~,..I?$III?+:,.                                                                                 
             .:7OOZ=7777OMMMM8Z$7.$.Z$$77I?I=:..7?7?I?I+:.                                                                                  
             ..:$O$8$$$8~MMMO$77I...777III++?=,.$I$II+I+:.                                                                                  
             ...,$$ZZ$OOMMMDOZ$$7IIII??7??++II:.+?7$=I?=:,.                                                                                 
               ..OII777ZDMMDOZ$77I???+++??++I7=:..+++++=:.                                                                                  
                 ,~7Z777$MMNOZ$7I?+?++=~~=I?I77=,.,:~:,,,..                                                                                 
                 .,.Z$$$$MMNZ$$7I????++=~~~=7I$I~,.......                                                                                   
                   .8$7$$7MMOZ$7I++?++==~~~~~~ZO=:..                                                                                        
                   ..~O77ZMMOZ$7I++?++==~~~~~~+O+~..                                                                                        
                     .,OZ$DMZZ$7I++?+++=~~~:=?+++~,.                                                                                        
                      ..D$$MZZ77I+??+=++=??++=~~:,,.                                                                                        
                       ..+$NZ$$II+II???++=~::,,....                                                                                         
                        .,ZZZZ7III??++=~:,,....                                                                                             
                        ...,Z7+?++=~:,,,........                                                                                            
                           .:==~::,,.... .                                                                                                  
                          . ..,,.....                                                                                                       
                            ....                                                                                                            
                                                                                                                                            
                                                                                                                                            
 
*/