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


osg::Vec3d linear_interpolate(osg::Vec3d* A, osg::Vec3d* B, float period, float elapsed)
{	
	osg::Vec3d vec_ntrplt;
	if(elapsed== 0)
	{

		return * A;
	}
	if(elapsed< period)
	{
		osg::Vec3d AB = *B- *A;
		float fraction = elapsed/period;
		float mag = AB.length();
		float dist = fraction* mag;
		AB.normalize();
		vec_ntrplt = *A +(AB * dist);
	}
	return vec_ntrplt;
}

///
// CEvent_UpdateViewer
///
//OSG world coordinate system (where X is east, Y is north and Z is up) 
//and OpenGl (where X is to the right, Y is up, and Z is coming out of the screen). 
bool CEvent_UpdateViewerAnimate::Create(CEventDesign* p_design, CEnvironment* p_env)
{
	m_active_viewer_index = 0;

	if(p_design->m_viewer_update_type == "UpdateCamera")
	{
		m_viewer_update_type = ViewerUpdateType_UpdateCamera;		
	}
	else if(p_design->m_viewer_update_type == "UpdateStats")
	{
		m_viewer_update_type = ViewerUpdateType_UpdateStats;		
	}

	if(p_design->m_camera_update_type == "AnimationPath")
	{
		m_camera_update_type = CameraUpdateType_AnimationPath;		
	}
	else if(p_design->m_camera_update_type == "SetProjectionMatrix")
	{
		m_camera_update_type = CameraUpdateType_SetProjectionMatrix;		
	}
	else if(p_design->m_camera_update_type == "SetViewMatrix")
	{
		m_camera_update_type = CameraUpdateType_SetViewMatrix;		
	}
	else if(p_design->m_camera_update_type == "SetViewMatrixLookAt")
	{
		m_camera_update_type = CameraUpdateType_SetViewMatrixLookAt;		
	}
	else if(p_design->m_camera_update_type == "FollowPath")
	{
		m_camera_update_type = CameraUpdateType_FollowPath;		
	}
	

	//p_env->m_custom_camera.m_position.set(0,0,0);
	//p_env->m_custom_camera.m_yaw= 0;
	//p_env->m_custom_camera.m_pitch= 0;
	//p_env->m_custom_camera.m_roll= 0;

	if(p_env->mp_input_state_type!= NULL)
	{
		p_env->mp_input_state_type->m_mouse_push =false;
		p_env->mp_input_state_type->m_mouse_release =false;
		p_env->mp_input_state_type->m_mouse_active= false;
		p_env->mp_input_state_type->m_mouse_is_moving= false;
		p_env->mp_input_state_type->m_mouse_button_is_pushed= false;
	}

	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{			
		if(iter->m_identifier == "AnimationPath")
		{
			m_ref_animation_path.m_index =  p_env->index_from_name(iter->m_str_value, p_env->mv_animation_path);
		}		
	}

	//temp	
	CCameraLocation location;
	m_local_elapsed_time= 0;
	location.m_position.set(0,0,0);
	location.m_yaw =0;
	location.m_pitch =0;
	location.m_roll =0;
	mv_location.push_back(location);
	m_animation_elapsed_time = 0;
	m_animation_period= 5;

	if(p_env->mv_animation_control_point.size()>=2) {						
		index_A = 0;
		index_B = 1;

		m_animation_period= p_env->mv_animation_control_point[index_B].m_time- p_env->mv_animation_control_point[index_A].m_time;
	}
	return true;
}



bool CEvent_UpdateViewerAnimate::Update(CEnvironment* p_env)
{	
	bool update=false;
	if(p_env->mp_input_state_type->m_toggle_cam_position) {
		p_env->mp_input_state_type->m_toggle_cam_position= false;		
		if(p_env->mv_animation_control_point.size()>=1) {
			m_index_control_point++;
			if(m_index_control_point>= p_env->mv_animation_control_point.size()) {
				m_index_control_point= 0;				
			}
			osg::Vec3d pos = p_env->mv_animation_control_point[m_index_control_point].m_control_point.getPosition();
			p_env->m_custom_camera.m_position = pos;
			update = true;
		}
	}
	
	switch(m_viewer_update_type)
	{
		case ViewerUpdateType_UpdateCamera:
		{
			switch(m_camera_update_type)
			{				
				case CameraUpdateType_AnimationPath:
				{					
					if(p_env->mv_animation_control_point.size()>=2)
					{						
						eTimeType m_time_type= TimeType_real;	
						m_animation_elapsed_time+= p_env->get_delta_time();					
						if(m_animation_elapsed_time> m_animation_period)
						{
							m_animation_elapsed_time= 0;
							index_A++;
							index_B++;
							if(index_B>= p_env->mv_animation_control_point.size()) {
								index_A= 0;	
								index_B= 1;				
							}
							m_animation_period= p_env->mv_animation_control_point[index_B].m_time- p_env->mv_animation_control_point[index_A].m_time;
						}													
						osg::Vec3d A = p_env->mv_animation_control_point[index_A].m_control_point.getPosition();
						osg::Vec3d B = p_env->mv_animation_control_point[index_B].m_control_point.getPosition();
						p_env->m_custom_camera.m_position = linear_interpolate(&A, &B, m_animation_period, m_animation_elapsed_time);
						update = true;
					}
					if(update)
					{
						osg::Matrixd CameraInvMatrix = p_env->mv_viewer[m_active_viewer_index].mp_viewer->getCamera()->getViewMatrix();
						osg::Matrixd CameraMatrix = p_env->m_custom_camera.update_position(10, CameraInvMatrix);
						p_env->mv_viewer[m_active_viewer_index].mp_viewer->getCamera()->setViewMatrix(CameraMatrix); 
					}
					break;
				}
				case CameraUpdateType_SetProjectionMatrix:
				{

					break;
				}
				case CameraUpdateType_ScrollThroughControlPoints:
				{					
					m_animation_elapsed_time+= p_env->get_delta_time();
					if(m_animation_elapsed_time> m_animation_period)
					{
						m_animation_elapsed_time= 0;
						if(p_env->mv_animation_control_point.size()>=1) {
							m_index_control_point++;
							if(m_index_control_point>= p_env->mv_animation_control_point.size()) {
								m_index_control_point= 0;				
							}
							osg::Vec3d pos = p_env->mv_animation_control_point[m_index_control_point].m_control_point.getPosition();
							p_env->m_custom_camera.m_position = pos;
							update = true;
						}
					}
					if(update)
					{
						osg::Matrixd CameraInvMatrix = p_env->mv_viewer[m_active_viewer_index].mp_viewer->getCamera()->getViewMatrix();
						osg::Matrixd CameraMatrix = p_env->m_custom_camera.update_position(10, CameraInvMatrix);
						p_env->mv_viewer[m_active_viewer_index].mp_viewer->getCamera()->setViewMatrix(CameraMatrix); 
					}
					break;
				}				
				case CameraUpdateType_FollowPath:
				{
					
	
					break;
				}
				case CameraUpdateType_SetViewMatrix:
				{
					p_env->m_custom_camera.m_x= 0;
					p_env->m_custom_camera.m_y= 0;
					p_env->m_custom_camera.m_z= 0;
					p_env->m_custom_camera.m_delta_mouse_x =0;
					p_env->m_custom_camera.m_delta_mouse_y =0;					
					

					//acceleration= force/ mass
					if(p_env->mp_input_state_type)
					{
						if(p_env->mp_input_state_type->m_forward)
						{							
							p_env->m_custom_camera.m_z+=1;
							p_env->mp_input_state_type->m_forward = false;
							update = true;
						}
						if(p_env->mp_input_state_type->m_backward) {							
							p_env->m_custom_camera.m_z-=1;
							p_env->mp_input_state_type->m_backward = false;
							update = true;
						}
						if(p_env->mp_input_state_type->m_left)	{							
							p_env->m_custom_camera.m_x+=1;
							p_env->mp_input_state_type->m_left = false;
							update = true;
						}
						if(p_env->mp_input_state_type->m_right)	{							
							p_env->m_custom_camera.m_x-=1;
							p_env->mp_input_state_type->m_right = false;
							update = true;
						}
						if(p_env->mp_input_state_type->m_drag)	{		
							p_env->mp_input_state_type->m_drag = false;
							if(!p_env->mp_input_state_type->m_mouse_is_moving)	{
								p_env->m_custom_camera.m_previous_mouse_x= p_env->mp_input_state_type->m_mouse_x;
								 p_env->m_custom_camera.m_previous_mouse_y= p_env->mp_input_state_type->m_mouse_y;								
							}
							p_env->mp_input_state_type->m_mouse_is_moving = true;
							 
							update = true;
						}
						else
						{
							p_env->mp_input_state_type->m_mouse_is_moving = false;
						}

						if(p_env->mp_input_state_type->m_mouse_push)
						{	
						     p_env->mp_input_state_type->m_mouse_push = false;
							 p_env->mp_input_state_type->m_mouse_button_is_pushed= true;

							
							 update = true;
						}
						else if(p_env->mp_input_state_type->m_mouse_release)
						{	
						     p_env->mp_input_state_type->m_mouse_release = false;
							 p_env->mp_input_state_type->m_mouse_button_is_pushed= false;						
						}	

						if(p_env->mp_input_state_type->m_mouse_is_moving)
						{
							p_env->m_custom_camera.m_delta_mouse_x = p_env->mp_input_state_type->m_mouse_x - p_env->m_custom_camera.m_previous_mouse_x;
							p_env->m_custom_camera.m_delta_mouse_y = p_env->mp_input_state_type->m_mouse_y - p_env->m_custom_camera.m_previous_mouse_y;	
							p_env->m_custom_camera.m_previous_mouse_x= p_env->mp_input_state_type->m_mouse_x;
							p_env->m_custom_camera.m_previous_mouse_y= p_env->mp_input_state_type->m_mouse_y;	
						}
					}
					if(update)
					{
						//osg::Matrixd CameraInvMatrix = p_env->mv_viewer[m_active_viewer_index].mp_viewer->getCamera()->getInverseViewMatrix();
						osg::Matrixd CameraInvMatrix = p_env->mv_viewer[m_active_viewer_index].mp_viewer->getCamera()->getViewMatrix();
						osg::Matrixd CameraMatrix = p_env->m_custom_camera.update_position(10, CameraInvMatrix);
						p_env->mv_viewer[m_active_viewer_index].mp_viewer->getCamera()->setViewMatrix(CameraMatrix); 
					}
					break;
				}
				case CameraUpdateType_SetViewMatrixLookAt:
				{
					if(p_env->mp_input_state_type)
					{
						if(p_env->mp_input_state_type->m_forward)
						{	
							Vec3d Veye;
							Vec3d Vcentre;
							Vec3d Vup;
							Veye.set(20,20,20);
							Vcentre.set(0,0,0);
							Vup.set(0,0,1);
							p_env->mv_viewer[m_active_viewer_index].mp_viewer->getCamera()->setViewMatrixAsLookAt(Veye, Vcentre, Vup);
						}
					}
					break;
				}
			}
			break;
		}
		case ViewerUpdateType_UpdateStats:
		{

			break;
		}
	}
	
	return true;
}
