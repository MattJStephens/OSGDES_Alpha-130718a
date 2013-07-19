/* Picker class implementation - Janusz Goldasz */
#include "Picker.h"
#include "PhysXSphere.h"
//#include "RayCastReport.h"

void Picker::shoot(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer)
{
	osgUtil::LineSegmentIntersector* picker;

	osg::Viewport* viewport = viewer->getCamera()->getViewport();
	float mx = viewport->x() + (int)((float)viewport->width()*(ea.getXnormalized()*0.5f+0.5f));
	float my = viewport->y() + (int)((float)viewport->height()*(ea.getYnormalized()*0.5f+0.5f));

	if (!_useRayCast)
	{
		picker = new osgUtil::LineSegmentIntersector( osgUtil::Intersector::WINDOW, mx, my );

		osgUtil::IntersectionVisitor iv(picker);

		viewer->getCamera()->accept(iv);

		if (picker->containsIntersections())
		{
			osgUtil::LineSegmentIntersector::Intersection intersection = picker->getFirstIntersection();

			osg::Vec3f point = intersection.getWorldIntersectPoint();
			osg::Vec3f eye, center, up(0,0,1);
			double dist(4.0);

			viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up, dist);
			osg::Vec3f dir;
			dir = point - eye;			  
			dir.normalize();

			CPhysXSphere* _sphere = new CPhysXSphere(_scene, 1.0f, osg::Vec3f(eye.x(), eye.z(), eye.y()));
			if (!_useForce)
			{
				NxVec3 _speed(_curSpeed*NxVec3(dir.x(), dir.z(), dir.y()));
				_sphere->getActor()->setLinearVelocity(_speed);
			}
			else
			{
				NxVec3 _force(_curForce*NxVec3(dir.x(), dir.z(), dir.y()));
				_sphere->getActor()->addForce(_force);
			}
			viewer->getSceneData()->asGroup()->addChild(_sphere);
		}
	}
	else
	{
		// NxRayCast
		// use NxRayCast
		static osg::Vec3f prev_eye;
		osg::Vec3f eye, center, dir, up(0,0,1);
		float dist(1.0);
		viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);

		dir = center - eye;
		dir.normalize();


		CPhysXSphere* _sphere = new CPhysXSphere(_scene, 1.0f, osg::Vec3f(eye.x(), eye.z(), eye.y()));
		if (!_useForce)
		{
			NxVec3 _speed(_curSpeed*NxVec3(dir.x(), dir.z(), dir.y()));
			_sphere->getActor()->setLinearVelocity(_speed);
		}
		else
		{
			NxVec3 _force(_curForce*NxVec3(dir.x(), dir.z(), dir.y()));
			_sphere->getActor()->addForce(_force);
		}
		viewer->getSceneData()->asGroup()->addChild(_sphere);

	}
}

void Picker::pick(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer)
{
	osg::Node* scene = viewer->getSceneData();
	if (!scene) return;

	osg::notify(osg::NOTICE)<<std::endl;

	osg::Node* node = 0;
	osg::Group* parent = 0;

	osgUtil::LineSegmentIntersector* picker;
	osg::Viewport* viewport = viewer->getCamera()->getViewport();
	float mx = viewport->x() + (int)((float)viewport->width()*(ea.getXnormalized()*0.5f+0.5f));
	float my = viewport->y() + (int)((float)viewport->height()*(ea.getYnormalized()*0.5f+0.5f));
	if (!_useRayCast)
	{
		picker = new osgUtil::LineSegmentIntersector( osgUtil::Intersector::WINDOW, mx, my );

		osgUtil::IntersectionVisitor iv(picker);

		viewer->getCamera()->accept(iv);

		if (picker->containsIntersections())
		{
			osgUtil::LineSegmentIntersector::Intersection intersection = picker->getFirstIntersection();

			osg::Vec3f point = intersection.getWorldIntersectPoint();
			osg::Vec3f eye, center, up;
			double dist(4.0);

			viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up, dist);
			NxRay ray = convertToCameraDir(eye, point);

			NxRaycastHit _hit;
			NxVec3 gDir = ray.dir;
			NxShape* _shape = _scene->raycastClosestBounds(ray, NX_DYNAMIC_SHAPES, _hit);
			if (_shape && _hit.shape)
			{

				tryRigidBodyHit(_shape, ray.dir, _hit);
			}

		}
	}
	else
	{
		// use NxRayCast
		osg::Vec3f eye, center, dir, up(0,0,1);
		float dist(2.0);
		viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);
		NxRay ray = convertToCameraDir(eye, center);		
		NxRaycastHit _hit;


		NxShape* shape = _scene->raycastClosestBounds(ray, NX_DYNAMIC_SHAPES, _hit);
		if (shape && _hit.shape)
		{

			tryRigidBodyHit(shape, ray.dir, _hit);
		}
		else
		{		
			trySoftBodyHit(ray.dir);
		}

	}
}


void Picker::trySoftBodyHit(NxVec3 gDir)
{
	// try soft bodies
	NxVec3 hit;
	NxU32 vertexId;
	int gSelectedSoftBodyVertex = -1;
	NxReal distance = NX_MAX_REAL;
	NxSoftBody *gSelectedSoftBody = NULL;
	NxRay ray;

	NxSoftBody **softBodies = _scene->getSoftBodies();	
	for (NxU32 i = 0; i < _scene->getNbSoftBodies(); i++) 
	{
		if (softBodies[i]->raycast(ray, hit, vertexId)) 
		{
			if(hit.magnitude() < distance)
			{
				gSelectedSoftBody = softBodies[i];
				gSelectedSoftBodyVertex = vertexId;
				if (gSelectedSoftBody)
				{
					if (_useForce)
					{
						NxVec3 _force(_curForce*gDir);
						gSelectedSoftBody->addForceAtVertex(_force,vertexId);
					}
					else
					{
						NxVec3 _speed(_curSpeed*gDir);
						gSelectedSoftBody->setVelocity(_speed, vertexId);
					}
				}
			}
		}
	}
}

void Picker::tryRigidBodyHit(NxShape* shape, NxVec3 gDir, NxRaycastHit hit)
{
	NxActor* _selection = &shape->getActor();
	if (_selection->isDynamic())
	{
		if (_useForce)
		{					
			NxVec3 _force(_curForce*gDir);			
			_selection->addForceAtPos( _force, NxVec3(hit.worldImpact));
		}
		else
		{
			NxVec3 _speed(_curSpeed*gDir);			
			_selection->setLinearVelocity(_speed);
		}
	}
}

NxRay Picker::convertToCameraDir(osg::Vec3f eye, osg::Vec3f center)
{

	osg::Vec3f dir = center - eye;

	NxRay _worldRay;
	NxVec3 gDir(dir.x(), dir.z(), dir.y());
	gDir.normalize();

	_worldRay.orig.x	= eye.x();
	_worldRay.orig.y	= eye.z();
	_worldRay.orig.z	= eye.y();

	_worldRay.dir	= gDir;
	_worldRay.dir.normalize();

	return _worldRay;

}