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
// CEvent_UpdatePositionParametricExpression
///
bool CEvent_UpdatePositionParametricExpression::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	m_index_PAT = p_env->PAT_index_from_name(p_design->m_ref_name);
	
	CExpVar expr_varx;
	expr_varx.m_name= "t";
	m_expression_variables_x.mv_variable.push_back(expr_varx);
	m_expression_variables_y.mv_variable.push_back(expr_varx);
	m_expression_variables_z.mv_variable.push_back(expr_varx);
	std::string str_expression_x = "10 * sin(t)";
	std::string str_expression_y = "10 * cos(t)";
	std::string str_expression_z = "1";

	long i = 0;
	std::vector<string>::iterator str_iter;
	for(str_iter = p_design->mv_str_expression.begin();str_iter < p_design->mv_str_expression.end();++str_iter)
	{
		switch(i)
		{
			case 0:
			{
				str_expression_x = *str_iter;
				break;
			}
			case 1:
			{
				str_expression_y = *str_iter;
				break;
			}
			case 2:
			{
				str_expression_z = *str_iter;
				break;
			}
		}
		i++;
	}	
	m_expr_x_index = m_expression_list_x.analyze(str_expression_x, &m_expression_variables_x);
	m_expr_y_index = m_expression_list_y.analyze(str_expression_y, &m_expression_variables_y);
	m_expr_z_index = m_expression_list_z.analyze(str_expression_z, &m_expression_variables_z);

	return true;
}
bool CEvent_UpdatePositionParametricExpression::Update(CEnvironment* p_env)
{	 
	float x,y,z;
	m_expression_variables_x.mv_variable[0].m_value= p_env->get_elapsed_time();
	x = m_expression_list_x.evaluate(m_expr_x_index);
	m_expression_variables_y.mv_variable[0].m_value= p_env->get_elapsed_time();
	y = m_expression_list_y.evaluate(m_expr_y_index);
	m_expression_variables_z.mv_variable[0].m_value= p_env->get_elapsed_time();
	z = m_expression_list_z.evaluate(m_expr_z_index);

	Vec3d vec;
	vec.set(x,y,z);
	CPositionAttitudeTransform *p_PAT = p_env->get_PAT(m_index_PAT);
	if(p_PAT!= NULL) p_PAT->mp_PAT->setPosition(vec);
	
	return true;
}
