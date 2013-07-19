#pragma once

#include <osg/Node>
#include <osg/StateSet>
#include <osg/Geode>
#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/Program>
#include <osg/MatrixTransform>
#include "NxPhysics.h"

/* cloth update callback */
struct ClothUpdateCallback : public osg::Drawable::UpdateCallback
{
public:
	ClothUpdateCallback(NxCloth* cloth) : _cloth(cloth)
	{
	
	}
	/* need to get control over individual vertices */
    virtual void update(osg::NodeVisitor*, osg::Drawable* drawable)
    {
		NxClothMeshDesc desc;
		
		NxMeshData msh = _cloth->getMeshData();
		
		NxVec3* p = (NxVec3*)msh.verticesPosBegin;
		int* n = (int*)msh.numVerticesPtr;

		if (n==0) return;
		if (*n>0)
		{
			NxVec3 *buff = (NxVec3*)malloc(sizeof(NxVec3) * (*n));
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
	NxCloth* _cloth;
};
