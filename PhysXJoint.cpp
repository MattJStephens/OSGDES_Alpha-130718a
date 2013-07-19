/* Joint implementation */
#include "PhysXJoint.h"
#include "Link.h"

void setJointParameters(NxJointDesc& desc, NxActor* a, NxActor* b, osg::Vec3f jointPos)
{
    desc.setToDefault();
    desc.actor[0] = a;
    desc.actor[1] = b;
    desc.setGlobalAnchor(
		NxVec3( jointPos.x(), jointPos.z(), jointPos.y()));
}

osg::Group* CPhysXJoint(NxScene* scene, osg::Vec3f anchorPos, float length)
{
	osg::Group* group = new osg::Group;
	
	float size=1.0f;
//	CPhysXCube* mass = new CPhysXCube( scene, osg::Vec3f(size, size, size), osg::Vec3f( anchorPos.x(), anchorPos.y(), anchorPos.z() - length), "cube.3ds", "3ds");
	CPhysXCube* mass;
	

	NxSphericalJointDesc desc;
	setJointParameters( desc, mass->getActor(), NULL, anchorPos);
	scene->createJoint(desc);
	// link
	Link* link = new Link( mass->getActor(), NULL, anchorPos, osg::Vec4f(1.0,1.0,0.0,1.0));
	osg::Geode* geolink = new osg::Geode();
	geolink->addDrawable(link);

	group->addChild(mass->mp_matrix_transform);
	group->addChild(geolink);
	return group;
}
