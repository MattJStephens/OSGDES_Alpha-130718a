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

#define NOMINMAX
#include "windows.h"

#include "OSG_DES.h"
#include "OSG_Event.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Tools.h"
#include "OSG_XML_Save_event_tree.h"
#include "OSG_XML_Save_environment_tree.h"

#include <string>

using  namespace std;
using  namespace osg;

extern CEventHandler g_event_handler;



///
// CEvent_Empty
///
bool CEvent_Empty::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	
	return true;
}
bool CEvent_Empty::Update(CEnvironment* p_env)
{	

	return true;
}

///
// CEvent_GetEntityDetails
///
bool CEvent_GetEntityDetails::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	return true;
}
bool CEvent_GetEntityDetails::Update(CEnvironment* p_env)
{
	return true;
}

///
// CEvent_Innate
///
bool CEvent_Innate::Create(CEventDesign* p_design, CEnvironment* p_env)
{

	return true;
}
bool CEvent_Innate::Update(CEnvironment* p_env)
{	

	return true;
}

///
// CEvent_CreateEntity
///
bool CEvent_CreateEntity::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{		
		if(iter->m_identifier == "Entity_type")
		{
			//m_command_line = iter->m_str_value;
		}		
	}
	
	return true;
}

bool CEvent_CreateEntity::Update(CEnvironment* p_env)
{	
	
	return true;
}


///
// CEvent_CreateProcess
///
bool CEvent_CreateProcess::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{		
		if(iter->m_identifier == "CommandLine")
		{
			m_command_line = iter->m_str_value;
		}		
	}
	
	return true;
}

bool CEvent_CreateProcess::Update(CEnvironment* p_env)
{	
	TCHAR sz[1024];
	lstrcpy(sz, m_command_line.c_str());
	LPTSTR lpCommandLine = sz;	
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
	  if( !CreateProcess( NULL,   // No module name (use command line)
        lpCommandLine,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return true;
    }
	    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
	return true;
}

///
// CEvent_LoadScript
///
bool CEvent_LoadScript::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {	
		if(iter->m_identifier == "file_name") {	m_file_name = iter->m_str_value; }		
	}		

	return true;
}
bool CEvent_LoadScript::Update(CEnvironment* p_env)
{	
	p_env->XML_EnvironmentParser(m_file_name);
	p_env->LinkReferences();
	return true;
}



///
// CEvent_UpdateGraph
///
bool CEvent_UpdateGraph::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{		
		if(iter->m_identifier == "Logistics_node")
		{
			m_ref_logistics_node.m_index = p_env->index_from_name(iter->m_str_value, p_env->mv_logistics_node);
			p_env->mv_logistics_node[m_ref_logistics_node.m_index].m_record= true;
		}
		else if(iter->m_identifier == "Graph")
		{
			m_ref_graph.m_index = p_env->index_from_name(iter->m_str_value, p_env->mv_graph);
		}	
	}
	return true;
}
bool CEvent_UpdateGraph::Update(CEnvironment* p_env)
{	
	std::vector<osg::Vec3d> v_vec;
	std::vector<CLogistics_throughput>::iterator i_vec;
	float mass= 0;
	float time= 0;
	for(i_vec = p_env->mv_logistics_node[m_ref_logistics_node.m_index].mv_throughput.begin(); 
		i_vec < p_env->mv_logistics_node[m_ref_logistics_node.m_index].mv_throughput.end(); 
		++i_vec)
	{
		osg::Vec3d vec;
		time+= i_vec->m_time;
		mass+= i_vec->m_mass;
		vec.set(time, mass, 0);
		v_vec.push_back(vec);		
	}
	p_env->mv_graph[m_ref_graph.m_index].update(&v_vec);
	return true;
}

///
// CEvent_ReplaceDrawable
///
bool CEvent_ReplaceDrawable::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{
		if(iter->m_identifier == "Type")
		{
			m_sub_type = iter->m_str_value;			
		}	
		if(iter->m_identifier == "Logistics_producer")
		{
			m_Logistics_producer_index = p_env->index_from_name(iter->m_str_value, p_env->mv_Logistics_producer);
		}
		if(iter->m_identifier == "Geode")
		{
			m_geode_index = p_env->index_from_name(iter->m_str_value, p_env->mv_geode);
		}
	}
	return true;
}
bool CEvent_ReplaceDrawable::Update(CEnvironment* p_env)
{	
	if(m_Logistics_producer_index!= UINT_MAX)
	{
		double angle_rad = 3.14159/6.0;
		double height= p_env->mv_Logistics_producer[m_Logistics_producer_index].get_height_from_angle_and_volume(angle_rad);
		if(m_geode_index!= UINT_MAX)
		{
			double radius = height/tan(angle_rad);
			Vec3d center = Vec3d (0,0,0);			
			p_env->mv_geode[m_geode_index].mp_geode->replaceDrawable(p_env->mv_geode[m_geode_index].mp_geode->getDrawable(0), new ShapeDrawable(new Cone(center, radius, height)));
		}
	}
	
	return true;
}

///
// CEvent_Render
///
bool CEvent_Render::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	return true;
}
bool CEvent_Render::Update(CEnvironment* p_env)
{

	return true;
}

///
// CEvent_SaveEventTree
///
bool CEvent_SaveEventTree::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	m_file_name = p_design->m_file_name;
	return true;
}
bool CEvent_SaveEventTree::Update(CEnvironment* p_env)
{	
	XML_Save_event_tree(m_file_name, &(g_event_handler.m_event));		
	return true;
}


///
// CEvent_SaveEnvironmentTree
///
bool CEvent_SaveEnvironmentTree::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	m_file_name = p_design->m_file_name;
	return true;
}
bool CEvent_SaveEnvironmentTree::Update(CEnvironment* p_env)
{	
	XML_Save_environment_tree(m_file_name, p_env);		
	return true;
}


///
//CEvent_Sequencer
///
bool CEvent_Sequencer::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	m_period = p_design->m_period;
	return true;
}
bool CEvent_Sequencer::Update(CEnvironment* p_env)
{

	return true;
}

///
// CEvent_UpdateSimulationTime
///
bool CEvent_UpdateSimulationTime::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	p_env->set_simulation_time_increment(p_design->m_elapsed_simulation_time_increment);
	return true;
}
bool CEvent_UpdateSimulationTime::Update(CEnvironment* p_env)
{
	p_env->update_simulation_time();
	return true;
}

/*                                                                               
                                                                                                     
    DDDD        8DDD                D.    D.   DD                                                   
   .DDZD      .8DMDD               DD    DD.  .DD                                                   
   .DD.DN     .DDDDD      DDD     .DD ...DD.  .DD  :N: .       DD.    ...      .   ,                
   .DD.8D,.   8D.DDD  .DDDDDDDN   DDDDD DDDDD .DDDDNNDND.   DDDDNNDD  8D    DDD    DD.              
   .DD  D8. ..DD.DDD  8D...   DD   DD    DD.  .DDD.  .8D   DD. .  .DN  DN  .DDD    DD               
   .DD   D. .NN  DDD      . .DDD   DD    DD.  .DD..   DD .,DNDDDDDDDD..DD  .D.DD  8N                
   .DD  .DD  DD. DDD   8DDDDD,DD   DD    DD.  .DD     DD  .DN8DDDDDDD  .DN DN.,D .ND                
   .DD   ,D.DD.. DDD  DD    .,DD   DD    DD.  .DD     DD ..DD.       .  DD.D. .DDDD                 
   .DD    DNDD   DDD  DN..  .DDD   DN    DD   .DD     DD   NDN.. .DDN   .DND  .D8DD.                
   .DD    .D8.   DDD   NNDDDD.DD.   DDD. .DDD..DD     DD     DDDDD8.     MDD   .DD.                 
                        ....        .. .   . .                 ..                                   
          . .                                                                                        
          DD                                                                                        
          DD                                                                                        
          DD    ...DDO .   .   .DN  . :NO..   .  ONO     ...,DD                                     
          DD  ..DDDDDNDD.  DD.DDNDD8DDDDDND... NNNDDDD .  DNDDDDDN                                  
          DD  .DD     8ND  DDD   .DD8.  ,DD  .D8.    DD   DD..  D                                   
          DD    .. .DDDDD  DD     DD     DD .DDNDDDDDNDD..DND8N                                     
     .    DD  .DDNDDD DDD  DD.    DD     DD .DN.        .    DDDNN                                  
   8D8   .DD  ,DD   ..NDD  DD.    DD     DD  DD8.     .  .D     ,DD                                 
   .ND. .DDD  .DD   .NDDD  DD.    DD     DD   DDN ..,DD.  DD.   DDD                                 
     DDD8D      DDDDD .8D  DD.    DD    .DD.  .,DDDDDD     DDDDDN                                   
                 

       ..DD .       .                             ..                                                
     8NDDDDDDD8    8D                            .DD                                                
    .8N.    .D8.   DD                            .DD                                                
    DDD     ..DD. D8DND     8DDD     ND .8D8     .DD 8DD8       8DD8     .DD  8DN       DDDD        
    .DDDN         DDDDD  .DDD.,DDN  .D8ND8.DD8 . .DND8.DDN    NDD, NDD,.  DDDN .DDD   NND  DD8      
    .  DDDDDD8     DD    D8      D8 .DDD.    NN. .DN .   DD .DN .    NN   DD    .DD   DD.  .  ..    
        .  .DDDD   DD   DDNDDDDDNDD..DD      DD  .DN    .DD  DDDDDDDDDDD  D8     DD.. 8DDNN8        
    DD      . DD   DD   DD...........DD     .DD  .DD    .DD  DD ........  DD     DD.   .,DDDD8      
   .DDO.      DD   DD   .DD     .D ..DDD     DD. .DD    .DD .ND      :O   DD     DD..,8D  . .DD     
     DD8D  .DDD.   DD.  .8D8 ..8DN  .DDD8  DDD.  .DD    .DD   DD8, .DDD   DD     DD.. DD8  .DDD.    
    .. DDDDDN  .    DDD  . DDDDD.   .DD 8DDD .   .DD    .DD     NDDDD ..  DD     DD.  ..DDDDD  .    
                    .       .  .    .DD  .                                                          
                                    .DD                                                             
                                     DD                                                             
                                                                                                    
                                                                                                    
  */