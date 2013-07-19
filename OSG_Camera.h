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

#ifndef _Camera_
#define _Camera_

#include <string>
#include <fstream>
#include <osg/MatrixTransform>
#include <osgParticle/Particle>
#include <osgViewer/Viewer>

enum eNavigateUpAxis { NavigateUpAxisX, NavigateUpAxisY, NavigateUpAxisZ, NavigateUpAxisNegX, NavigateUpAxisNegY, NavigateUpAxiNegsZ };

class CCustomCamera
{
	public:
		CCustomCamera();
		std::string m_name;
		std::string m_file_name;
		osg::Vec3d m_xyz_acceleration;
		osg::Matrixd m_CameraMatrix;
		osg::Matrixd m_cameraRotation;
		osg::Matrixd m_cameraTrans;
		osg::Vec3d m_position;
		osg::Vec3d m_velocity;
		osg::Vec3d m_acceleration;
		float m_previous_mouse_x;
		float m_previous_mouse_y;
		float m_delta_mouse_x;
		float m_delta_mouse_y;
		float m_yaw;
		float m_pitch;
		float m_roll;
		float m_x;
		float m_y;
		float m_z;
		void intialise(void);
		osg::Matrixd update_position(float delta_t, osg::Matrixd CameraInvMatrix);	
		eNavigateUpAxis m_up_axis;
		float m_velocity_coefficient;
		float m_mouse_coefficient;		
};
#endif