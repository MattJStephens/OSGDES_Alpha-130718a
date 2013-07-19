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
bool CEvent_ScreenGrab::Create(CEventDesign* p_design, CEnvironment* p_env)
{	

	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{
		if(iter->m_identifier == "Ref_image")
		{
			m_ref_image.m_ref_name = iter->m_str_value;
			p_env->update_reference(&m_ref_image, p_env->mv_image);
			
		}
		else if(iter->m_identifier == "Height")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_height;			
		}
		else if(iter->m_identifier == "Width")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_width;		
		}
		else if(iter->m_identifier == "BitColor")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_bit_color;		
		}
		else if(iter->m_identifier == "FileName")
		{
			m_file_name = iter->m_str_value;
		}
		else if(iter->m_identifier == "FileExtension")
		{
			m_file_extension = iter->m_str_value;
		}
		
	}


	CImage* p_image = p_env->get_Image(&m_ref_image);
	if(p_image!= NULL)
	{
		p_image->mp_image->allocateImage(m_width, m_height, m_bit_color, GL_RGB, GL_UNSIGNED_BYTE);
		p_env->mv_viewer[p_env->m_active_viewer_index].mp_viewer->getCamera()->attach(osg::Camera::COLOR_BUFFER, p_image->mp_image);
	}
	m_count = 0;
	return true;
}

bool CEvent_ScreenGrab::Update(CEnvironment* p_env)
{	
	
	CImage* p_image = p_env->get_Image(&m_ref_image);
	if(p_image!= NULL)
	{

		std::ostringstream oss_beta;
		oss_beta.width(10);
		oss_beta.fill('0');
		oss_beta << m_count;
		
	
		std::ostringstream oss;
		oss << "ScreenGrab\\";
		oss << p_env->m_str_script;
		oss << "_";
		oss << oss_beta.str();
		oss << ".";
		oss << m_file_extension;

		if(p_image->mp_image!= NULL)
		{
			try { osgDB::writeImageFile(*(p_image->mp_image), oss.str()); }
			catch(...) { std::cout << "osgDB::writeImageFile error..." << std::endl << std::endl;	} 
		}
		m_count++;
	
	}

	return true;
}

