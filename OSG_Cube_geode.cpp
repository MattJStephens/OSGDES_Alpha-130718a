#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/StateSet>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osg/Texture2D>

#include "OSG_Cube_geode.h"

using namespace osg;

Geode *createUnitCube() {
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

osg::Node* createHeightField(std::string heightFile, std::string texFile) 
{ 
    osg::Image* heightMap = osgDB::readImageFile(heightFile);
	osg::HeightField* heightField = new osg::HeightField();
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
