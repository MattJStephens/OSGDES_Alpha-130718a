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

#include <cmath>
#include <string>
#include "OSG_tools.h"
#include "stdafx.h"
#include "OSG_expression.h"

#include <boost/math/special_functions/zeta.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/beta.hpp>
#include <boost/math/special_functions/erf.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// TOOL function definitions 1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//inline int round_double(double n)
//{
//	return static_cast<int>(floor(n+= 0.5));
//}
//inline int round_float(float n)
//{
//	return static_cast<int>(floor(n+= 0.5));
//}
//inline float random_float(float max)
//{
//	float r = (float)rand()/(float)RAND_MAX; 
//	return r*max;
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TOOL function definitions 2
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string remove_white_space(std::string in)
{
	std::string out;
	std::string temp;
	std::istringstream iss(in);
	while(iss>> temp)
	{
		out+= temp;
	}

	return out;
}
void check_expression_variable(std::string* expr, CExpressionVariableList* p_expr_var)
{
	std::string cache= *expr;	
	std::vector<CExpVar>::iterator exp_var_iter;
	for(exp_var_iter= p_expr_var->mv_variable.begin(); exp_var_iter!= p_expr_var->mv_variable.end(); exp_var_iter++)
	{
		std::basic_string<char>::size_type index= expr->find(exp_var_iter->m_name);
		if(index!= std::string::npos)
		{		
			std::size_t st= exp_var_iter->m_name.size();
			expr->erase(index, index+ st);	
		}
	}	
	std::string answer= *expr;
	*expr= cache;
}
std::string remove_string_in_brackets(std::string* in)
{
	std::string out;
	std::size_t startPos = 0;
	std::size_t endPos = std::string::npos;

	startPos= in->find_first_of("(");
	endPos= in-> find_last_of(")");

	return out;
}
bool are_brackets_balanced(std::string str, std::size_t* count)
{
	std::size_t iPos = 0;
	std::size_t start_iPos = 0;
	unsigned i= 0;
	do
	{
		iPos= str.find_first_of("(", start_iPos);
		start_iPos= iPos+ 1;
		i++;		
	}while (iPos!= std::string::npos);

	start_iPos= 0;
	iPos = 0;
	unsigned j= 0;
	do
	{
		iPos= str.find_first_of(")", start_iPos);
		start_iPos= iPos+ 1;
		j++;		
	}while (iPos!= std::string::npos);
    
	*count= i-1;
	if(i==j) return true;

	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// expression function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
e_unary_op_type string_to_uOP(std::string str)
{
	if(str== "sin") return uOP_sin;
	if(str== "cos") return uOP_cos;
	if(str== "tan") return uOP_tan;
	if(str== "asin") return uOP_asin;
	if(str== "acos") return uOP_acos;
	if(str== "atan") return uOP_atan;
	if(str== "sinh") return uOP_sinh;
	if(str== "cosh") return uOP_cosh;
	if(str== "tanh") return uOP_tanh;
	if(str== "log") return uOP_log;
	if(str== "log10") return uOP_log10;
	if(str== "exp") return uOP_exp;
	if(str== "sqrt") return uOP_sqrt;
	if(str== "~") return uOP_ones_compliment;
	if(str== "!") return uOP_logical_negation;
	if(str== "floor") return uOP_floor;
	if(str== "ceil") return uOP_ceil;
	if(str== "zeta") return uOP_zeta;
	if(str== "gamma") return uOP_gamma;
	if(str== "beta") return uOP_beta;
	if(str== "bessel") return uOP_bessel;
	if(str== "++") return uOP_postfix_increment;
	if(str== "--") return uOP_postfix_decrement;
	return uOP_error;
}
e_binary_op_type string_to_bOP(std::string str)
{	
	//additive
	if(str== "+") return bOP_addition;
	if(str== "-") return bOP_subtract;	

	//multiplicative
	if(str== "*") return bOP_multiplication;
	if(str== "/") return bOP_division;	
	if(str== "%") return bOP_modulus;	

	//to the power of
	if(str== "^") return bOP_power;//NOTE: xor	

	//assignment
	if(str== "=") return bOP_assignment;		
	if(str== "+=") return bOP_addition_assignment;		
	if(str== "-=") return bOP_subtraction_assignment;		
	if(str== "*=") return bOP_multiplication_assignment;		
	if(str== "/=") return bOP_division_assignment;		
	if(str== "%=") return bOP_modulus_assignment;		
	if(str== "<<=") return bOP_left_shift_assignment;	
	if(str== ">>=") return bOP_right_shift_assignment;		
	if(str== "&=") return bOP_bitwise_AND_assignment;	
	if(str== "|=") return bOP_bitwise_inclusive_OR_assignment;	
	if(str== "^=") return bOP_bitwise_exclusive_OR_assignment;

	//shift
	if(str== "<<") return bOP_left_shift;	
	if(str== ">>") return bOP_right_shift;

	//bitwise
	if(str== "&") return bOP_bitwise_AND;	
	if(str== "|") return bOP_bitwise_inclusive_OR;
	if(str== "xor") return bOP_bitwise_exclusive_OR;//NOTE: ^	
	
	//logical
	if(str== "||") return bOP_logical_OR;	
	if(str== "&&") return bOP_logical_AND;

	//conditional
	if(str== "<") return bOP_less_than;
	if(str== "<=") return bOP_less_than_or_equal_to;
	if(str== ">") return bOP_greater_than;
	if(str== ">=") return bOP_greater_than_or_equal_to;
	if(str== "==") return bOP_equality;
	if(str== "!=") return bOP_not_equal;

	return bOP_error;
}
bool find_specific_principal_operator(std::string op, std::string* in, std::string* sub_exp_left, std::string* expr_operator, std::string* sub_exp_right)
{
	if(in->size()>2)//??
	{
		std::basic_string <char>::size_type index= in->find(op);
		if(index!= std::string::npos)
		{
			sub_exp_left->erase(index, std::string::npos);	
			sub_exp_right->erase(0, index+ op.size());				
			*expr_operator= op;
			return true;
		}
	}
	return false;
}
bool find_principal_operator(std::string* in, std::string* sub_exp_left, std::string* expr_operator, std::string* sub_exp_right)
{	
	if(in->size() == 0)
	{
			return false;
	}
	
	*sub_exp_left= *in;
	*sub_exp_right= *in;		
		

	//NOTE: the order defines operator precedence	
	if(find_specific_principal_operator("+", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("-", in, sub_exp_left, expr_operator, sub_exp_right))
	{
		std::size_t lsize= sub_exp_left->size();
		if(lsize!= 0)
		{
			//char ch= (*sub_exp_left)[lsize- 1];			
			return true;	
		}
		else
		{
			*sub_exp_left= *in;
			*sub_exp_right= *in;		
		}
	}
	if(find_specific_principal_operator("*", in, sub_exp_left, expr_operator, sub_exp_right)) return true;
	if(find_specific_principal_operator("/", in, sub_exp_left, expr_operator, sub_exp_right)) return true;
	if(find_specific_principal_operator("^", in, sub_exp_left, expr_operator, sub_exp_right)) return true;

	//put hyperbolics and arcs before cos the strings're longer
	if(find_specific_principal_operator("asin", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("acos", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("atan", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("sinh", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("cosh", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("tanh", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	

	if(find_specific_principal_operator("sin", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("cos", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("tan", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		

	if(find_specific_principal_operator("log10", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	
	if(find_specific_principal_operator("log", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("exp", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("sqrt", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		

	if(find_specific_principal_operator("zeta", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("beta", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("gamma", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("bessel", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		

	if(find_specific_principal_operator("&&", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	
	if(find_specific_principal_operator("&=", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	
	if(find_specific_principal_operator("&", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	
	if(find_specific_principal_operator("|", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	
	if(find_specific_principal_operator("~", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	
	if(find_specific_principal_operator("!", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	
	if(find_specific_principal_operator("!=", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	
	if(find_specific_principal_operator("||", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	
	if(find_specific_principal_operator("|=", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	
	if(find_specific_principal_operator("xor_eq", in, sub_exp_left, expr_operator, sub_exp_right)) return true;	
	if(find_specific_principal_operator("xor", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	
	if(find_specific_principal_operator("<", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("<=", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator(">", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator(">=", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		
	if(find_specific_principal_operator("==", in, sub_exp_left, expr_operator, sub_exp_right)) return true;		



	
	return false;
}
bool create_sub_expression(std::string* new_expr, CExpressionVariableList* p_expr_var, CExpressionList *p_expression_list)
{
	std::string original_expr= *new_expr;
	*new_expr= "";
	
	std::size_t opening_bracket_position = 0;
	std::size_t closing_bracket_position = 0;	
	bool found_opening_bracket= false;	
	unsigned long num_of_opening_brackets= 0;
	unsigned long num_of_closing_brackets= 0;
	
	std::string::iterator str_iter= original_expr.begin();
	while(str_iter!= original_expr.end())
	{
		if(*str_iter== '(')
		{
			num_of_opening_brackets++;
			if(!found_opening_bracket)
			{
				found_opening_bracket= true;
				opening_bracket_position= std::distance(original_expr.begin(), str_iter);
			}
		}
		else if(*str_iter== ')')
		{
			num_of_closing_brackets++;
			if(num_of_opening_brackets== num_of_closing_brackets)
			{
				std::string sub_expr;					
				closing_bracket_position= std::distance(original_expr.begin(), str_iter);				
				std::string::iterator sub_str_iter= original_expr.begin()+ opening_bracket_position+ 1;				
				while(sub_str_iter!= original_expr.begin()+closing_bracket_position)
				{
					sub_expr+= (*sub_str_iter);
					++sub_str_iter;
				}
				std::size_t cmp_expr_index = p_expression_list->analyze(sub_expr, p_expr_var);
				std::string ID = p_expression_list->get_ID(cmp_expr_index);
				(*new_expr)+= ID;	
				found_opening_bracket= false;
				opening_bracket_position = 0;
				closing_bracket_position = 0;	
				num_of_opening_brackets= 0;
				num_of_closing_brackets= 0;
			}
		}
		else if(!found_opening_bracket)
		{
			(*new_expr)+= (*str_iter);
		}
		//TODO what about after the brackets??
		++str_iter;
	}	

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CExpressionVariableList
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CExpressionVariableList::CExpressionVariableList()
{
}
CExpressionVariableList::~CExpressionVariableList()
{
	/*std::vector<CExpression>::iterator iter;
	for(iter= mv_compound_expression.begin(); iter!= mv_compound_expression.end();  ++iter)
	{
		delete (*iter);
	}*/
}


std::string CExpressionList::CExpression::get_expression(void)
{
	return m_expression;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CExpression
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CExpressionList::CExpression::synthesize(std::string expression, std::size_t* sub_expression_index)
{	
	*sub_expression_index = UINT_MAX;
	m_expression= remove_white_space(expression);	
	check_expression_variable(&expression, mp_expr_var);

	//return analyse();
	if(m_expression.size()== 0) return false;	
	
	//brackets
	std::size_t bracket_count;	
	if(!are_brackets_balanced(m_expression, &bracket_count))
	{
		return false;
	}	

	//compound expression
	std::string original_expression = m_expression;//temp copy
	if(bracket_count> 0)
	{				
		create_sub_expression(&m_expression, mp_expr_var, mp_expression_list);		
	}

	//look for operator of precedence
	bool found_operator = false;
	found_operator = find_principal_operator(&m_expression, &m_sub_exp_left, &m_exp_operator, &m_sub_exp_right);	
	if(found_operator)
	{
		if(string_to_uOP(m_exp_operator)!= uOP_error)
		{
			m_expr_type= expr_type_unary;
			m_sub_expression_alpha_index = mp_expression_list->analyze(m_sub_exp_right, mp_expr_var);			
			m_unary_mop_type= string_to_uOP(m_exp_operator);			
		}
		else if(string_to_bOP(m_exp_operator)!= bOP_error)
		{	
			m_expr_type= expr_type_binary;		
			m_sub_expression_alpha_index = mp_expression_list->analyze(m_sub_exp_left, mp_expr_var);	
			m_sub_expression_beta_index = mp_expression_list->analyze(m_sub_exp_right, mp_expr_var);	
			m_binary_mop_type= string_to_bOP(m_exp_operator);			
		}		
		return true;
	}

	*sub_expression_index= mp_expression_list->get_index_from_ID(m_expression);
	if(*sub_expression_index!= UINT_MAX)
	{
		return true;
	}


	////search compound expression list to see if this expression is a compound expression;
	////then set the expression type and the m_compound_expression_index 
	//std::vector<std::size_t>::iterator iter;
	//for(iter= mp_expr_var->mv_compound_expression_index.begin(); iter!= mp_expr_var->mv_compound_expression_index.end();  ++iter)
	//{
	//	if(mp_expression_list->get_ID(*iter)== m_expression)
	//	{
	//		m_expr_type= expr_compound;
	//		m_compound_expression_index= std::distance(mp_expr_var->mv_compound_expression_index.begin(), iter);
	//		return true;
	//	}
	//}

	//loop throup variable list
	//is expression a variable?
	std::vector<CExpVar>::iterator exp_var_iter;
	if(mp_expr_var!= NULL)
	{
		for(exp_var_iter= mp_expr_var->mv_variable.begin(); exp_var_iter!= mp_expr_var->mv_variable.end(); exp_var_iter++)
		{
			std::basic_string<char>::size_type index= m_expression.find(exp_var_iter->m_name);
			if(index!= std::string::npos)
			{		
				m_expr_type= expr_variable;			
				m_active_variable_index= std::distance(mp_expr_var->mv_variable.begin(), exp_var_iter);
				return true;
			}
		}
	}
	
	//no sub expressions so must be a constant!?!	
	m_expr_type= expr_constant;
	std::istringstream iss(m_expression);
	iss >> m_constant;	

	return true;
}
float CExpressionList::CExpression::evaluate(void)
{
	float answer= FLT_MAX;
	switch(m_expr_type)
	{
		case expr_constant:
		{
			answer = m_constant;
			break;
		}
		case expr_variable:
		{
			if(mp_expr_var!= NULL)
			{
				if(m_active_variable_index< mp_expr_var->mv_variable.size())
				{
					answer =  mp_expr_var->mv_variable[m_active_variable_index].m_value;
				}
			}
			break;
		}
		case expr_type_unary:
		{
			answer = evaluate_unary_math_operation(m_unary_mop_type, mp_expression_list->evaluate(m_sub_expression_alpha_index));				
			break;
		}		
		case expr_type_binary:
		{					
			answer = evaluate_binary_math_operation(mp_expression_list->evaluate(m_sub_expression_alpha_index), m_binary_mop_type, mp_expression_list->evaluate(m_sub_expression_beta_index));												
			break;
		}	
		/*case expr_compound:
		{
			answer = mp_expression_list->evaluate(m_compound_expression_index);													
			break;
		}		*/	
	}

	return answer;
}
float CExpressionList::CExpression::evaluate_unary_math_operation(e_unary_op_type unary_mop_type, float right)
{	
	float result= FLT_MAX;
	
	switch(unary_mop_type)
	{			
		case uOP_sin:
		{			
			result= sinf(right);
			break;
		}		
		case uOP_cos:
		{			
			result= cosf(right);
			break;
		}		
		case uOP_tan:
		{			
			result= tanf(right);
			break;
		}	
		case uOP_asin:
		{			
			result= asinf(right);
			break;
		}		
		case uOP_acos:
		{			
			result= acosf(right);
			break;
		}		
		case uOP_atan:
		{			
			result= atanf(right);
			break;
		}	
		case uOP_sinh:
		{			
			result= sinhf(right);
			break;
		}		
		case uOP_cosh:
		{			
			result= coshf(right);
			break;
		}		
		case uOP_tanh:
		{			
			result= tanhf(right);
			break;
		}	
		case uOP_log:
		{			
			result= logf(right);
			break;
		}	
		case uOP_log10:
		{			
			result= log10f(right);
			break;
		}	
		case uOP_exp:
		{			
			result= expf(right);
			break;
		}	
		case uOP_sqrt:
		{			
			result= sqrtf(right);
			break;
		}	
		case uOP_bessel:
		{
			result= boost::math::cyl_bessel_j(0, right);
			break;
		}
		case uOP_gamma:
		{
			result= boost::math::tgamma(right);
			break;
		}
		case uOP_zeta:
		{
			if(right==1)
				result = 0;
			else
				result= boost::math::zeta(right);
			break;
		}
		case uOP_ones_compliment:
		{			
			int n_right= round_float(right);
			result= (float)(~n_right);
			break;
		}	
		case uOP_logical_negation:
		{		
			//note: 
			//bool b_right= 1;			
			//if(right== 0) b_right= 0;

			int n_right= round_float(right);//fuzzy
			result= (float)(!n_right);
			break;
		}	
		case uOP_floor:
		{			
			result= floorf(right);
			break;
		}	
		case uOP_ceil:
		{			
			result= ceilf(right);
			break;
		}	
		case uOP_postfix_increment:
		{		
			if(result< 0.0)
			{
				result= (right+ 1.0f);
			}
			else if(right<= (FLT_MAX- 1.0))
			{
				result= (right+ 1.0f);
			}
			else result = FLT_MAX;

			break;
		}	
		case uOP_postfix_decrement:
		{			
			if(result> 0.0)
			{
				result= (right- 1.0f);
			}
			else if(right>= (FLT_MIN+ 1.0))
			{
				result= (right- 1.0f);
			}
			else result = FLT_MIN;//should we do this or produce an error?

			break;
		}	
	};
	
	return result;
}
float CExpressionList::CExpression::evaluate_binary_math_operation(float left, e_binary_op_type binary_mop_type, float right)
{	
	float result= FLT_MAX;
	
	switch(binary_mop_type)
	{	
		//additive
		case bOP_addition:
		{		
			result= left+ right;
			break;
		}
		case bOP_subtract:
		{			
			result= left- right;
			break;
		}	

		//multiplicative
		case bOP_multiplication:
		{			
			result= left* right;
			break;
		}
		case bOP_division:
		{		
			if(right!= 0)
				result= left/ right;			
			break;
		}	
		
		//to the power of
		case bOP_power:
		{			
		    result= powf(left, right);
			break;
		}

		//assignment
		case bOP_assignment:
		{	
			left= right;
			return left;
			break;
		}	
		case bOP_addition_assignment:
		{	
			left+= right;
			return left;
			break;
		}	
		case bOP_subtraction_assignment:
		{	
			left-= right;
			return left;
			break;
		}	
		case bOP_multiplication_assignment:
		{	
			left*= right;
			return left;
			break;
		}	
		case bOP_division_assignment:
		{	
			left/= right;
			return left;
			break;
		}	
		case bOP_modulus_assignment:
		{	
			int n_left= round_float(left);
			int n_right= round_float(right);
			n_left%= n_right;
			return (float)n_left;
			break;
		}	
		case bOP_left_shift_assignment:
		{	
			int n_left= round_float(left);
			int n_right= round_float(right);
			n_left<<= n_right;
			return (float)n_left;
			break;
		}	
		case bOP_right_shift_assignment:
		{	
			int n_left= round_float(left);
			int n_right= round_float(right);
			n_left>>= n_right;
			return (float)n_left;
			break;
		}	
		case bOP_bitwise_inclusive_OR_assignment:
		{				
			int n_left= round_float(left);
			int n_right= round_float(right);
			n_left|= n_right;
			return (float)n_left;
			break;
		}	
		case bOP_bitwise_AND_assignment:
		{				
			int n_left= round_float(left);
			int n_right= round_float(right);
			n_left&= n_right;
			return (float)n_left;
			break;		
		}			
		case bOP_bitwise_exclusive_OR_assignment:
		{				
			int n_left= round_float(left);
			int n_right= round_float(right);
			n_left^= n_right;
			return (float)n_left;
			break;		
		}	

		//shift
		case bOP_left_shift:
		{				
			int n_left= round_float(left);
			int n_right= round_float(right);
			result = (float)(n_left << n_right);
			break;
		}	
		case bOP_right_shift:
		{				
			int n_left= round_float(left);
			int n_right= round_float(right);
			result = (float)(n_left >> n_right);
			break;
		}	

		//bitwise
		case bOP_bitwise_AND:
		{				
			int n_left= round_float(left);
			int n_right= round_float(right);
			result = (float)(n_left & n_right);
			break;
		}	
		case bOP_bitwise_inclusive_OR:
		{				
			int n_left= round_float(left);
			int n_right= round_float(right);
			result = (float)(n_left | n_right);
			break;
		}	
		case bOP_bitwise_exclusive_OR: //XOR
		{				
			int n_left= round_float(left);
			int n_right= round_float(right);
			result = (float)(n_left ^ n_right);
			break;
		}

		//logical (BOOLEAN)		
		case bOP_logical_OR:
		{							
			bool b_left= true;			
			if(left== 0.0) b_left= false;	
			bool b_right= true;		
			if(right== 0.0) b_right= false;	
			result = (float)(b_left || b_right);
			break;
		}
		case bOP_logical_AND:
		{				
			bool b_left= true;			
			if(left== 0.0) b_left= false;	
			bool b_right= true;			
			if(right== 0.0) b_right= false;	
			result = (float)(b_left && b_right);
			break;
		}	

		//conditional	
		case bOP_less_than:
		{				
			if(left< right) result =  1.0;
			else result = 0.0;
			break;
		}		
		case bOP_less_than_or_equal_to:
		{				
			if(left<= right) result =  1.0;
			else result = 0.0;
			break;
		}		
		case bOP_greater_than:
		{				
			if(left> right) result =  1.0;
			else result = 0.0;
			break;
		}		
		case bOP_greater_than_or_equal_to:
		{				
			if(left>= right) result =  1.0;
			else result = 0.0;
			break;
		}		
		case bOP_equality:
		{				
			if(left== right) result =  1.0;
			else result = 0.0;
			break;
		}		
		case bOP_not_equal:
		{				
			if(left!= right) result =  1.0;
			else result = 0.0;
			break;
		}			
	};
	
	return result;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CExpressionList
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CExpressionList::CExpressionList()
{

}
std::size_t CExpressionList::analyze(std::string str_expression, CExpressionVariableList* p_expr_var)
{
	mp_expr_var = p_expr_var;
	std::size_t sub_expression_index;
	CExpression expression(mp_expr_var, this);	
	bool creation_successful = expression.synthesize(str_expression, &sub_expression_index);
	std::size_t index = UINT_MAX;
	if(sub_expression_index== UINT_MAX)
	{
		index = mv_expression.size();
		std::ostringstream oss_ID;
		oss_ID << "#" << index << "#";
		expression.m_ID = oss_ID.str();
		mv_expression.push_back(expression);
	}
	else
	{
		index = sub_expression_index;
	}	
	if(!creation_successful)
	{
		index = UINT_MAX;
	}
	//m_index = index;
	return index;
}
std::string CExpressionList::get_ID(std::size_t index)
{
	if(index< mv_expression.size())
	{
		return mv_expression[index].m_ID;
	}
	return "UNDEFINED";
}
void CExpressionList::set_ID(std::size_t index, std::string ID)
{
	if(index< mv_expression.size())
	{
		mv_expression[index].m_ID = ID;
	}
}
std::size_t CExpressionList::get_index_from_ID(std::string ID)
{
	std::vector<CExpression>::iterator iter;
	for(iter = mv_expression.begin(); iter != mv_expression.end(); ++iter)
	{
		if(ID == iter->m_ID)
		{
			return std::distance(mv_expression.begin(), iter);
		}
	}
	return UINT_MAX;
}
float CExpressionList::evaluate(std::size_t index)
{
	if(index< mv_expression.size())
	{
		return mv_expression[index].evaluate();
	}
	return FLT_MAX;


}
void CExpressionList::set_compound_expression_index(std::size_t index, std::size_t compound_expression_index)
{
	if(index< mv_expression.size())
	{
		mv_expression[index].m_compound_expression_index = compound_expression_index;
	}
}
void CExpressionList::set_expr_type(std::size_t index, e_expr_type expr_type)
{
	if(index< mv_expression.size())
	{
		mv_expression[index].m_expr_type = expr_type;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFunctionOfOneVariable
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFunctionOfOneVariable::create(std::vector<Vec3d>* p_v3DPt)
{
	CExpressionVariableList EVL;
	CExpressionList m_expression_list;	
	CExpVar expr_var;
	expr_var.m_name= "x";
	EVL.mv_variable.push_back(expr_var);
	m_index = m_expression_list.analyze(m_expression, &EVL);
	mx_incr= (m_end_x- m_start_x)/ m_num_pts_x;
	float x= m_start_x;
	float y=0;
	for(long dw_x= 0; dw_x< m_num_pts_x; dw_x++) {
		Vec3d vec;
		EVL.mv_variable[0].m_value= x;		
		y= m_expression_list.evaluate(m_index);
		vec.set(x,y,0);
		p_v3DPt->push_back(vec);
		x+= mx_incr;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFunctionOfTwoVariables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFunctionOfTwoVariables::create(std::vector<Vec3d>* p_v3DPt)
{
	CExpressionVariableList EVL;
	CExpressionList m_expression_list;
	CExpVar expr_varx;
	expr_varx.m_name= "x";
	CExpVar expr_varz;
	expr_varz.m_name= "y";
	EVL.mv_variable.push_back(expr_varx);
	EVL.mv_variable.push_back(expr_varz);	
	m_index = m_expression_list.analyze(m_expression, &EVL);
	mx_incr= (m_end_x- m_start_x)/ m_num_pts_x;
	my_incr= (m_end_y- m_start_y)/ m_num_pts_y;	
	float y= m_start_y;
	float x= m_start_x;
	float z= 0;
	for(long dw_y= 0; dw_y< m_num_pts_y; dw_y++) {
		x= m_start_x;
		for(long dw_x= 0; dw_x< m_num_pts_x; dw_x++) {
			Vec3d vec;
			EVL.mv_variable[0].m_value= x;
			EVL.mv_variable[1].m_value= y;		
			z = m_expression_list.evaluate(m_index);
			vec.set(x,y,z);
			x+= mx_incr;
			p_v3DPt->push_back(vec);
		}
		y+= my_incr;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CParametricEquation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CParametricEquation::create(std::vector<Vec3d>* p_v3DPt)
{	
	//std::vector<CExpressionList> v_ExprLst;
	CExpressionList EL1;
	CExpressionList EL2;
	CExpressionList EL3;

	std::size_t index1, index2, index3;

	CExpressionVariableList EVL;	
	CExpVar expr_var;
	expr_var.m_name = m_interval.m_name;
	EVL.mv_variable.push_back(expr_var);
	
	index1= EL1.analyze(mv_str_expression[0], &EVL);
	index2= EL2.analyze(mv_str_expression[1], &EVL);
	index3= EL3.analyze(mv_str_expression[2], &EVL);

	m_interval.m_incr= (m_interval.m_end- m_interval.m_start)/ m_interval.m_num_pts;
	float t= m_interval.m_start;
	for(long dt= 0; dt<= m_interval.m_num_pts; dt++) {
		Vec3d vec;
		EVL.mv_variable[0].m_value= t;		
		float x=0;
		float y=0;
		float z=0;
		x = EL1.evaluate(index1);
		y = EL2.evaluate(index2);
		z = EL3.evaluate(index3);
		vec.set(x, y, z);
		p_v3DPt->push_back(vec);
		t+= m_interval.m_incr;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CParametricEquation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CVectorField::create(std::vector<Vec3d>* p_v3DPt, unsigned long* count)
{
	
	//std::vector<CExpressionList> v_ExprLst;
	CExpressionList EL0;
	CExpressionList EL1;
	CExpressionList EL2;

	std::size_t index0, index1, index2;

	CExpressionVariableList EVL;	
	std::vector<CExprInterval>::iterator interval_iter;
	for(interval_iter = mv_expr_interval.begin(); interval_iter != mv_expr_interval.end(); ++interval_iter)
	{
		CExpVar expr_var;
		expr_var.m_name = interval_iter->m_name;
		EVL.mv_variable.push_back(expr_var);
	}
	
	index0= EL0.analyze(mv_str_expression[0], &EVL);
	index1= EL1.analyze(mv_str_expression[1], &EVL);
	index2= EL2.analyze(mv_str_expression[2], &EVL);

	float x=0;
	float y=0;
	float z=0;
	
	mv_expr_interval[0].m_incr= (mv_expr_interval[0].m_end- mv_expr_interval[0].m_start)/ mv_expr_interval[0].m_num_pts;
	mv_expr_interval[1].m_incr= (mv_expr_interval[1].m_end- mv_expr_interval[1].m_start)/ mv_expr_interval[1].m_num_pts;
	mv_expr_interval[2].m_incr= (mv_expr_interval[2].m_end- mv_expr_interval[2].m_start)/ mv_expr_interval[2].m_num_pts;	

	*count= 0;
	long nx,ny,nz;
	x= mv_expr_interval[0].m_start;
	for(nx= 0; nx < mv_expr_interval[0].m_num_pts; nx++) 
	{	
		y= mv_expr_interval[1].m_start;		
		for(ny= 0; ny < mv_expr_interval[1].m_num_pts; ny++) 
		{
			z= mv_expr_interval[2].m_start;			
			for(nz= 0; nz < mv_expr_interval[2].m_num_pts; nz++) 
			{		
				Vec3d vecA, vecB;
				vecA.set(x,y,z);				

				EVL.mv_variable[0].m_value= x;		
				EVL.mv_variable[1].m_value= y;		
				EVL.mv_variable[2].m_value= z;		
				float A0= m_scale* EL0.evaluate(index0);
				float A1= m_scale* EL1.evaluate(index1);
				float A2= m_scale* EL2.evaluate(index2);

				vecB.set(x+ A0,y+ A1,z+ A2);

				p_v3DPt->push_back(vecA);
				(*count)++;
				p_v3DPt->push_back(vecB);
				(*count)++;

				z+= mv_expr_interval[2].m_incr;
			}
			y+= mv_expr_interval[1].m_incr;
		}
		x+= mv_expr_interval[0].m_incr;
	}	
	
	
	return true;
}


/*
........................................................................................................................
........................................................................................................................
........................................................................................................................
........................................................................................................................
............................................................................................................,...........
................................................................................................................,.......
............................................................................................................,...........
...............................................................................:..,............. ...,:=:=?+.~+::,.......
.....................................................................:~?I=:==~+.:.................,:,...,......:=+~.....
..............................................,....................:?:,,.,..,....,=,.,........ .=:...............:,+....
........................................,::~::~::.,..,............~~..,......:..,.::~.........=~...,.,....,:=+:~....~,..
......................................,=?=+~.,:~?I+............~=+,.....,.,..,...~..?.......,+....:..:....,,,,:~:+,.~...
..................................,::+I=~.,.,....~.=++::......~~..........=:7~=+..,,.+..,...:,.:+?+......,..:=~,,.?,,:.:
............ .,,,:,...............,I?=~.,,....,.:,::=I~..:...~:~,.,......,,+~,==?7~..::....~..7=,:,:...:.=II7+7?~~:..,..
.......:7?Z$Z$ZZZ$$7++~~.........=I7$+=:,....,::,II+=?$,,.:.:~..,.::.:...+.??Z$$77+I:......,+?.:::,~.....~+?$ZZ$$=.:.,..
....7$ZZZZZZZZZZZZZZZZI+~,......+IZZ$?=:.,..,.~Z7?$Z$I7+....+:.?+I++7,..,I:IZZZZZ$I?.~=...:~:I7?7?::~,..?+?ZZZZZ7Z:.:...
..=Z?=:7ZZZZ7$ZZZ77Z$Z$Z$:,.....:7ZZZ$+:....,:?$$ZZZZ7I=....==I+7$Z?I:~..:.7$ZZZZ$??~......~OI77777.+:.,=+7ZZZZZI+:~:...
..Z.....+Z$ZZZ$ZZZOZZZZZZI:....,IZZZ$Z~.....:~77ZZZZZZI~,..~=:$ZZZ$7~=:..:,$$Z$ZZZ7?,+=,.:,IIZ7$Z?..~,..=~?IZZZ$7?~.?..,
.,+......ZZZZZZZ7$Z7ZZZZZZ:...,:ZZZZZ$I,...:.+7Z$ZZZZZI~.,.:.?ZZZZZ$?$~....I77ZZZ$??==...:?~ZZZZZ$$,,=..=~?IZZZZ7?=.:.::
.,$.....,ZOZZZZZZ?IZ7ZZZZZ~:...=ZZZZZZ7,....,:$?ZZZZZZ+...,+:IZZZ$ZZ7,+,.:,=+7$$$$I~::..,:=7ZZZZZ7,=:?...~+?I$$Z,?::..:.
..?O?=~7Z$ZZZZZ$I=:~7$ZZZ7?:...=?7ZZZ$I.....:=+I=7$I7~....=~,IOZZZZZI,~:..~.+???I~,,=:..,~~=ZOZZZ$~,..:..+:??+=~=:.=..:?
....7$7ZZ$ZZZZ++.,..??$777=:,...I$$ZZZ~,.....,I+==~,~....,.+~:$ZZZ$=,:=~.....,~+:==~~,,.,:,?7ZZZ$I=,.~...,~~~?~?...:...+
........,+~:,........~+I7??~::.:??ZZZ7:.......:+~~::,....:..=II??$I+:~?.. .~..::I?~~.,.,,7.,=7?I$.::.....:=+:.:,~.=....:
.....................=,+Z=?+,+~~I=$Z7:.........~?::.,.,...::.=$,I=,:.,+..,..:~.,.:.:,.....,~+~II.,+...:..,..$~:..:+....:
........................:+?$O7$ZIZ++~,..........~++,,,.,.......,.,.::Z.........,.. .......+.,:..:+..:~,. :..:I:~+?:..:..
.........................,,.,=:,:~.,.............,7+:=,............,7~.:,.,...~:...........:I:+I,...?:,.....+.,.=~,.....
....................................................,,+:=.,...,:.~~~...........,:::,.........~....,~.. ...........,.....
....................................................,.,+?:+==,~=::,..............,+=:,, .......:.:~.............,.......
...............................................................,.,,..................,=~+=,::=~+:.,..............=......
........................................................,.,..,.:........................::?~+......,..............==....
......................................................................................,.....,..........................:
........................................................................................,:..............................
..............................................................................................,.........................
........................................................................................................................
........................................................................................................................
........................................................................................................................
........................................................................................................................
........................................................................................................................
*/