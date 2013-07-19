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
/////////////////////////////////////////////////////////////////////// 

#ifndef _XMLEXPR_
#define _XMLEXPR_

#include <string>
#include <vector>
#include <sstream>
#include <complex>
//#include "OSG_BOOST_random.h"
#include "OSG_Reference.h"
#include <osg/PositionAttitudeTransform>

enum eEXPR_operator_type { 
	EXPR_operator_undefined, 
	EXPR_operator_constant,
	EXPR_operator_variable,
	EXPR_operator_operation,
	EXPR_operator_random,
	EXPR_operator_solve,
	EXPR_operator_expansion,
	EXPR_operator_factorisation,
	EXPR_operator_differentiation,
	EXPR_operator_intergration,
	EXPR_operator_sin, EXPR_operator_cos, EXPR_operator_tan, 
	EXPR_operator_asin, EXPR_operator_acos, EXPR_operator_atan, EXPR_operator_atan2, EXPR_operator_sinh, EXPR_operator_cosh, EXPR_operator_tanh, 
	EXPR_operator_log, EXPR_operator_log10, 
	EXPR_operator_sqrt, 						
	EXPR_operator_exp, 
	EXPR_operator_ones_compliment, EXPR_operator_logical_negation, 
	EXPR_operator_floor, EXPR_operator_ceil, 
	EXPR_operator_postfix_increment, EXPR_operator_postfix_decrement, 
	EXPR_operator_power, 
	EXPR_operator_multiplication, EXPR_operator_division, EXPR_operator_addition, EXPR_operator_subtract, 
	EXPR_operator_modulus, 						
	EXPR_operator_assignment, EXPR_operator_addition_assignment, EXPR_operator_subtraction_assignment, EXPR_operator_multiplication_assignment, EXPR_operator_division_assignment,
	EXPR_operator_modulus_assignment, EXPR_operator_left_shift_assignment, EXPR_operator_right_shift_assignment,
	EXPR_operator_bitwise_AND_assignment, EXPR_operator_bitwise_inclusive_OR_assignment, EXPR_operator_bitwise_exclusive_OR_assignment,
	EXPR_operator_left_shift, EXPR_operator_right_shift,
	EXPR_operator_bitwise_AND, EXPR_operator_bitwise_inclusive_OR, EXPR_operator_bitwise_exclusive_OR,
	EXPR_operator_logical_OR, EXPR_operator_logical_AND,
	EXPR_operator_less_than, EXPR_operator_less_than_or_equal_to, EXPR_operator_greater_than, EXPR_operator_greater_than_or_equal_to, EXPR_operator_equality, EXPR_operator_not_equal, 
	EXPR_operator_error };

enum eEXPR_operand_type { EXPR_operand_type_constant, EXPR_operand_type_variable, EXPR_operand_type_operation };

enum eEXPR_variable_type { EXPR_variable_type_default, EXPR_variable_type_time, EXPR_variable_type_random, EXPR_variable_type_normal, EXPR_variable_type_external, EXPR_variable_type_node_mass  };

eEXPR_operator_type string_to_EXPR_operator_type(std::string str);

class CEXPR_OperationReference;
class CEXPR_OperationList;
class CEXPR;


class CEXPR_Operand_position
{
	public:
		eEXPR_operand_type m_type;
		std::size_t m_index;
};

class CEXPR_Variable
{
	public:
		CEXPR_Variable();
		std::string m_ID;
		eEXPR_variable_type m_type;
		double get();
		void set(double value){ m_value=value;}
		double m_mean;
		double m_standard_deviation;
		std::vector<CReference> mv_ref;
		void link(void);
	private:	
		double m_value;
};

class CEXPR_VariableList
{
	public:
		std::size_t get_index_from_ID(std::string ID);
		std::string get_ID_from_index(std::size_t index);		
		void add(CEXPR_Variable variable);
		double get(std::size_t index);
		void set(std::size_t index, double value);
		void set_with_str(std::string str, double value);
		CEXPR_Variable* get_variable(std::size_t index);
	public:
		std::vector<CEXPR_Variable> mv_variable;
};

class CEXPR_VariableReference
{
	public:
		CEXPR_VariableReference() { m_set= false; }
		std::string m_ID;
		std::size_t get_index();
		double get(CEXPR * p_expr);
		void set(std::size_t index);
		void update(double value);
	private:	
		bool m_set;
		std::size_t m_index;
		CEXPR_VariableList* mp_variable_list;
};

class CEXPR_Operand
{
	public:
		CEXPR_Operand();		
		std::size_t m_active_index;
		eEXPR_operand_type m_type;
		std::vector<double> mv_constant;
		std::vector<CEXPR_VariableReference> mv_variable_reference;
		std::vector<CEXPR_OperationReference> mv_operation_reference;
		void add_variable_reference (CEXPR_VariableReference variable_reference);
		void add_operation_reference (CEXPR_OperationReference operation_reference);	
		bool link_variable_and_operation_references(CEXPR * p_expr);
		double get(CEXPR * p_expr);	
};

class CEXPR_Operation
{
	public:	
		CEXPR_Operation();		
		std::string m_ID;	
		//unsigned m_arity;
		eEXPR_operator_type m_operator_type;
		std::vector<CEXPR_Operand> mv_operand;	
		double evaluate(CEXPR * p_expr);
		CEXPR differentiate(CEXPR * p_expr, std::string WRT);
		void set_operator_type(eEXPR_operator_type operator_type){ m_operator_type= operator_type; }
		bool link_operand_references(CEXPR *p_expr);
		std::vector<std::size_t> get_ref_operation_indices(void);
		std::vector<std::size_t> get_ref_variable_indices(void);
		std::vector<double> get_ref_constants(void);
		std::vector<CEXPR_Operand_position> get_operand_positions(void);
};

class CEXPR_OperationList
{
	public:		
		CEXPR_OperationList();
		std::size_t get_index_from_ID(std::string ID);		
		std::string get_ID_from_index(std::size_t index);		
		void add(CEXPR_Operation operation);
		double evaluate(CEXPR * p_expr, std::size_t index);
		CEXPR differentiate(CEXPR * p_expr, std::size_t index, std::string WRT);
		CEXPR_Operation* get_operation(std::size_t index);
		bool link_operation_references(CEXPR* p_expr);		
		std::size_t size(void);				
	private:
		std::vector<CEXPR_Operation> mv_operation;
};

class CEXPR_OperationReference
{
	public:
		CEXPR_OperationReference();
		std::string m_ID;
		std::size_t get_index();
		void set(std::size_t index);
		double evaluate(CEXPR * p_expr);	
	private:	
		bool m_set;
		std::size_t m_index;
		CEXPR_OperationList* mp_operation_list;
};

class CEXPR
{
	public:
		CEXPR();
		std::size_t m_active_index;
		std::string m_ID;
		CEXPR_VariableList m_variable_list;
		CEXPR_OperationList m_operation_list;
		double evaluate_i(std::size_t index);	
		double evaluate();

		CEXPR differentiate(std::size_t index, std::string WRT);
		CEXPR differentiate(std::string WRT);
};


class CMDEXPRVarRef
{
	public:		
		CMDEXPRVarRef()  {m_validated= false; }
		std::string m_EXPR_ref_name;
		std::size_t m_EXPR_index;
		bool m_validated;
};

class CMDEXPRReference
{
	public:		
		std::string m_EXPR_ref_name;
		std::size_t m_EXPR_index;
		std::vector<CMDEXPRVarRef> mv_EXPR_var_ref;
};

class CMDEXPR
{
	public:
		std::vector<CMDEXPRReference> mv_expr_ref;	
		bool link();
		osg::Vec2d evaluate_2D(osg::Vec2d v);
		osg::Vec3d evaluate_3D(osg::Vec3d v);
		std::vector<double> evaluate_MD(std::vector<double> v);
};

class CEXPRInterval
{
	public:
		std::string m_name;
		double m_start;
		double m_end;
		long m_num_pts;
		double m_incr;		
};

class CEXPRVectorField
{
	public:	
		CEXPRVectorField();
		CMDEXPR m_MDEXPR;
		std::vector<CEXPRInterval> mv_expr_interval;
		double m_scale;
		bool create(std::vector<osg::Vec3d>* p_v3DPt, unsigned long* count);	
};

class CNumber
{
	public:		
		std::vector<double> mv_lf;
};

class CSequence
{
public:	
	double m_start;
	double m_end;
	double m_increment;

};

class CMultidimensionalSequence
{
public:
	std::vector<CSequence> mv_sequence;
};

class CEXPR_Set
{
public:
	unsigned long m_cardinality;
	std::vector<CNumber> mv_number;
};


CEXPR_Variable create_variable(CEXPR * p_expr, std::size_t index);

CEXPR_Operand create_constant_operand(double value);
CEXPR_Operand create_variable_operand(CEXPR * p_expr, std::size_t index);
CEXPR_Operand create_operation_operand(CEXPR * p_expr, std::string operation_ID);

CEXPR_Operation create_operation(std::string str_ID, std::string str_oprtr_type);

CEXPR_Operation create_operation_Const(CEXPR * p_expr, std::string str_ID, std::string str_oprtr_type, double const_value);
CEXPR_Operation create_operation_Var(CEXPR * p_expr, std::string str_ID, std::string str_oprtr_type, std::size_t var_index);

CEXPR_Operation create_operation_Const_Var(CEXPR * p_expr, std::string str_ID, double const_value, std::string str_oprtr_type, std::size_t var_index);
CEXPR_Operation create_operation_Var_Const(CEXPR * p_expr, std::string str_ID, std::size_t var_index, std::string str_oprtr_type, double const_value);

CEXPR_Operation create_operation_Const_Oprn(CEXPR * p_expr, std::string str_ID, double const_value, std::string str_oprtr_type, std::string str_oprn_ID);
CEXPR_Operation create_operation_Oprn_Const(CEXPR * p_expr, std::string str_ID, std::string str_oprn_ID, std::string str_oprtr_type, double const_value);

CEXPR_Operation create_operation_Var_Oprn(CEXPR * p_expr, std::string str_ID, std::size_t var_index, std::string str_oprtr_type, std::string str_oprn_ID);
CEXPR_Operation create_operation_Oprn_Var(CEXPR * p_expr, std::string str_ID, std::string str_oprn_ID, std::string str_oprtr_type, std::size_t var_index);

CEXPR_Operation create_operation_Oprn_Oprn(CEXPR * p_expr, std::string str_ID, std::string str_oprn1_ID, std::string str_oprtr_type, std::string str_oprn2_ID);


#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) ¤ Måtthëw Jãmês §tèphéñs ¤  All rights reserved.
//  2000
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//_EXPR_
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////