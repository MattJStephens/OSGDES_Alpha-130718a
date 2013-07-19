
/* Matthew Jimmy Zed Stephens */
/* STEPHENS ATOMICS */



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
bool CEventType_CreatePhysXActor::Create(CEventDesign* p_design, CEnvironment* p_env)
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


	/*CPhysX_Actor PhysX_actor;
	XML_Parse_PhysX_Actor(spXMLNextNode, &PhysX_actor);					
	PhysX_actor.mp_nx_actor = mp_scene->createActor(PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc);
	if(PhysX_actor.m_matrix_transform.mp_matrix_transform!= NULL) {
		if (PhysX_actor.m_applyShaderColor) {
			PhysX_actor.m_matrix_transform.mp_matrix_transform->setUpdateCallback(new MotionCallback(PhysX_actor.mp_nx_actor, PhysX_actor.m_matrix_transform.mp_matrix_transform->getOrCreateStateSet()));	
		}
		else  {
			PhysX_actor.m_matrix_transform.mp_matrix_transform->setUpdateCallback(new MotionCallback(PhysX_actor.mp_nx_actor));
		}	
		if(PhysX_actor.m_geode.mp_geode!= NULL)	{
			PhysX_actor.m_matrix_transform.mp_matrix_transform->addChild(PhysX_actor.m_geode.mp_geode);
		}
	}
	mv_PhysX_actor.push_back(PhysX_actor);
*/

bool CEventType_CreatePhysXActor::Update(CEnvironment* p_env)
{	
	
	if(p_env->mp_input_state_type!= NULL)
	{
		if(p_env->mp_input_state_type->m_emitter)
		{
			p_env->mp_input_state_type->m_emitter= false;
			if(m_ref_PhysX_actor.m_index!= UINT_MAX)
			{
				CPhysX_Actor PhysX_actor;
				PhysX_actor.m_PhysX_actor_desc = p_env->mv_PhysX_actor[m_ref_PhysX_actor.m_index].m_PhysX_actor_desc;
				/*if(p_env->mv_PhysX_actor[m_ref_PhysX_actor.m_index].m_PhysX_actor_desc.m_nx_body_desc.m_NULL)
				{
					PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.body = NULL;
				}
				else
				{
					PhysX_actor.m_PhysX_actor_desc.m_nx_body_desc.m_nx_body_desc = p_env->mv_PhysX_actor[m_ref_PhysX_actor.m_index].m_PhysX_actor_desc.m_nx_body_desc.m_nx_body_desc;
					PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.body = &(PhysX_actor.m_PhysX_actor_desc.m_nx_body_desc.m_nx_body_desc);
				}
*/

				PhysX_actor.m_geode.mp_geode = new osg::Geode();
				PhysX_actor.m_geode = p_env->mv_PhysX_actor[m_ref_PhysX_actor.m_index].m_geode;
				PhysX_actor.m_matrix_transform.mp_matrix_transform = new osg::MatrixTransform();				
				PhysX_actor.m_matrix_transform = p_env->mv_PhysX_actor[m_ref_PhysX_actor.m_index].m_matrix_transform;
				PhysX_actor.m_applyShaderColor = p_env->mv_PhysX_actor[m_ref_PhysX_actor.m_index].m_applyShaderColor;
				

				/*PhysX_actor.m_PhysX_actor_desc.m_nx_box_shape_desc.m_nx_box_shape_desc = p_env->mv_PhysX_actor[m_ref_PhysX_actor.m_index].m_PhysX_actor_desc.m_nx_box_shape_desc.m_nx_box_shape_desc;
				PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc.shapes.pushBack(&(p_env->mv_PhysX_actor[m_ref_PhysX_actor.m_index].m_PhysX_actor_desc.m_nx_box_shape_desc.m_nx_box_shape_desc));		*/



				PhysX_actor.mp_nx_actor = p_env->mp_scene->createActor(PhysX_actor.m_PhysX_actor_desc.m_nx_actor_desc);
				if(PhysX_actor.m_matrix_transform.mp_matrix_transform!= NULL) {
					if (PhysX_actor.m_applyShaderColor) {
						PhysX_actor.m_matrix_transform.mp_matrix_transform->setUpdateCallback(new MotionCallback(PhysX_actor.mp_nx_actor, PhysX_actor.m_matrix_transform.mp_matrix_transform->getOrCreateStateSet()));	
					}
					else  {
						PhysX_actor.m_matrix_transform.mp_matrix_transform->setUpdateCallback(new MotionCallback(PhysX_actor.mp_nx_actor));
					}	
					if(PhysX_actor.m_geode.mp_geode!= NULL)	{
						PhysX_actor.m_matrix_transform.mp_matrix_transform->addChild(PhysX_actor.m_geode.mp_geode);
					}
				}				
				std::size_t index = p_env->mv_PhysX_actor.size();	
				p_env->mv_PhysX_actor.push_back(PhysX_actor);
				if(m_ref_Group.m_index!= UINT_MAX)
				{
					p_env->mv_group[m_ref_Group.m_index].mp_group->addChild(p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform);
				}

				/*if(p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform!= NULL) {
					if (PhysX_actor.m_applyShaderColor) {
						p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform->setUpdateCallback(new MotionCallback(p_env->mv_PhysX_actor[index].mp_nx_actor, p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform->getOrCreateStateSet()));	
					}
					else  {
						p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform->setUpdateCallback(new MotionCallback(p_env->mv_PhysX_actor[index].mp_nx_actor));
					}	
					if(PhysX_actor.m_geode.mp_geode!= NULL)	{
						p_env->mv_PhysX_actor[index].m_matrix_transform.mp_matrix_transform->addChild(PhysX_actor.m_geode.mp_geode);
					}
				}			*/	

			}
		}
			
	}

	return true;
}

