#ifndef _PICKER_H_
#define _PICKER_H_

#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include "NxPhysics.h"

/* force/speed inputs */
const float fm = 1.0e+6;
const float vm = 30.0;
const float df = 0.01;
const float dv = 0.01;

extern bool bRun;
/* picker class - rigid actors only, cloth picking not implemented yet */
class Picker : public osgGA::GUIEventHandler 
{
public: 

	Picker(NxScene* scene):
	  _scene(scene), _mx(0),_my(0), 
		  _useForce(true), _curForce(fm), 
		  _curSpeed(vm), _useRayCast(false) {}

	  ~Picker() {}

	  bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	  {
		  osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		  if (!viewer) return false;

		  switch(ea.getEventType())
		  {
		  case(osgGA::GUIEventAdapter::KEYUP):
			  {
				  if (ea.getKey()=='r')
				  {
					bRun = !bRun;					
				  }
				  if (ea.getKey()=='v') // change shooting/force mode
				  {
					  _useForce = !_useForce;
				  }
				  if (ea.getKey()=='a') // increase force or speed
				  {
					  _curForce = (_useForce==true)? (_curForce + _curForce*df) : fm;
					  _curSpeed = (_useForce!=true)? (_curSpeed + _curSpeed*dv) : vm;
				  }
				  if (ea.getKey()=='q') // decrease force or speed
				  {
					  _curForce = (_useForce==true)? (_curForce - _curForce*df) : fm;
					  _curSpeed = (_useForce!=true)? (_curSpeed - _curSpeed*dv) : vm;

					  _curForce = __max(0, _curForce);
					  _curSpeed = __max(0, _curSpeed);
				  }
				  if (ea.getKey()=='s') // shoot
				  {
					  shoot(ea, viewer);
				  }  
				  return false;
			  }
		  case(osgGA::GUIEventAdapter::PUSH):
		  case(osgGA::GUIEventAdapter::MOVE):
			  {
				  _mx = ea.getX();
				  _my = ea.getY();
				  return false;
			  }
		  case(osgGA::GUIEventAdapter::DOUBLECLICK): // pick on double-click
			  {
				  if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
				  {
					  if (_mx == ea.getX() && _my == ea.getY())
					  {
						  pick(ea,viewer);
					  }					  
				  }  
				  return true;
			  }

		  default:
			  return false;
			  }
		  }
		  // picking
		  void pick(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer);
		  // shooting
		  void shoot(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer);
		  void trySoftBodyHit(NxVec3 gDir);
		  void tryRigidBodyHit(NxShape* shape, NxVec3 gDir, NxRaycastHit hit);
		  NxRay convertToCameraDir(osg::Vec3f eye, osg::Vec3f center);
	  private:		  
		  float _mx, _my;
		  NxScene* _scene;
		  bool _useForce;
		  float _curForce;
		  float _curSpeed;
		  bool _useRayCast;		  
	  };

#endif