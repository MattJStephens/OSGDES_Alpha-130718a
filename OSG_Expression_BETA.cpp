////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) ¤ Måtthëw Jãmês §tèphéñs ¤  All rights reserved.
//  2000
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <ostream>
#include "OSG_expression.h"
#include "OSG_tools.h"

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


//acos, asin, atan, atan2, cos, cosh, sin, sinh, tan, tanh
float CExpression::evaluate_unary_math_operation(e_unary_op_type unary_mop_type, float right)
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

float CExpression::evaluate_binary_math_operation(float left, e_binary_op_type binary_mop_type, float right)
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

bool find_specific_principal_operator(std::string op, std::string* in, std::string* sub_exp_left, std::string* expr_operator, std::string* sub_exp_right)
{
	if(in->size()>2)//??
	{
		std::basic_string <char>::size_type index= in->find(op);
		if(index!= std::string::npos)
		{
			sub_exp_left->erase(index, std::string::npos);	
			sub_exp_right->erase(0, index+ op.size());		

			if(expr_operator== NULL)
			{
				cout << "el problimo";
				return false;
			}
			*expr_operator= op;
			return true;
		}
	}
	return false;
}

bool find_principal_operator(std::string* in, std::string* sub_exp_left, std::string* expr_operator, std::string* sub_exp_right)
{	
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CExpression
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

void check_expression_variable(std::string* expr, CExpressionVariables* p_expr_var)
{
	std::string cache= *expr;

	//loop throup variable list
	//is expression a variable?
	std::vector<CExpVar>::iterator exp_var_iter;

	for(exp_var_iter= p_expr_var->mv_variable.begin(); exp_var_iter!= p_expr_var->mv_variable.end(); exp_var_iter++)
	{
		std::basic_string<char>::size_type index= expr->find(exp_var_iter->m_name);
		if(index!= std::string::npos)
		{		
			std::size_t st= exp_var_iter->m_name.size();
			expr->erase(index, index+ st);
			//m_expr_type= expr_variable;			
			//m_active_variable_index= std::distance(mp_expr_var->mv_variable.begin(), exp_var_iter);
			//return true;
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

CExpressionVariables::CExpressionVariables()
{
}

CExpressionVariables::~CExpressionVariables()
{
	mv_compound_expression.clear();
}

std::string create_compound_expression(std::string str_expr, CExpressionVariables* p_expr_var)
{
	if(p_expr_var == NULL)
	{
		cout << "p_expr_var == NULL";
	}
	
	std::ostringstream oss_out;
	std::string original_expr= str_expr;
	std::string::iterator str_iter= original_expr.begin();
	std::size_t opening_bracket_position = 0;
	std::size_t closing_bracket_position = 0;	
	bool found_opening_bracket= false;	
	unsigned long num_of_opening_brackets= 0;
	unsigned long num_of_closing_brackets= 0;
	unsigned long num_of_sub_expressions= 0;
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
				std::string name;
				std::string expr;
				std::size_t index;

				closing_bracket_position= std::distance(original_expr.begin(), str_iter);
				std::ostringstream oss;
				oss << "?" << num_of_sub_expressions;
				name= oss.str();
				std::string::iterator sub_str_iter= original_expr.begin()+ opening_bracket_position+ 1;				
				while(sub_str_iter!= original_expr.begin()+closing_bracket_position)
				{
					expr+= (*sub_str_iter);
					++sub_str_iter;
				}				

				index=p_expr_var->mv_compound_expression.size();
				CExpression com_expr;
				com_expr.create(expr, p_expr_var);
				com_expr.m_name = name;
				p_expr_var->mv_compound_expression.push_back(com_expr);
				/*if(index< p_expr_var->mv_compound_expression.size())
				{
					p_expr_var->mv_compound_expression[index].create(expr, p_expr_var);
					p_expr_var->mv_compound_expression[index].m_name = name;
				}
				else
				{
					cout << "ci< p_expr_var->mv_compound_expression.size()";

				}*/


				oss_out << name;	
				num_of_sub_expressions++;
				found_opening_bracket= false;
				opening_bracket_position = 0;
				closing_bracket_position = 0;	
				num_of_opening_brackets= 0;
				num_of_closing_brackets= 0;
			}
		}
		else if(!found_opening_bracket)
		{
			oss_out << *str_iter;
		}
		++str_iter;
	}	

	return oss_out.str();
}

CExpression::CExpression()
{		
	m_active_variable_index= ULONG_MAX;	
}

CExpression::~CExpression()
{
	

	mv_sub_expression.clear();
}

bool CExpression::create(std::string expression, CExpressionVariables* p_expr_var)
{
	mp_expr_var= p_expr_var;
	m_expression= remove_white_space(expression);	
	check_expression_variable(&expression, p_expr_var);
	return analyse();
}

bool CExpression::analyse(void)
{			
	if(m_expression.size()== 0) return false;	

	//check for brackets
	//if there are brackets then split expression up into a compound form
	std::size_t count;
	if(are_brackets_balanced(m_expression, &count))
	{
		if(count> 0)
		{		
			std::string str_in = m_expression;
			std::string str_out;
			str_out = create_compound_expression(str_in, mp_expr_var);		
			m_expression = str_out;
		}
	}
	else
	{		
		//LogMessage("error", "brackets are not balanced");
		return false;
	}

	//look for operator of precedence
	if(find_principal_operator(&m_expression, &m_sub_exp_left, &m_exp_operator, &m_sub_exp_right))
	{
		//is string a unary operator ?
		if(string_to_uOP(m_exp_operator)!= uOP_error)
		{
			//if m_sub_exp_left.size()> bad!!
			m_expr_type= expr_type_unary;		
			
			CExpression expr;
			expr.create(m_sub_exp_right, mp_expr_var);
			mv_sub_expression.push_back(expr);

			m_unary_mop_type= string_to_uOP(m_exp_operator);			
		}
		//is string a binary operator ?
		else if(string_to_bOP(m_exp_operator)!= bOP_error)
		{	
			m_expr_type= expr_type_binary;				

			CExpression alpha_expr;
			alpha_expr.create(m_sub_exp_left, mp_expr_var);
			mv_sub_expression.push_back(alpha_expr);

			CExpression beta_expr;
			beta_expr.create(m_sub_exp_right, mp_expr_var);
			mv_sub_expression.push_back(beta_expr);

			m_binary_mop_type= string_to_bOP(m_exp_operator);			
		}
		
		return true;
	}

	//search compound expression list to see if this expression is an existing compound one;
	std::vector<CExpression>::iterator iter;
	for(iter= mp_expr_var->mv_compound_expression.begin(); iter!= mp_expr_var->mv_compound_expression.end();  ++iter)
	{
		if(iter->m_name== m_expression)
		{
			m_expr_type= expr_compound;
			m_compound_index= std::distance(mp_expr_var->mv_compound_expression.begin(), iter);

			return true;
		}		
	}

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

float CExpression::evaluate(void)
{
	switch(m_expr_type)
	{
		case expr_constant:
		{
			return m_constant;
			break;
		}
		case expr_variable:
		{
			if(mp_expr_var!= NULL)
			{
				if(m_active_variable_index< mp_expr_var->mv_variable.size())
				{
					return mp_expr_var->mv_variable[m_active_variable_index].m_value;
				}
			}
			break;
		}
		case expr_type_unary:
		{
			return evaluate_unary_math_operation(m_unary_mop_type, mv_sub_expression[0].evaluate());				
			break;
		}		
		case expr_type_binary:
		{					
			return evaluate_binary_math_operation(mv_sub_expression[0].evaluate(), m_binary_mop_type, mv_sub_expression[1].evaluate());												
			break;
		}	
		case expr_compound:
		{
			return mp_expr_var->mv_compound_expression[m_compound_index].evaluate();													
			break;
		}			
	}

	return FLT_MAX;
}

bool CFunctionOfTwoVariables::create(std::vector<Vec3d>* p_v3DPt)
{
	CExpressionVariables expression_variables;
	CExpVar expr_varx;
	expr_varx.m_name= "x";
	CExpVar expr_varz;
	expr_varz.m_name= "y";
	expression_variables.mv_variable.push_back(expr_varx);
	expression_variables.mv_variable.push_back(expr_varz);
	CExpression expression;
	if(!expression.create(m_expression, &expression_variables))
	{
		//LogMessage("invalid expression in enivironment creation", m_expression);
		return false;

	}
	mx_incr= (m_end_x- m_start_x)/ m_num_pts_x;
	my_incr= (m_end_y- m_start_y)/ m_num_pts_y;			

	float y= m_start_y;
	float x= m_start_x;
	for(long dw_y= 0; dw_y< m_num_pts_y; dw_y++) {
		x= m_start_x;
		for(long dw_x= 0; dw_x< m_num_pts_x; dw_x++) {
			Vec3d vec;
			expression_variables.mv_variable[0].m_value= x;
			expression_variables.mv_variable[1].m_value= y;			
			vec.set(x,y,expression.evaluate());
			x+= mx_incr;
			p_v3DPt->push_back(vec);
		}
		y+= my_incr;
	}

	//float m_width= (float)m_num_pts_x;
	//float m_height= (float)m_num_pts_y;
	return true;
}


bool CFunctionOfOneVariable::create(std::vector<Vec3d>* p_v3DPt)
{
	CExpressionVariables expression_variables;
	CExpVar expr_varx;
	expr_varx.m_name= "x";
	expression_variables.mv_variable.push_back(expr_varx);
	CExpression expression;
	if(!expression.create(m_expression, &expression_variables))
	{
		//LogMessage("invalid expression in enivironment creation", m_expression);
		return false;
	}
	mx_incr= (m_end_x- m_start_x)/ m_num_pts_x;
	float x= m_start_x;
	for(long dw_x= 0; dw_x< m_num_pts_x; dw_x++) {
		Vec3d vec;
		expression_variables.mv_variable[0].m_value= x;		
		vec.set(x,expression.evaluate(),0);
		p_v3DPt->push_back(vec);
		x+= mx_incr;
	}
	return true;
}


bool CParametricFunction::create(std::vector<Vec3d>* p_v3DPt)
{
	CExpressionVariables expression_variables;
	CExpVar expr_var;
	std::vector<CExprVariable>::iterator expr_iter;
	for(expr_iter = mv_variables.begin(); expr_iter != mv_variables.end(); ++expr_iter) {
		expr_var.m_name = expr_iter->m_name;
		expression_variables.mv_variable.push_back(expr_var);
	}
		
	if(expression_variables.mv_variable.size()== 0) 
		return false;
	
	std::vector<CExprFunction>::iterator func_iter;
	for(func_iter = mv_expr_function.begin(); func_iter != mv_expr_function.end(); ++func_iter)	{
		if(!(func_iter->create(func_iter->m_str_expression, &expression_variables))) {
			//LogMessage("invalid expression in enivironment creation", m_expression);
			return false;
		}
	}

	if(mv_expr_function.size()< 3) 
		return false;

	m_incr= (m_end- m_start)/ m_num_pts;

	float t= m_start;
	for(long dt= 0; dt<= m_num_pts; dt++) {
		Vec3d vec;
		expression_variables.mv_variable[0].m_value= t;		
		float x=0;
		float y=0;
		float z=0;
		x = mv_expr_function[0].evaluate();
		y = mv_expr_function[1].evaluate();
		z = mv_expr_function[2].evaluate();
		vec.set(x, y, z);
		p_v3DPt->push_back(vec);
		t+= m_incr;
	}
	return true;
}

