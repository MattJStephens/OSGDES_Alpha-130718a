#pragma once
#include <osg/Node>
#include <osg/StateSet>
#include <osg/Program>
#include <osg/MatrixTransform>
#include "NxPhysics.h"


// motion callback to update the dynamic actor's position - used by CPhysXCube, Sphere actors
class MotionCallback : public osg::NodeCallback
{
public:
	MotionCallback(NxActor* actor) : 
	  _actor(actor), 
		  _applyShaderColors(false)
	  {

	  }

	  MotionCallback(NxActor* actor, osg::StateSet* stateset) : 
	  _actor(actor), 
		  _stateset(stateset), 
		  _applyShaderColors(true),
		  _smooth(true)
	  {
		  osg::Program* program = new osg::Program;
		  osg::Shader* vsh = new osg::Shader( osg::Shader::VERTEX );
		  osg::Shader* fsh = new osg::Shader( osg::Shader::FRAGMENT );

		  // hot color map + cartoon style shading mix 
		  vsh->loadShaderSourceFromFile( "../shaders/hot.vert" );
		  fsh->loadShaderSourceFromFile( "../shaders/hot.frag" );

		  program->addShader( vsh );
		  program->addShader( fsh );
		  _stateset->setAttributeAndModes( program, osg::StateAttribute::ON);


		  _uvm = new osg::Uniform( "vm", 1.0f );
		  _stateset->addUniform( _uvm );
		  _stateset->addUniform( new osg::Uniform("vmax", 750.0f) );
		  _stateset->addUniform( new osg::Uniform("vmax", 1.0f) );
	  }

	  virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	  {
		  float m[16];
		  // get the transformation matrix
		  _actor->getGlobalPose().getColumnMajor44(m);//temp_mjs
		  // quaternion
		  NxQuat _quat = _actor->getGlobalOrientationQuat();
		  // now the graphic object
		  osg::MatrixTransform* _mat = dynamic_cast<osg::MatrixTransform*>(node);
		  if (_mat)
		  {
			  // apply the position + orientation to the graphic object
			  _mat->setMatrix(osg::Matrix::rotate(osg::Quat(_quat.x, _quat.y, _quat.z, _quat.w))*
				  osg::Matrix::translate(osg::Vec3f(m[12],m[14],m[13])));	
		  }

		  if (_applyShaderColors)
		  {
			  _uvm->set(_actor->getLinearVelocity().magnitude());
		  }		  
		  traverse(node, nv);
	  }
private:
	NxActor* _actor;
	osg::StateSet* _stateset;
	bool _applyShaderColors;
	osg::Uniform* _uvm;
	bool _smooth;
};