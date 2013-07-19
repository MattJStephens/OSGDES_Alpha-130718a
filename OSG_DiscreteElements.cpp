///////////////////////////////
//
// Matt Jim. Stephens 2012
//
// Discrete elements
///////////////////////

#include "OSG_DiscreteElements.h"
#include "OSG_Environment.h"
#include <algorithm>
#include <functional>

extern CEnvironment g_environment;


CLogistics_particle::CLogistics_particle()
{
	m_position.set(0,0,0);
	m_velocity.set(0,0,0);
}
void CLogistics_particle::update_position(double time)
{
	m_position += m_velocity*time;	
}
bool slay_Logistics_operative(const CLogistics_operative &Logistics_operative)
{
	if(Logistics_operative.m_slay)
		return true;
	else return false;
}
CLogistics_operative::CLogistics_operative()
{ 
	m_slay = false;
	m_unique_id = UINT_MAX;
	m_task_duration =0;
	m_task_elapsed_time =0;
	//mp_PAT = new osg::PositionAttitudeTransform();
}
CLogistics_operation::CLogistics_operation()
{

	m_DELogistics_operative_unique_id=0;
}
CReference CLogistics_operation::add_Logistics_operative(CLogistics_operative Logistics_operative) 
{ 		
	CReference ref;
	ref.m_index = mv_Logistics_operative.size();
	ref.m_unique_id = m_DELogistics_operative_unique_id;	
	Logistics_operative.m_unique_id = m_DELogistics_operative_unique_id;
	Logistics_operative.m_state = Logistics_operativeState_in_storage;	
	Logistics_operative.m_slay = false;
	mv_Logistics_operative.push_back(Logistics_operative);
	m_DELogistics_operative_unique_id++;
	return ref;
}
CReference CLogistics_operation::get_reference(CLogistics_operative* p_Logistics_operative)
{
	CReference ref;
	ref.m_index= Logistics_operative_index_from_unique_ID(p_Logistics_operative->m_unique_id);
	ref.m_unique_id = p_Logistics_operative->m_unique_id;
	return ref;
}
CLogistics_operative* CLogistics_operation::get_Logistics_operative(CReference* p_ref) 
{ 
	if( p_ref->m_index<  mv_Logistics_operative.size())
		return &(mv_Logistics_operative[p_ref->m_index]);
	else return NULL;
}
std::size_t CLogistics_operation::Logistics_operative_index_from_name(std::string name)
{
	std::size_t i= 0;
	std::vector<CLogistics_operative>::iterator iter;
	for (iter = mv_Logistics_operative.begin(); iter< mv_Logistics_operative.end(); ++iter)
	{
		if(iter->m_name == name)
		{
			return i;
		}
		i++;
	}
	return UINT_MAX;
}
std::size_t CLogistics_operation::Logistics_operative_index_from_unique_ID(unsigned ID)
{
	std::size_t i= 0;
	std::vector<CLogistics_operative>::iterator iter;
	for (iter = mv_Logistics_operative.begin(); iter< mv_Logistics_operative.end(); ++iter)
	{
		if(iter->m_unique_id == ID)
		{
			return i;
		}
		i++;
	}
	return UINT_MAX;
}


bool CLogistics_operative::set_on_route(eLogistics_operativeState op_state, eTimeType time_type, float speed)
{
	m_state = op_state;

	osg::Vec3d vecA = g_environment.get_Logistics_node_position(&m_ref_start_node);
	osg::Vec3d vecB =  g_environment.get_Logistics_node_position(&m_ref_destination_node);	
	osg::Vec3d vecAB = vecB - vecA;
	m_position= vecA;			
	m_destination = vecB;			
	m_distance = vecAB.length();
	vecAB.normalize();		
	m_velocity = vecAB * speed;
	
	m_task_duration = m_distance/speed; //average and constant velocity 
	m_foot_print.m_n_ID= 1;
	m_foot_print.m_str_ID= "set_on_route";
	m_time_task_complete = g_environment.get_elapsed_time(time_type, &m_foot_print)+ m_task_duration;	

	return true;
}


/*
bool CLogistics_operation::set_on_route(CReference* p_ref_start_node, CReference* p_ref_destination_node, CReference* p_ref_Logistics_operative, eLogistics_operativeState op_state, float speed, double elapsed_time)
{		
	//set Logistics_operative velocity to go from the producer node to the demand node
	CLogistics_operative *p_Logistics_operative = get_Logistics_operative(p_ref_Logistics_operative);
	if(p_Logistics_operative!= NULL)
	{
		if(p_ref_destination_node->m_ref_name == "Logistics_node5")
		{
			p_Logistics_operative->m_name = "DN5";
			bool wot = true;
		}

		osg::Vec3d vecA = g_environment.get_Logistics_node_position(p_ref_start_node);
		osg::Vec3d vecB =  g_environment.get_Logistics_node_position(p_ref_destination_node);

		p_Logistics_operative->m_ref_start_node = *p_ref_start_node;
		p_Logistics_operative->m_ref_destination_node = *p_ref_destination_node;
		osg::Vec3d vecAB = vecB - vecA;
		p_Logistics_operative->m_position= vecA;			
		p_Logistics_operative->m_destination = vecB;			
		p_Logistics_operative->m_distance = vecAB.length();
		vecAB.normalize();		
		p_Logistics_operative->m_velocity = vecAB * speed;
		p_Logistics_operative->m_state = op_state;
		p_Logistics_operative->m_task_duration = p_Logistics_operative->m_distance/speed; //average and constant velocity 
		m_foot_print.m_n_ID= 1;
		m_foot_print.m_str_ID= "set_on_route";
		p_Logistics_operative->m_time_task_complete = g_environment.get_elapsed_time(m_time_type, &m_foot_print)+ p_Logistics_operative->m_task_duration;			
	}
	else return false;		

	return true;
}

*/

bool CLogistics_operation::update_operation(double delta_time)
{			
	unsigned count = 0;
	std::vector<CLogistics_operative>::iterator iter;	
	for(iter = mv_Logistics_operative.begin(); iter <mv_Logistics_operative.end(); ++iter)
	{		
		bool time_is_up= false;
		if(iter->m_state==Logistics_operativeState_in_storage)
		{		
			iter->mp_PAT->setPosition(iter->m_position);		
		}
		else
		{			
			iter->update_position(delta_time);				
			m_foot_print.m_str_ID = "in_transit_store_at_destination";
			m_foot_print.m_n_ID= 1;
			if(g_environment.get_elapsed_time(m_time_type, &m_foot_print) >= iter->m_time_task_complete)
			{
				time_is_up= true;
				iter->mp_PAT->setPosition(iter->m_destination);
			}
			else
			{
				osg::Vec3d vecA = g_environment.get_Logistics_node_position(&(iter->m_ref_start_node));			
				osg::Vec3d vecAC = iter->m_position - vecA;		
				osg::Vec3d vecAB = iter->m_destination - vecA;	
				if(vecAC.length()> vecAB.length())
				{
					iter->mp_PAT->setPosition(iter->m_destination);
				}
				else
				{
					iter->mp_PAT->setPosition(iter->m_position);		
				}
			}
			//has the Logistics_operative reached its destination?
			switch(iter->m_state)
			{
				case Logistics_operativeState_in_transit_store_at_destination:
				{				
					if(time_is_up)
					{
						iter->m_state = Logistics_operativeState_in_storage;
						iter->m_velocity = osg::Vec3d(0,0,0);		
						iter->m_ref_rest_node= iter->m_ref_destination_node;
						//CReference ref_destination_node = iter->m_ref_destination_node;
						//CReference ref_Logistics_operative =  get_reference(&(*iter));						
						//g_environment.mv_Logistics_node[ref_destination_node.m_index].mv_ref_Logistics_operatives_in_storage.push_back(ref_Logistics_operative);	
					}
					break;
				}	
				case Logistics_operativeState_in_transit_destroy_at_destination:
				{			
					if(time_is_up)
					{
						iter->m_state = Logistics_operativeState_slay;
						iter->m_slay = true;
						g_environment.mv_group[g_environment.m_active_group_index].mp_group->removeChild(iter->mp_PAT);		
					}
					break;
				}		
			}
		}		
	}	
	
	return true;
}
void CLogistics_operation::slay_marked_Logistics_operatives(void)
{
	//std::size_t size1 = mv_Logistics_operative.size();
	std::vector<CLogistics_operative>::iterator OP_iter;
	OP_iter = std::remove_if(mv_Logistics_operative.begin(), mv_Logistics_operative.end(), slay_Logistics_operative);
	mv_Logistics_operative.erase(OP_iter, mv_Logistics_operative.end());
	//std::size_t size2 = mv_Logistics_operative.size();
}
std::size_t CLogistics_node::get_PAT_index(void)
{
	return m_ref_PAT.m_index;
}
std::size_t CLogistics_node::get_geo_index(void)
{
	return m_ref_geo.m_index;

	/*switch(m_Logistics_node_type)
	{
		case(Logistics_nodeType_Node)
		{
			return m_ref_geo.m_index;
			break;
		}
	}*/
}

bool slay_order(const CLogistics_order &order)
{
	if(order.m_slay == true)
		return true;
	else return false;
}

osg::Vec3d CLogistics_node::get_position(void)
{
	osg::Vec3d vec;
	CPositionAttitudeTransform* p_PAT = g_environment.get_PAT(&m_ref_PAT);	

	if(p_PAT!= NULL)
	{
		vec = p_PAT->mp_PAT->getPosition();	
	}

	return vec;

}
void CLogistics_node::slay_flagged_orders(void)
{
	std::vector<CLogistics_order>::iterator order_iter;
	order_iter = std::remove_if(mv_order.begin(), mv_order.end(), slay_order);
	mv_order.erase(order_iter, mv_order.end());	
}
void CLogistics_node::slay_flagged_storage(void)
{	
	/*std::vector<CReference>::iterator ref_end;
	ref_end = std::remove_if(mv_ref_Logistics_operatives_in_storage.begin(), mv_ref_Logistics_operatives_in_storage.end(), slay_reference);
	mv_ref_Logistics_operatives_in_storage.erase(ref_end, mv_ref_Logistics_operatives_in_storage.end());	*/
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
                                                               
