#ifndef _INCL_ZETA_OSG_XML_Writer
#define _INCL_ZETA_OSG_XML_Writer

#define NOMINMAX

#include "XMLDOM_Helper.h"
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <sstream>
//#include "OSG_DES.h"
//#include "OSG_Environment.h"

void Zeta_XMLSaveFloat(IXMLDOMDocument *pXMLDom, IXMLDOMElement*pParentNode, float f);
void Zeta_XMLSaveDouble(IXMLDOMDocument *pXMLDom, IXMLDOMElement*pParentNode, double lf);
void Zeta_XMLSaveInt(IXMLDOMDocument *pXMLDom, IXMLDOMElement*pParentNode, int n);
void Zeta_XMLSaveString(IXMLDOMDocument *pXMLDom,  IXMLDOMElement*pParentNode, std::string str);
//void Zeta_XMLSaveRectangle(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, int x,int y, int width, int height);
//void Zeta_XMLSaveRectangle_lf(IXMLDOMDocument *pXMLDom, IXMLDOMElement *pParentNode, double left, double right, double top, double bottom);
//void Zeta_XMLSaveVec4(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec4 vec);
//void Zeta_XMLSaveVec3(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec);
//void Zeta_XMLSaveReference(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CReference* p_reference);
//void Zeta_XMLSaveGroup(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CGroup* p_group);
//void Zeta_XMLSaveGeometry(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CGeometry* p_geometry);
//void Zeta_XMLSaveSphere(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r);
//void Zeta_XMLSaveBox(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, osg::Vec3 half_lengths);
//void Zeta_XMLSaveCapsule(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r, float h);
//void Zeta_XMLSaveCone(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r, float h);
//void Zeta_XMLSaveCylinder(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, osg::Vec3 vec, float r, float h);
//void Zeta_XMLSaveGeode(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CGeode* p_geode);
//void Zeta_XMLSaveViewer(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CViewer* p_viewer);
//void Zeta_XMLSaveClearNode(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CClearNode* p_clear_node);
//void Zeta_XMLSaveMaterial(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMaterial* p_material);
//void Zeta_XMLSaveLight(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CLight* p_light);
//void Zeta_XMLSaveLightSource(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CLightSource* p_light_source);
//void Zeta_XMLSavePAT(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CPositionAttitudeTransform* p_PAT);
//void Zeta_XMLSaveVec3Array(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CVec3Array* p_Vec3Array);
//void Zeta_XMLSaveDrawArrays(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CDrawArrays* p_draw_arrays);
void Zeta_XMLSaveText(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CText* p_Text);
void Zeta_XMLSaveProjection(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CProjection* p_Projection);
void Zeta_XMLSaveMatrixTransform(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMatrixTransform* p_matrix_transform);
void Zeta_XMLSaveMemoryMapClient(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMemoryMapClient* p_MemoryMapClient);
void Zeta_XMLSaveMemoryMapServer(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CMemoryMapServer* p_MemoryMapServer);





#endif