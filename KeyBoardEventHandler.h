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

#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

#include <osgGA/GUIEventHandler>
#include <osg/PositionAttitudeTransform>

class CInputDeviceStateType
{
public:
    CInputDeviceStateType();  
	bool m_forward;
	bool m_backward;
	bool m_left;
	bool m_right;
	bool m_mouse_active;
	bool m_mouse_push;
	bool m_mouse_release;
	bool m_mouse_move;
	bool m_mouse_is_moving;
	bool m_drag;
	bool m_static;
	bool m_none;
	bool m_scroll;
	bool m_double_click;
	bool m_left_mouse_button;
	bool m_middle_mouse_button;
	bool m_right_mouse_button;
	bool m_emitter;
	int m_mouse_button;
	bool m_mouse_button_is_pushed;
	float m_mouse_x;
	float m_mouse_y;
	bool m_toggle_cam_position;
	bool m_start_animation;
	bool m_stop_animation;
};

class CKeyboardEventHandler : public osgGA::GUIEventHandler {
public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&);
	 virtual void accept(osgGA::GUIEventHandlerVisitor& v)   { v.visit(*this); };

    CKeyboardEventHandler(CInputDeviceStateType* tids)
    {
        p_input_device_state = tids;
    }
// ...
protected:
    CInputDeviceStateType* p_input_device_state;
};


//
//class CUpdatePosCallback : public osg::NodeCallback 
//{
//    public:
//       //CUpdatePosCallback::CUpdatePosCallback(CInputDeviceStateType* p_tankIDevState) : rotation(0.0) , tankPos(-15.,0.,0.)
//		CUpdatePosCallback::CUpdatePosCallback(CInputDeviceStateType* p_tankIDevState) 
//       {
//          p_input_device_state = p_tankIDevState;
//       }
//       virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
//       {
//          osg::PositionAttitudeTransform* pat =  dynamic_cast<osg::PositionAttitudeTransform*> (node);
//          if(pat)
//          {
//             if (p_input_device_state->moveFwdRequest)
//             {
//                tankPos.set(tankPos.x()+.01,0,0);
//                pat->setPosition(tankPos);
//             }
//          }
//          traverse(node, nv); 
//       }
//    protected:
//       osg::Vec3d tankPos;
//       CInputDeviceStateType* p_input_device_state; 
//};




//
//
//class keyboardEventHandler : public osgGA::GUIEventHandler
//{
//public:
//
//   typedef void (*functionType) ();
//   enum keyStatusType
//   {
//      KEY_UP, KEY_DOWN 
//   };
//
//   // A struct for storing current status of each key and 
//   // function to execute. Keep track of key's state to avoid
//   // redundant calls. (If the key is already down, don't call the
//   // key down method again.)
//   struct functionStatusType
//   {
//      functionStatusType() {keyState = KEY_UP; keyFunction = NULL;}
//      functionType keyFunction;
//      keyStatusType keyState;
//   };
//
//   // Storage for list of registered key, function to execute and 
//   // current state of key.
//   typedef std::map<int, functionStatusType > keyFunctionMap;
//
//   // Function to associate a key with a function. If the key has not
//   // been previously registered, key and function are added to the
//   // map of 'key down' events and 'true' is returned. Otherwise, no
//   // entry made and false is returned.
//   bool addFunction(int whatKey, functionType newFunction);
//
//   // Overloded version allows users to specify if the function should 
//   // be associated with KEY_UP or KEY_DOWN event.
//   bool addFunction(int whatKey, keyStatusType keyPressStatus, 
//                                         functionType newFunction);
//
//   // The handle method checks the current key down event against 
//   // list of registered key/key status entries. If a match is found 
//   // and it's a new event (key was not already down) corresponding 
//   // function is invoked.
//   virtual bool handle(const osgGA::GUIEventAdapter& ea,
//                                         osgGA::GUIActionAdapter&);
//
//   // Overloaded accept method for dealing with event handler visitors
//   virtual void accept(osgGA::GUIEventHandlerVisitor& v)
//                                               { v.visit(*this); };
//
//protected:
//
//   // Storage for registered 'key down' methods and key status
//   keyFunctionMap keyFuncMap;
//
//   // Storage for registered 'key up' methods and key status
//   keyFunctionMap keyUPFuncMap;
//};



#endif

