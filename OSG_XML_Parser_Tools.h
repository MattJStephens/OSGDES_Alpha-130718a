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

#ifndef _INCL_OSG_XML_PARSER_TOOLS
#define _INCL_OSG_XML_PARSER_TOOLS

#include <atlbase.h>
#include <msxml.h>
#include <comdef.h>

#include "OSG_Entities.h"
#include "OSG_expression.h"
#include "OSG_Camera.h"

using namespace osg;
using namespace std;

class CVarAssoc; //class forward
class CColorSet{
public:
	unsigned m_count;
	std::vector<osg::Vec4> mv_color;
};


EntityType string_to_entity_type(std::string str);
ReferenceType string_to_reference_type(std::string str);

Material *createSimpleMaterial(Vec4 color);
CComPtr<IXMLDOMNode> XML_NextNode(CComPtr<IXMLDOMNodeList> spXMLDOMNodeList, string* p_str_node_name, string* p_str_node_text);

void XML_Parse_Attribute(CComPtr<IXMLDOMNode> spXMLNode, Node *node);
void XML_Parse_AlphaFunc(CComPtr<IXMLDOMNode> spXMLNode, CAlphaFunc *alphafunc);
void XML_Parse_AnimationControlPoint(CComPtr<IXMLDOMNode> spXMLNode, CAnimationControlPoint *animation_control_point);
void XML_Parse_AnimationPath(CComPtr<IXMLDOMNode> spXMLNode, CAnimationPath *animation_path);
void XML_Parse_BoundingBox(CComPtr<IXMLDOMNode> spXMLNode,  CBoundingBox *bounding_box);
void XML_Parse_Box(CComPtr<IXMLDOMNode> spXMLNode,  osg::Vec3d *cen, float *x, float *y, float *z);
void XML_Parse_Billboard(CComPtr<IXMLDOMNode> spXMLNode,  CBillboard *billboard);
void XML_Parse_Capsule(CComPtr<IXMLDOMNode> spXMLNode,  osg::Vec3d *cen, float *radius, float *height);
void XML_Parse_ColorSet(CComPtr<IXMLDOMNode> spXMLNode, CColorSet *p_color_set);
void XML_Parse_Cone(CComPtr<IXMLDOMNode> spXMLNode,  osg::Vec3d *cen, float *radius, float *height);
void XML_Parse_Camera(CComPtr<IXMLDOMNode> spXMLNode, CCamera *p_camera);	void XML_Parse_ClearNode(CComPtr<IXMLDOMNode> spXMLNode, CClearNode *p_clear_node);
void XML_Parse_CMatrix(CComPtr<IXMLDOMNode> spXMLNode, CMatrix* p_matrix);
void XML_Parse_CustomCamera(CComPtr<IXMLDOMNode> spXMLNode, CCustomCamera *p_camera);
void XML_Parse_Cylinder(CComPtr<IXMLDOMNode> spXMLNode,  osg::Vec3d *cen, float *radius, float *height);
void XML_Parse_DrawElementsUInt(CComPtr<IXMLDOMNode> spXMLNode, CDrawElementsUInt *p_DEUI);	
void XML_Parse_InsertControlPoint(CComPtr<IXMLDOMNode> spXMLNode, double *time, osg::Vec3d *pt);	
void XML_Parse_Logistics_component(CComPtr<IXMLDOMNode> spXMLNode, CLogistics_component *p_component);	
void XML_Parse_Logistics_demand(CComPtr<IXMLDOMNode> spXMLNode, CLogistics_demand *p_demand);	
void XML_Parse_Logistics_node(CComPtr<IXMLDOMNode> spXMLNode, CLogistics_node *p_node);	
void XML_Parse_Logistics_operation(CComPtr<IXMLDOMNode> spXMLNode, CLogistics_operation *p_operation);	
void XML_Parse_Logistics_producer(CComPtr<IXMLDOMNode> spXMLNode, CLogistics_producer *p_producer);	
void XML_Parse_EntityArray(CComPtr<IXMLDOMNode> spXMLNode, CEntityArray *p_entity_array);	
void XML_Parse_DrawArrays(CComPtr<IXMLDOMNode> spXMLNode, CDrawArrays *p_draw_arrays, unsigned long* p_count);	
void XML_Parse_ExplosionEffect(CComPtr<IXMLDOMNode> spXMLNode, CExplosionEffect *p_explosion);	
void XML_Parse_ExplosionDebrisEffect(CComPtr<IXMLDOMNode> spXMLNode, CExplosionDebrisEffect *p_explosion_debris);	

void XML_Parse_EXPR(CComPtr<IXMLDOMNode> spXMLNode, CEXPR *expr);
void XML_Parse_EXPR_Variable(CComPtr<IXMLDOMNode> spXMLNode, CEXPR_Variable *EXPR_variable);
void XML_Parse_EXPR_Operation(CComPtr<IXMLDOMNode> spXMLNode, CEXPR_Operation *EXPR_Operation);
void XML_Parse_EXPR_Operand(CComPtr<IXMLDOMNode> spXMLNode, CEXPR_Operand *EXPR_Operand);


void XML_Parse_ExprFunction(CComPtr<IXMLDOMNode> spXMLNode, CExprFunction *func);
void XML_Parse_ExprInterval(CComPtr<IXMLDOMNode> spXMLNode, CExprInterval *var);
void XML_Parse_EXPRInterval(CComPtr<IXMLDOMNode> spXMLNode, CEXPRInterval *var);

void XML_Parse_FireEffect(CComPtr<IXMLDOMNode> spXMLNode, CFireEffect *p_fire);	
void XML_Parse_FunctionOfTwoVariables(CComPtr<IXMLDOMNode> spXMLNode, CFunctionOfTwoVariables *fotv);
void XML_Parse_FunctionOfOneVariable(CComPtr<IXMLDOMNode> spXMLNode, CFunctionOfOneVariable *fotv);
void XML_Parse_Geode(CComPtr<IXMLDOMNode> spXMLNode, CGeode* p_geode);
void XML_Parse_Geometry(CComPtr<IXMLDOMNode> spXMLNode, CGeometry *p_geometry);
void XML_Parse_Graph(CComPtr<IXMLDOMNode> spXMLNode, CGraph *p_graph);
void XML_Parse_Group(CComPtr<IXMLDOMNode> spXMLNode, CGroup *p_group, std::size_t graph_node_count);
void XML_Parse_Image(CComPtr<IXMLDOMNode> spXMLNode, CImage *p_image);
void XML_Parse_Imposter(CComPtr<IXMLDOMNode> spXMLNode, CImposter *p_imposter);
void XML_Parse_HeightField(CComPtr<IXMLDOMNode> spXMLNode,  CHeightField *p_height_field);
void XML_Parse_HeightFieldFromFile(CComPtr<IXMLDOMNode> spXMLNode,  string *height_file, string *texture_file);
void XML_Parse_Light(CComPtr<IXMLDOMNode> spXMLNode, CLight *p_light);	
void XML_Parse_Light_Source(CComPtr<IXMLDOMNode> spXMLNode, CLightSource *p_light_source);
void XML_Parse_Material(CComPtr<IXMLDOMNode> spXMLNode, CMaterial *p_material);
void XML_Parse_Matrix(CComPtr<IXMLDOMNode> spXMLNode, osg::Matrix* p_matrix);
void XML_Parse_MatrixTransform(CComPtr<IXMLDOMNode> spXMLNode, CMatrixTransform* p_matrix_transform);
void XML_Parse_MemoryMapClient(CComPtr<IXMLDOMNode> spXMLNode, CMemoryMapClient* p_memory_map_client);
void XML_Parse_MemoryMapServer(CComPtr<IXMLDOMNode> spXMLNode, CMemoryMapServer* p_memory_map_server);
void XML_Parse_MDEXPR(CComPtr<IXMLDOMNode> spXMLNode, CMDEXPR* p_md_expr);
void XML_Parse_MDEXPRVarRef(CComPtr<IXMLDOMNode> spXMLNode, CMDEXPRVarRef *ref);
void XML_Parse_MDEXPRReference(CComPtr<IXMLDOMNode> spXMLNode, CMDEXPRReference *ref);

void XML_Parse_MultiplePAT(CComPtr<IXMLDOMNode> spXMLNode, CMultiplePAT *p_multiple_PAT);
void XML_Parse_MultipleRandomEntities(CComPtr<IXMLDOMNode> spXMLNode, CMultipleRandomEntities *p_multiple_random_entities,  CBillboard *p_billboard);
void XML_Parse_Node(CComPtr<IXMLDOMNode> spXMLNode,  CNode *p_node);

void XML_Parse_PhysX_Actor(CComPtr<IXMLDOMNode> spXMLNode,  CPhysX_Actor *p_PhysX_actor);
void XML_Parse_PhysX_ActorDesc(CComPtr<IXMLDOMNode> spXMLNode,  CPhysX_ActorDesc *p_PhysX_actor_desc);

void XML_Parse_NxBodyDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxBodyDesc *p_nx_body_desc);

void XML_Parse_NxBoxShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxBoxShapeDesc *p_nx_box_shape_desc);
void XML_Parse_NxCapsuleShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxCapsuleShapeDesc *p_nx_capsule_shape_desc);
void XML_Parse_NxConvexShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxConvexShapeDesc *p_nx_convex_shape_desc);
void XML_Parse_NxSphereShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxSphereShapeDesc *p_nx_sphere_shape_desc);
void XML_Parse_NxTriangleMeshShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxTriangleMeshShapeDesc *p_nx_triangle_mesh_shape_desc);

void XML_Parse_NxCloth(CComPtr<IXMLDOMNode> spXMLNode,  NxCloth *p_nx_cloth);
void XML_Parse_NxClothDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxClothDesc *p_nx_cloth_desc);
void XML_Parse_NxClothMesh(CComPtr<IXMLDOMNode> spXMLNode,  NxClothMesh *p_nx_cloth_mesh);
void XML_Parse_NxClothMeshDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxClothMeshDesc *p_nx_cloth_mesh_desc);
void XML_Parse_NxFluid(CComPtr<IXMLDOMNode> spXMLNode,  NxFluid *p_nx_fluid);
void XML_Parse_NxFluidDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxFluidDesc *p_nx_fluid_desc);
void XML_Parse_NxMat34(CComPtr<IXMLDOMNode> spXMLNode,  NxMat34* p_NxMat34);
void XML_Parse_NxPlaneShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxPlaneShapeDesc *p_nx_plane_shape_desc);
void XML_Parse_NxScene(CComPtr<IXMLDOMNode> spXMLNode,  NxScene *p_nx_scene);
void XML_Parse_NxSphereShapeDesc(CComPtr<IXMLDOMNode> spXMLNode,  NxSphereShapeDesc *p_nx_sphere_shape_desc);
void XML_Parse_NxVec3(CComPtr<IXMLDOMNode> spXMLNode, NxVec3 *vec);

void XML_Parse_ParametricFunction(CComPtr<IXMLDOMNode> spXMLNode, CParametricEquation *para);
void XML_Parse_Particle(CComPtr<IXMLDOMNode> spXMLNode, CParticle* p_particle);
void XML_Parse_PolygonMode(CComPtr<IXMLDOMNode> spXMLNode, CPolygonMode* p_pm);
void XML_Parse_Projection(CComPtr<IXMLDOMNode> spXMLNode, CProjection* p_projection);
void XML_Parse_Program(CComPtr<IXMLDOMNode> spXMLNode, CProgram* p_pm);
void XML_Parse_PhysXCube(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXCube* p_physX_cube);
void XML_Parse_PhysXPlane(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXPlane* p_physX_plane);
void XML_Parse_PhysXCloth(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXCloth* p_physX_cloth);
void XML_Parse_PhysXSphere(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXSphere* p_physX_sphere);
void XML_Parse_PhysXSoftBody(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXSoftBody* p_physX_soft_body);
void XML_Parse_PhysXFluid(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXFluid* p_physX_fluid);
void XML_Parse_PhysXTerrain(CComPtr<IXMLDOMNode> spXMLNode, CEntityPhysXTerrain* p_physX_terrain);
void XML_Parse_PositionAttitudeTransform(CComPtr<IXMLDOMNode> spXMLNode, CPositionAttitudeTransform *p_PAT);	
void XML_Parse_Orienation(CComPtr<IXMLDOMNode> spXMLNode, COrientation *orientation);
void XML_Parse_Quaternion(CComPtr<IXMLDOMNode> spXMLNode, osg::Quat *q);
void XML_Parse_Range(CComPtr<IXMLDOMNode> spXMLNode,  unsigned int * childNo, float * min, float * max);
void XML_Parse_Reference(CComPtr<IXMLDOMNode> spXMLNode,  CReference *p_ref);
void XML_Parse_ReferenceArray(CComPtr<IXMLDOMNode> spXMLNode,  CReference *p_ref, unsigned int* n);
void XML_Parse_Rect(CComPtr<IXMLDOMNode> spXMLNode,  CRect *p_rect);
void XML_Parse_SmokeEffect(CComPtr<IXMLDOMNode> spXMLNode, CSmokeEffect *p_smoke);	
void XML_Parse_Socket(CComPtr<IXMLDOMNode> spXMLNode, CSocket *p_socket);	
void XML_Parse_Sphere(CComPtr<IXMLDOMNode> spXMLNode,  osg::Vec3d *cen, float * rad);
void XML_Parse_ShapeDrawable(CComPtr<IXMLDOMNode> spXMLNode,  CShapeDrawable * sd);
void XML_Parse_Shader(CComPtr<IXMLDOMNode> spXMLNode,  CShader * shader);
void XML_Parse_StateSet(CComPtr<IXMLDOMNode> spXMLNode,  CStateSet * p_stateset);
void XML_Parse_Text(CComPtr<IXMLDOMNode> spXMLNode, osgText::Text *p_text, string* name);
//void XML_Parse_Texture(CComPtr<IXMLDOMNode> spXMLNode, CTexture *p_texture);
void XML_Parse_Texture2D(CComPtr<IXMLDOMNode> spXMLNode, CTexture2D *p_texture);
void XML_Parse_Text(CComPtr<IXMLDOMNode> spXMLNode, osgText::Text *p_text, string* name);
void XML_Parse_Uniform(CComPtr<IXMLDOMNode> spXMLNode, CUniform* p_uniform);
void XML_Parse_VarAssoc(CComPtr<IXMLDOMNode> spXMLNode, CVarAssoc *p_var_assoc);	
void XML_Parse_Vec2Array(CComPtr<IXMLDOMNode> spXMLNode, CVec2Array *p_array);	
void XML_Parse_Vec3Array(CComPtr<IXMLDOMNode> spXMLNode, CVec3Array *p_array, CXGMML_graph* p_graph);	
void XML_Parse_Vec4Array(CComPtr<IXMLDOMNode> spXMLNode, CVec4Array *p_array);
void XML_Parse_Vec2(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec2 *vec);
void XML_Parse_Vec3(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec3 *vec);
void XML_Parse_Vec3d(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec3d *vec);
void XML_Parse_Vec3f(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec3f *vec);
void XML_Parse_Vec4(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec4 *vec);
void XML_Parse_Vec4d(CComPtr<IXMLDOMNode> spXMLNode, osg::Vec4d *vec);
void XML_Parse_Variable(CComPtr<IXMLDOMNode> spXMLNode, CParsedVariable *p_variable);
void XML_Parse_Viewer(CComPtr<IXMLDOMNode> spXMLNode, CViewer *p_viewer);
void XML_Parse_VectorField(CComPtr<IXMLDOMNode> spXMLNode, CVectorField *vector_field);
void XML_Parse_EXPRVectorField(CComPtr<IXMLDOMNode> spXMLNode, CEXPRVectorField *vector_field);
void XML_Parse_WindowRect(CComPtr<IXMLDOMNode> spXMLNode, CWindowRect *p_window_rect);

bool XML_Parse_XGMML_Node(CComPtr<IXMLDOMNode> spXMLNode, CXGMML_node* p_node);
bool XML_Parse_XGMML_Edge(CComPtr<IXMLDOMNode> spXMLNode, CXGMML_edge* p_edge);
bool XML_Parse_XGMML_Att(CComPtr<IXMLDOMNode> spXMLNode, CXGMML_att* p_att);
bool XML_Parse_XGMML_Graphics(CComPtr<IXMLDOMNode> spXMLNode, CXGMML_graphics* p_graphics);



#endif