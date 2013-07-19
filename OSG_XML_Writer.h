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

#ifndef _INCL_OSG_XML_Writer
#define _INCL_OSG_XML_Writer

#define NOMINMAX

#include "XMLDOM_Helper.h"
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <sstream>
//#include "OSG_DES.h"
#include "OSG_Environment.h"

void XMLSaveFloat(IXMLDOMDocument *pXMLDom, IXMLDOMElement*pParentNode, float f);
void XMLSaveDouble(IXMLDOMDocument *pXMLDom, IXMLDOMElement*pParentNode, double lf);
void XMLSaveInt(IXMLDOMDocument *pXMLDom, IXMLDOMElement*pParentNode, int n);
void XMLSaveString(IXMLDOMDocument *pXMLDom,  IXMLDOMElement*pParentNode, std::string str);
void XMLSaveRectangle(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, int x,int y, int width, int height);
void XMLSaveRectangle_lf(IXMLDOMDocument *pXMLDom, IXMLDOMElement *pParentNode, double left, double right, double top, double bottom);
void XMLSaveVec4(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec4 vec);
void XMLSaveVec3(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec);
void XMLSaveReference(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CReference* p_reference);
void XMLSaveGroup(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CGroup* p_group);
void XMLSaveGeometry(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CGeometry* p_geometry);
void XMLSaveSphere(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r);
void XMLSaveBox(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, osg::Vec3 half_lengths);
void XMLSaveCapsule(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r, float h);
void XMLSaveCone(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r, float h);
void XMLSaveCylinder(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r, float h);
void XMLSaveGeode(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CGeode* p_geode);
void XMLSaveViewer(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CViewer* p_viewer);
void XMLSaveClearNode(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CClearNode* p_clear_node);
void XMLSaveMaterial(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMaterial* p_material);
void XMLSaveLight(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CLight* p_light);
void XMLSaveLightSource(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CLightSource* p_light_source);
void XMLSavePAT(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CPositionAttitudeTransform* p_PAT);
void XMLSaveVec3Array(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CVec3Array* p_Vec3Array);
void XMLSaveDrawArrays(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CDrawArrays* p_draw_arrays);
void XMLSaveText(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CText* p_Text);
void XMLSaveProjection(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CProjection* p_Projection);
void XMLSaveMatrixTransform(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMatrixTransform* p_matrix_transform);
void XMLSaveMemoryMapClient(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMemoryMapClient* p_MemoryMapClient);
void XMLSaveMemoryMapServer(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMemoryMapServer* p_MemoryMapServer);





#endif