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

#include "KeyBoardEventHandler.h"

CInputDeviceStateType::CInputDeviceStateType()
{
	m_forward =false;
	m_backward =false;
	m_left =false;
	m_right =false;
	m_toggle_cam_position = false;
	m_start_animation = false;
	m_stop_animation = false;
	m_emitter = false;
}
 
 bool CKeyboardEventHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{	
	switch(ea.getEventType())
	{
		case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			switch(ea.getKey())
			{
				case 'w':
					p_input_device_state->m_forward = true;
					return false;
					break;
				case 's':
					p_input_device_state->m_backward = true;
					return false;
					break;
				case 'a':
					p_input_device_state->m_left = true;
					return false;
					break;
				case 'd':
					p_input_device_state->m_right = true;
					return false;
					break;
				case 't':
					p_input_device_state->m_toggle_cam_position = true;
					return false;
					break;
				case ' ':
					p_input_device_state->m_emitter = true;
					return false;
					break;
				default:
					return false;
			} 
			break;		
		}
		
		case(osgGA::GUIEventAdapter::RELEASE):
		{
			p_input_device_state->m_mouse_release= true;
			//return false;
			break;
		}
		case(osgGA::GUIEventAdapter::MOVE):
		{
			p_input_device_state->m_mouse_move= true;
			//p_input_device_state->m_mouse_x = ea.getX();
			//p_input_device_state->m_mouse_y = ea.getY();

			//return false;
			break;
		}
		case(osgGA::GUIEventAdapter::PUSH):
		{
			p_input_device_state->m_mouse_push= true;
			p_input_device_state->m_mouse_button = ea.getButton();
			//return false;
			break;
		}
		case(osgGA::GUIEventAdapter::DRAG):
		{
			p_input_device_state->m_drag= true;
			p_input_device_state->m_mouse_x = ea.getX();
			p_input_device_state->m_mouse_y = ea.getY();
			//return false;
			break;
		}		
		case(osgGA::GUIEventAdapter::NONE):
		{
			p_input_device_state->m_none= true;
			//return false;
			break;
		}
		case(osgGA::GUIEventAdapter::SCROLL):
		{
			p_input_device_state->m_scroll= true;
			//return false;
			break;
		}		
		case(osgGA::GUIEventAdapter::DOUBLECLICK):
		{
			p_input_device_state->m_double_click= true;
			//return false;
			break;
		}		
		default:
		return false;
	}

	return false;
}
  
//
// bool CKeyboardEventHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
//{
//	switch(ea.getEventType())
//	{
//		case(osgGA::GUIEventAdapter::KEYDOWN):
//			{
//				switch(ea.getKey())
//				{
//				case 'w':
//				std::cout << " w key pressed" << std::endl;
//				return false;
//				break;
//				default:
//				return false;
//				} 
//			}
//		default:
//			return false;
//	}
//}
//
//if(m_pSilk->m_direct_input.is_key_down(DIK_W)) {	  
//			XYZ_acceleration.z+= (m_design.m_fwd_Nps* elapsed_time)/m_pSilk->m_locator_hive.mv_locator[*dw_iter].get_mass();
//		}
//		if(m_pSilk->m_direct_input.is_key_down(DIK_S)) {		
//			XYZ_acceleration.z-= (m_design.m_bkwd_Nps* elapsed_time)/m_pSilk->m_locator_hive.mv_locator[*dw_iter].get_mass();
//		}	
//		if(m_pSilk->m_direct_input.is_key_down(DIK_D)) {	  
//			XYZ_acceleration.x+= (m_design.m_right_Nps* elapsed_time)/m_pSilk->m_locator_hive.mv_locator[*dw_iter].get_mass();
//		}
//		if(m_pSilk->m_direct_input.is_key_down(DIK_A)) {	  
//			XYZ_acceleration.x-= (m_design.m_left_Nps* elapsed_time)/m_pSilk->m_locator_hive.mv_locator[*dw_iter].get_mass();
//		}


//...........,..............,:......,,....,..............,................,...,,,,
//....,.........,.........,I+$$7.....................,........,,,,,,,,,,,,,:,,,,:,
//..,,....................,Z~:ON.........,.................................,..,...
//......,,,,,,.,...........,ZONN,...,..,...........................,.,.....,..,..,
//.........,,.,,,,.,,.......INMN?.....,............Matthew James Stephens 2011..,,
//..........................IOMD8.............................................,,.,
//..........................=$NMD,.........................................,,,,,,,
//..........................,78D$.......,................,,...............,...,,,,
//..,,....................,,.OZZZOD7.=Z$ZOZ$?,..,,,.,......,..,.......,..,,,,,.,,,
//....................,,....O$$ZZZZ$O$==7MMZ$7~..,....,,,,,,,,,..,..........,,,,,,
//,,,...........,......,,.=7$$$$77$7$I++?$7OI.....,.,,,,,,.................,,..,,,
//,,.,,,,.,........,,.,.,+?I7?77$$7I7?~~=IOZZ..,,,,,,,,,.......,..,,,.,..,,,,,,,,,
//,,..,..,....,........,$$?I7I?7ZOZ$I7+,++$Z$.,,,,,,.~,,,,,,.,..,,,,,,,,,,,,..,,,,
//.....................D.=+:$+777ZN$?+=~~?$:I8Z8DMDMO~,,..,,,......,...,,....,..,,
//............,,,,...:NZ.,$$DN888OM8$ZOOOO$7~,....,,,,,...,...,,,.....,..........,
//..............,...........,,,,MMO$OOO8O8888OZI~.,,,.,,,,......,,,,.............,
//........,.,,.,,,,,............+.?I,,,,,,,~7$::.:.,...,,,,,::,,.,.,,,...,.....,,,
//.............................. .....,.............,,,...............,,,,,..,.,,,
//..............................................,...,.,N$..............,,,....,,,,
//...............................................,,....~7?,..............,,:..,,,.
//...............................................................,..............,,