#pragma once

#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/Vec3>
#include "NxPhysics.h"

/*  Link class for use with Joints - the implementation is probably not quite correct. 
	I need to look into that again. 
*/

struct LinkUpdateCallback : public osg::Drawable::UpdateCallback
{
	LinkUpdateCallback(NxActor* a, NxActor* b, osg::Vec3f anchorPos) : 
_a(a), _b(b), _anchorPos(anchorPos)
{

}

virtual void update(osg::NodeVisitor*, osg::Drawable* drawable)
{
	osg::Geometry* geom = dynamic_cast<osg::Geometry*>(drawable);
	if (geom)
	{
		osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
		if (_a && _b)
		{
			float ma[16];
			// get the transformation matrix
			_a->getGlobalPose().getColumnMajor44(ma);
			(*vertices)[0].set(osg::Vec3f(ma[12],ma[14],ma[13]));

			float mb[16];
			// get the transformation matrix
			_b->getGlobalPose().getColumnMajor44(mb);
			(*vertices)[1].set(osg::Vec3f(mb[12],mb[14],mb[13]));
		}

		if (_a && !_b)
		{
			float ma[16];
			// get the transformation matrix
			_a->getGlobalPose().getColumnMajor44(ma);
			(*vertices)[0].set(osg::Vec3f(ma[12],ma[14],ma[13]));

			(*vertices)[1].set(_anchorPos);
		}

		if (!_a && _b)
		{
			float mb[16];
			// get the transformation matrix
			_b->getGlobalPose().getColumnMajor44(mb);
			(*vertices)[0].set(osg::Vec3f(mb[12],mb[14],mb[13]));

			(*vertices)[1].set(_anchorPos);
		}
		geom->setVertexArray(vertices);
	}
}
private:
	NxActor* _a;
	NxActor* _b;
	osg::Vec3f _anchorPos;
};


class Link : public osg::Geometry
{
public:
	Link(NxActor* a, NxActor* b, osg::Vec3f anchorPos, osg::Vec4f color) :
	  _a(a), _b(b), _anchorPos(anchorPos), _color(color)
	  {
		  osg::Vec3Array* vertices = new osg::Vec3Array(2);
		  if (_a && _b)
		  {
			  float ma[16];
			  // get the transformation matrix
			  _a->getGlobalPose().getColumnMajor44(ma);
			  (*vertices)[0].set(osg::Vec3f(ma[12],ma[14],ma[13]));

			  float mb[16];
			  // get the transformation matrix
			  _b->getGlobalPose().getColumnMajor44(mb);
			  (*vertices)[1].set(osg::Vec3f(mb[12],mb[14],mb[13]));
		  }

		  if (_a && !_b)
		  {
			  float ma[16];
			  // get the transformation matrix
			  _a->getGlobalPose().getColumnMajor44(ma);
			  (*vertices)[0].set(osg::Vec3f(ma[12],ma[14],ma[13]));

			  (*vertices)[1].set(_anchorPos);
		  }

		  if (!_a && _b)
		  {
			  float mb[16];
			  // get the transformation matrix
			  _b->getGlobalPose().getColumnMajor44(mb);
			  (*vertices)[0].set(osg::Vec3f(mb[12],mb[14],mb[13]));

			  (*vertices)[1].set(_anchorPos);
		  }
		  setVertexArray(vertices);
		  osg::Vec4Array* colors = new osg::Vec4Array;
		  colors->push_back(_color);
		  setColorArray(colors);
		  setColorBinding(osg::Geometry::BIND_OVERALL);

		  osg::Vec3Array* normals = new osg::Vec3Array;
		  normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
		  setNormalArray(normals);
		  setNormalBinding(osg::Geometry::BIND_OVERALL);

		  addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));

		  setUpdateCallback(new LinkUpdateCallback(_a, _b, _anchorPos));

		  osg::StateSet* _stateset = this->getOrCreateStateSet();
		  _stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		  setStateSet(_stateset);
	  }
public:
	~Link(void){};
private:
	NxActor* _a;
	NxActor* _b;

	osg::Vec3f _anchorPos;
	osg::Vec4f _color;
};
