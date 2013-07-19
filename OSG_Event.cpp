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
#include "OSG_XML_Save_environment_tree.h"
#include "OSG_expression.h"
#include "OSG_Default.h"
#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
using  namespace std;
using  namespace osg;

extern CEventHandler g_event_handler;
extern CScript g_innate_script;

bool create_mapped_memory(std::string mem_map_name, unsigned size)
{
	try
	{
		boost::interprocess::shared_memory_object::remove(mem_map_name.c_str()); 
		boost::interprocess::shared_memory_object shm(boost::interprocess::create_only, mem_map_name.c_str(), boost::interprocess::read_write);			
		shm.truncate(size);	
		boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);
		return true;
		
	}
	catch(...) {
		std::cout << "get_mapped_memory error..." << " " << std::endl << std::endl;
	} 
	return false;
}

bool read_char_mapped_memory(std::string mem_map_name, unsigned index, char c)
{
	try
	{
		boost::interprocess::shared_memory_object shm (boost::interprocess::open_only, mem_map_name.c_str(), boost::interprocess::read_only);
		boost::interprocess::mapped_region region(shm, boost::interprocess::read_only);
		char* mem= static_cast<char*>(region.get_address());
		if(mem[index] == c)	{ return true; }
	}
	catch(...) {
		std::cout << "get_mapped_memory error..." << " " << std::endl << std::endl;
	} 
	return false;
}

bool read_string_mapped_memory(std::string mem_map_name, unsigned index, std::string* p_str, unsigned size)
{
	try
	{
		boost::interprocess::shared_memory_object shm (boost::interprocess::open_only, mem_map_name.c_str(), boost::interprocess::read_only);
		boost::interprocess::mapped_region region(shm, boost::interprocess::read_only);
		char* mem= static_cast<char*>(region.get_address());
		std::ostringstream oss;
		for(std::size_t i = 0; i < region.get_size(); ++i) { 
			if((*mem == '\0')||(*mem == '\n')||(*mem == '\r'))
			{
				*p_str = oss.str();
				return true;
			}
			else
			{
				oss << (*mem++);
			}
		}
		*p_str = oss.str();
		return true;
	}
	catch(...) {
		std::cout << "get_mapped_memory error..." << " " << std::endl << std::endl;
	} 
	return false;

}

bool write_char_mapped_memory(std::string mem_map_name, unsigned index, char c)
{
	try
	{
		boost::interprocess::shared_memory_object shm (boost::interprocess::open_only, mem_map_name.c_str(), boost::interprocess::read_write);
		boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);
		char* mem= static_cast<char*>(region.get_address());
		mem[index] = c;
		return true;
	}
	catch(...) {
		std::cout << "write_mapped_memory error..." << " " << std::endl << std::endl;
	} 
	return false;
}

bool write_string_mapped_memory(std::string mem_map_name, unsigned index, std::string str, unsigned size)
{
	try
	{
		boost::interprocess::shared_memory_object shm (boost::interprocess::open_only, mem_map_name.c_str(), boost::interprocess::read_write);
		boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);
		std::memcpy(region.get_address(), str.c_str(), str.length());	
		return true;
	}
	catch(...) {
		std::cout << "write_mapped_memory error..." << " " << std::endl << std::endl;
	} 
	return false;
}

///
// CEventNymph
///
CEventNymph::CEventNymph()
{	

	m_local_elapsed_time= 0;	
}

///
// CEventDesign
///
CEventDesign::CEventDesign()
{	
	m_name = "default";
	m_type = EventType_Empty;
	m_position.set(0,0,0);
	m_velocity.set(0,0,0);
	m_acceleration.set(0,0,0);
	m_orientation_axis.set(0,0,1);
	m_orientation= 0;
	m_angular_velocity= 0;
	m_angular_acceleration= 0;
	m_mass= 1;
	m_inception = Inception_Spontaneous;	
	m_subordination = Subordination_None;
	m_occurance= Occurance_discrete;
	m_recurrance = Recurrance_perpetual;
	m_concurrance= Concurrancy_polling;	
}

///
// CEventHandler
///
CEventHandler::CEventHandler()
{
	m_save_event_tree= true;
}

bool CEventHandler::InnateCreate(CEnvironment* p_env)
{			
	g_innate_script.mv_str_event_file_name.push_back("event_innate.xml");
	std::vector<std::string>::iterator vec_str_iter;
	for(vec_str_iter = g_innate_script.mv_str_event_file_name.begin(); vec_str_iter != g_innate_script.mv_str_event_file_name.end(); ++vec_str_iter) {
		if(!g_event_handler.XML_EventParser(*vec_str_iter, p_env, &(this->m_event)))	{ cout <<  "failed to parse events"; return 0; }
	}		
	return true;
}

bool CEventHandler::InnateUpdate(CEnvironment* p_env)
{	
	return m_innate_events.Update(p_env);
}
bool CEventHandler::Update(CEnvironment* p_env)
{
	m_save_event_tree = false;
	if(m_save_event_tree) {
		m_save_event_tree= false;
		std::ostringstream oss;
		oss << "test.xml";
		XML_Save_event_tree(oss.str(), &m_event);		
	}

	return m_event.Update(p_env);
}

///
// CEventCrysalis
///
CEventCrysalis::CEventCrysalis()
{
	m_active_child_index= 0;
	m_local_elapsed_time = 0;
	m_slay = false;
	m_local_elapsed_time= 0;
	m_debug_info= false;
}


bool CEventCrysalis::Update(CEnvironment* p_env)
{
	if(mp_event_nymph!= NULL) {

		if(m_debug_info) { cout  <<  "Event: " << mp_event_nymph->m_name << " - Update \n"; }

		mv_xml_file_name.clear();
		mv_self_destruct_index.clear();		
		m_local_elapsed_time += p_env->get_delta_time();	
		switch(mp_event_nymph->m_inception)	{			
			case Inception_Spontaneous: {
				if(mp_event_nymph->Update(p_env)) {
					if(mp_event_nymph->m_recurrance == Recurrance_singular) { m_slay= true; };
				}
				break;
			}	
			case Inception_Periodic:
			{
				if(m_local_elapsed_time> m_period)	{
					UpdateChild(p_env, mv_child.begin()+ m_active_child_index, m_active_child_index);									
					m_active_child_index++;
					m_local_elapsed_time= 0;						
					if(m_active_child_index>= mv_child.size())	{
						m_active_child_index= 0;
					}						
				}
				if(mp_event_nymph->m_inception_condition.evaluate()) { 
					if(mp_event_nymph->Update(p_env)) {
						if(mp_event_nymph->m_recurrance == Recurrance_singular) { m_slay= true; };
					}
				}
				break;
			}	
			case Inception_Conditional:
			{
				if(mp_event_nymph->m_inception_condition.evaluate()) { 
					if(mp_event_nymph->Update(p_env)) {
						if(mp_event_nymph->m_recurrance == Recurrance_singular) { m_slay= true; };
					}
				}
				break;
			}	
			case Inception_MemMap_Conditional: {			
				if(mp_event_nymph->m_mem_map_index< p_env->mv_memory_map_server.size())	{
					if(read_char_mapped_memory(p_env->mv_memory_map_server[mp_event_nymph->m_mem_map_index].m_memory_map_name, mp_event_nymph->m_mem_map_address,  mp_event_nymph->m_mem_test))	{						
						mp_event_nymph->Update(p_env);		
						write_char_mapped_memory(p_env->mv_memory_map_server[mp_event_nymph->m_mem_map_index].m_memory_map_name, mp_event_nymph->m_mem_map_address, '0');					
						if(mp_event_nymph->m_recurrance == Recurrance_singular) { m_slay= true; };
					}					
				}
				break;
			}	
		}		
		switch(mp_event_nymph->m_subordination)
		{
			case Subordination_Simultaneous:
			{
				UpdateAllChildren(p_env);
				break;
			}			
			case Subordination_PeriodicSimultaneous:
			{							
				if(m_local_elapsed_time> m_period)	{
					m_local_elapsed_time= 0;				
					UpdateAllChildren(p_env);
				}						
				break;
			}
			case Subordination_Sequential:
			{			
				if(mv_child.size()> 0)
				{							
					UpdateChild(p_env, mv_child.begin()+ m_active_child_index, m_active_child_index);				
					m_active_child_index++;					
					if(m_active_child_index>= mv_child.size())
					{
						m_active_child_index= 0;
					}					
				}
				break;
			}
			case Subordination_PeriodicSequential:
			{				
				if(mv_child.size()> 0)
				{				
					if(m_local_elapsed_time> m_period)
					{
						UpdateChild(p_env, mv_child.begin()+ m_active_child_index, m_active_child_index);									
						m_active_child_index++;
						m_local_elapsed_time= 0;						
						if(m_active_child_index>= mv_child.size())
						{
							m_active_child_index= 0;
						}						
					}
				}
				break;
			}			
		}	

		std::vector<string>::iterator str_iter;
		for(str_iter = mv_xml_file_name.begin(); str_iter< mv_xml_file_name.end(); ++str_iter)
		{
			if(m_debug_info) { cout  <<  "Event: " << mp_event_nymph->m_name << " - Add Child: " << *str_iter << "\n"; }

			CEventCrysalis m_event;		
			g_event_handler.XML_EventParser(*str_iter, p_env, &m_event);
			this->mv_child.push_back(m_event);		
		}

		slay_marked_events();
		return true;
	}
	
	return false;
}


bool CEventCrysalis::UpdateAllChildren(CEnvironment* p_env)
{
	std::size_t index= 0;
	for(m_event_crysalis_iter= mv_child.begin(); m_event_crysalis_iter< mv_child.end(); ++m_event_crysalis_iter)
	{
		UpdateChild(p_env, m_event_crysalis_iter, index);
		index++;
	}
	return true;
}

bool CEventCrysalis::UpdateChild(CEnvironment* p_env, vector<CEventCrysalis>::iterator event_crysalis_iter, std::size_t index)
{	
	if(m_debug_info) { cout  <<  "Event: " << mp_event_nymph->m_name << " - UpdateChild \n"; }
	event_crysalis_iter->Update(p_env);
	if(event_crysalis_iter->m_slay)
	{
		if(m_debug_info) { cout  <<  "Event: " << event_crysalis_iter->mp_event_nymph->m_name << " - Response \n"; }

		std::vector<string>::iterator str_iter;
		for(str_iter = event_crysalis_iter->mp_event_nymph->mv_response_xml.begin(); 
			str_iter<event_crysalis_iter->mp_event_nymph->mv_response_xml.end(); ++str_iter)
		{
			mv_xml_file_name.push_back(*str_iter);								
		}
	}	

	return true;
}

void CEventCrysalis::slay_marked_events(void)
{
	std::vector<CEventCrysalis>::iterator OP_iter;
	OP_iter = std::remove_if(mv_child.begin(), mv_child.end(), slay_event_crysalis);
	mv_child.erase(OP_iter, mv_child.end());
}

bool CEventCrysalis::LinkAssocVariables(CEventDesign* p_design, CEnvironment* p_env)
{
	//cache
	std::vector<CVarAssoc>::iterator iter;
	for(iter = p_design->mv_assoc_var.begin(); iter != p_design->mv_assoc_var.end(); ++iter)
	{
		mp_event_nymph->mv_assoc_var.push_back(*iter);
	}	
	
	for(iter = mp_event_nymph->mv_assoc_var.begin(); iter != mp_event_nymph->mv_assoc_var.end(); ++iter)
	{
		switch(iter->m_assoc)
		{
			case VariableAssoc_PAT_vec:
			{
				iter->m_assoc_index = p_env->PAT_index_from_name(iter->m_ref_assoc);
				//iter->m_assoc_index = p_env->PAT_index_from_name(iter->m_ref_assoc,  p_env->mv_PAT);
				if(iter->m_assoc_index== UINT_MAX)
				{
					cout <<  "cannot link variable to PAT";	
					return false;
				}			
				break;
			}
		}
		iter->m_expr_index =UINT_MAX;
		long index= 0;
		std::vector<CExpVar>::iterator exp_iter;
		for(exp_iter = mp_event_nymph->m_expression_variables.mv_variable.begin(); exp_iter != mp_event_nymph->m_expression_variables.mv_variable.end(); ++exp_iter)
		{
			if(iter->m_ref_expr == exp_iter->m_name)
			{
				iter->m_expr_index = index;
			}
			index++;

		}
		if(iter->m_expr_index== UINT_MAX)
		{
			cout <<  "cannot link variable to PAT";	
			return false;
		}			
	}
	return true;
}
bool CEventCrysalis::Morph(CEventDesign* p_design, CEnvironment* p_env)
{	
	if(m_debug_info) { cout  << "Morph: "<< event_type_to_string(p_design->m_type) << ", event: "  << p_design->m_name << "\n"; }

	std::vector<CParsedVariable>::iterator iter;
	m_crysalis_name = p_design->m_name;
	m_period = p_design->m_period;
	mp_event_nymph = NULL;
	switch(p_design->m_type)
	{				
		case EventType_AlterOrientation:
		{
			mp_event_nymph = new CEvent_AlterOrientation();
			break;
		}	
		case EventType_AlterPosition:
		{
			mp_event_nymph = new CEvent_AlterPosition();			
			break;
		}
		case EventType_AlterText:
		{
			mp_event_nymph = new CEvent_AlterText();
			break;
		}	
		case EventType_CreatePhysXActor:
		{
			mp_event_nymph = new CEvent_CreatePhysXActor();
			break;
		}		
		case EventType_CreateEntity:
		{
			mp_event_nymph = new CEvent_CreateEntity();
			break;
		}				
		case EventType_CreateProcess:
		{
			mp_event_nymph = new CEvent_CreateProcess();
			break;
		}			
		case EventType_CreateWave:
		{
			mp_event_nymph = new CEvent_CreateWave();
			break;
		}		
		case EventType_Create3ds:
		{
			mp_event_nymph = new CEvent_Create3ds();
			break;
		}		
		case EventType_DE_Logistics:
		{
			mp_event_nymph = new CEvent_DE_Logistics();
			break;
		}	
		case EventType_DiscreteEventSimulation:
		{
			mp_event_nymph = new CEvent_DiscreteEventSimulation();
			break;
		}			
		case EventType_Empty:
		{
			mp_event_nymph = new CEvent_Empty();			
			break;
		}
		case EventType_Emitter:
		{
			mp_event_nymph = new CEvent_Emitter();			
			break;
		}		
		case EventType_GetEntityDetails:
		{
			mp_event_nymph = new CEvent_GetEntityDetails();			
			break;
		}			
		case EventType_Innate:
		{
			mp_event_nymph = new CEvent_Innate();			
			break;
		}		
		
		case EventType_LoadScript:
		{
			mp_event_nymph = new CEvent_LoadScript();			
			break;
		}	
		case EventType_Math:
		{
			mp_event_nymph = new CEvent_Math();			
			break;
		}			
		case EventType_MoveGlobal:
		{
			mp_event_nymph = new CEvent_MoveGlobal();			
			break;
		}
		case EventType_ReplaceDrawable:
		{
			mp_event_nymph = new CEvent_ReplaceDrawable();			
			break;
		}		
		case EventType_Ruby:
		{
			mp_event_nymph = new CEvent_Ruby();			
			break;
		}	
		case EventType_Python:
		{
			mp_event_nymph = new CEvent_Python();			
			break;
		}	
		case EventType_PlayWav:
		{
			mp_event_nymph = new CEvent_PlayWav();			
			break;
		}			
		case EventType_Render:
		{
			mp_event_nymph = new CEvent_Render();			
			break;
		}			
		case EventType_SaveEventTree:
		{
			mp_event_nymph = new CEvent_SaveEventTree();
			break;
		}	
		case EventType_SaveEnvironmentTree:
		{
			mp_event_nymph = new CEvent_SaveEnvironmentTree();
			break;
		}			
		case EventType_ScreenGrab:
		{
			mp_event_nymph = new CEvent_ScreenGrab();
			break;
		}	
		case EventType_Sequencer:
		{
			mp_event_nymph = new CEvent_Sequencer();			
			break;
		}	
		case EventType_Sensor:
		{
			mp_event_nymph = new CEvent_Sensor();			
			break;
		}	
		case EventType_SMTP:
		{
			mp_event_nymph = new CEvent_SMTP();			
			break;
		}				
		case EventType_Socket:
		{
			mp_event_nymph = new CEvent_Socket();			
			break;
		}			
				
		case EventType_TheatreOfOperations:
		{
			mp_event_nymph = new CEvent_UpdateTheatreOfOperations();			
			break;
		}			
		case EventType_UpdateGraph:
		{
			mp_event_nymph = new CEvent_UpdateGraph();
			break;
		}	
		case EventType_UpdateNxActor:
		{
			mp_event_nymph = new CEvent_UpdatePhysXActor();
			break;
		}	
		case EventType_UpdatePositionParametricExpression:
		{
			mp_event_nymph = new CEvent_UpdatePositionParametricExpression();
			break;
		}		
		case EventType_UpdateText:
		{
			mp_event_nymph = new CEvent_UpdateText();
			break;
		}				
		case EventType_UpdateSimulationTime:
		{
			mp_event_nymph = new CEvent_UpdateSimulationTime();
			break;
		}				
		case EventType_UpdateVariable:
		{
			mp_event_nymph = new CEvent_UpdateVariable();
			break;
		}				
		case EventType_UpdateVelocity:
		{
			mp_event_nymph = new CEvent_UpdateVelocity();
			break;
		}			
		case EventType_UpdateViewer:
		{
			mp_event_nymph = new CEvent_UpdateViewer();
			break;
		}	
		case EventType_UpdateViewerAnimate:
		{
			mp_event_nymph = new CEvent_UpdateViewerAnimate();
			break;
		}
	}
	if(mp_event_nymph!= NULL)//variables common to all events passed on from design to nymph;
	{
		mp_event_nymph->Create(p_design, p_env);
		mp_event_nymph->m_name = p_design->m_name;
		mp_event_nymph->m_type = p_design->m_type;
		mp_event_nymph->m_inception = p_design->m_inception;
		mp_event_nymph->m_occurance = p_design->m_occurance;
		mp_event_nymph->m_recurrance = p_design->m_recurrance;
		mp_event_nymph->m_concurrance = p_design->m_concurrance;
		mp_event_nymph->m_subordination = p_design->m_subordination;	
		//std::vector<eConsequence> mv_consequence;
		
//		mp_event_nymph->m_self_destruct = p_design->m_self_destruct_first_pass;		
		mp_event_nymph->m_inception_condition =  p_design->m_inception_condition;
		mp_event_nymph->m_climax_condition =  p_design->m_climax_condition;

		for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {				
			if(iter->m_identifier == "mem_map_address") { std::istringstream iss(iter->m_str_value); iss >> mp_event_nymph->m_mem_map_address;	} 
			else if(iter->m_identifier == "mem_test") {  std::istringstream iss(iter->m_str_value); iss >> mp_event_nymph->m_mem_test;	} 		
			else if(iter->m_identifier == "mem_map_str") { 
				mp_event_nymph->m_mem_map_str = iter->m_str_value;	
				mp_event_nymph->m_mem_map_index = p_env->index_from_name(iter->m_str_value, p_env->mv_memory_map_server);
			}		
		}		

		std::vector<string>::iterator str_iter;
		for(str_iter = p_design->mv_response_xml.begin(); str_iter < p_design->mv_response_xml.end(); ++str_iter) {
			mp_event_nymph->mv_response_xml.push_back(*str_iter);
		}

		for(str_iter = p_design->mv_str_climax_varibles.begin(); str_iter < p_design->mv_str_climax_varibles.end(); ++str_iter)	{
			CExpVar expr_varx;
			expr_varx.m_name= *str_iter;
			mp_event_nymph->m_expression_variables.mv_variable.push_back(expr_varx);
		}

		mp_event_nymph->m_associateyVy =false;
		for(str_iter = p_design->mv_varible_association.begin(); str_iter < p_design->mv_varible_association.end(); ++str_iter)	{		
			if(*str_iter == "y=Ry")
				mp_event_nymph->m_associateyVy =true;		
		}
	
		/*if(p_design->m_str_climax_condition.size()> 0) {
			mp_event_nymph->m_test_climax = true;
			mp_event_nymph->m_climax_expression_index = mp_event_nymph->m_climax_condition_expression_list.analyze(p_design->m_str_climax_condition, &(mp_event_nymph->m_expression_variables));
		}*/
		
		LinkAssocVariables(p_design, p_env);
		return true;
	}
	cout <<  p_design->m_name.c_str() << " mp_event_nymph == NULL\n";	
	//m_slay = true;
	return false;
}

bool slay_event_crysalis(const CEventCrysalis &ec)
{
	if(ec.m_slay)
		return true;
	else return false;
}


///
// event_type_to_string
///
string event_type_to_string(eEventType eT)
{
	switch(eT)
	{	
		case EventType_AlterPosition:
		{
			return "EventType_AlterPosition";
			break;
		}	
		case EventType_AlterOrientation:
		{
			return "EventType_AlterOrientation";
			break;
		}			
		case EventType_AlterText:
		{
			return "EventType_AlterText";
			break;
		}	
		case EventType_DE_Logistics:
		{
			return "DE_Logistics";
			break;
		}	
		case EventType_CreatePhysXActor:
		{
			return "CreatePhysXActor";
			break;
		}			
		case EventType_CreateEntity:
		{
			return "CreateEntity";
			break;
		}	
		case EventType_CreateProcess:
		{
			return "CreateProcess";
			break;
		}
		case EventType_CreateWave:
		{
			return "CreateWave";
			break;
		}		
		case EventType_Create3ds:
		{
			return "Create3ds";
			break;
		}		
		case EventType_DiscreteEventSimulation:
		{
			return "DiscreteEventSimulation";
			break;
		}			
		case EventType_Empty:
		{
			return "EventType_Empty";
			break;
		}		
		case EventType_Emitter:
		{
			return "EventType_Emitter";
			break;
		}		
		case EventType_GetEntityDetails:
		{
			return "EventType_GetEntityDetails";
			break;
		}			
		case EventType_Innate:
		{
			return "EventType_Innate";
			break;
		}		
		case EventType_LoadScript:
		{
			return "EventType_LoadScript";
			break;
		}			
		case EventType_Math:
		{
			return "EventType_Math";
			break;
		}			
		case EventType_MoveGlobal:
		{
			return "EventType_MoveGlobal";
			break;
		}	

		case EventType_ReplaceDrawable:
		{
			return "EventType_ReplaceDrawable";
			break;
		}					
		case EventType_SaveEventTree:
		{
			return "EventType_SaveEventTree";
			break;
		}	
		case EventType_Python:
		{
			return "EventType_Python";
			break;
		}	
		case EventType_PlayWav:
		{
			return "EventType_PlayWav";
			break;
		}			
		case EventType_Ruby:
		{
			return "EventType_Ruby";
			break;
		}	

		case EventType_Render:
		{
			return "EventType_Render";
			break;
		}			
		case EventType_SaveEnvironmentTree:
		{
			return "EventType_SaveEnvironmentTree";
			break;
		}	
		case EventType_Sequencer:
		{
			return "EventType_Sequencer";
			break;
		}	
		case EventType_Sensor:
		{
			return "EventType_Sensor";
			break;
		}	
		case EventType_SMTP:
		{
			return "EventType_SMTP";
			break;
		}	
		case EventType_Socket:
		{
			return "EventType_Socket";
			break;
		}
	
		case EventType_ScreenGrab:
		{
			return "EventType_ScreenGrab";
			break;
		}			
		case EventType_TheatreOfOperations:
		{
			return "EventType_TheatreOfOperations";
			break;
		}			
		case EventType_UpdateGraph:
		{
			return "EventType_UpdateGraph";
			break;
		}			
		case EventType_UpdateNxActor:
		{
			return "EventType_UpdateNxActor";
			break;
		}			
		case EventType_UpdatePositionParametricExpression:
		{
			return "EventType_UpdatePositionParametricExpression";
			break;
		}
		case EventType_UpdateSimulationTime:
		{
			return "EventType_UpdateSimulationTime";
			break;
		}
		case EventType_UpdateText:
		{
			return "EventType_UpdateText";
			break;
		}
		case EventType_UpdateVariable:
		{
			return "EventType_UpdateVariable";
			break;
		}		
		case EventType_UpdateVelocity:
		{
			return "EventType_UpdateVelocity";
			break;
		}	
		case EventType_UpdateViewer:
		{
			return "EventType_UpdateViewer";
			break;
		}		
		case EventType_UpdateViewerAnimate:
		{
			return "EventType_UpdateViewerAnimate";
			break;
		}	
	}
	return "undefined";
}
/*
             .................     .. MMMMMM...  .......................... .. .  . ... .  ......... . . .. .     .. .  ....                
            MMMMMMMMMMMMMMMMMMM   ,MMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMMMMMMM  MMMM.   .MMMM.  MMMMMMMMMMM  MMMMM   MMMM  .MMMMM.               
           MMMM. ..MMMM ..MMMM:  MMMMM. ..MMMMM .  ,MMMMM      .MMMMM.    MMMMM    MMMMM.  MMMN.        MMMM.  MMMMM. .MMMM                 
          MMMMM  .MMMM....MMMM, .MMMM,.. .MMMM.    NMMMM      . MMMM.    ,NMMM,...,NMMM.. MMMMM...     MMMMM   MMMM ..MMMMM.                
        . MMMMM. MMMMM  .MMMM   MMMMMMMMMMMMMM    .MMMMM      .MMMMM   . MMMMMMMMMMMMMM  .MMMMMMMMM ...MMMM, .MMMMM ..MMMM.                 
         .MMMM . MMMMN  MMMMM  .MMMM.....NMMM..  .MMMMM.      .MMMM.    .MMMM.....MMMN.  MMMMM. ..   .NMMMM. .MMMM.. MMMMM                  
        .MMMMM  MMMMM.. MMMM.  MMMMM.   MMMMM .   MMMM       ,MMMMM.   .MMMMM.   MMMMM...MMMM.      .,MMMM.. MMMMM  .MMMM                   
      . MMMMM,  MMMMM..MMMMM  .MMMM   . MMMM     MMMMM..     .MMMM.     MMMMM    MMMM  .MMMMMMMMMMM. MMMMMMMMMMMMMMMMMMM                    
      . ...., .. ...   ... .. ,....     ...,     ......       ...        ... .    ... . ..........  .  ................         
                                                                                                                                                                                                                                                      
               MMMMN  ,.MMMMMMMMMMM   .MMMMMMMMMMMMMMMMMM.  .MMMMMMMMMMM..,MMMMMMMMM                                                        
             ,MMMMM.. MMMMM   MMMMM   MMMMM. MMMMM  :MMMM.  .MMMM,     ..MMMMM,                                                             
              MMMMM  MMMMM . .MMMMM   MMMM . MMMMM. .MMMM.  MMMMM      .MMMMM...                                                            
             .MMMM. .MMMMN   .MMMM..,MMMMM  .MMMM . MMMMM. .MMMM      ..MMMMMM   .                                                          
            .MMMM,..MMMMMMMMMNMMMM...MMMM  .MMMMM  .MMMM.. MMMMMMMMMM    .  .MMMMM                                                          
           .MMMMM...MMMM     MMMM . MMMMM.  MMMM.  MMMMM  .MMMM.       ..  ...MMMM                                                          
        .  MMMMM   MMMMM.  ,MMMMM  .MMMM. ,NMMMM. MMMMN,  MMMMM             .MMMMM                                                          
      .MMMMMMM   . MMMM.    MMMM,. MMMMM  ,MMMM ..MMMMN .MMMMMMMMMMM..MMMMMMMMMM .                                                          
                                                                                                 
	               ..................................  ...........    ..... .    ................ ............. .   .. .......  .                 
           ,MMMMMMMMMM..MMMMMMMMMMM  MMMMMMMMMM . MMMMMMMMMMMM  MMMMM,   MMMMM   MMMMMMMMMMM..MMMMMMMMMMMMM   NMMMMMMMMM                    
          MMMMN..       ..,MMMM..  .MMMMM        MMMMM. .MMMMM .MMMMM   .MMMMM  .NMMM       . MMMM.. .MMMMM ,MMMMM                          
        ..MMMMM. .        MMMMM    .MMMM .     . MMMM  .MMMMM .,MMMM.   ,MMMM   NMMMM.    .. MMMMM    MMMM.. MMMM..    .                    
        ..MMMMMMMMMM      MMMM.    MMMMMMMMMM.. MMMMMMMMMMMM . MMMMMMMMMMMMMM...MMMMMMMMMM.  MMMM.   MMMMM. .MMMMMMMMMM                     
           ..  MMMMM .  .NMMMM   . MMMM.....   .MMMMMMMMM.    .MMMM ...NMMMM . MMMMM......  MMMMM   .MMMM..       MMMMM.                    
              .MMMMN.   NMMMM .   MMMMM        MMMMM        . MMMM.   .MMMMN   MMMM.       .MMMM.  .MMMMM..     . NMMMN                     
             MMMMMM..   MMMMM .  .MMMM .       MMMM         ..MMMM.    MMMM.  MMMMM.       MMMMM....MMMMM.      MMMMMM.                     
      . MMMMMMM   .    .MMMM.    MMMMMMMMMMM. NMMMM.        .MMMMM.  .MMMMM.  MMMMMMMMMM . MMMM.   MMMMM. MMMMMMMM,. .                      
                                                                                                                              
*/
