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

#include "OSG_DES.h"
#include "OSG_XML_Save_environment_tree.h"
#include "XMLDOM_Helper.h"
//#include <OSGBaseTypes>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <sstream>
#include "OSG_XML_Writer.h"

void XML_Save_environment_tree(std::string File_name,  CEnvironment* p_env)
{
	CoInitialize(NULL);

	HRESULT hr = S_OK;
    IXMLDOMDocument *pXMLDom = NULL;
    IXMLDOMElement *pRoot = NULL;
    IXMLDOMElement *pXMLDomNode = NULL; 
    IXMLDOMDocumentFragment *pDF = NULL;

    BSTR bstrXML = NULL;
    VARIANT varFileName;
    VariantInit(&varFileName);

    CHK_HR(CreateAndInitDOM(&pXMLDom));

    // Create a processing instruction element.
    CHK_HR(CreateAndAddPINode(pXMLDom, L"xml", L"version='1.0'"));

    // Create a comment element.
    CHK_HR(CreateAndAddCommentNode(pXMLDom, L"mjs OpenSceneGraph Extensible Markup Language Process Description Script (OSG XML PDS)"));   

    // Create the root element.
    CHK_HR(CreateElement(pXMLDom, L"Environment", &pRoot));
	
	XML_Create_tree_of_environment_nodes(pXMLDom, pRoot, p_env);

	SAFE_RELEASE(pXMLDomNode);

	CHK_HR(AppendChildToParent(pRoot, pXMLDom));

    CHK_HR(pXMLDom->get_xml(&bstrXML));
    printf("Dynamically created DOM:\n%S\n", bstrXML);

	BSTR bsFileName;
	bsFileName = _bstr_t (File_name.c_str());
	//File_name
    CHK_HR(VariantFromString(bsFileName, varFileName));
    //CHK_HR(VariantFromString(L"dynamDOM.xml", varFileName));
    CHK_HR(pXMLDom->save(varFileName));
    printf("DOM saved to dynamDOM.xml\n");

CleanUp:
    SAFE_RELEASE(pXMLDom);
    SAFE_RELEASE(pRoot);
    SAFE_RELEASE(pXMLDomNode);
    SAFE_RELEASE(pDF);   
    SysFreeString(bstrXML);
    VariantClear(&varFileName);
	
	// Stop COM
	CoUninitialize();

	//return grp;
}


void XML_Create_tree_of_environment_nodes(IXMLDOMDocument *pXMLDom,  IXMLDOMElement *pParentNode, CEnvironment* p_env)
{
	HRESULT hr = S_OK;
	std::ostringstream oss1;
	std::ostringstream oss2;
	IXMLDOMElement *pSubNode = NULL;	
	std::size_t sz = 0;
	std::size_t n = 0;

	osgDB::FilePathList pathList = osgDB::getDataFilePathList();
	for(n=0; n<pathList.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"FilePath", L"\n\t", pParentNode, &pSubNode));			
		XMLSaveString(pXMLDom, pSubNode, pathList[n]);
	}	

	CHK_HR(CreateAndAddElementNode(pXMLDom, L"ActiveViewerIndex", L"\n\t", pParentNode, &pSubNode));	
	XMLSaveInt(pXMLDom, pSubNode, p_env->m_active_viewer_index);	
	
	CHK_HR(CreateAndAddElementNode(pXMLDom, L"ActiveGroupIndex", L"\n\t", pParentNode, &pSubNode));		
	XMLSaveInt(pXMLDom, pSubNode, p_env->m_active_group_index);	

	for(n=0; n< p_env->mv_group.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Group", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveGroup(pXMLDom, pSubNode,  &(p_env->mv_group[n]));
	}

	for(n=0; n< p_env->mv_viewer.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Viewer", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveViewer(pXMLDom, pSubNode,  &(p_env->mv_viewer[n]));
	}

	for(n=0; n< p_env->mv_clear_node.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"ClearNode", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveClearNode(pXMLDom, pSubNode,  &(p_env->mv_clear_node[n]));
	}
	
	for(n=0; n< p_env->mv_material.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Material", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveMaterial(pXMLDom, pSubNode,  &(p_env->mv_material[n]));
	}

	for(n=0; n< p_env->mv_light.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Light", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveLight(pXMLDom, pSubNode,  &(p_env->mv_light[n]));
	}

	for(n=0; n< p_env->mv_light.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"LightSource", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveLightSource(pXMLDom, pSubNode,  &(p_env->mv_light_source[n]));
	}

	for(n=0; n< p_env->PAT_size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"PositionAttitudeTransform", L"\n\t", pParentNode, &pSubNode));		
		XMLSavePAT(pXMLDom, pSubNode,  p_env->get_PAT(n));
	}

	for(n=0; n< p_env->mv_Vec3Array.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Vec3Array", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveVec3Array(pXMLDom, pSubNode,  &(p_env->mv_Vec3Array[n]));
	}

	for(n=0; n< p_env->mv_draw_arrays.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"DrawArrays", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveDrawArrays(pXMLDom, pSubNode,  &(p_env->mv_draw_arrays[n]));
	}

	for(n=0; n< p_env->mv_geometry.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Geometry", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveGeometry(pXMLDom, pSubNode,  &(p_env->mv_geometry[n]));
	}

	for(n=0; n< p_env->mv_geode.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Geode", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveGeode(pXMLDom, pSubNode,  &(p_env->mv_geode[n]));
	}

	for(n=0; n< p_env->mv_text.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Text", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveText(pXMLDom, pSubNode,  &(p_env->mv_text[n]));
	}

	for(n=0; n< p_env->mv_projection.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"Projection", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveProjection(pXMLDom, pSubNode,  &(p_env->mv_projection[n]));
	}

	for(n=0; n< p_env->mv_matrix_transform.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"MatrixTransform", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveMatrixTransform(pXMLDom, pSubNode,  &(p_env->mv_matrix_transform[n]));
	}

	for(n=0; n< p_env->mv_memory_map_server.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"MemoryMapServer", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveMemoryMapServer(pXMLDom, pSubNode,  &(p_env->mv_memory_map_server[n]));
	}
		
	for(n=0; n< p_env->mv_memory_map_server.size(); n++) {
		CHK_HR(CreateAndAddElementNode(pXMLDom, L"MemoryMapClient", L"\n\t", pParentNode, &pSubNode));		
		XMLSaveMemoryMapClient(pXMLDom, pSubNode,  &(p_env->mv_memory_map_client[n]));
	}
	
		
CleanUp:
	 SAFE_RELEASE(pSubNode);	
}