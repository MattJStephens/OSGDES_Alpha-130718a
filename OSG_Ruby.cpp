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
#include <iostream> 
//#include <ruby.h>

//using namespace std; 
///
// CEvent_DE_Logistics
///
bool CEvent_Ruby::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {	
	
	}		
	
	return true;
}

bool CEvent_Ruby::Update(CEnvironment* p_env)
{	
	//ruby_init();
	//ruby_init_loadpath();
	//int status; 
	//rb_load_protect(rb_str_new2("./test.rb"), 0, &status); 
	//if (status) 
	//{ 
	//	VALUE rbError = rb_funcall(rb_gv_get("$!"), rb_intern("message"), 0);
	//	cerr << StringValuePtr(rbError) << endl; 
	//};
	//	
	//ruby_finalize(); 
	//	
	//	//return status; 


	return true;
}

