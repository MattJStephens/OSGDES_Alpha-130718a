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

#include "OSG_XML_Writer.h"

void XMLSaveFloat(IXMLDOMDocument *pXMLDom, IXMLDOMElement*pParentNode, float f)
{
	HRESULT hr = S_OK;	
	std::ostringstream oss;
	BSTR bsStr;
	oss << f;
	bsStr= _bstr_t(oss.str().c_str());
	CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pParentNode));
CleanUp:
	 SysFreeString(bsStr);	
}

void XMLSaveDouble(IXMLDOMDocument *pXMLDom, IXMLDOMElement*pParentNode, double lf)
{
	HRESULT hr = S_OK;	
	std::ostringstream oss;
	BSTR bsStr;
	oss << lf;
	bsStr= _bstr_t(oss.str().c_str());
	CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pParentNode));
CleanUp:
	 SysFreeString(bsStr);	
}

void XMLSaveInt(IXMLDOMDocument *pXMLDom, IXMLDOMElement*pParentNode, int n)
{
	HRESULT hr = S_OK;	
	std::ostringstream oss;
	BSTR bsStr;
	oss << n;
	bsStr= _bstr_t(oss.str().c_str());
	CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pParentNode));
CleanUp:
	 SysFreeString(bsStr);	
}

void XMLSaveString(IXMLDOMDocument *pXMLDom,  IXMLDOMElement*pParentNode, std::string str)
{
	HRESULT hr = S_OK;		
	BSTR bsStr;	
	bsStr= _bstr_t(str.c_str());
	CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pParentNode));
CleanUp:
	 SysFreeString(bsStr);	
}

void XMLSaveRectangle(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, int x,int y, int width, int height)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"x", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveInt(pXMLDom, pSubNode, x);
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"y", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveInt(pXMLDom, pSubNode, y);
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"width", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveInt(pXMLDom, pSubNode, width);
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"height", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveInt(pXMLDom, pSubNode, height);
CleanUp:
	 SAFE_RELEASE(pSubNode);
	 
}


void XMLSaveRectangle_lf(IXMLDOMDocument *pXMLDom, IXMLDOMElement *pParentNode, double left, double right, double top, double bottom)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Left", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveDouble(pXMLDom, pSubNode, left);
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Right", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveDouble(pXMLDom, pSubNode, right);
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Top", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveDouble(pXMLDom, pSubNode, top);
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Bottom", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveDouble(pXMLDom, pSubNode, bottom);
CleanUp:
	 SAFE_RELEASE(pSubNode);
	 
}


void XMLSaveVec4(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec4 vec)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"x", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, vec.x());
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"y", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, vec.y());
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"z", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, vec.z());
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"w", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, vec.w());
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}


void XMLSaveVec3(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"x", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, vec.x());
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"y", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, vec.y());
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"z", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, vec.z());
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveReference(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CReference* p_reference)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"RefName", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_reference->m_ref_name);
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"EntityType", L"\n\t", pParentNode, &pSubNode));			
	XMLSaveString(pXMLDom, pSubNode, p_reference->get_entity_type_str());		
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"RefType", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_reference->get_ref_type_str());
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveGroup(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CGroup* p_group)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_group->m_name);	
	std::size_t n=0;		
	for(n=0; n<p_group->mv_ref.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Ref", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveReference(pXMLDom, pSubNode,  &(p_group->mv_ref[n]));
	}
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}



void XMLSaveGeometry(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CGeometry* p_geometry)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_geometry->m_name);	
	std::size_t n=0;		
	for(n=0; n<p_geometry->mv_ref.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Ref", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveReference(pXMLDom, pSubNode,  &(p_geometry->mv_ref[n]));
	}
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveSphere(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Center", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec3(pXMLDom, pSubNode, vec);	
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Radius", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, r);	
	
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveBox(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, osg::Vec3 half_lengths)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Center", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec3(pXMLDom, pSubNode, vec);	
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"x", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, half_lengths.x()*2);	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"y", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, half_lengths.y()*2);	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"z", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, half_lengths.z()*2);	
	
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveCapsule(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r, float h)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Center", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec3(pXMLDom, pSubNode, vec);	
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Radius", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, r);	

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Height", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, h);	
	
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveCone(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r, float h)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Center", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec3(pXMLDom, pSubNode, vec);	
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Radius", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, r);	

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Height", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, h);	
	
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveCylinder(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r, float h)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Center", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec3(pXMLDom, pSubNode, vec);	
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Radius", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, r);	

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Height", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, h);	
	
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveGeode(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CGeode* p_geode)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	unsigned int num_drawables=0;
	unsigned int i = 0;
	osg::Drawable* p_drawable= NULL;
	osg::Shape* p_shape= NULL;

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_geode->m_name);	

	std::size_t n=0;		
	for(n=0; n<p_geode->mv_ref.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Ref", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveReference(pXMLDom, pSubNode,  &(p_geode->mv_ref[n]));
	}		
	num_drawables = p_geode->mp_geode->getNumDrawables();
	for(i = 0; i< num_drawables; i++) {
		p_drawable= NULL;	
		p_drawable = p_geode->mp_geode->getDrawable(i);
		if(p_drawable!= NULL) {
			std::string drawable_name = p_drawable->getName();
			p_shape= NULL;
			p_shape = p_drawable->getShape();
			if(p_shape!= NULL) {
				std::string shape_name = p_shape->getName();
				std::string str_shape_class_name= p_shape->className();
				if(str_shape_class_name == "Sphere") {					
					osg::Sphere* p_sphere = dynamic_cast<osg::Sphere*>(p_shape->cloneType());
					CHK_HR(CreateAndAddElementNode(pXMLDom, L"Sphere", L"\n\t", pParentNode, &pSubNode));		
					XMLSaveSphere(pXMLDom, pSubNode, p_sphere->getCenter(), p_sphere->getRadius());						
				}		
				else if(str_shape_class_name == "Box") {					
					osg::Box* p_box = dynamic_cast<osg::Box*>(p_shape->cloneType());
					CHK_HR(CreateAndAddElementNode(pXMLDom, L"Box", L"\n\t", pParentNode, &pSubNode));		
					XMLSaveBox(pXMLDom, pSubNode, p_box->getCenter(), p_box->getHalfLengths());						
				}	
				else if(str_shape_class_name == "Capsule") {					
					osg::Capsule* p_capsule = dynamic_cast<osg::Capsule*>(p_shape->cloneType());
					CHK_HR(CreateAndAddElementNode(pXMLDom, L"Capsule", L"\n\t", pParentNode, &pSubNode));		
					XMLSaveCapsule(pXMLDom, pSubNode, p_capsule->getCenter(), p_capsule->getRadius(), p_capsule->getHeight());						
				}	
				else if(str_shape_class_name == "Cone") {					
					osg::Cone* p_cone = dynamic_cast<osg::Cone*>(p_shape->cloneType());
					CHK_HR(CreateAndAddElementNode(pXMLDom, L"Cone", L"\n\t", pParentNode, &pSubNode));		
					XMLSaveCone(pXMLDom, pSubNode, p_cone->getCenter(), p_cone->getRadius(), p_cone->getHeight());						
				}	
				else if(str_shape_class_name == "Cylinder") {					
					osg::Cylinder* p_cylinder = dynamic_cast<osg::Cylinder*>(p_shape->cloneType());
					CHK_HR(CreateAndAddElementNode(pXMLDom, L"Cylinder", L"\n\t", pParentNode, &pSubNode));		
					XMLSaveCylinder(pXMLDom, pSubNode, p_cylinder->getCenter(), p_cylinder->getRadius(), p_cylinder->getHeight());						
				}	
			}
		}
	}

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"SetNodeMask", L"\n\t", pParentNode, &pSubNode));		
	osg::Node::NodeMask nm = p_geode->mp_geode->getNodeMask();
	XMLSaveInt(pXMLDom, pSubNode, nm);	

	//setNodeMask
	

CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}


void XMLSaveViewer(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CViewer* p_viewer)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	BSTR bsStr;	
	std::string str;
	std::size_t sz=0;
	std::size_t n=0;
	osgViewer::ViewerBase::Views views;
	osgViewer::ViewerBase::Windows windows;
	osgViewer::GraphicsWindow* p_w;
	osg::Camera *p_camera;
//	osg::Node::NodeMask node_mask;
	std::ostringstream oss1;

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	bsStr= _bstr_t(p_viewer->m_name.c_str());
	CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pSubNode));
	
	osgGA::CameraManipulator* pCM= p_viewer->mp_viewer->getCameraManipulator();
	if(pCM!= NULL)
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"CameraManipulator", L"\n\t", pParentNode, &pSubNode));	
		str = pCM->className();
		if(str == "OrbitManipulator")
		{
			bsStr= _bstr_t("OrbitManipulator");
			CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pSubNode));
		}
		else if(str == "TrackballManipulator")
		{
			bsStr= _bstr_t("TrackballManipulator");
			CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pSubNode));
		}
		else if(str == "FlightManipulator")
		{
			bsStr= _bstr_t("FlightManipulator");
			CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pSubNode));
		}
		else if(str == "DriveManipulator")
		{
			bsStr= _bstr_t("DriveManipulator");
			CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pSubNode));
		}		
	}	
			
	for(n=0; n<p_viewer->mv_ref.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Ref", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveReference(pXMLDom, pSubNode,  &(p_viewer->mv_ref[n]));
	}

	p_viewer->mp_viewer->getViews(views);
	for(n=0; n<views.size(); n++) {
		osgViewer::View* v =  views[n];
		p_camera = v->getCamera();		
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"SetCullMask", L"\n\t", pParentNode, &pSubNode));	
		XMLSaveInt(pXMLDom, pSubNode, p_camera->getCullMask());	
	}
	
	p_viewer->mp_viewer->getWindows(windows);
	for(n=0; n<windows.size(); n++) {		
		p_w=  windows[n];		
		int x,y,width,height;
		p_w->getWindowRectangle(x,y,width, height);
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"SetUpViewInWindow", L"\n\t", pParentNode, &pSubNode));	
		XMLSaveRectangle(pXMLDom, pSubNode, x, y, width, height);
	}	

CleanUp:
	 SysFreeString(bsStr);		 
	 SAFE_RELEASE(pSubNode);
	 
}


void XMLSaveClearNode(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CClearNode* p_clear_node)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	BSTR bsStr;
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	bsStr= _bstr_t(p_clear_node->m_name.c_str());
	CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pSubNode));
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"ClearColor", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec4(pXMLDom, pSubNode, p_clear_node->mp_clear_node->getClearColor());

CleanUp:
	 SysFreeString(bsStr);	
	 SAFE_RELEASE(pSubNode);	 
}




void XMLSaveMaterial(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMaterial* p_material)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	BSTR bsStr;
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	bsStr= _bstr_t(p_material->m_name.c_str());
	CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pSubNode));
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Diffuse", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec4(pXMLDom, pSubNode, p_material->mp_material->getDiffuse(Material::FRONT));

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Specular", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec4(pXMLDom, pSubNode, p_material->mp_material->getSpecular(Material::FRONT));

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Ambient", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec4(pXMLDom, pSubNode, p_material->mp_material->getAmbient(Material::FRONT));

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Emission", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec4(pXMLDom, pSubNode, p_material->mp_material->getEmission(Material::FRONT));

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Shininess", L"\n\t", pParentNode, &pSubNode));	
	XMLSaveFloat(pXMLDom, pSubNode, p_material->mp_material->getShininess(Material::FRONT));

CleanUp:
	 SysFreeString(bsStr);	
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveLight(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CLight* p_light)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	BSTR bsStr;
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	bsStr= _bstr_t(p_light->m_name.c_str());
	CHK_HR(CreateAndAddTextNode(pXMLDom, bsStr, pSubNode));

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Position", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec4(pXMLDom, pSubNode, p_light->mp_light->getPosition());
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Diffuse", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec4(pXMLDom, pSubNode, p_light->mp_light->getDiffuse());

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Specular", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec4(pXMLDom, pSubNode, p_light->mp_light->getSpecular());

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Ambient", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec4(pXMLDom, pSubNode, p_light->mp_light->getAmbient());

CleanUp:
	 SysFreeString(bsStr);	
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveLightSource(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CLightSource* p_light_source)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	osg::StateAttribute::GLMode mode;
	osg::StateAttribute::GLModeValue  mode_value;
	osg::StateSet::ModeList mode_list;
	std::size_t n=0;

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));			
	XMLSaveString(pXMLDom, pSubNode, p_light_source->m_name);	

	
	for(n=0; n<p_light_source->mv_ref.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Ref", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveReference(pXMLDom, pSubNode,  &(p_light_source->mv_ref[n]));
	}	
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"SetLocalStateSetModes", L"\n\t", pParentNode, &pSubNode));	
	osg::StateSet* ss = p_light_source->mp_light_source->getStateSet();
	mode = osg::StateAttribute::ON;
	mode_value = ss->getMode(GL_LIGHT0);
	if(osg::StateAttribute::ON ==  mode_value)
	{
		XMLSaveString(pXMLDom, pSubNode, "ON");	
	}

	

CleanUp:	
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSavePAT(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CPositionAttitudeTransform* p_PAT)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_PAT->m_name);
	
	std::size_t n=0;		
	for(n=0; n<p_PAT->mv_ref.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Ref", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveReference(pXMLDom, pSubNode,  &(p_PAT->mv_ref[n]));
	}

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Position", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec3(pXMLDom, pSubNode, p_PAT->mp_PAT->getPosition());
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Scale", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec3(pXMLDom, pSubNode, p_PAT->mp_PAT->getScale());
	

CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}


void XMLSaveVec3Array(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CVec3Array* p_Vec3Array)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	std::vector<Vec3>::iterator iter;
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_Vec3Array->m_name);		
	for(iter = p_Vec3Array->mp_Vec3Array->begin(); iter != p_Vec3Array->mp_Vec3Array->end(); ++iter) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Vec3d", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveVec3(pXMLDom, pSubNode,  *iter);
	}		

CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}



void XMLSaveDrawArrays(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CDrawArrays* p_draw_arrays)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	std::vector<Vec3>::iterator iter;

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_draw_arrays->m_name);		

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"PrimitiveSet", L"\n\t", pParentNode, &pSubNode));	
	if(p_draw_arrays->mp_draw_arrays->getMode()== osg::PrimitiveSet::Mode::POINTS) { XMLSaveString(pXMLDom, pSubNode, "POINTS");	}
	else if(p_draw_arrays->mp_draw_arrays->getMode()== osg::PrimitiveSet::Mode::LINES) { XMLSaveString(pXMLDom, pSubNode, "LINES");	}
	else if(p_draw_arrays->mp_draw_arrays->getMode()== osg::PrimitiveSet::Mode::LINE_STRIP) { XMLSaveString(pXMLDom, pSubNode, "LINE_STRIP");	}
	else if(p_draw_arrays->mp_draw_arrays->getMode()== osg::PrimitiveSet::Mode::LINE_LOOP) { XMLSaveString(pXMLDom, pSubNode, "LINE_LOOP");	}
	else if(p_draw_arrays->mp_draw_arrays->getMode()== osg::PrimitiveSet::Mode::TRIANGLES) { XMLSaveString(pXMLDom, pSubNode, "TRIANGLES");	}
	else if(p_draw_arrays->mp_draw_arrays->getMode()== osg::PrimitiveSet::Mode::TRIANGLE_STRIP) { XMLSaveString(pXMLDom, pSubNode, "TRIANGLE_STRIP");	}
	else if(p_draw_arrays->mp_draw_arrays->getMode()== osg::PrimitiveSet::Mode::TRIANGLE_FAN) { XMLSaveString(pXMLDom, pSubNode, "TRIANGLE_FAN");	}
	else if(p_draw_arrays->mp_draw_arrays->getMode()== osg::PrimitiveSet::Mode::QUADS) { XMLSaveString(pXMLDom, pSubNode, "QUADS");	}
	else if(p_draw_arrays->mp_draw_arrays->getMode()== osg::PrimitiveSet::Mode::QUAD_STRIP) { XMLSaveString(pXMLDom, pSubNode, "QUAD_STRIP");	}
	else if(p_draw_arrays->mp_draw_arrays->getMode()== osg::PrimitiveSet::Mode::TRIANGLE_FAN) { XMLSaveString(pXMLDom, pSubNode, "TRIANGLE_FAN");	}
	else if(p_draw_arrays->mp_draw_arrays->getMode()== osg::PrimitiveSet::Mode::POLYGON) { XMLSaveString(pXMLDom, pSubNode, "POLYGON");	}

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"SetFirst", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveInt(pXMLDom, pSubNode, p_draw_arrays->mp_draw_arrays->getFirst());		
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"SetCount", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveInt(pXMLDom, pSubNode, p_draw_arrays->mp_draw_arrays->getCount());				
	
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}


void XMLSaveText(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CText* p_Text)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	std::vector<Vec3>::iterator iter;
	std::string str;
	osgText::String text;

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_Text->m_name);		

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"CharacterSize", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveFloat(pXMLDom, pSubNode, p_Text->mp_text->getCharacterHeight());	

	//TODO: remove folder from path leaving filename only
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Font", L"\n\t", pParentNode, &pSubNode));	
	const osgText::Font * p_font = p_Text->mp_text->getFont();	
	XMLSaveString(pXMLDom, pSubNode, p_font->getFileName());		

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"InitText", L"\n\t", pParentNode, &pSubNode));		
	text = p_Text->mp_text->getText();		
	str = text.createUTF8EncodedString();
	XMLSaveString(pXMLDom, pSubNode, str);		

	
	osgText::TextBase::AxisAlignment aa = p_Text->mp_text->getAxisAlignment();
	if(aa == osgText::TextBase::XY_PLANE)
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"AxisAlignment", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "XY_PLANE");	
	}
	else if(aa == osgText::TextBase::REVERSED_XY_PLANE)
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"AxisAlignment", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "REVERSED_XY_PLANE");	
	}
	else if(aa == osgText::TextBase::XZ_PLANE )
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"AxisAlignment", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "XZ_PLANE");	
	}
	else if(aa == osgText::TextBase::REVERSED_XZ_PLANE )
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"AxisAlignment", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "REVERSED_XZ_PLANE");	
	}
	else if(aa == osgText::TextBase::YZ_PLANE)
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"AxisAlignment", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "YZ_PLANE");	
	}
	else if(aa == osgText::TextBase::REVERSED_YZ_PLANE)
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"AxisAlignment", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "REVERSED_YZ_PLANE");	
	}
	else if(aa == osgText::TextBase::SCREEN)
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"AxisAlignment", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "SCREEN");	
	}
	else if(aa == osgText::TextBase::USER_DEFINED_ROTATION )
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"AxisAlignment", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "USER_DEFINED_ROTATION");	
	}

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Position", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec3(pXMLDom, pSubNode, p_Text->mp_text->getPosition());

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Color", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveVec4(pXMLDom, pSubNode, p_Text->mp_text->getColor());

CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}


void XMLSaveProjection(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CProjection* p_Projection)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	std::vector<Vec3>::iterator iter;
	std::size_t n=0;
	osg::Matrix matrix;
	double f_left, f_right, f_top, f_bottom, f_near, f_far;

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_Projection->m_name);	

	
	for(n=0; n<p_Projection->mv_ref.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Ref", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveReference(pXMLDom, pSubNode,  &(p_Projection->mv_ref[n]));
	}

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Ortho2D", L"\n\t", pParentNode, &pSubNode));		
	matrix= p_Projection->mp_projection->getMatrix();
	matrix.getOrtho(f_left, f_right, f_bottom, f_top, f_near, f_far); 
	XMLSaveRectangle_lf(pXMLDom, pSubNode, f_left, f_right, f_top, f_bottom);	

CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveMatrixTransform(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMatrixTransform* p_matrix_transform)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;
	osg::Matrix matrix;
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_matrix_transform->m_name);

	std::size_t n=0;		
	for(n=0; n<p_matrix_transform->mv_ref.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Ref", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveReference(pXMLDom, pSubNode,  &(p_matrix_transform->mv_ref[n]));
	}


	osg::Transform::ReferenceFrame rf = p_matrix_transform->mp_matrix_transform->getReferenceFrame();	
	if(rf == osg::Transform::ABSOLUTE_RF)
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"ReferenceFrame", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "ABSOLUTE_RF");
	}

	matrix = p_matrix_transform->mp_matrix_transform->getMatrix();
	if(matrix.isIdentity()== true)
	{
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Identity", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveString(pXMLDom, pSubNode, "TRUE");
	}

	
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}



void XMLSaveMemoryMapClient(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMemoryMapClient* p_MemoryMapClient)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_MemoryMapClient->m_name);
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"MemoryMapName", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_MemoryMapClient->m_memory_map_name);		

CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}

void XMLSaveMemoryMapServer(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMemoryMapServer* p_MemoryMapServer)
{
	HRESULT hr = S_OK;	
	IXMLDOMElement *pSubNode = NULL;	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Name", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_MemoryMapServer->m_name);
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"MemoryMapName", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveString(pXMLDom, pSubNode, p_MemoryMapServer->m_memory_map_name);		
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"Size", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveInt(pXMLDom, pSubNode, p_MemoryMapServer->m_size);
CleanUp:
	 SAFE_RELEASE(pSubNode);	 
}
