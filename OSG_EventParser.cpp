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

#include "OSG_Event.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Tools.h"
#include <string>

using  namespace std;
using  namespace osg;

bool CEventHandler::XML_EventParser(string str, CEnvironment* p_env, CEventCrysalis* p_event_crysalis)
{	
	CoInitialize(NULL);
	try {	
		CComPtr<IXMLDOMDocument> spXMLDOM;
		HRESULT hr = spXMLDOM.CoCreateInstance(__uuidof(DOMDocument));
		if ( FAILED(hr) ){
			throw "Unable to create Event XML parser object";
			return false;
		}
		if ( spXMLDOM.p == NULL ) {
			throw "Unable to create Event XML parser object";
			return false;
		}
		VARIANT_BOOL bSuccess = false;
		hr = spXMLDOM->load(CComVariant(str.c_str()),&bSuccess);
		if ( FAILED(hr) ) {
			throw "Unable to load Event XML document into the parser";
			return false;
		}
		if ( !bSuccess ) {
			throw "Unable to load Event XML document into the parser";
			return false;
		}
		CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
		hr = spXMLDOM->get_childNodes(&spXMLDOMNodeList);
		if ( FAILED(hr) ) {
			throw "Unable to get Event node list";		
			return false;
		}
		long listLength;
		hr =spXMLDOMNodeList->get_length(&listLength);
		if ( FAILED(hr) ) {
			throw "Unable to get Event list length";
			return false;
		}
		for(long i= 0; i< listLength; i++) {
			string str_node_name;
			string str_node_text;
			CComPtr<IXMLDOMNode> spXMLNextNode;
			spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
			if(spXMLNextNode!= NULL) {
				if(str_node_name== "Event")	{
					XML_ParseEvent(spXMLNextNode, p_env, p_event_crysalis);
				}
			}			
		}
		std::cout << "Event Parser complete "  << str << " " << std::endl << std::endl;
	} 
	catch(char* lpstrErr) { std::cout << lpstrErr << std::endl << std::endl; }
	catch(...) { std::cout << "Unknown error..." << str << " " << std::endl << std::endl; } 	
	CoUninitialize();	
	return true;
}

eEventType string_to_event_type(std::string str)
{
	if(str == "AlterPosition") { return EventType_AlterPosition; }
	else if(str == "AlterOrientation") { return EventType_AlterOrientation; }	
	else if(str == "AlterText") { return EventType_AlterText; }	
	else if(str == "DE_Logistics") { return EventType_DE_Logistics; }
	else if(str == "CreatePhysXActor") { return EventType_CreatePhysXActor;	}		
	else if(str == "CreateEntity") { return EventType_CreateEntity;	}				
	else if(str == "CreateProcess") { return EventType_CreateProcess; }						
	else if(str == "CreateWave") { return EventType_CreateWave; }						
	else if(str == "Create3ds") { return EventType_Create3ds; }						
	else if(str == "DiscreteEventSimulation") { return EventType_DiscreteEventSimulation; }					
	else if(str == "Empty") { return EventType_Empty; }
	else if(str == "Emitter") {	return EventType_Emitter; }
	else if(str == "GetEntityDetails") { return EventType_GetEntityDetails;	}			
	else if(str == "Innate") { return EventType_Innate;	}				
	else if(str == "LoadScript") { return EventType_LoadScript;	}		
	else if(str == "Math") { return EventType_Math; }
	else if(str == "MoveGlobal") { return EventType_MoveGlobal;	}
	else if(str == "ReplaceDrawable") {	return EventType_ReplaceDrawable; }	
	else if(str == "Render") { return EventType_Render; }	
	else if(str == "Python") { return EventType_Python;	}	
	else if(str == "PlayWav") { return EventType_PlayWav;	}		
	else if(str == "Ruby") { return EventType_Ruby;	}		
	else if(str == "SaveEventTree") { return EventType_SaveEventTree; }	
	else if(str == "SaveEnvironmentTree") { return EventType_SaveEnvironmentTree; }	
	else if(str == "Sequencer") { return EventType_Sequencer; }
	else if(str == "Sensor") { return EventType_Sensor; }				
	else if(str == "ScreenGrab") { return EventType_ScreenGrab; }				
	else if(str == "SMTP") { return EventType_SMTP; }								
	else if(str == "Socket") { return EventType_Socket;	}					
	else if(str == "UpdateGraph") {	return EventType_UpdateGraph; }	
	else if(str == "UpdateNxActor")	{ return EventType_UpdateNxActor; }			
	else if(str == "UpdatePositionParametricExpression") { return EventType_UpdatePositionParametricExpression;	}					
	else if(str == "UpdateSimulationTime") { return EventType_UpdateSimulationTime;	}	
	else if(str == "UpdateText") { return EventType_UpdateText;	}	
	else if(str == "UpdateTheatreOfOperations") { return EventType_TheatreOfOperations;	}	
	else if(str == "UpdateVariable") { return EventType_UpdateVariable;	}	
	else if(str == "UpdateVelocity") { return EventType_UpdateVelocity;	}	
	else if(str == "UpdateViewer") { return EventType_UpdateViewer;	}		
	else if(str == "UpdateViewerAnimate") {	return EventType_UpdateViewerAnimate; }		
	return EventType_Unknown; 
}

bool CEventHandler::XML_ParseEvent(CComPtr<IXMLDOMNode> spXMLNode, CEnvironment* p_env, CEventCrysalis* p_event_crysalis)
{	
	CEventDesign event_design;
	HRESULT hr;
	long listLength;
	CComPtr<IXMLDOMNodeList> spXMLDOMNodeList;
	hr = spXMLNode->get_childNodes(&spXMLDOMNodeList);	
	hr = spXMLDOMNodeList->get_length(&listLength);
	for(long i= 0; i< listLength; i++) {
		string str_node_name;
		string str_node_text;
		CComPtr<IXMLDOMNode> spXMLNextNode;
		spXMLNextNode= XML_NextNode(spXMLDOMNodeList, &str_node_name, &str_node_text);
		if(spXMLNextNode!= NULL) {	
			if(str_node_name == "AssociateVariable") {
				CVarAssoc var_assoc;	
				XML_Parse_VarAssoc(spXMLNextNode, &var_assoc);	
				event_design.mv_assoc_var.push_back(var_assoc);
			}
			else if(str_node_name == "AlterString")	{ event_design.m_alter_string = str_node_text; }
			else if(str_node_name== "AngularVelocity") {
				istringstream iss(str_node_text);
				iss >> event_design.m_angular_velocity;							
			}	
			else if(str_node_name== "AngularAcceleration") {
				istringstream iss(str_node_text);
				iss >> event_design.m_angular_acceleration;							
			}
			else if(str_node_name == "CameraUpdateType") { event_design.m_camera_update_type = str_node_text; }	
			else if(str_node_name == "ClimaxCondition") { event_design.m_str_climax_condition = str_node_text; }		
			else if(str_node_name == "ClimaxVariable") { event_design.mv_str_climax_varibles.push_back(str_node_text); }				
			else if(str_node_name== "ClimaxConditionEXPR") { 
				XML_Parse_EXPR(spXMLNextNode, &(event_design.m_climax_condition)); 
				event_design.m_climax_condition.m_operation_list.link_operation_references(&event_design.m_climax_condition);
			}
			else if(str_node_name== "DeltaTime") {
				istringstream iss(str_node_text);
				iss >> event_design.m_delta_time;							
			}	
			else if(str_node_name== "Logistics_node_name") { event_design.m_Logistics_node_name = str_node_text; }				
			else if(str_node_name== "Event") {
				CEventCrysalis m_child_event;	
				XML_ParseEvent(spXMLNextNode, p_env, &m_child_event);	
				p_event_crysalis->mv_child.push_back(m_child_event);
			}	
			else if(str_node_name == "Expression") { event_design.mv_str_expression.push_back(str_node_text); }			
			else if(str_node_name== "EXPR") { 
				CEXPR expr;
				XML_Parse_EXPR(spXMLNextNode, &expr); 				
				event_design.mv_expr.push_back(expr);
				//event_design.m_climax_condition.m_operation_list.link_operation_references(&event_design.m_climax_condition);
			}
			else if(str_node_name== "FileName")	{ event_design.m_file_name = str_node_text; }	
			else if(str_node_name== "GeodeRef") { event_design.m_geode_name = str_node_text; }				
			else if(str_node_name== "InceptionConditionEXPR") { 
				XML_Parse_EXPR(spXMLNextNode, &(event_design.m_inception_condition)); 
				event_design.m_inception_condition.m_operation_list.link_operation_references(&event_design.m_inception_condition);
			
			}
			else if(str_node_name== "Name")	{ event_design.m_name = str_node_text; }	
			else if(str_node_name== "Orientation") { XML_Parse_Vec4d(spXMLNextNode, &(event_design.m_Qorientation)); }	
			else if(str_node_name== "OrientationAxis") { XML_Parse_Vec3d(spXMLNextNode, &(event_design.m_orientation_axis)); }	
			else if(str_node_name== "Origin") { XML_Parse_Vec3d(spXMLNextNode, &(event_design.m_origin)); }			
			else if(str_node_name== "Orientation") {
				istringstream iss(str_node_text);
				iss >> event_design.m_orientation;							
			}		
			else if(str_node_name== "PATRef") { event_design.m_PAT_name = str_node_text; }	
			else if(str_node_name== "Period") {
				istringstream iss(str_node_text);
				iss >> event_design.m_period;							
			}	
			else if(str_node_name== "Position")	{ XML_Parse_Vec3d(spXMLNextNode, &(event_design.m_position)); }	
			else if(str_node_name== "RefName") { event_design.m_ref_name = str_node_text; }	
			else if(str_node_name== "Ref") {
				CReference ref;
				XML_Parse_Reference(spXMLNextNode, &ref);				
				event_design.mv_ref.push_back(ref);					
			}	
			else if(str_node_name == "Response_xml") { 
				event_design.mv_response_xml.push_back(str_node_text); 
			}										
			else if(str_node_name == "SimulationTimeIncrement")	{
				istringstream iss(str_node_text);
				iss >> event_design.m_elapsed_simulation_time_increment;			
			}
			else if(str_node_name== "SimulationTime") {
				istringstream iss(str_node_text);
				iss >> event_design.m_elapsed_simulation_time;							
			}	
			else if(str_node_name== "SourceType") {	event_design.m_source_type = str_node_text;	}				
			else if(str_node_name== "StartTime") {
				istringstream iss(str_node_text);
				iss >> event_design.m_start_time;							
			}
			else if(str_node_name== "Type")	{ event_design.m_type= string_to_event_type(str_node_text);	}		
			else if(str_node_name == "TimeType") { event_design.m_str_time_type = str_node_text; }	
			else if(str_node_name== "Variable")	{
				CParsedVariable variable;
				XML_Parse_Variable(spXMLNextNode, &variable);	
				event_design.mv_variable.push_back(variable);
			}			
			else if(str_node_name== "Velocity")	{ XML_Parse_Vec3d(spXMLNextNode, &(event_design.m_velocity)); }	
			else if(str_node_name == "ViewerUpdateType") { event_design.m_viewer_update_type = str_node_text; }	
			else if(str_node_name== "Inception") {
				if(str_node_text == "Spontaneous") { event_design.m_inception= Inception_Spontaneous; }
				else if(str_node_text == "MemMap_Conditional") { event_design.m_inception= Inception_MemMap_Conditional; }		
				else if(str_node_text == "Conditional") { event_design.m_inception= Inception_Conditional; }		
			}	
			else if(str_node_name== "Subordination") {
				if(str_node_text == "Simultaneous") { event_design.m_subordination= Subordination_Simultaneous;	}
				else if(str_node_text == "PeriodicSimultaneous") { event_design.m_subordination= Subordination_PeriodicSimultaneous; }	
				else if(str_node_text == "Sequential") { event_design.m_subordination= Subordination_Sequential; }			
				else if(str_node_text == "PeriodicSequential") { event_design.m_subordination= Subordination_PeriodicSequential; }				
			}		
			else if(str_node_name== "Occurance") {
				if(str_node_text == "Discrete") { event_design.m_occurance= Occurance_discrete; }
				else if(str_node_text == "Interpolate") { event_design.m_occurance= Occurance_interpolate; }
			}	
			else if(str_node_name== "Recurrance") {
				if(str_node_text == "Singular") { event_design.m_recurrance= Recurrance_singular; }
				else if(str_node_text == "Perpetual") { event_design.m_recurrance= Recurrance_perpetual; }
			}	
			else if(str_node_name== "Concurrance") {
				if(str_node_text == "Polling") { event_design.m_concurrance= Concurrancy_polling; }
				else if(str_node_text == "Threaded") { event_design.m_concurrance= Concurrancy_threaded; }
			}	
			else if(str_node_name == "VariableAssociation")	{
				event_design.mv_varible_association.push_back(str_node_text);
			}				
		}
	}						
	if(!p_event_crysalis->Morph(&event_design, p_env))	
	{
		std::cout << "morph failure exit and fix script";
		return false;		
	}
	return true;	
}

void CEventHandler::insert(std::size_t index)
{

}

void CEventHandler::erase(std::size_t index)
{

}

 /*                                                                                                                      
                                                                                                                     
                                                                                                                        
                                                                                                                        
                                                                                                                        
                                                                                                                        
                                                         ..             .                                               
                   D      ..D                            N,      DD.    .DD.                                            
                 .DD       8N                           DN.      D       D                                              
                  . .    ..                             D      .D8.    .DD                                              
                 DD .     DD                           DD..    .D..     D                                               
                , D.     D D                       .   N,.   ..NN   .  DD                                               
                .8.    ...N.                        ..DD ..    D.....  D .                                              
               D.N      . D                         ..D       N8.     N8                                                
              D .D.   .N.8D.        . .DN8   8D    ..DN     ,8D      DD  DD8 .      ...8D.    .D8   .DN    ..           
             D. DD   .D. DD         .D.    D D      .D.     DDD..   8NDD..  D       .8.. D    D..   .D      .           
            ... D8   D   D          D. .    DD . .  ND.    D 8     D.D..   .D.      D. ..8   D8.   .ON.    .8           
   . ..         D   ,..  D        .DD     ..N.    . D.   , .D8    D DD    .DN   .D.8D   .   8D     .D..  . D D.N        
          D     D        D     N   8D      8D     .DD  .. . D.   D. D .   .D..  D. 8D DD    DN   ..ND    .D             
   ..    D      D....   .D.  .D     D,.  .DDD .    ND  D.   8 ...  DD..    D   D    D,.    DDD.. ..ND....,              
                           8                                                                                            
                          .                                                                                             
                                                                                                                        
                                                                                                                        
                                                                                                                        
                                                                                                                        
                                                                                                                        
                                                                                                                        
                .....,                                                                                                  
            .8      D                                                                                                   
         ..N      88.                                                                                                   
          N..     D                                                                                                     
                :8.                                                                                                     
         .     .ND ,.                                                                                                   
         .. .  8D. N.                                                                                                   
           ..D8D8 .                                                                                                     
             .DD.          8D8  ...      .D    .DD    .DD .      .. .D    .D                                            
             .D,.        D ..  D N      D  DD.D.  DD.D   DN       88 .    ..N                                           
             DD..8.     D     . 8D      .  DN  .  DD     DD      DD.   . D .. D                                         
          . DD. .      8..    ..D     .    D .    D .  ..N,  . D N    D    .  88.                                       
       . .. N .        8.     .DD... .    8D .  .DN     ND    D  8 .D .   D   .N .  .                                   
       N.  .           .D   . DDD ..    ..D     .D.     DD. .,.   D.     D    DN..N,                                    
      8...,D.             DDD.  .D. .    ..       .      .D.  .  .. DDD. . DDDDD. ..                                    
     ..   D..                                                                                                           
      . N                                                                                                               
      .                                                                                                                 
                                                                                                                        
                                                                                                                        
                                                                                                                        
                                                                                                                        
                     . .                                                                                                
                   ..D. D.      D.                             .,D                                                      
                   .    .,     DN                              .DD                                                      
          8        ,.  .D     .8 .                             OD..                                                     
         .       .D .  N    . ND                             . D.                                                       
                 .D   8.     .N.               8N            .D8.                                                       
          8.    ...DD .   . .NN...             D              N                                                         
           .DDDD88        .. N                DD.           .DD                                                         
           .     N          D8              .DD            .DD                                         N                
               .ND        .ND.     .NN. N,   DD  . .D       ND    .D.       N8  8   N .ND.8 .  N.    ..                 
               ,N,         ND     .D.   N .D D..    D    .. DD.    D      .8    D  D  .DD ..   N     ... 8D.            
     .       . ND..     ..,D     DDD    .   ND .   DN   ...ND    .DD.  .. DD  .  . . D NN     8D.  .8 .   DN    .,      
   . .        DD.         DD   . .DN  8.   D8.   .,D   .8  D     .D      .DD. N.  .   8D .    D. ..D     . D            
   . D.      ND           DD    .. D...   .DN      D   D .DD.    .D..      N...   .D  DD      D.  N..     8D. D..       
     . DDDDD  .         . .,D.,.    .NDDD.DD..   . .DD.  ,D        DD,.    ..NDDN .  DD       .D  .   NDDDD8  .         
                                          DD .                                                                          
                                        . D                                                                             
                                        .DO.                                                                            
                                        .D..                                                                            
                                        DD.                                                                             
                                       8D                                                                               
                                       DN                                                                               
                                        .                          
*/