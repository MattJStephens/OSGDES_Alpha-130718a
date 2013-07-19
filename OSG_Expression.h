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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CExpression
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _EXPRESSION_
#define _EXPRESSION_

#include <string>
#include <vector>
#include <sstream>
#include <osg/Vec3d>

using namespace std;
using namespace osg;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// enum
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum e_unary_op_type { uOP_undefined, uOP_sin, uOP_cos, uOP_tan, uOP_asin, uOP_acos, uOP_atan, uOP_sinh, uOP_cosh, uOP_tanh, uOP_log, uOP_log10, uOP_sqrt, 
						uOP_exp, uOP_ones_compliment, uOP_logical_negation, uOP_floor, uOP_ceil, uOP_postfix_increment, uOP_postfix_decrement, uOP_beta,
						uOP_neumann, uOP_bessel, uOP_error_function, uOP_gamma, uOP_zeta, uOP_error };

enum e_binary_op_type { bOP_undefined, bOP_power, bOP_multiplication, bOP_division, bOP_addition, bOP_subtract, bOP_modulus, 						
						bOP_assignment, bOP_addition_assignment, bOP_subtraction_assignment, bOP_multiplication_assignment, bOP_division_assignment,
						bOP_modulus_assignment, bOP_left_shift_assignment, bOP_right_shift_assignment,
						bOP_bitwise_AND_assignment, bOP_bitwise_inclusive_OR_assignment, bOP_bitwise_exclusive_OR_assignment,
						bOP_left_shift, bOP_right_shift,
						bOP_bitwise_AND, bOP_bitwise_inclusive_OR, bOP_bitwise_exclusive_OR,								
						bOP_logical_OR, bOP_logical_AND,
						bOP_less_than, bOP_less_than_or_equal_to, bOP_greater_than, bOP_greater_than_or_equal_to, bOP_equality, bOP_not_equal, 
						bOP_error };
enum e_expr_type { expr_undefined, expr_constant, expr_variable, expr_type_unary, expr_type_binary };
enum eVariableAssociationType { VariableAssoc_default, VariableAssoc_PAT_vec, VariableAssoc_elapsed_time };
enum eVariableAssociationSubType { VariableAssocSub_default, VariableAssocSub_x, VariableAssocSub_y, VariableAssocSub_z, VariableAssocSub_mag };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// class forwards
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CExpressionList;	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
class CExpressionVariableList
{
	public:
		CExpressionVariableList();
		~CExpressionVariableList();
		std::vector<CExpVar> mv_variable;
};

class CExpressionList
{
	private:
		class CExpression
		{
			public:
				CExpression(CExpressionVariableList* p_expr_var, CExpressionList *p_expression_list)
				{
					mp_expression_list = p_expression_list;
					mp_expr_var = p_expr_var;
				}	
			public:
				CExpressionVariableList* mp_expr_var;
				CExpressionList *mp_expression_list;
				std::string m_ID;	
				std::size_t m_compound_expression_index;
				e_expr_type m_expr_type;
				bool synthesize(std::string expression, std::size_t* sub_expression_index);
				float evaluate(void);		
				std::string get_expression(void);
			private:	
				std::string m_sub_exp_left, m_exp_operator, m_sub_exp_right;	
				std::string m_expression;				
				float m_constant;					
				std::size_t m_sub_expression_alpha_index;
				std::size_t m_sub_expression_beta_index;		
				std::size_t m_active_variable_index;		
				e_unary_op_type m_unary_mop_type;
				e_binary_op_type m_binary_mop_type;		
				float evaluate_unary_math_operation(e_unary_op_type unary_mop_type, float right);
				float evaluate_binary_math_operation(float left, e_binary_op_type binary_mop_type, float right);		
		};
	public:		
		CExpressionVariableList* mp_expr_var;	
		std::vector<CExpression>mv_expression;
	public:
		CExpressionList();
		void set_ID(std::size_t index, std::string ID);
		std::string get_ID(std::size_t index);
		std::size_t get_index_from_ID(std::string ID);
		void set_compound_expression_index(std::size_t index, std::size_t compound_expression_index);
		void set_expr_type(std::size_t index, e_expr_type expr_type);
		std::size_t analyze(std::string str_expression, CExpressionVariableList* p_expr_var);
		float evaluate(std::size_t index);
		std::size_t m_index;
		
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// function class definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CFunctionOfTwoVariables {
public:
	std::size_t m_index;
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
	std::size_t m_index;
	float m_start_x;
	float m_end_x;
	long m_num_pts_x;
	float mx_incr;
	std::string m_expression;
	bool create(std::vector<Vec3d>* p_v3DPt);
};
class CExprInterval
{
	public:
		std::string m_name;
		float m_start;
		float m_end;
		long m_num_pts;
		float m_incr;		
};
class CExprFunction : public CExpressionList
{
	public:
		std::size_t m_index;
		std::string m_str_expression;
		std::vector<CExprInterval> mv_expr_interval;
};
class CParametricEquation
{
	public:	
		std::vector<std::string> mv_str_expression;		
		CExprInterval m_interval;
		bool create(std::vector<Vec3d>* p_v3DPt);	
};

class CVectorField
{
	public:	
		
		std::vector<std::string> mv_str_expression;		
		std::vector<CExprInterval> mv_expr_interval;
		float m_scale;	
		bool create(std::vector<Vec3d>* p_v3DPt, unsigned long* count);	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool create_sub_expression(std::string* new_expr, CExpressionVariableList* p_expr_var, CExpressionList *p_expression_list);	
e_unary_op_type string_to_uOP(std::string str);
e_binary_op_type string_to_bOP(std::string str);

#endif