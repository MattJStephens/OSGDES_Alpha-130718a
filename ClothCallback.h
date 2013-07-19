#pragma once

#include <osg/Node>
#include <osg/StateSet>
#include <osg/Geode>
#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/Program>
#include <osg/MatrixTransform>
#include "PxPhysics.h"

/* cloth update callback */
struct ClothUpdateCallback : public osg::Drawable::UpdateCallback
{
public:
	ClothUpdateCallback(PxCloth* cloth) : _cloth(cloth)
	{
	
	}
	/* need to get control over individual vertices */
    virtual void update(osg::NodeVisitor*, osg::Drawable* drawable)
    {
		PxClothMeshDesc desc;
		
		PxMeshData msh = _cloth->getMeshData();
		
		PxVec3* p = (PxVec3*)msh.verticesPosBegin;
		int* n = (int*)msh.numVerticesPtr;

		if (n==0) return;
		if (*n>0)
		{
			PxVec3 *buff = (PxVec3*)malloc(sizeof(PxVec3) * (*n));
			if (buff)
			{
					_cloth->getPositions(buff);
					osg::Geometry* geo = dynamic_cast<osg::Geometry*>(drawable);
				if (geo && p)
				{
					osg::Vec3Array* vertices = new osg::Vec3Array;			
					for (int i = 0; i < *n; i++) 
					{			
						vertices->push_back(osg::Vec3f(buff->x, buff->z, buff->y));
						buff++;
					}	
					geo->setVertexArray(vertices);
				}
			}
		}							
    }
private:
	PxCloth* _cloth;
};
