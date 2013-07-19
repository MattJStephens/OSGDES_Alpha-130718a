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

#include "OSG_Entities.h"
#include <cmath>

#include <boost/math/special_functions/erf.hpp>
#include <boost/math/distributions/normal.hpp>


CMemoryMapServer::CMemoryMapServer()
{
	 m_size = 1;
}


CXGMML_graph::CXGMML_graph()
{
	/*mp_edge_array = NULL;
	mp_weight= NULL;*/
	//mp_graph = NULL;
}
CXGMML_graph::~CXGMML_graph()
{
	/*if(mp_edge_array!= NULL)
	{
		delete []mp_edge_array;
	}
	if(mp_weight!= NULL)
	{
		delete []mp_weight;
	}*/
	//if(mp_graph!= NULL)
	//	delete mp_graph;
}



/*for(XGMML_node_iter = p_graph->mv_node.begin(); XGMML_node_iter != p_graph->mv_node.end(); ++XGMML_node_iter)	{
	p_node =  &(*XGMML_node_iter);	
	for(XGMML_attribute_iter = p_node->m_graphics.mv_attribute.begin(); XGMML_attribute_iter != p_node->m_graphics.mv_attribute.end(); ++XGMML_attribute_iter) {
		p_attribute = &(*XGMML_attribute_iter);
		if(XGMML_attribute_iter->m_name == "x")	{
			std::istringstream iss(XGMML_attribute_iter->m_value);
			iss >> x;
			vec_att.x() = x;
		}
		if(XGMML_attribute_iter->m_name == "y")	{
			std::istringstream iss(XGMML_attribute_iter->m_value);
			iss >> y;
			vec_att.y() = y;
		}
		if(XGMML_attribute_iter->m_name == "z")	{
			std::istringstream iss(XGMML_attribute_iter->m_value);
			iss >> z;
			vec_att.z() = z;
		}							
	}		*/			
	

osg::Vec3d CXGMML_graph::get_vec_at_node(std::string str_node)
{
	osg::Vec3d vec;
	vec.set(0,0,0);
	std::vector<CXGMML_edge>::iterator XGMML_edge_iter;
	std::vector<CXGMML_node>::iterator XGMML_node_iter;
	std::vector<CXGMML_attribute>::iterator XGMML_attribute_iter;
	for(XGMML_node_iter = mv_node.begin();  XGMML_node_iter != mv_node.end(); ++XGMML_node_iter)
	{
		if(XGMML_node_iter->mv_att[1].mv_attribute[2].m_value== str_node)
		{
			for(XGMML_attribute_iter = XGMML_node_iter->m_graphics.mv_attribute.begin(); XGMML_attribute_iter!=  XGMML_node_iter->m_graphics.mv_attribute.end(); ++XGMML_attribute_iter) {								
				if(XGMML_attribute_iter->m_name == "x")	{
					std::istringstream iss_av(XGMML_attribute_iter->m_value);
					iss_av >> vec.x();
				}
				if(XGMML_attribute_iter->m_name == "y")	{
					std::istringstream iss_av(XGMML_attribute_iter->m_value);
					iss_av >> vec.y();
				}
			}	
		}
	}
	return vec;
}

	/*for(XGMML_edge_iter = p_graph->mv_edge.begin(); XGMML_edge_iter != p_graph->mv_edge.end(); ++XGMML_edge_iter)	{					
					std::string name = XGMML_edge_iter->mv_att[0].mv_attribute[2].m_name;
					std::string value = XGMML_edge_iter->mv_att[0].mv_attribute[2].m_value;								
					std::istringstream iss_v(value);
					iss_v >> A;
					iss_v >> type;
					iss_v >> B;
					std::istringstream iss_A(A);
					iss_A >> XGMML_edge_iter->m_A;					
					std::istringstream iss_B(B);
					iss_B >> XGMML_edge_iter->m_B;					
					vec_A.z()= 0;
					vec_B.z()= 0;*/
					

void CXGMML_graph::create_boost_graph(void)
{
	std::vector<CXGMML_edge>::iterator XGMML_edge_iter;
	std::vector<CXGMML_node>::iterator XGMML_node_iter;
	std::vector<CXGMML_attribute>::iterator XGMML_attribute_iter;
	std::string A,type,B;		
	int node_count= 0;
	double magnitude=0;
	osg::Vec3d vec_A;
	osg::Vec3d vec_B;
	osg::Vec3d vec_C;
	int nA,nB;
	for(XGMML_edge_iter = mv_edge.begin(); XGMML_edge_iter != mv_edge.end(); ++XGMML_edge_iter)	{			
		std::string name = XGMML_edge_iter->mv_att[0].mv_attribute[2].m_name;
		std::string value = XGMML_edge_iter->mv_att[0].mv_attribute[2].m_value;								
		std::istringstream iss_v(value);
		iss_v >> A;
		iss_v >> type;
		iss_v >> B;
		vec_A = get_vec_at_node(A);
		vec_B = get_vec_at_node(B);
		vec_C = vec_B- vec_A;
		magnitude = vec_C.length();
		std::istringstream iss_A(A.erase(0, 4));
		iss_A >> XGMML_edge_iter->m_A;			
		nA = XGMML_edge_iter->m_A;
		std::istringstream iss_B(B.erase(0, 4));
		iss_B >> XGMML_edge_iter->m_B;		
		nB = XGMML_edge_iter->m_B;
		mv_edge_array.push_back(Edge(nA,nB));
		mv_weight.push_back(magnitude);
		node_count++;
	}
	int szEdge= sizeof(Edge);
	int szArray= mv_edge_array.size();
	unsigned num_arcs = szArray / szEdge;	

	
	//mv_graph.push_back(graph_t(mv_edge_array.data(), mv_edge_array.data()+num_arcs, node_count));

	//int i= 0;
	//for(i= 0; i< node_count; i++)
	//{
	//	boost::add_edge(mv_edge_array[i].first, mv_edge_array[i].second, (mv_graph[0]));
	//	//boost::add_we
	//}
	
	//boost::property_map<graph_t, boost::edge_weight_t>::type weightmap = boost::get(boost::edge_weight, mv_graph[0]);
	////property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
	//for (std::size_t j = 0; j < num_arcs; ++j) {
	//	edge_descriptor e;
	//	bool inserted;
	//	boost::tie(e, inserted) = boost::add_edge(mv_edge_array[j].first, mv_edge_array[j].second, (mv_graph[0]));
	//	weightmap[e] = mv_weight[j];
	//}


	//mv_graph.push_back(graph_t(mv_edge_array.data(), mv_edge_array.data() + num_arcs, mv_weight.data(), node));
}

CGaussian::CGaussian()
{
	//double mean_life = 1100.;
	//double life_standard_deviation = 100.;	
	//boost::math::normal bulbs(mean_life, life_standard_deviation); 
	//double expected_life = 1000.;
	// // P(X <= 1000)
	//cout << "Fraction of bulbs that will last at best <= "  << expected_life << " is "<< cdf(bulbs, expected_life) << endl;
	//// P(X > 1000)
	//cout << "Fraction of bulbs that will last at least > " << expected_life << " is "<< cdf(complement(bulbs, expected_life)) << endl;
	//double min_life = 900;
	//double max_life = 1200;
	//cout << "Fraction of bulbs that will last between "	<< min_life << " and " << max_life << " is " << cdf(bulbs, max_life) - cdf(bulbs, min_life) << endl; 

	/*std::ofstream ofs_gaussian("gaussian.txt");
	CGaussian gaussian;
	double mean= 100;
	double sd= 10;	
	double x = 0;
	for (x= 0; x< 110; x+= 1)
	{					
		ofs_gaussian << gaussian.cumulative_distribution(x, mean, sd) << "\n";
	}
	ofs_gaussian.close();*/
}

double CGaussian::erf(double x)
{
	return  boost::math::erf(x);
}

double CGaussian::get_phi(double x)
{	
	//return boost::math::pdf(boost::math::normal(mean, standard_deviation), x);

	return 0;
}


double CGaussian::cumulative_distribution(double x, double mean, double standard_deviation)
{
	//return 0.5 *  boost::math::erfc(-(x-mean)/(standard_deviation* boost::math::sqrt(2)));
	//mean = 0
	//standard_deviation = 1;
	return boost::math::cdf(boost::math::normal(mean, standard_deviation), x);
}


CGraph::CGraph()
{
	m_initialised= false;

}

bool CGraph::update(std::vector<Vec3d>* p_vec)
{
	mp_Vec3Array->clear();

	double offset_x= (double)m_offset_x;
	double offset_y= (double)m_offset_y;
	Vec3d vec;
	vec.set(offset_x, offset_y,0);
	mp_Vec3Array->push_back(vec);

	vec.set(offset_x, offset_y+m_height,0);
	mp_Vec3Array->push_back(vec);

	vec.set(offset_x, offset_y,0);
	mp_Vec3Array->push_back(vec);
	
	vec.set(offset_x+m_width,offset_y,0);
	mp_Vec3Array->push_back(vec);	

	if(p_vec->size()>1)
	{
		unsigned count = 0;
		double min_x= 0;
		double max_x= 0;
		double min_y= 0;
		double max_y= 0;
		double scale_x= 1;
		double scale_y= 1;
		double delta_x= 1;
		double delta_y= 1;	

		//get max and min
		std::vector<Vec3d>::iterator vec_iter;
		for(vec_iter = p_vec->begin(); vec_iter != p_vec->end(); ++vec_iter)
		{
			if(vec_iter->x() <min_x)
			{
				min_x = vec_iter->x();
			}
			if(vec_iter->x() > max_x)
			{
				max_x = vec_iter->x();
			}
			if(vec_iter->y() <min_y)
			{
				min_y = vec_iter->y();
			}
			if(vec_iter->y() > max_y)
			{
				max_y = vec_iter->y();
			}
			count++;
		}

		delta_x = max_x - min_x;
		delta_y = max_y - min_y;

		if(max_x!= 0)
		{
			scale_x = m_width/delta_x;
		}

		if(max_y!= 0)
		{
			scale_y = m_height/delta_y;
		}

		//scale
		vec_iter = p_vec->begin();
		Vec3d previous; 
		previous.x()= offset_x+ (vec_iter->x()*scale_x);
		previous.y()= offset_y+ (vec_iter->y()*scale_y);
	
		for(vec_iter = p_vec->begin()+1; vec_iter != p_vec->end(); ++vec_iter)
		{
			vec_iter->x()= offset_x+ (vec_iter->x()*scale_x);
			vec_iter->y()= offset_y+ (vec_iter->y()*scale_y);
			mp_Vec3Array->push_back(previous);	
			mp_Vec3Array->push_back(*vec_iter);	
			previous = *vec_iter;
		}
	
		mp_draw_arrays->setFirst(0);
		mp_draw_arrays->setCount((count*2)+4);
		mp_geometry->setVertexArray(mp_Vec3Array);

		if(!m_initialised)
		{
			//osg::Geometry::PrimitiveSetList PSL;
			//PSL = mp_geometry->getPrimitiveSetList();
			//mp_geometry->removePrimitiveSet(PSL.size());
			mp_geometry->addPrimitiveSet(mp_draw_arrays);
			

			mp_geode->addDrawable(mp_geometry);
			//mp_geode->getOrCreateStateSet()->setAttribute(mv_material[ref_iter->m_index].mp_material);

			mp_matrix_transform->setMatrix(osg::Matrix::identity());		
			mp_matrix_transform->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
			mp_matrix_transform->addChild(mp_geode);
			mp_projection->setMatrix(osg::Matrix::ortho2D(0,1024,0,768));
			mp_projection->addChild(mp_matrix_transform);
			m_initialised= true;
		}
	}
	

	return true;
}


CEntityPhysXCharacterController::CEntityPhysXCharacterController()
{

}
CEntityPhysXCharacterController::~CEntityPhysXCharacterController()
{

}

CEntityPhysXCube::CEntityPhysXCube()
{
	
}
CEntityPhysXCube::~CEntityPhysXCube()
{
	m_file_name = "cube.osg";
}

CEntityPhysXCloth::CEntityPhysXCloth()
{
	m_w = 8.0f;
	m_h = 8.0f;
	m_d = 0.2f;
	m_thickness = 0.2f;
	m_bendingStiffness = 0.5f;
	m_stretchingStiffness = 0.8f;
	m_density = 1.0f;
	m_friction = 0.5f;
	m_dampingCoefficient = 0.5f;
	m_solverIterations = 5;
	m_tearLines = false;
}
CEntityPhysXCloth::~CEntityPhysXCloth()
{

}

CEntityPhysXPlane::CEntityPhysXPlane()
{

}
CEntityPhysXPlane::~CEntityPhysXPlane()
{

}

CEntityPhysXPicker::CEntityPhysXPicker()
{

}
CEntityPhysXPicker::~CEntityPhysXPicker()
{


}

CEntityPhysXJoint::CEntityPhysXJoint()
{

}

CEntityPhysXJoint::~CEntityPhysXJoint()
{

}

CEntityPhysXSoftBody::CEntityPhysXSoftBody()
{

}

CEntityPhysXSoftBody::~CEntityPhysXSoftBody()
{

}


CEntityPhysXSphere::CEntityPhysXSphere()
{

}


CEntityPhysXSphere::~CEntityPhysXSphere()
{

}



CEntityPhysXFluid::CEntityPhysXFluid()
{
	p_fluid = NULL;
}


CEntityPhysXFluid::~CEntityPhysXFluid()
{
	if(p_fluid!= NULL)
	{
//		delete p_fluid;
	}

}


CEntityPhysXTerrain::CEntityPhysXTerrain()
{


}


CEntityPhysXTerrain::~CEntityPhysXTerrain()
{


}
	
CViewer::CViewer()
{
	
}


//        ...    ....................  ......................   ..               .
//                            .......,.. .......................                ..
//                      ....       ..=:...  .................                  ...
//                        ..       ...I...  ...............                    ...
//    ..                    ..........+.......  ...........                .......
//                 .....  .............=I.....     ..........                .....
//               ..,I.....................+:..     ..........           ..........
//               ....?....................+I..     ....                ...........
//                 ...$...................++...    .....              ............
//                 ....7~.................+?...    ......             ............
//                 ......$:...............+?...    ......       ...........  .....
//.                   .....~=.............~+. .   .....    .......................
//.                  .......~=............+$. .          ... ..~I~... .....,+. ...
//                   .........$7..........I7. .        .....,+IZ:.,?O?...I=... ...
//       .           ...  .....I?I........I7.............~?7,........ .... . ..  .
//       .                 .....,7+......=7$...........7?7=................       
//      ..                 ......+77....,$O..........7I:...................       
//..    ..     ..            .....?$....+Z+.........7$$.........                  
//.                ..   ..........?I$...$O..=IO$$?.,ZI,.....                      
//..         .....................,I$...Z8..=$7ZOZ.7I7......    ....              
//..         ...........+I7+?,.....+OD..ZD.ZZZN7I?OII.......    ....           .  
//           ......:~?+?I..?7?II7...II=ZO8+IO7$7$$Z+.................             
//        .....,I?$+,.........=7II=.=:I8O?O88OO?OO+....:,,............           .
//        ...+7....  ...........,8$I$7Z77ZNN$8ZOZ.,:OIIIZ$$II.........           .
//       ..IZ..................IZ$O7??OOZDZN8?IZI$Z7Z?+.....+I?+........         .
//. ......O,...    .........Z8OZZZO?O?7O$ZD$O+IZ7=~,...........I?=......          
//.....,O+.. ..    ......ZOZ8~,...7:~~O=IO877??I$,... ...........?7=....          
//...$:.......    .....77Z.......=I.,O7~?7O$?=Z?77Z....   .........O+......       
//..            .....,?$......... .....:~7,..~7?~+$O...     .........8,......     
//            .....=$7..................==+...I+..,ZZ....    ..........Z:.... .   
//           ....:IO.. ..................~?:..7+....7$...    ..........,.=IZO$.   
//        ......IO........................... I~....+7....      .............     
//        .....I............              ... 7.....??.....       .............   
//        ....7....     ....              .. ,+.....,?.....       .............   
//        ...O,.                     ..   ...??......?,......       ...........   
//       ...8....                         ...O~......?I. .......      . ......... 
//      ..,Z.....                        ....Z.......I?.........       .......... 
//     ...$....                          ....Z........$..........     ........... 
//    ,~?.... .                          ....7........7...............     ..     
//   .+.. ..                             ...,7........O...............            
//.  ...                                 ...~I........O:..............            
//. ..                                   ...I, .......I:................          
//.                                      .....    .. .:?......Matt J. Stephens
//.                                      .....    ..  .$...........2011       
//..             ....              ..                ..7......................... 
//                                                    .=~......................   
//                                                     ........................   
//                                                         ....................  .