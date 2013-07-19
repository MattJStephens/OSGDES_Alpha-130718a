#ifndef _INCL_OSG_CEnvironment
#define _INCL_OSG_CEnvironment

#include <atlbase.h>
#include <msxml.h>
#include <comdef.h>

#include "OSG_Entities.h"


//class forwards
class CEventHandler;

///
//CEnvironment
///
class CEnvironment
{
	private:				
		void XML_Parse_Environment(CComPtr<IXMLDOMNode> spXMLNode);		
		int m_uniqueLightNumber;
		int m_LIGHTS;		
	public:
		CEnvironment();
		CEventHandler* mp_event_handler;
		void XML_EnvironmentParser(string str);		
		void LinkReferences(void);
		//Group *m_scene;//temp
		CTheatre m_theatre;
		Vec4d m_background_color;
		long m_active_viewer_index;
		long m_active_group_index;
		long array_index_from_name(string name);
		long camera_index_from_name(string name);
		long clear_node_index_from_name(string name);		
		long DrawElementsUInt_index_from_name(string name);
		long draw_arrays_index_from_name(string name);
		long geometry_index_from_name(string name);		
		long text_index_from_name(string name);
		long node_index_from_name(string name);
		long geode_index_from_name(string name);
		long group_index_from_name(string name);
		long light_source_index_from_name(string name);
		long light_index_from_name(string name);
		long material_index_from_name(string name);		
		long matrix_transform_index_from_name(string name);		
		long PAT_index_from_name(string name);		
		long projection_index_from_name(string name);
		long quat_index_from_name(string name);
		long Vec2Array_index_from_name(string name);
		long Vec3Array_index_from_name(string name);
		long Vec4Array_index_from_name(string name);
		long Viewer_index_from_name(string name);	

		bool add_operative_to_theatre(COperative operative);
		bool add_PAT_operative_to_theatre(unsigned *p_PAT_index, unsigned geometry_index, eSourceGeometryType SourceGeometryType);

		void set_PAT_position(unsigned* p_index, Vec3d* p_vec);
		void set_PAT_orientation(unsigned* p_index, osg::Quat* p_quat);
		void set_PAT_orientation(unsigned* p_index, Vec3d* p_axis, float* p_angle);

		double m_elapsedTime;
		double m_simulation_time;
		double m_simulation_time_increment;
		StateSet *m_lightStateSet;				
		
		std::vector<CArray> mv_array;		
		std::vector<CClearNode> mv_clear_node;		
		std::vector<CCamera> mv_camera;
		std::vector<CDrawElementsUInt> mv_DrawElementsUInt;		
		std::vector<CDrawArrays> mv_draw_arrays;				
		std::vector<CGeode> mv_geode;
		std::vector<CGeometry> mv_geometry;
		std::vector<CGroup> mv_group;		
		std::vector<CReference> mv_ref;
		std::vector<CLight> mv_light;
		std::vector<CLightSource> mv_light_source;
		std::vector<CParticle> mv_particle;
		std::vector<CPositionAttitudeTransform> mv_PAT;
		std::vector<CProjection> mv_projection;
		std::vector<CMaterial> mv_material;
		std::vector<CMatrixTransform> mv_matrix_transform;	
		std::vector<CNode> mv_node;		
		std::vector<CQuat> mv_quat;	
		std::vector<CText> mv_text;		
		std::vector<CVec2Array> mv_Vec2Array;		
		std::vector<CVec3Array> mv_Vec3Array;		
		std::vector<CVec4Array> mv_Vec4Array;			
		std::vector<CXParticle> mv_xparticle;	
		std::vector<CViewer> mv_viewer;			

		std::vector<CViewer> mv_viewer;		
		CInputDeviceStateType
		
		osg::ref_ptr<osgText::Text> GetText(long index); 	
};

#endif