#include "ControllerHandler.h"
#include <iostream>
#define COLLIDABLE_MASK	(1<<GROUP_COLLIDABLE_NON_PUSHABLE) | (1<<GROUP_COLLIDABLE_PUSHABLE)


void CControllerHandler::triggerMotion(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer)
{
	_prevtime = ea.getTime();

	osg::Vec3f eye, center, pos , up(0,0,1);
	double dist(4.0);

	pos = _pController->getCurrentPos(0);

	viewer->getCamera()->getViewMatrixAsLookAt(eye, center , up, dist);

	osg::Vec3f _tmp = pos - eye;
	_dispVec = NxVec3(_tmp.x(), _tmp.z(), _tmp.y());
	_dispVec.y = 0.0;  _dispVec.normalize();
	
}

void CControllerHandler::triggerJump(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer)
{	

	// to do

}

bool CControllerHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (!viewer) return false;

	switch(ea.getEventType())
	{	
	case(osgGA::GUIEventAdapter::KEYUP):
		{
			if (ea.getKey()=='s') // stop
			{
				_allowMotion = !_allowMotion;
				return true;
			}			
			return false;
		}
	case(osgGA::GUIEventAdapter::FRAME):
		{
			_mx = ea.getX();
			_my = ea.getY();

			_curtime = ea.getTime();
			_delta_t = _curtime - _prevtime;
			_dispVec = (_allowMotion==true) ? _dispVec : NxVec3(0,0,0);
			_pController->moveCharacter( 0, _scene, _dispVec, __min(_delta_t, 1.0), COLLIDABLE_MASK , 0.0 );
			_prevtime = _curtime;

			return false;
		}
	case(osgGA::GUIEventAdapter::DOUBLECLICK): // move on double-click
		{
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				if (_mx == ea.getX() && _my == ea.getY())
				{					
					triggerMotion(ea,viewer);
					return true;
				}					  
			} 
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				if (_mx == ea.getX() && _my == ea.getY())
				{
					triggerJump(ea,viewer);
					return true;
				}					  
			}
			return false;
		}

	default:
		return false;
	}
}