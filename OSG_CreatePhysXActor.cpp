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
#include "OSG_Event.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Tools.h"
#include "OSG_XML_Save_event_tree.h"

#include <string>
#include <sstream>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

///
// CEvent_DE_Logistics
///
bool CEvent_CreatePhysXActor::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{
		if(iter->m_identifier == "PhysX_actor")
		{
			m_ref_PhysX_actor.m_index = p_env->index_from_name(iter->m_str_value, p_env->mv_PhysX_actor);
		}		
		if(iter->m_identifier == "Group")
		{
			m_ref_Group.m_index = p_env->index_from_name(iter->m_str_value, p_env->mv_group);
		}		
	}
	return true;
}


bool CEvent_CreatePhysXActor::Update(CEnvironment* p_env)
{	
	
	if(p_env->mp_input_state_type!= NULL)
	{
		if(p_env->mp_input_state_type->m_emitter)
		{
			p_env->mp_input_state_type->m_emitter= false;
			if(m_ref_PhysX_actor.m_index!= UINT_MAX)
			{					
				CPhysX_Actor PhysX_actor;
				PhysX_actor.m_PhysX_actor_desc.m_nx_body_desc = p_env->mv_PhysX_actor[m_ref_PhysX_actor.m_index].m_PhysX_actor_desc.m_nx_body_desc;		
				PhysX_actor.m_PhysX_actor_desc.m_nx_box_shape_desc = p_env->mv_PhysX_actor[m_ref_PhysX_actor.m_index].m_PhysX_actor_desc.m_nx_box_shape_desc;

				///global pose and matrix transform
				osg::Matrix mat = p_env->mv_viewer[p_env->m_active_viewer_index].mp_viewer->getCamera()->getViewMatrix();
				osg::Vec3f Veye,Vcentre,Vup;
				p_env->mv_viewer[p_env->m_active_viewer_index].mp_viewer->getCamera()->getViewMatrixAsLookAt(Veye,Vcentre,Vup);
				osg::Vec3f AB= Vcentre- Veye;
				PhysX_actor.m_PhysX_actor_desc.m_nx_body_desc.linearVelocity.x = AB.x();
				PhysX_actor.m_PhysX_actor_desc.m_nx_body_desc.linearVelocity.y = AB.z();
				PhysX_actor.m_PhysX_actor_desc.m_nx_body_desc.linearVelocity.z = AB.y();

				float speed = 25;
				PhysX_actor.m_PhysX_actor_desc.m_nx_body_desc.linearVelocity*= speed;

				PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.globalPose.t.x = Veye.x()+2;
				PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.globalPose.t.y = Veye.z();
				PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.globalPose.t.z = Veye.y();
				///				

				PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.body = &(PhysX_actor.m_PhysX_actor_desc.m_nx_body_desc);							
				PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.density = 10;				
				PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.shapes.pushBack(&(PhysX_actor.m_PhysX_actor_desc.m_nx_box_shape_desc));	
				
				//create
				PhysX_actor.mp_nx_actor = p_env->mp_scene->createActor(PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc);
				if(PhysX_actor.mp_nx_actor!= NULL)
				{					
					std::size_t index = p_env->mv_PhysX_actor.size();	
					p_env->mv_PhysX_actor.push_back(PhysX_actor);

					p_env->mv_PhysX_actor[index].m_geode.mp_geode = new osg::Geode();
					
					osg::ref_ptr<osg::Material> material= new osg::Material();
					material->setDiffuse ( osg::Material::FRONT, osg::Vec4(random_float(1),random_float(1), random_float(1), 1.0f) );	     
					p_env->mv_PhysX_actor[index].m_geode.mp_geode->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );

					p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform = new osg::MatrixTransform();			
				
					if(p_env->mv_PhysX_actor[index].m_PhysX_actor_desc.m_shape == "Box")
					{						
						p_env->mv_PhysX_actor[index].m_geode.mp_geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0)
							, PhysX_actor.m_PhysX_actor_desc.m_nx_box_shape_desc.dimensions.x * 2
							, PhysX_actor.m_PhysX_actor_desc.m_nx_box_shape_desc.dimensions.y * 2
							, PhysX_actor.m_PhysX_actor_desc.m_nx_box_shape_desc.dimensions.z * 2)));	
					}
					else if(p_env->mv_PhysX_actor[index].m_PhysX_actor_desc.m_shape == "Sphere") {						
						osg::Vec3 center = osg::Vec3(0,0,0);
						float radius = PhysX_actor.m_PhysX_actor_desc.m_nx_sphere_shape_desc.radius;
						p_env->mv_PhysX_actor[index].m_geode.mp_geode->addDrawable(new ShapeDrawable(new Sphere(center, radius)));												
					}
					
					p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform->setMatrix(osg::Matrix::translate(osg::Vec3f(PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.globalPose.t.x, 
					PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.globalPose.t.y, PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.globalPose.t.z))); 

					p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform->addChild(p_env->mv_PhysX_actor[index].m_geode.mp_geode);

					PhysX_actor.m_applyShaderColor = p_env->mv_PhysX_actor[m_ref_PhysX_actor.m_index].m_applyShaderColor;	
					
					if(p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform!= NULL) {
						if (PhysX_actor.m_applyShaderColor) {
							p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform->setUpdateCallback(new MotionCallback(p_env->mv_PhysX_actor[index].mp_nx_actor, PhysX_actor.m_matrix_transform.mp_matrix_transform->getOrCreateStateSet()));	
						}
						else  {
							p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform->setUpdateCallback(new MotionCallback(p_env->mv_PhysX_actor[index].mp_nx_actor));
						}	
						if(PhysX_actor.m_geode.mp_geode!= NULL)	{
							p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform->addChild(p_env->mv_PhysX_actor[index].m_geode.mp_geode);
						}
					}		
					if(m_ref_Group.m_index!= UINT_MAX)
					{
						p_env->mv_group[m_ref_Group.m_index].mp_group->addChild(p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform);
					}						
				}
			}
		}			
	}

	return true;
}

