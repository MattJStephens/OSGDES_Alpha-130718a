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
// CEvent_AlterText
///
bool CEvent_AlterText::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	m_subject_index = UINT_MAX;
	m_text_index = p_env->index_from_name(p_design->m_ref_name, p_env->mv_text);
	if(p_design->m_alter_string == "ElapsedTime")	{
		m_alter_string = AlterString_elapsed_time;
	}
	else if(p_design->m_alter_string == "ElapsedSimulationTime")	{
		m_alter_string = AlterString_elapsed_simulation_time;
	}
	else if(p_design->m_alter_string == "NodeThroughput")	{
		m_alter_string = AlterString_node_throughput;
	}	
	else if(p_design->m_alter_string == "NodeMass")	{
		m_alter_string = AlterString_node_mass;
	}	
	else if(p_design->m_alter_string == "TimeMode")	{
		m_alter_string = AlterString_TimeMode;
	}	
	else if(p_design->m_alter_string == "Socket")	{
		m_alter_string = AlterString_Socket;
		if(p_design->mv_ref.size()> 0)		{
			m_subject_index =  p_env->index_from_name(p_design->mv_ref[0].m_ref_name, p_env->mv_socket);
		}
	}	
	else if(p_design->m_alter_string == "Camera")	{
		m_alter_string = AlterString_Camera;	
		if(p_design->mv_ref.size()> 0)
		{
			if( p_env->mv_viewer.size()> 0)
			{
				m_subject_index = 0 ;
			}			
		}
	}	
	else if(p_design->m_alter_string == "PAT")	{
		m_alter_string = AlterString_PAT;
		if(p_design->mv_ref.size()> 0)		{
			m_subject_index = p_env->PAT_index_from_name(p_design->mv_ref[0].m_ref_name);
		}
	}
	

	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{		
		if(iter->m_identifier == "Logistics_node")
		{
			m_logistics_node_index = p_env->index_from_name(iter->m_str_value, p_env->mv_logistics_node);
		}		
		if(iter->m_identifier == "Operation")
		{
			std::istringstream iss(iter->m_str_value);
			int n;
			iss >>n;
			m_operation_index = n;
		}	
	}

	
	return true;
}
bool CEvent_AlterText::Update(CEnvironment* p_env)
{	
	switch (m_alter_string)	{
		case AlterString_elapsed_time:	{			
			osg::ref_ptr<osgText::Text> text = p_env->GetText(m_text_index);
			ostringstream oss;		
			oss << "Real Time: " << p_env->get_elapsed_real_time();
			if(text!= NULL)	text->setText(oss.str());
			break;
		}
		case AlterString_elapsed_simulation_time:	{
			osg::ref_ptr<osgText::Text> text = p_env->GetText(m_text_index);
			ostringstream oss;		
			oss << "Simulation Time: " << p_env->get_elapsed_simulation_time();
			if(text!= NULL)	text->setText(oss.str());			
			break;
		}
		case AlterString_TimeMode:	{
			osg::ref_ptr<osgText::Text> text = p_env->GetText(m_text_index);
			ostringstream oss;		

			if(p_env->m_active_time_type == TimeType_simulation)
			{
				oss << "DES mode";
			}
			else
			{
				oss << "Continuous mode, ";
				oss << p_env->m_active_continuous_operatives;
			}
			if(text!= NULL)	text->setText(oss.str());			
			break;
		}				
		case AlterString_Socket:	{
			if(m_subject_index!= UINT_MAX)	{
				osg::ref_ptr<osgText::Text> text = p_env->GetText(m_text_index);
				ostringstream oss;		
				oss << p_env->mv_socket[m_subject_index].m_str_data;
				if(text!= NULL)	text->setText(oss.str());
			}
			break;
		}
		case AlterString_PAT:	{
			if(m_subject_index!= UINT_MAX)	{
				osg::ref_ptr<osgText::Text> text = p_env->GetText(m_text_index);
				ostringstream oss;				
				Vec3d vec;
				CPositionAttitudeTransform *p_PAT = p_env->get_PAT(m_subject_index);
				if(p_PAT!= NULL) 
					vec = p_PAT->mp_PAT->getPosition();
				else
				{
					bool bad=true;
				}
				oss << "PAT x,y,z: " << vec.x() << "," << vec.y()  << "," << vec.z();
				if(text!= NULL)	text->setText(oss.str());
			}
			break;
		}
		case AlterString_node_throughput:	{
			if(m_logistics_node_index!= UINT_MAX)	{
				osg::ref_ptr<osgText::Text> text = p_env->GetText(m_text_index);
				ostringstream oss;								
				CLogistics_node * p_ln;
				if(m_logistics_node_index<   p_env->mv_logistics_node.size()) {			
					p_ln = &(p_env->mv_logistics_node[m_logistics_node_index]);
				}
				std::vector<CLogistics_throughput>::iterator i_vec;
				float mass= 0;
				float time= 0;
				for(i_vec = p_ln->mv_throughput.begin(); i_vec < p_ln->mv_throughput.end();  ++i_vec) {
					time+= i_vec->m_time;
					mass+= i_vec->m_mass;
				}					
				oss << "NODE: " << m_logistics_node_index << "\n";
				oss << "MASS: " << mass;
				if(text!= NULL)	text->setText(oss.str());
			}
			break;
		}								
		case AlterString_node_mass:	{
			if(m_logistics_node_index!= UINT_MAX)	{
				osg::ref_ptr<osgText::Text> text = p_env->GetText(m_text_index);
				ostringstream oss;								
				CLogistics_node * p_ln;
				if(m_logistics_node_index<   p_env->mv_logistics_node.size()) {			
					p_ln = &(p_env->mv_logistics_node[m_logistics_node_index]);
				}				
				float mass=  p_env->mv_logistics_operation[m_operation_index].mass_of_operatives_at_node(m_logistics_node_index);								
				oss << "NODE: " << m_logistics_node_index << "\n";
				oss << "MASS: " << mass;
				if(text!= NULL)	text->setText(oss.str());
			}
			break;
		}					
		case AlterString_Camera:
		{
			if(m_subject_index!= UINT_MAX)
			{
				osg::ref_ptr<osgText::Text> text = p_env->GetText(m_text_index);
				osg::Vec3f Veye,Vcentre,Vup;
				p_env->mv_viewer[p_env->m_active_viewer_index].mp_viewer->getCamera()->getViewMatrixAsLookAt(Veye,Vcentre,Vup);
				ostringstream oss;
					oss << "pos x,y,z: " << Veye.x() 
					<< ", " << Veye.y() 
					<< ", " << Veye.z() 
					<< "\n, yaw:" << p_env->m_custom_camera.m_yaw 
					<< ", pitch:" << p_env->m_custom_camera.m_pitch
					<< ", X:" << p_env->m_custom_camera.m_previous_mouse_x
					<< ", Y:" << p_env->m_custom_camera.m_previous_mouse_y
					<< "\n, LM:" << p_env->mp_input_state_type->m_mouse_button_is_pushed
					<< "\n, delta:" << p_env->mp_input_state_type->m_mouse_is_moving;				
		
				if(text!= NULL)	text->setText(oss.str());
			}
			break;
		}		
	}		
	return true;
}


///
// CEvent_UpdateText
///
bool CEvent_UpdateText::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{		
		if(iter->m_identifier == "Text")
		{
			m_text_index = p_env->index_from_name(iter->m_str_value,  p_env->mv_text);
		}		
		if(iter->m_identifier == "MemoryMapClient")
		{			
			m_mmc_index = p_env->index_from_name(iter->m_str_value, p_env->mv_memory_map_client);
		}	
	}

	return true;
}
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <cstring>

bool CEvent_UpdateText::Update(CEnvironment* p_env)
{	
	if(m_mmc_index!= UINT_MAX)
	{
		osg::ref_ptr<osgText::Text> text = p_env->GetText(m_text_index);
			
		//Open already created shared memory object.
		boost::interprocess::shared_memory_object shm (boost::interprocess::open_only, p_env->mv_memory_map_client[m_mmc_index].m_memory_map_name.c_str(), boost::interprocess::read_only);

		//Map the whole shared memory in this process
		boost::interprocess::mapped_region region(shm, boost::interprocess::read_only);

		ostringstream oss;
		char *mem = static_cast<char*>(region.get_address());
		for(std::size_t i = 0; i < region.get_size(); ++i)
		{				
			char c = *mem;
			oss << c;
			*mem++;
		}
		
		if(text!= NULL)	text->setText(oss.str());
	}
	return true;
}