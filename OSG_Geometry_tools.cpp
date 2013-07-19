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

#include "OSG_Geometry_tools.h"

#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osg/Texture2D>

#include <osg/PointSprite>
#include <osg/BlendFunc>
#include <osg/StateAttribute>
#include <osg/Point>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/GLExtensions>
#include <osg/TexEnv>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>

using namespace osg;

///-///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///- AnimationPath
///-///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::AnimationPath* createAnimationPath(const osg::Vec3& center,float radius, double looptime)
{
    // set up the animation path 
    osg::AnimationPath* animationPath = new osg::AnimationPath;
    animationPath->setLoopMode(osg::AnimationPath::LOOP);
    
    int numSamples = 40;
    float yaw = 0.0f;
    float yaw_delta = 2.0f*osg::PI/((float)numSamples-1.0f);
    float roll = osg::inDegrees(30.0f);
    
    double time=0.0f;
    double time_delta = looptime/(double)numSamples;
    for(int i=0;i<numSamples;++i)
    {
        osg::Vec3 position(center+osg::Vec3(sinf(yaw)*radius,cosf(yaw)*radius,0.0f));
        osg::Quat rotation(osg::Quat(roll,osg::Vec3(0.0,1.0,0.0))*osg::Quat(-(yaw+osg::inDegrees(90.0f)),osg::Vec3(0.0,0.0,1.0)));
        
        animationPath->insert(time,osg::AnimationPath::ControlPoint(position,rotation));

        yaw += yaw_delta;
        time += time_delta;

    }
    return animationPath;    
}
///-///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///- computeTerrainIntersection
///-///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Vec3 computeTerrainIntersection(osg::Node* subgraph,float x,float y)
{
    const osg::BoundingSphere& bs = subgraph->getBound();
    float zMax = bs.center().z()+bs.radius();
    float zMin = bs.center().z()-bs.radius();
    
    osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = 
        new osgUtil::LineSegmentIntersector(osg::Vec3(x,y,zMin),osg::Vec3(x,y,zMax));

    osgUtil::IntersectionVisitor iv(intersector.get());

    subgraph->accept(iv);

    if (intersector->containsIntersections())
    {
        return intersector->getFirstIntersection().getWorldIntersectPoint();
    }

    return osg::Vec3(x,y,0.0f);
}
osg::Geode *createUnitCube() {
	// vertex array
	Vec3Array *vertexArray = new Vec3Array();

	// bottom front left
	vertexArray->push_back(Vec3(-1, -1,  0));
	vertexArray->push_back(Vec3(-1, -1,  0));
	vertexArray->push_back(Vec3(-1, -1,  0));
	// bottom front right
	vertexArray->push_back(Vec3(+1, -1,  0));
	vertexArray->push_back(Vec3(+1, -1,  0));
	vertexArray->push_back(Vec3(+1, -1,  0));
	// bottom back right
	vertexArray->push_back(Vec3(+1, +1,  0));
	vertexArray->push_back(Vec3(+1, +1,  0));
	vertexArray->push_back(Vec3(+1, +1,  0));
	// bottom back left
	vertexArray->push_back(Vec3(-1, +1,  0));
	vertexArray->push_back(Vec3(-1, +1,  0));
	vertexArray->push_back(Vec3(-1, +1,  0));

	// top front left
	vertexArray->push_back(Vec3(-1, -1,  2));
	vertexArray->push_back(Vec3(-1, -1,  2));
	vertexArray->push_back(Vec3(-1, -1,  2));
	// top front right
	vertexArray->push_back(Vec3(+1, -1,  2));
	vertexArray->push_back(Vec3(+1, -1,  2));
	vertexArray->push_back(Vec3(+1, -1,  2));
	// top back right
	vertexArray->push_back(Vec3(+1, +1,  2));
	vertexArray->push_back(Vec3(+1, +1,  2));
	vertexArray->push_back(Vec3(+1, +1,  2));
	// top back left
	vertexArray->push_back(Vec3(-1, +1,  2));
	vertexArray->push_back(Vec3(-1, +1,  2));
	vertexArray->push_back(Vec3(-1, +1,  2));


	// face array
	DrawElementsUInt *faceArray = new DrawElementsUInt(PrimitiveSet::TRIANGLES, 0);

	// bottom
	faceArray->push_back(0); // face 1
	faceArray->push_back(9);
	faceArray->push_back(3);
	faceArray->push_back(9); // face 2
	faceArray->push_back(6);
	faceArray->push_back(3);
	// top
	faceArray->push_back(21);  //face 3
	faceArray->push_back(12);
	faceArray->push_back(18);
	faceArray->push_back(12);  //face 4
	faceArray->push_back(15);
	faceArray->push_back(18);
	// left
	faceArray->push_back(22);  //face 5
	faceArray->push_back(10);
	faceArray->push_back(13);
	faceArray->push_back(10);  //face 6
	faceArray->push_back(1);
	faceArray->push_back(13);
	// right
	faceArray->push_back(16);  //face 7
	faceArray->push_back(4);
	faceArray->push_back(19);
	faceArray->push_back(4);  //face 8
	faceArray->push_back(7);
	faceArray->push_back(19);
	// front
	faceArray->push_back(14);  //face 9
	faceArray->push_back(2);
	faceArray->push_back(17);
	faceArray->push_back(2);   //face 10
	faceArray->push_back(5);
	faceArray->push_back(17);
	// back
	faceArray->push_back(20);  //face 11
	faceArray->push_back(8);
	faceArray->push_back(23);
	faceArray->push_back(8);   //face 12
	faceArray->push_back(11);
	faceArray->push_back(23);

	// normal array
	Vec3Array *normalArray = new Vec3Array();
	normalArray->push_back(Vec3(+1, 0, 0));
	normalArray->push_back(Vec3(-1, 0, 0));
	normalArray->push_back(Vec3(0, +1, 0));
	normalArray->push_back(Vec3(0, -1, 0));
	normalArray->push_back(Vec3(0, 0, +1));
	normalArray->push_back(Vec3(0, 0, -1));

	// normal index
	TemplateIndexArray<unsigned int, Array::UIntArrayType, 24, 4> *normalIndexArray;
	normalIndexArray = new TemplateIndexArray<unsigned int, Array::UIntArrayType, 24, 4>();

	// bottom front left					
	normalIndexArray->push_back(5);
	normalIndexArray->push_back(3);
	normalIndexArray->push_back(0);
	// bottom front right
	normalIndexArray->push_back(5);
	normalIndexArray->push_back(2);
	normalIndexArray->push_back(0);
	// bottom back right
	normalIndexArray->push_back(5);
	normalIndexArray->push_back(2);
	normalIndexArray->push_back(1);
	// bottom back left
	normalIndexArray->push_back(5);
	normalIndexArray->push_back(3);
	normalIndexArray->push_back(1);

	// top front left					
	normalIndexArray->push_back(4);
	normalIndexArray->push_back(3);
	normalIndexArray->push_back(0);
	// top front right
	normalIndexArray->push_back(4);
	normalIndexArray->push_back(2);
	normalIndexArray->push_back(0);
	// top back right
	normalIndexArray->push_back(4);
	normalIndexArray->push_back(2);
	normalIndexArray->push_back(1);
	// top back left
	normalIndexArray->push_back(4);
	normalIndexArray->push_back(3);
	normalIndexArray->push_back(1);

	Geometry *geometry = new Geometry();
	geometry->setVertexArray(vertexArray);

	geometry->setNormalArray(normalArray);
	geometry->setNormalIndices(normalIndexArray);
	geometry->setNormalBinding(Geometry::BIND_PER_VERTEX);
	geometry->addPrimitiveSet(faceArray);

	Geode *cube = new Geode();
	cube->addDrawable(geometry);
	return cube;
}
osg::Geode* createHeightField(std::string heightFile, std::string texFile) 
{ 
    osg::Image* heightMap = osgDB::readImageFile(heightFile);
	if(heightMap==NULL) return NULL;
	osg::HeightField* heightField = new osg::HeightField();
	if(heightField==NULL) return NULL;
	heightField->allocate(heightMap->s(), heightMap->t());
	heightField->setOrigin(osg::Vec3(-heightMap->s() / 2, -heightMap->t() / 2, 0));
	heightField->setXInterval(1.0f);
	heightField->setYInterval(1.0f);
	heightField->setSkirtHeight(1.0f);
	 
	for (unsigned int r = 0; r < heightField->getNumRows(); r++) {
		for (unsigned int c = 0; c < heightField->getNumColumns(); c++) {
				heightField->setHeight(c, r, ((*heightMap->data(c, r)) / 255.0f) * 80.0f);
				        }
			    }
	
	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(new osg::ShapeDrawable(heightField));
	osg::Texture2D* tex = new osg::Texture2D(osgDB::readImageFile(texFile));
	tex->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR_MIPMAP_LINEAR);
	tex->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
	tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex);
 
	return geode;
}
osg::Geode *makeGalaxy(unsigned nvertices)
{
    osg::Geode *geode = new osg::Geode();
    osg::Geometry *galaxy = new osg::Geometry();
    osg::Vec3Array *vertices = new osg::Vec3Array();
    osg::Vec4Array *colors = new osg::Vec4Array();
    osg::Vec4 ini(1,1,0,1);
    osg::Vec4 fin(0,0,1,1);

    /** Formula for the two spirals */
    for (unsigned i=0;i<nvertices/2;i++) {
        float val = (i*2/(float)nvertices * 2 * 3.14159265359);
        float modx1 = rand() / (float)RAND_MAX*2;
        float mody1 = rand() / (float)RAND_MAX*2;
        float modx2 = rand() / (float)RAND_MAX*2;
        float mody2 = rand() / (float)RAND_MAX*2;
        float modz1 = ((rand()-RAND_MAX/2) / (float)(RAND_MAX))*3/(val+1);
        float modz2 = ((rand()-RAND_MAX/2) / (float)(RAND_MAX))*3/(val+1);
        vertices->push_back(osg::Vec3(cos(val)*val+modx1, sin(val)*val+mody1, modz1));
        vertices->push_back(osg::Vec3(-cos(val)*val+modx2, -sin(val)*val+mody2, modz2));

        colors->push_back(ini+(fin-ini)*(i*2/(float)nvertices));
        colors->push_back(ini+(fin-ini)*(i*2/(float)nvertices));
    }
    galaxy->setVertexArray(vertices);
    galaxy->setColorArray(colors);
    galaxy->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    galaxy->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, nvertices));
    geode->addDrawable(galaxy);
    return geode;
}
osg::Node* createMovingModel(const osg::Vec3& center, float radius)
{
    float animationLength = 10.0f;

    osg::AnimationPath* animationPath = createAnimationPath(center,radius,animationLength);

    osg::Group* model = new osg::Group;

    osg::Node* glider = osgDB::readNodeFile("glider.osgt");
    if (glider)
    {
        const osg::BoundingSphere& bs = glider->getBound();
        float size = radius/bs.radius()*0.15f;

        osg::MatrixTransform* positioned = new osg::MatrixTransform;
        positioned->setDataVariance(osg::Object::STATIC);
        positioned->setMatrix(osg::Matrix::translate(-bs.center())*
                                     osg::Matrix::scale(size,size,size)*
                                     osg::Matrix::rotate(osg::inDegrees(-90.0f),0.0f,0.0f,1.0f));
    
        positioned->addChild(glider);
    
        osg::PositionAttitudeTransform* xform = new osg::PositionAttitudeTransform;
        xform->setDataVariance(osg::Object::DYNAMIC);
        xform->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
        xform->setUpdateCallback(new osg::AnimationPathCallback(animationPath,0.0,0.5));
        xform->addChild(positioned);

        model->addChild(xform);
    }
 
    osg::Node* cessna = osgDB::readNodeFile("cessna.osgt");
    if (cessna)
    {
        const osg::BoundingSphere& bs = cessna->getBound();
        float size = radius/bs.radius()*0.15f;

        osg::MatrixTransform* positioned = new osg::MatrixTransform;
        positioned->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
        positioned->setDataVariance(osg::Object::STATIC);
        positioned->setMatrix(osg::Matrix::translate(-bs.center())*
                                     osg::Matrix::scale(size,size,size)*
                                     osg::Matrix::rotate(osg::inDegrees(180.0f),0.0f,0.0f,1.0f));
    
        //positioned->addChild(cessna);
        positioned->addChild(cessna);
    
        osg::MatrixTransform* xform = new osg::MatrixTransform;
        xform->setDataVariance(osg::Object::DYNAMIC);
        xform->setUpdateCallback(new osg::AnimationPathCallback(animationPath,0.0f,1.0));
        xform->addChild(positioned);

        model->addChild(xform);
    }
    
    return model;
}
osg::StateSet* makeStateSet(float size)
{
    osg::StateSet *set = new osg::StateSet();

    /// Setup cool blending
    set->setMode(GL_BLEND, osg::StateAttribute::ON);
    osg::BlendFunc *fn = new osg::BlendFunc();
    fn->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::DST_ALPHA);
    set->setAttributeAndModes(fn, osg::StateAttribute::ON);

    /// Setup the point sprites
    osg::PointSprite *sprite = new osg::PointSprite();
    set->setTextureAttributeAndModes(0, sprite, osg::StateAttribute::ON);

    /// Give some size to the points to be able to see the sprite
    osg::Point *point = new osg::Point();
    point->setSize(size);
    set->setAttribute(point);

    /// Disable depth test to avoid sort problems and Lighting
    set->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
    set->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    /// The texture for the sprites
    osg::Texture2D *tex = new osg::Texture2D();
    tex->setImage(osgDB::readImageFile("particle.rgb"));
    set->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON);

    return set;
}


//
//using namespace osg;
//
//int uniqueLightNumber = 0;
//int const LIGHTS = 3;
//
//PositionAttitudeTransform *cubeTransform;
//PositionAttitudeTransform *lightTransform[LIGHTS];
//StateSet *lightStateSet;
//
//Geode *createCube() {
//	// vertex array
//	Vec3Array *vertexArray = new Vec3Array();
//
//	// bottom front left
//	vertexArray->push_back(Vec3(-1, -1,  0));
//	vertexArray->push_back(Vec3(-1, -1,  0));
//	vertexArray->push_back(Vec3(-1, -1,  0));
//	// bottom front right
//	vertexArray->push_back(Vec3(+1, -1,  0));
//	vertexArray->push_back(Vec3(+1, -1,  0));
//	vertexArray->push_back(Vec3(+1, -1,  0));
//	// bottom back right
//	vertexArray->push_back(Vec3(+1, +1,  0));
//	vertexArray->push_back(Vec3(+1, +1,  0));
//	vertexArray->push_back(Vec3(+1, +1,  0));
//	// bottom back left
//	vertexArray->push_back(Vec3(-1, +1,  0));
//	vertexArray->push_back(Vec3(-1, +1,  0));
//	vertexArray->push_back(Vec3(-1, +1,  0));
//
//	// top front left
//	vertexArray->push_back(Vec3(-1, -1,  2));
//	vertexArray->push_back(Vec3(-1, -1,  2));
//	vertexArray->push_back(Vec3(-1, -1,  2));
//	// top front right
//	vertexArray->push_back(Vec3(+1, -1,  2));
//	vertexArray->push_back(Vec3(+1, -1,  2));
//	vertexArray->push_back(Vec3(+1, -1,  2));
//	// top back right
//	vertexArray->push_back(Vec3(+1, +1,  2));
//	vertexArray->push_back(Vec3(+1, +1,  2));
//	vertexArray->push_back(Vec3(+1, +1,  2));
//	// top back left
//	vertexArray->push_back(Vec3(-1, +1,  2));
//	vertexArray->push_back(Vec3(-1, +1,  2));
//	vertexArray->push_back(Vec3(-1, +1,  2));
//
//
//	// face array
//	DrawElementsUInt *faceArray = new DrawElementsUInt(PrimitiveSet::TRIANGLES, 0);
//
//	// bottom
//	faceArray->push_back(0); // face 1
//	faceArray->push_back(9);
//	faceArray->push_back(3);
//	faceArray->push_back(9); // face 2
//	faceArray->push_back(6);
//	faceArray->push_back(3);
//	// top
//	faceArray->push_back(21);  //face 3
//	faceArray->push_back(12);
//	faceArray->push_back(18);
//	faceArray->push_back(12);  //face 4
//	faceArray->push_back(15);
//	faceArray->push_back(18);
//	// left
//	faceArray->push_back(22);  //face 5
//	faceArray->push_back(10);
//	faceArray->push_back(13);
//	faceArray->push_back(10);  //face 6
//	faceArray->push_back(1);
//	faceArray->push_back(13);
//	// right
//	faceArray->push_back(16);  //face 7
//	faceArray->push_back(4);
//	faceArray->push_back(19);
//	faceArray->push_back(4);  //face 8
//	faceArray->push_back(7);
//	faceArray->push_back(19);
//	// front
//	faceArray->push_back(14);  //face 9
//	faceArray->push_back(2);
//	faceArray->push_back(17);
//	faceArray->push_back(2);   //face 10
//	faceArray->push_back(5);
//	faceArray->push_back(17);
//	// back
//	faceArray->push_back(20);  //face 11
//	faceArray->push_back(8);
//	faceArray->push_back(23);
//	faceArray->push_back(8);   //face 12
//	faceArray->push_back(11);
//	faceArray->push_back(23);
//
//	// normal array
//	Vec3Array *normalArray = new Vec3Array();
//	normalArray->push_back(Vec3(+1, 0, 0));
//	normalArray->push_back(Vec3(-1, 0, 0));
//	normalArray->push_back(Vec3(0, +1, 0));
//	normalArray->push_back(Vec3(0, -1, 0));
//	normalArray->push_back(Vec3(0, 0, +1));
//	normalArray->push_back(Vec3(0, 0, -1));
//
//	// normal index
//	TemplateIndexArray<unsigned int, Array::UIntArrayType, 24, 4> *normalIndexArray;
//	normalIndexArray = new TemplateIndexArray<unsigned int, Array::UIntArrayType, 24, 4>();
//
//	// bottom front left					
//	normalIndexArray->push_back(5);
//	normalIndexArray->push_back(3);
//	normalIndexArray->push_back(0);
//	// bottom front right
//	normalIndexArray->push_back(5);
//	normalIndexArray->push_back(2);
//	normalIndexArray->push_back(0);
//	// bottom back right
//	normalIndexArray->push_back(5);
//	normalIndexArray->push_back(2);
//	normalIndexArray->push_back(1);
//	// bottom back left
//	normalIndexArray->push_back(5);
//	normalIndexArray->push_back(3);
//	normalIndexArray->push_back(1);
//
//	// top front left					
//	normalIndexArray->push_back(4);
//	normalIndexArray->push_back(3);
//	normalIndexArray->push_back(0);
//	// top front right
//	normalIndexArray->push_back(4);
//	normalIndexArray->push_back(2);
//	normalIndexArray->push_back(0);
//	// top back right
//	normalIndexArray->push_back(4);
//	normalIndexArray->push_back(2);
//	normalIndexArray->push_back(1);
//	// top back left
//	normalIndexArray->push_back(4);
//	normalIndexArray->push_back(3);
//	normalIndexArray->push_back(1);
//
//	Geometry *geometry = new Geometry();
//	geometry->setVertexArray(vertexArray);
//
//	geometry->setNormalArray(normalArray);
//	geometry->setNormalIndices(normalIndexArray);
//	geometry->setNormalBinding(Geometry::BIND_PER_VERTEX);
//	geometry->addPrimitiveSet(faceArray);
//
//	Geode *cube = new Geode();
//	cube->addDrawable(geometry);
//	return cube;
//}
//
//Light *createLight(Vec4 color)
//{	
//	Light *light = new Light();
//	// each light must have a unique number
//	light->setLightNum(uniqueLightNumber++);
//	// we set the light's position via a PositionAttitudeTransform object
//	light->setPosition(Vec4(0.0, 0.0, 0.0, 1.0));
//	light->setDiffuse(color);
//	light->setSpecular(Vec4(1.0, 1.0, 1.0, 1.0));
//	light->setAmbient( Vec4(0.0, 0.0, 0.0, 1.0));
//
//	return light;
//}
//
//Material *createSimpleMaterial(Vec4 color)
//{
//	Material *material = new Material();
//	material->setDiffuse(Material::FRONT,  Vec4(0.0, 0.0, 0.0, 1.0));
//	material->setEmission(Material::FRONT, color);
//
//	return material;
//}
//
//
//Node *startup() {
//	// we need the scene's state set to enable the light for the entire scene
//	Group *scene = new Group();
//	lightStateSet = scene->getOrCreateStateSet();
//
//	Geode *sphere = new Geode();
//	sphere->addDrawable(new ShapeDrawable(new Sphere(Vec3(-2, 0, 0), 1)));
//
//	Geode *cube = createCube();
//	cubeTransform = new PositionAttitudeTransform();
//	cubeTransform->addChild(cube);
//	cubeTransform->setPosition(Vec3(2, 0, -1));
//
//	// create white material
//	Material *material = new Material();
//	material->setDiffuse(Material::FRONT,  Vec4(1.0, 1.0, 1.0, 1.0));
//	material->setSpecular(Material::FRONT, Vec4(0.0, 0.0, 0.0, 1.0));
//	material->setAmbient(Material::FRONT,  Vec4(0.1, 0.1, 0.1, 1.0));
//	material->setEmission(Material::FRONT, Vec4(0.0, 0.0, 0.0, 1.0));
//	material->setShininess(Material::FRONT, 25.0);
//
//	// assign the material to the sphere and cube
//	sphere->getOrCreateStateSet()->setAttribute(material);
//	cube->getOrCreateStateSet()->setAttribute(material);
//
//	// Create Lights - Red, Green, Blue
//	Vec4 lightColors[] = {Vec4(1.0, 0.0, 0.0, 1.0), Vec4(0.0, 1.0, 0.0, 1.0), Vec4(0.0, 0.0, 1.0, 1.0)};
//
//	Geode *lightMarker[LIGHTS];
//	LightSource *lightSource[LIGHTS];
//
//	for (int i = 0; i < LIGHTS; i++) {
//		lightMarker[i] = new Geode();
//		lightMarker[i]->addDrawable(new ShapeDrawable(new Sphere(Vec3(), 1)));
//		lightMarker[i]->getOrCreateStateSet()->setAttribute(createSimpleMaterial(lightColors[i]));
//
//		lightSource[i] = new LightSource();
//		lightSource[i]->setLight(createLight(lightColors[i]));
//		lightSource[i]->setLocalStateSetModes(StateAttribute::ON);
//		lightSource[i]->setStateSetModes(*lightStateSet, StateAttribute::ON);
//
//		lightTransform[i] = new PositionAttitudeTransform();
//		lightTransform[i]->addChild(lightSource[i]);
//		lightTransform[i]->addChild(lightMarker[i]);
//		lightTransform[i]->setPosition(Vec3(0, 0, 5));
//		lightTransform[i]->setScale(Vec3(0.1,0.1,0.1));
//
//		scene->addChild(lightTransform[i]);
//	}
//
//	scene->addChild(sphere);
//	scene->addChild(cubeTransform);
//	return scene;
//}
//
//void update(float time) {
//	lightTransform[0]->setPosition(Vec3(cos(time), sin(time),  0) * 4);
//	lightTransform[1]->setPosition(Vec3(0, cos(time),  sin(time)) * 4);
//	lightTransform[2]->setPosition(Vec3(sin(time), cos(time),  sin(time)) * 4);
//}
//
//int main() {
//	Node *scene = startup();
//
//	osgViewer::Viewer viewer;
//	viewer.setSceneData(scene);
//	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
//	viewer.realize();
//
//	while (!viewer.done()) {
//		update(viewer.elapsedTime());
//		viewer.frame();
//	}
//}



/*
ZZZZZZZZZZZZZZZZ$$ZZ$ZZZZZZZZZZZZZZZZZZ$Z$$ZZZOZZZZZZ$ZZZZZZZZZZZZZZZZZZZZZOZZZZOZZZZZZZZOZOOOZOZOOZZOOZOOZOOOOOOOOOOOOOOOOOOOOOOOOOOOOOZZZZ
ZZZZZZZZ$$$ZZZ$ZZZ$ZZZZ$ZZZZZZZZZZZZZZZZ$Z$ZZZZZOZZZZZZZZZZZZOZZZOZZZZZZZZZZZZOZOZZZZZZZZOOZOOOOOOZZZZZZOZO8OOOOOOOOOOOOO8OOOOOOOOOOOOO8OOOO
Z$$$$ZZZZZZZZZZZZ$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZOZOOZOZOOZZOZZZZOOOZZZOOOOOOZZOOOOOOOOOOOOOOOOOOOOOOOO8OOOOO8
ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ$$ZZ$ZZZZZZZZZZZZOZZOOZOOOOZZOOOOOOOOO8OOOOOOOOOOOOOOOOOOO8OOOOOOO8
ZOZZZZOZZZZZZZZZZZZZZZZZZ$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZOZZZZZZZOZOZOZOZZZZOZZZ$ZZZZZZOOZOOOOOOOOOOOOOZOOOOOOOOOOOOOOOOOOO8OOOO
ZZZZZZZZZZZZZZZZZZ$ZZZ$ZZZZZZZZ$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZOOZZZOZZZOZOOOOOOOOOOOOOOOOOOOOOZOOOOOOOOOOOOOOOOOOOOOOOOOOOO
ZZ$ZZZZZZZZZZ$Z$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ$ZZZZZZZZZOZZZOZZZZZZZZZZZOZZZOZOOOO8OOOOOOOOOZOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO8OOOOOOO
ZZZZZZZZZZZZZZ$ZZZZZZZZZZ$ZZZZZ$ZZ$$ZZZZZZZZZ$ZZZZZZZZZZZ$ZZZZZZZZZZZZOZZZOZZZOOZZOZOOZZZZOOOOOZOOOOOOOOOOOOOOO8ZOOOOOO8OOOOOOO888OOOOOOO88O
ZZZ$ZZZZZZZZZ$$$ZZZZZ$ZZZZZZZ$$ZZ$ZZZZZ$ZZZZZZZZZ$ZZZZZZZZZZZZZOZOZZZZOZZOOOZZZOZZOZZOZOZZOZOOOZZOOZOOZZOZOOOOOOOOOOOOOO8OOOOOOOZOOOOOOOOOO8
Z$ZZZZ$Z$ZOZ$Z$ZZZZZ$Z$$$ZZZZZZZZ$ZZ$ZZZZ$ZZ$ZZ$$ZZZZZZZZZZZZZZZZZZZZOZZZZZZZZZOZZZZZZOZZOOZZOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO8OOOO8OOOOOO
ZZ$ZZZZZZ$$ZZZZZ$$$$$ZZZ$ZZZZZ$$$Z$ZZ$ZZZZZZZZZ$$ZZZZZZZ$ZZZZZZZ$ZZZZZZZZZZZOZOZZZZZZZZZZOOOOOOOOOOOOOOOOOOZOOOOOOOOOOOOOOOOOOOOOOOOOO8OOO8O
Z$ZZZ$Z$ZZZZZ$$ZZ$Z$$$Z$ZZ$Z$$ZZ$ZZZZZZZZZZZZZ$Z$$ZZ$ZZZZZZZZZZZZZZZZZZZZZZZZZZOZZZZZZZZZZOOOZOOOOZOZOZZZZOZZZZZOZZOOOOOOOOOOOOOOOOOOOOOOOOO
ZZZZZZZZZZZZZZZZZ$$$ZZ$$$Z$$ZZ$ZZZZ$ZZZZZZZ$ZZZ$ZZZZZZ$ZZ$ZZZZZZZZZ$ZZZZZZOZZZZZZZZZZZZZZOOZZOOOZZZOOOZOZOOOOOZ8OOOOOOOOOOOOOOOOOOOOOOOOOO8O
ZZZZZZ$ZZZZ$ZZZ$$ZZZZZ$$$ZZZZZZZZ$ZZZZZZZ$ZZZZZZZ$$Z$ZZ$ZZZZZZZZZZZZZZZZZZZZZZ$ZZZZZZOZZZOOOOOZZZZZOZOOOOOOOOOOOOOOOO8ZOOOOOOOOOZZOOOOOOOOOO
ZZZZZZZ$ZZZZZZZZZZ$Z$Z$ZZZ$Z$$ZZZZZZZZ$ZZZZZZZ$$$ZZZZZZZ$Z$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZOZZZZOOZZOOOOZZZZZOZZOOZOZOOZOOOOOOOZOZOOOOO8OOOOOOO
ZZZ$ZZ$ZZ$$ZZ$$Z$Z$$Z$$$$Z$Z$ZZZ$$$ZZZZZZZZZZZZ$Z$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZOZZZZZZZOOOZOZZZOZZZOOOOOZZOZZZO8OOZOZZOOOOZZOOZOOOOOO88OOO
$ZZZZ$ZZZZZZ$Z$ZZZZZ$$ZZ$Z$ZZ$ZZ$Z$Z$$$$ZZ$ZZZZZZZZZZZZZZZZZZZZZZ$OZZZZZZZZZZZZZOZZZZZZZOZZZZOZZOZZZZZZZZZZOOZZZOZZOOOZZOOOOOOOOOOOOOOOOOOOO
ZZZ$$Z$$ZZZZ$$$$Z$Z$$$Z$Z$$$ZZZZ$$ZZ$$Z$Z$$$$ZZZ$ZZ$ZZZZZZZZ$ZZ$ZZZZ$8ZZZZZZZZ$ZZZZZZOZZZOZZOOZZOOZZZOOZZZZOZZOOZOZZZOOZOZOOOOZZOOOOOOOOOOO8
ZZZ$ZZZ$Z$$ZZZZ$$ZZZZZZZZZ$Z$$$ZZ$Z$$$$$ZZZZZ$$ZZZZZZZZ$$ZZ$$ZZZ$$ZZZ7$$ZZZZZZOZZOZZZZZZZZZZZZOZOOZOOOOOOZZZOZOZZZOOOOOOOZ8OOOOZOOZZOOOOOOOO
ZZZZ$Z$ZZZZZ$O$Z$Z$$Z$$Z$$$Z$$$$$Z$ZZZZ$$Z$$$$$$77Z$$Z$ZZ$ZZ$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZOZZOZOZZOZOZZZOOOOOZOZOOOZZZOOOOOOOOO
ZZZZZZ$$ZZZ$$$$$ZZ$$$$$Z$$$$Z$Z$$$Z$Z$ZZ$$ZZ$Z$$$$$ZZ$$$$ZZ$$ZZ$$Z$ZZ$OZZ$$ZZZZOZZZZZZZZZZZZZZZOZZZOZZOOZOOZZOZZOZZZZOOOOZOOOOOOOOOOOOOO8OOO
Z$ZZ$$$$$Z$$Z$$$$$$Z$$$$$$$$$$$$Z$Z$$ZZZ$Z$Z$$ZZ$$Z$Z$Z$$$$ZZZ$ZZZ$Z$$$$OZZZOZZZZZZOZZZZZZZZZZOZZOZZZZZZZZZZZZZZOOOOOOOOZOOOOOOOOOOOOOOOOOOO
$Z$ZZ$$$Z$$$7$$$$$$$$$$$$$Z$$Z$$$$ZZ$Z$ZZ$$$ZZ$ZZZZZZ$ZZ$$$Z$$Z$Z$ZZZ$ZZZZ$$ZZZZZZZ$OZZZZZZZOZZZZZZZZZOZZZZOZOZZOOOOZZOOOOOOOOOOOOOO8OOOOO8O
$$$Z$$$$Z$ZZ$$$Z$$$Z$$$$Z$$$$$$$$Z$$ZZ$$Z$$Z$ZZZZ$ZZ$$$ZZZZ$Z$ZZ$Z$Z$Z$$Z$ZZZZZZZZZZZZZZZZZZZOZZZZZZZOZZOOOOOOOOOOOOOOOOOOOOOOOOOOOOO8OOOOOO
$ZZ$$ZZ$$$$Z$$$$$$Z$$$$$$$$$$$Z7Z$Z$ZZZ$$O$$Z$$$$$Z$Z$$Z$ZZ$$$$$Z$ZZZZ$ZZZZZ$ZZOZZ$ZZOZZZZOZZZZZZZOOOZOOOOOOOOOOOZZZOZOOZOOOOOOOOZOOOOOOOOOO
$$$$Z$$$$$$$$$$$$$$$$$$$$$$$$$$$Z$Z$O$$$$$$Z$Z$$Z$O$$Z$$ZZZ$$$$$$$ZZZZZZZZ$ZO$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZOZZOZZ$ZZZZOZOZZOZZZOOOOZOOO
$Z$$$$$$$$$Z$$$$$ZZZ$$$$$$$ZZ$$$Z$$$OZZZ$$$$$$Z$Z$ZZZZ$Z$7$$$ZZ$$$Z$ZZZZZ$$ZZZ$$ZZZZZZZZZZZZZZOZZZZZ$ZZZZZZZZZZZZZZZZOZZZZZZOZZOZZOZZZOOOOOO
Z$$ZZ$Z$$$$$$Z$Z$Z$$$$$$ZZ$ZZ$$$ZZ$$Z$$$ZZ$Z$ZZZZ$$Z$$ZZ$$$ZO$$$ZZZZ$Z$$Z$$Z$$ZZZZ$ZZZZZZZZZZZZZZZZZZZZZZOZZZZZZZOZZZZOZZZZZZZZZOOOOOOOOZZOZ
$$ZZZ7Z$$Z$$Z$$$$$ZO88+==~~~~====+?7$$ZZ$$Z$Z$$$$ZZZZZZ$$$$$Z$ZZ$$$Z$ZZZ$$Z$+7$$$$$$$$$$ZZZZZZZZZZZZZZZZOZZZZZZZZZOOZOOOOOZZOOZOOZOOOOOZOZOZ
ZZZ$$Z$$$$$$$$$$78DOZD8?+===~~~~=~~=====+=+?IZZZ$~~~:IZZZ$$77?==~~~::::::,,,:,:,:::::::~~=+~:=ZZZZZZZZZZZZZZZ$ZZOOZOZZZZZOZOOZOOOOOOOOOOOOOO
$ZZ$Z$ZZ$Z$$ZZ$$$Z77$$$$$$$OD?..,:~~:~=========++=,...=~~::,,,,,,,,.......,,,,,....,,,.,,,,,,,.+?ZZZZZOZZZZZZZOZOZZZZZZOZZOZZZZOOOOOOOOOOOOO
Z$$$ZZ$$$Z$Z$ZZ$$7$7Z$Z$$$$$$...........,,~~====~:,...,,.,...................................,..~++++++?7ZOOZZZOOOOZZZOZO8ZOO8ZOOOZZOOOOOOOO
Z$ZZ$$Z$$$$$$$$$Z$Z$$$$Z$$$$$ZZ?,.......,=+=~::,,............. .. ....... ...:~:+::::,........,,..++===~=~~7MM=..,:+$ZZOZOZOOOOOOZZOZOOOOOOZ
ZZZ$$$$ZZZZZZZZZ$$Z$$$Z$$Z$$$Z$$$$$7?+=~::,..........:... .?=:I~++$   .88:?8O$~88$+I~7........,,,~,,,:,:7NNNNDZOZOZOZOZZZOOOOZOZOOZOOOOOOOOO
$Z$$Z$Z$$ZZ$Z$Z$$Z$$$$$$$$$$$$$$$I=~:,,........ .,Z=I........?=~.  ...,O?Z8=I7ZODO7$$I,.......,,.,,,:,OMDMMOZZ$$ZZZOOZOZZOZZOOOOOZOOOOOOOOOO
$$Z$$$$ZZZZZZ$$$$$$$7$$$7$Z$$$=::,,............. ...:...7Z.......  .:.,,8OI8:$?~$OZII=?.......,,,,,,MNMN8ZZZZZZZZOZZOZZ$ZZZZOOZOOOOOOOOOO8OO
$$ZZ$Z$$$$$$$$$$$$$$$$$$$$$?,,............       .. .....I:=I?.+=,... ,,,,:=,,,,.=I?~...........,,MNNMZZZZZZZZZZOOOOZZ$OZZOOOOOOOOOOOOOOOOOO
$$$ZZZ$$$$$$$$$$$$$$$$$$8O88O=,,......  .    .$~$I::........................  .     ...........,ZNOZZZZZZZZZZZZZOZOZOOOOOOOZZOOZOOZOOOOOOOOO
$Z$$$$$$$$$$$$$$$$$$$ZMMMMMMMM8O......   ............,,,,,,,:,:,,,,,,.. ....       ..:?Z8OO$$$$$$ZZOZZZZZZZZZZZZZZOOZZZZOOZZOOZZOOOOOOOOOOZO
Z$ZZ$ZZ$$$$$$$$$$$$$$$$$8MMMMDO..........,,,:::::::::::::::::::,,,,,,,,.,?$7+:Z$ZZ$$$$$Z$Z$$ZZZZZZZZZ$ZZZOOZOOOZZZOZOOOOOOOZOOOOOOOOZOOOOOOO
$$$Z$$Z$$$$$$$$$$$$$$$$$7$$$$$Z7~::~:.,:::::::~~~:~:~::~~~~~~::~~~::,,:Z$ZZ$$$$$Z$$$$ZZZZ$$7$$$ZZZZZOOZOZZZZZOZZOOOOZOOOZZOZOOOOOOOZOOOOZZ88
$$$Z$$$$$$$$$$77$$$$$$$$$$$$$$$$$$$$$$$7=:::::,.:~~~~~~:::::~::::,,,$$$Z$$$$$$$ZZZZ$$ZZZ$$$$ZZZZZZZZZZZZZOZOZOZOZZZZZZZOOOZZOOOOOOZOOOOOOOOO
$$$$$$$$$$$$$$77$$$$$$$$$$$7$$$$77$$7$$7$$7$$$7~~~~~~::::~::::,,,$$Z$$$ZZ$Z$$$$$$Z$$$$$Z$ZZZZZZZZZZ$OOZOOZZZZZOZOZOZZZOZZZOOOOOOZOOOZOOOOOOO
$$Z$$$$$$$$$$$$$$$77$$$$$$$$$$$$$7$$$$$$$7$$?~~~~==:~:~~:::,,,,Z$$Z$$Z$$$$$Z$$$$$$$$7$Z$Z$Z$ZZZZZZZ$$$$$$OZZZZZZ$ZZZZZOOZOOOOZZOOOOOOOOOOOOO
$$$$7$$$$$$$$$$$$$$777$7$$$$$$$$$$$7:+777?=~+?M77I::::::,,,,Z$$ZZ$$$$$$Z$$$$$$7$$ZZZ$ZZZZZZZZZZOZZZ$ZZZ$ZZZZZZOZOOZZZZZZZZZOOOOOOOOOOOOOOOOO
$$$$$$$$7$$$$$$$$$$$$$$7$$7777$$$::,:,,~=~~~8::=::::::,,,=ZZZZ$$$$$$$Z$$Z$Z$$$$ZZ$ZZZ$ZZZZZZZZOZZZZOZZZZZZZZZOZZOZOZOOOOZZOZOOOOZOOOOOOOZOOO
$$Z$$$$$$$7Z$$$$$7$$$$$$$$$$$?,,,,,,:~~~?$8M~:::::,,,,:777777$$$7$$$$$$7$$$$$$ZZZZZZ$$ZZZZ$ZZ$Z$ZZZOZ$Z$$ZZZZZOOZZOOOZOOZOZOZZOZZOOOZOOZZOOO
$Z$$$$$$$$$$$$$$$$$$$$$$$$$$,.,,,,~~=I?Z+N~:::::,,,,?$$7Z$$$$7$77$$$$$$ZZZZZZZZZZZOZZZ$$ZZZZZZZZZZZZZZOZZZZZZZZZZOOOOZZOOOOOZZOZZZOOOOOOOOOO
$Z$$ZZZ$Z$$$$7$$$$$$7$$777NNN87=~::~~=+=::::,,,,,~$$Z$$Z$$$Z$$Z$$Z$$ZZ$$$$$$ZZZZZZ$Z$ZZ$$Z$ZZZZZZZ$ZZZZZ$$$ZOZOZOZOOZOZZOOOZOZOOOOZOOOOOOOOO
$$$Z$$Z$$$$$7Z7$7$$$$$$$$$NNNNNNNNN8:::::,,,,,,I$$7$$$$$Z$$$$$ZZZZZ$$ZZ$ZZZZZZ$$ZZ$$$$ZZ$$$ZOZZZZ$ZZZ$$$ZZZZZZOZZOZOOOZOZZOOOOOOOOOOOOOOOOOO
$$$$$$$$$$$$$$7$$$7$$$$$$$$$$$$NNNNNNNNN=,,,I7$7$$$$$$Z$$$$$ZZZ$$ZZ$Z$ZZZZZZ$Z$$ZZZZ$ZZZZZZZZZZZOZZOZZZZZZZOZZOZZZZOZOZOOOOOOOOOOOOOOOOOOOOO
$Z$$$$$$$7$$Z$$7$Z$77$$$Z$$$$$$Z$Z$NNNNNNN$$$ZZ$$Z$Z$ZZZZZZZZZZZZZZ$ZZZOZZZZ$$$$$$$ZZZZZZOZZZZZOZZZZZZZZOZZZZZZOOZOZZZOZZZZZZZZOZ$ZOOOOOOOOO
$$Z$77$$Z$$$$$$$$7$$$$$7$7$$$$$$$$$$$Z$Z$Z$$Z$Z$Z$ZZZZZZZZZZZZZZZZZZZZZ$ZZOZ$$Z$$ZZ$Z$$$Z$Z$ZZZZZZZZZOZOZZZZZOOZZOOOOZOOOOOZOOOOOOOOOOOOOOOO
$$$$$$$ZZ$$$$$$$7$7$$$77$$$$$$$$$$$$Z$$$$$7$Z$$$$$$Z$ZZZZ$ZZZ$$$Z$ZZZZZZZZZZZZZZZZZZZZZZZOZZZZZZZZZOZZZZOOOOOZZOOOOOOOZZOOOOZOZOOZOOOOOO8OO8
$$7$77$$$$$$$$777$$$Z$$$$$$$Z$$$$$$$Z$$$7Z$ZZ$ZZZZZ$$$Z$$Z$ZZOZZZZZZZZZZ$ZZZZZZ$ZZOZZZZZZZZZZZZZZZZZOOOZOZOZOZZZZZOOOOOOOOOOOOZOOZOOOOOOOOOO
77$7$$7$$$$$$$$$$$$$7$$Z$$$$$$$$$7$$$$$$$$$$$$$Z$$$$$$$$$ZZ$Z$$ZZZ$ZZZZZZZ$ZZZZZZZZZZZZZZZZZZZZZZZZZOOZZZZOZZOZZOZZZOOZOOOOOOOOOOOOOOOOOOO8O
$7$7$$777$$$$$$$$$$$$$$$$$$$$$$777ZZ$$$77$7$$$$$$$$$$Z$$Z$$Z$ZZZZZZZZ$$$ZZZZZZZZZ$$ZZZZZZOZZZZZOZOZOZOOZOZZZOZZZOOOOOOOOZOOOOOOOOOOOOOOOOO8O
$$$$$$ZZ$$$$$$$$$7$$$$$Z$$Z$$I777$77$$Z7$$$$$$Z$$$Z$7$I$ZZ$ZZ$$$$$ZZ7Z$Z$$OZZOZ$Z$ZZ$Z$Z$ZZZZZZZOOZZZOOOZOOOOOOZOOOOZOOOOOOZOOOOOOOOO888OOOO
$ZZZ$$$$$$$$$ZZ$$$$Z$Z$$$77$$$Z$$ZZ$$77$77$$7Z$$$7Z$Z$7$I7$$ZZZ$ZZ$$$$Z$$$$$$$Z$ZZZZZZZZOZZZZO$ZZOZZOZZZOOZOZOOOOOZOOZZOOOOOOOZOZOO8OO8OOOOO
$$Z$Z$$$$Z$$Z$$$$$$$$Z$ZZ$ZZ$$7$$$$$77$$777Z77$Z$Z$$777$$Z$ZZZ$$$Z$ZZZZZZZ$7$7$$$$ZZ$7$Z$ZZ$$Z$OOZZZZOOOOZOZOZOZZZZOOOOOOOOOOOOOOOOOO8OOOOOO
ZZZZZ$$$$$Z$$$ZZZ$$$$$$$Z$$$$$$$$$$$$$$$7Z$$77$7Z$7$ZZ$7$$7$$$$Z$$777ZZZZZZZZZZZ$ZZOOZZZZZZZZOZZZZZZZZZZZZZZZZOZZOZZOZZOZZOOOOOOOOOOOOO8OOOO
ZZZ$ZZ$$$$ZZZZ$$$Z$$Z$$$$$$$$$$$$$$Z$$$$ZZ$$$Z$$7Z$ZZZ$$7$7$$7OI7$$Z$$Z$ZZZZZZZZ$ZZ$$$ZZZZ$ZZOZZZZOZOZOZZOOOOOOZZZ8OOOOOOOZOOOO8OZOOOOOOOO8O
Z$ZZ$$$$ZZZ$$$Z$$ZZZZ$$$$$ZZZ$ZZZZZZZZ$Z$$$Z$$Z$Z$Z$$Z$$Z$Z$Z$Z$$$Z$ZZZZZZ$$ZZZZZOZ$Z$OOZZZZZZOOZZZZZZZZZOOOOOOO8OOOOOOOOOOOOOOOOO8OOOOOOOOO
Z$ZZZZZ$ZZZZZZ$$$$$$$$ZZZZ$$Z$Z$$Z$$$$$Z$ZZZ$$Z$$$$$$$Z$$Z$ZZZZ$ZOZZZZZZZ$$$ZZ$Z$Z$Z$$$$Z$ZZO$ZOZZZZZOZZOZZOOZZZOOZOZOOOOO8OO8OOO8OOOOOOOOOO
ZZ$$Z$Z$$$$Z$ZZZ$Z$Z$$Z$$$Z$$$Z$$Z$ZZ$$$$Z$$Z$Z$Z$ZZZZOZZZOOZZZZZZ$ZZZZZ$ZZZOZZZZZ$ZZZZ$ZOZO$$OZZZZZZZZZZOOOOZZZZOZZOOOOOO8OOOOOOOOO8OOOOOOO
Z$Z$$$$ZZ$$Z$$$$$ZZZZ$$$$$ZZZ$Z$$Z$$$$Z$$$$O$ZZZZ$$$ZZZZ$$ZZZ$$$Z$ZZZZZZZ$ZZZZ$OZZOZZZZZOZZZOOOZOZZZZZOOZZZZZZOOOOOOOOOOOZOOZOZO8OO88O8OOO8O
ZZZZZZZ$$$Z$$$$$$$$Z$Z$Z$$ZZZZZZZZZZZZZZZ$ZZZZZOZZ$$ZZZZZZZZO$ZZZZZZZZOZZZ$$ZZZZZOZZZZZZZOZZZZZZZZOZOOOOZZZZZZZZZZZOZOZOOOOOOO8OOOOOOOOOO88O
ZZZZZZZZZZZZ$$$$$$ZZZ$$$$Z$Z$ZZZZZZ$OZZZZOZZZZZZ$ZZZZZZZOZO$ZZZZZZZZZOZZZOZZZZOZZOZZZZZ$ZZOZOZZZOOOOOOOOOOOOOOOOOOOOOZOOOOOOOOOOOOOOOOO88OO8
ZZZZ$ZZZZZOZ$ZZZZZZ$ZZ$ZZ$Z$$Z$OZZZZZO$ZZ$Z$ZZZ$ZZ$$$ZZZ$Z$ZZOOZZZZZZZZZZZZOZZZ$Z$ZZZZZOOZOZZZZZZZOOOZOZOOOOOZOOOOOOOZOOO8OZOOOOOOOOOOOO8888
ZZ$$ZZZZ$Z$$ZZZZZO$$$$ZZZZ$$ZZZ$$ZZZZZZZZZZZ$Z$ZZ$Z$ZZ$$ZZZZZZZZZOZZZZZZZZZZZZZZZZOZZZZZZZZZZOOOOOOOOOOOOOOZOOZOOOOOOOOOOOOO8O8OOOOOOOOO8O88
ZZZZZZZZZZZZ$Z$Z$Z$ZZ$$Z$ZZZZ$$$Z$$ZZZZ$ZZ$O$$ZZZZ$ZZ$Z$Z$ZZOZZZZZZZOZZZZZZZZZZZZZZZZZOZOZOOZOZ8ZZOZOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO8OOOO8O88O
 */

