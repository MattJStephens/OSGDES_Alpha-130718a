////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) ¤ Måtthëw Jãmês §tèphéñs ¤  All rights reserved.
//  2000
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _EXPRESSION_
#define _EXPRESSION_

#include <string>
#include <vector>
#include <sstream>

#include <osg/Vec3d>

using namespace osg;
using namespace std;

enum e_unary_op_type { uOP_sin, uOP_cos, uOP_tan, uOP_asin, uOP_acos, uOP_atan, uOP_sinh, uOP_cosh, uOP_tanh, uOP_log, uOP_log10, uOP_sqrt, 
						uOP_exp, uOP_ones_compliment, uOP_logical_negation, uOP_floor, uOP_ceil, uOP_postfix_increment, uOP_postfix_decrement, uOP_error };

enum e_binary_op_type { bOP_power, bOP_multiplication, bOP_division, bOP_addition, bOP_subtract, bOP_modulus, 
						
						bOP_assignment, bOP_addition_assignment, bOP_subtraction_assignment, bOP_multiplication_assignment, bOP_division_assignment,
						bOP_modulus_assignment, bOP_left_shift_assignment, bOP_right_shift_assignment,
						bOP_bitwise_AND_assignment, bOP_bitwise_inclusive_OR_assignment, bOP_bitwise_exclusive_OR_assignment,
						bOP_left_shift, bOP_right_shift,
						bOP_bitwise_AND, bOP_bitwise_inclusive_OR, bOP_bitwise_exclusive_OR,								
						bOP_logical_OR, bOP_logical_AND,
						bOP_less_than, bOP_less_than_or_equal_to, bOP_greater_than, bOP_greater_than_or_equal_to, bOP_equality, bOP_not_equal, 
						bOP_error };

enum e_expr_type {expr_constant, expr_variable, expr_type_unary, expr_type_binary, expr_compound };

e_unary_op_type string_to_uOP(std::string str);
e_binary_op_type string_to_bOP(std::string str);

enum eVariableAssociationType { VariableAssoc_default, VariableAssoc_PAT_vec, VariableAssoc_elapsed_time };

enum eVariableAssociationSubType { VariableAssocSub_default, VariableAssocSub_x, VariableAssocSub_y, VariableAssocSub_z, VariableAssocSub_mag };

enum eCoordinateSystem { Cartesian, Polar, Spherical, Cylindrical, LogPolar, Curvilinear, Pluker, Generalised, Canonical, Parallel };

class CVarAssoc
{
public:
	eVariableAssociationType m_assoc;
	eVariableAssociationSubType m_sub_assoc;
	std::string m_ref_expr;
	std::string m_ref_assoc;
	long m_expr_index;
	long m_assoc_index;
};

class CExpVar
{
	public:
		std::string m_name;
		float m_value;
};

class CExpression;//class forward

class CExpressionVariables
{
	public:
		CExpressionVariables();
		~CExpressionVariables();
		std::vector<CExpVar> mv_variable;
		//std::vector<CExpression*> mv_compound_expression;
		std::vector<CExpression> mv_compound_expression;
};

std::string create_compound_expression(std::string str_expr, CExpressionVariables* p_expr_var);				

class CExpression
{
	public:
		CExpression();
		~CExpression();		
		std::string m_name;
		bool create(std::string expression, CExpressionVariables* p_expr_var);
		float evaluate(void);		
	private:			
		std::string m_expression;			
		CExpressionVariables* mp_expr_var;
		e_expr_type m_expr_type;
		float m_constant;							
		std::vector<CExpression> mv_sub_expression;
		std::size_t m_active_variable_index;
		e_unary_op_type m_unary_mop_type;
		e_binary_op_type m_binary_mop_type;		
		std::size_t m_compound_index;	

		bool analyse(void);
		std::string m_sub_exp_left, m_exp_operator, m_sub_exp_right;	
		float evaluate_unary_math_operation(e_unary_op_type unary_mop_type, float right);
		float evaluate_binary_math_operation(float left, e_binary_op_type binary_mop_type, float right);
		
};


class CExprVariable
{
	public:
		std::string m_name;
		float m_start;
		float m_end;
		long m_num_pts;
		float m_incr;		
};

class CExprFunction : public CExpression
{
	public:
		std::string m_str_expression;
		std::vector<CExprVariable> mv_variables;
};

class CFunctionOfTwoVariables {
public:
	float m_start_x;
	float m_start_y;
	float m_end_x;
	float m_end_y;
	long m_num_pts_x;
	long m_num_pts_y;
	float mx_incr;
	float my_incr;			
	std::string m_expression;
	bool create(std::vector<Vec3d>* p_v3DPt);
};

class CFunctionOfOneVariable {
public:
	float m_start_x;
	float m_end_x;
	long m_num_pts_x;
	float mx_incr;
	eCoordinateSystem m_coordinate_system;
	std::string m_expression;
	bool create(std::vector<Vec3d>* p_v3DPt);
};


class CParametricFunction
{
	public:
		float m_start;
		float m_end;
		long m_num_pts;
		float m_incr;		
		vector<CExprFunction> mv_expr_function;
		std::vector<CExprVariable> mv_variables;
		bool create(std::vector<Vec3d>* p_v3DPt);
};

#endif