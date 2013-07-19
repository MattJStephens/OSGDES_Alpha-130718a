/* Sphere type actor - rigid body, Janusz Goldasz */
#include "Sphere.h"

// create sphere
osg::Geode* Sphere::init(NxScene* scene, float radius, osg::Vec3f position)
{
	osg::Geode* _geo_sphere = (osg::Geode*)osgDB::readNodeFile(
		"../models/sphere.osg");
	if (_geo_sphere)
	{
		NxBodyDesc BodyDesc;
		BodyDesc.angularDamping	= 0.0f;
		
		NxSphereShapeDesc SphereDesc;
		SphereDesc.radius = radius;

		NxActorDesc ActorDesc;
		ActorDesc.shapes.pushBack(&SphereDesc);
		ActorDesc.body			= &BodyDesc;
		ActorDesc.density		= 10.0f;
		ActorDesc.globalPose.t  = NxVec3(position.x(), position.y(), position.z());
		NxActor* newActor = scene->createActor(ActorDesc);
		newActor->userData = (void*)0;

		_actor = newActor;

	}

	return _geo_sphere;
}

osg::Group* distributeSpheres(NxScene* gScene, float terrainSize, int n)
{
	srand( (unsigned)time( NULL ) );

	osg::Group* _group = new osg::Group;

	double r = 1.0f;
	for(int i=0; i<n; i++)
	{
		osg::Vec3f _position;
		_position.x() = randMinMax(0, terrainSize);
		_position.y() = randMinMax(0.5*terrainSize, terrainSize);
		_position.z() = randMinMax(0,terrainSize);
		_group->addChild(new Sphere(gScene, r, _position));
	}
	return _group;
}
