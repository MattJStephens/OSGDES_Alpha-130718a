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
// CEvent_UpdateViewer
///
//OSG world coordinate system (where X is east, Y is north and Z is up) 
//and OpenGl (where X is to the right, Y is up, and Z is coming out of the screen). 
bool CEvent_UpdateViewer::Create(CEventDesign* p_design, CEnvironment* p_env)
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

	if(p_env->mp_input_state_type!= NULL)
	{
		p_env->mp_input_state_type->m_mouse_push =false;
		p_env->mp_input_state_type->m_mouse_release =false;
		p_env->mp_input_state_type->m_mouse_active= false;
		p_env->mp_input_state_type->m_mouse_is_moving= false;
		p_env->mp_input_state_type->m_mouse_button_is_pushed= false;
		p_env->mp_input_state_type->m_middle_mouse_button= false;		
	}
	
	CCameraLocation location;
	m_local_elapsed_time= 0;
	location.m_position.set(0,0,0);
	location.m_yaw =0;
	location.m_pitch =0;
	location.m_roll =0;
	mv_location.push_back(location);
	return true;
}



bool CEvent_UpdateViewer::Update(CEnvironment* p_env)
{	
	switch(m_viewer_update_type)
	{
		case ViewerUpdateType_UpdateCamera:
		{
			switch(m_camera_update_type)
			{
				case CameraUpdateType_SetProjectionMatrix:
				{

					break;
				}
				case CameraUpdateType_FollowPath:
				{					
					if(m_index_location< mv_location.size())
					{	
						CCameraLocation location = mv_location[m_index_location];						
						osg::Quat quat(location.m_yaw, osg::Z_AXIS, location.m_pitch, osg::Y_AXIS, location.m_roll, osg::Y_AXIS);						
						osg::Matrixd CameraRotation = osg::Matrix::rotate(quat);
						osg::Matrixd CameraTrans = osg::Matrix::translate(location.m_position);
						osg::Matrixd CameraMatrix = CameraRotation * CameraTrans;						
						p_env->mv_viewer[m_active_viewer_index].mp_viewer->getCamera()->setViewMatrix(CameraMatrix); 
					}
	
					break;
				}
				case CameraUpdateType_SetViewMatrix:
				{
					p_env->m_custom_camera.m_x= 0;
					p_env->m_custom_camera.m_y= 0;
					p_env->m_custom_camera.m_z= 0;
					p_env->m_custom_camera.m_delta_mouse_x =0;
					p_env->m_custom_camera.m_delta_mouse_y =0;					
					bool update=false;
					if(p_env->mp_input_state_type)
					{
						if(p_env->mp_input_state_type->m_forward)
						{							
							p_env->m_custom_camera.m_z+=1;
							p_env->mp_input_state_type->m_forward = false;
							update = true;
						}
						if(p_env->mp_input_state_type->m_backward)
						{							
							p_env->m_custom_camera.m_z-=1;
							p_env->mp_input_state_type->m_backward = false;
							update = true;
						}
						if(p_env->mp_input_state_type->m_left)
						{							
							p_env->m_custom_camera.m_x+=1;
							p_env->mp_input_state_type->m_left = false;
							update = true;
						}
						if(p_env->mp_input_state_type->m_right)
						{							
							p_env->m_custom_camera.m_x-=1;
							p_env->mp_input_state_type->m_right = false;
							update = true;
						}
						if(p_env->mp_input_state_type->m_drag)
						{		
							p_env->mp_input_state_type->m_drag = false;
							if(!p_env->mp_input_state_type->m_mouse_is_moving)
							{
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
