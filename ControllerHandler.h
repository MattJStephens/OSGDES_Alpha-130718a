#ifndef __CONTROLLERHANDLER_H_
#define __CONTROLLERHANDLER_H_
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include "NxPhysics.h"
#include "PhysXCharacterController.h"

/* character controller event handler */
class CControllerHandler : public osgGA::GUIEventHandler 
{
public: 

	CControllerHandler(NxScene* scene, CPhysXCharacterController* pController):
	  _scene(scene), _pController(pController), _mx(0),_my(0), 
		  _allowJump(false), _allowMotion(true), 
		  _dispVec(NxVec3(0.0)), _prevtime(0.0), 
		  _curtime(0.0),  _delta_t(0.0)
	  {
					

	  }

	  ~CControllerHandler() {}

	  bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);	  
	  // move
	  void triggerMotion(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer);
	  // jump
	  void triggerJump(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer);	
private:		  
	float _mx, _my;
	NxScene* _scene;
	bool _useRayCast;	

	bool _allowJump;
	bool _allowMotion;

	NxVec3 _dispVec;

	double _curtime;
	double _prevtime;
	double _delta_t;
	double _jumptime;

	CPhysXCharacterController*  _pController;
};

#endif