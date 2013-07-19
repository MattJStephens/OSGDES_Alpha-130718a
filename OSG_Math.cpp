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

#include "mjs_wave.h"
#include "Framework.h"
#include "XML_EXPR.h"

///
// CEvent_Math
///
bool CEvent_Math::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	m_WithRespectTo= "x"; //default
	m_process= "Differentiate"; //default value
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {	
		if(iter->m_identifier == "Process") {	m_process = iter->m_str_value; }		
		if(iter->m_identifier == "WithRespectTo") {	m_WithRespectTo = iter->m_str_value; }		
	}		

	std::vector<CEXPR>::iterator expr_iter;
	for(expr_iter = p_design->mv_expr.begin(); expr_iter < p_design->mv_expr.end(); ++expr_iter) {	
		std::size_t index=mv_expr.size();
		mv_expr.push_back(*expr_iter);
		mv_expr[index].m_operation_list.link_operation_references(&(mv_expr[index]));
	}		
	
	return true;
}
bool CEvent_Math::Update(CEnvironment* p_env)
{	
	double result= 0;
	
	if(m_process == "Differentiate")
	{

		if(mv_expr.size()>0)
		{
			std::size_t index= mv_expr.size()-1;
			CEXPR op = mv_expr[index].differentiate(m_WithRespectTo);
			op.m_operation_list.link_operation_references(&(op));
			op.m_variable_list.set(0,2);
			result = op.evaluate();
		}
	}

	return true;
}
