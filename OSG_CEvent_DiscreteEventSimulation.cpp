/*                                                                                               
                                                                                                     
                                                    
  */




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
bool CEvent_DiscreteEventSimulation::Create(CEventDesign* p_design, CEnvironment* p_env)
{	

	//p_env->set_simulation_time_increment(p_design->m_elapsed_simulation_time_increment);
	return true;
}


bool slay_DE_state_change_list(const CDE_StateChangeList &SCL)
{
	if(SCL.m_slay == true)
		return true;
	else return false;
}

void slay_marked_DE_state_change_list(CEnvironment* p_env)
{
	std::vector<CDE_StateChangeList>::iterator SCL_iter;
	SCL_iter = std::remove_if(p_env->mv_state_change_tree.begin(), p_env->mv_state_change_tree.end(), slay_DE_state_change_list);
	p_env->mv_state_change_tree.erase(SCL_iter, p_env->mv_state_change_tree.end());
}

bool CEvent_DiscreteEventSimulation::Update(CEnvironment* p_env)
{		
	//p_env->update_simulation_time();
	CTimeFootprint foot_print;
	foot_print.m_n_ID= 101;
	foot_print.m_str_ID= "CEvent_DiscreteEventSimulation update";		
	if(p_env->mv_state_change_tree.size()> 0)
	{
		double delta_t= p_env->mv_state_change_tree[0].m_time- p_env->get_elapsed_time(TimeType_simulation_time, &foot_print);	
		p_env->update_simulation_time_beta(delta_t);
		p_env->mv_state_change_tree[0].m_slay = true;
	}	

	slay_marked_DE_state_change_list(p_env);

	return true;
}

