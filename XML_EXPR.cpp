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

//#include "stdafx.h"
#include "XML_EXPR.h"
#include "OSG_Environment.h"
#include <string>

extern CEnvironment g_environment;

///
//CEXPR_Variable
///
CEXPR_Variable::CEXPR_Variable()
{
	m_type= EXPR_variable_type_default;
	m_mean= 1;
	m_standard_deviation= 0;
}
void CEXPR_Variable::link(void)
{

}

double CEXPR_Variable::get()
{ 
	double ret= 0;
	switch(m_type)
	{
		case EXPR_variable_type_default:
		{
			ret= m_value;
			break;
		}
		case EXPR_variable_type_time:
		{
			ret = g_environment.get_elapsed_time();			
			break;
		}
		case EXPR_variable_type_node_mass:
		{
			if(mv_ref.size()> 0)
			{ 
				std::size_t op_index= 0;
				std::size_t node_index = mv_ref[0].m_index;
				double a=  g_environment.mv_logistics_operation[op_index].mass_of_operatives_at_node(node_index);				
				ret= a;
			}			
			break;
		}
		case EXPR_variable_type_normal:
		{
			Mersenne_Twister mersenne_twister;
			mersenne_twister.seed(rand());
			Normal_Distribution distribution(m_mean,m_standard_deviation);
			Normal_variate_generator  gen(mersenne_twister, distribution);
			ret= gen();
			break;
		}
	}
	return ret;
}

///
//CEXPR_VariableList
///
void CEXPR_VariableList::add(CEXPR_Variable variable)
{ 
	mv_variable.push_back(variable); 
}
double CEXPR_VariableList::get(std::size_t index)
{ 
	if(index< mv_variable.size())
	{
		return mv_variable[index].get(); 
	}
	return FLT_MAX;
}
void CEXPR_VariableList::set(std::size_t index, double value) {
	if(index< mv_variable.size())
	{
		mv_variable[index].set(value); 
	}
}
void CEXPR_VariableList::set_with_str(std::string str, double value)
{
	std::vector<CEXPR_Variable>::iterator var_iter;
	for(var_iter = mv_variable.begin(); var_iter != mv_variable.end(); ++var_iter)	{
		if(var_iter->m_ID == str) {
			var_iter->set(value);
		}
	}
}
std::string CEXPR_VariableList::get_ID_from_index(std::size_t index)
{
	if(index< mv_variable.size())
	{
		return mv_variable[index].m_ID;
	}	
	return "bad index";
}

std::size_t CEXPR_VariableList::get_index_from_ID(std::string ID)
{
	std::vector<CEXPR_Variable>::iterator iter;
	for(iter = mv_variable.begin(); iter != mv_variable.end(); ++iter) {
		if(ID == iter->m_ID) { return std::distance(mv_variable.begin(), iter); }
	}
	return UINT_MAX;
}

CEXPR_Variable* CEXPR_VariableList::get_variable(std::size_t index)
{
	if(index< mv_variable.size()) {
		return &(mv_variable[index]);
	}
	return NULL;
}

///
//CEXPR_VariableReference
///
std::size_t CEXPR_VariableReference::get_index()
{ 
	return m_index;
}

double CEXPR_VariableReference::get(CEXPR * p_expr) 
{ 
	double ret = DBL_MAX;
	if(m_set) {	ret = p_expr->m_variable_list.get(m_index); }
	else { cout << "ref not set\n"; }
	return ret;
}
void CEXPR_VariableReference::set(std::size_t index) 
{ 
	m_set = true;	
	m_index=index;	
}

void CEXPR_VariableReference::update(double value)
{
	if(m_set) { mp_variable_list->set(m_index, value); }
}

///
//CEXPR_OperationReference
///
CEXPR_OperationReference::CEXPR_OperationReference()
{
	m_set= false;
	
}
std::size_t CEXPR_OperationReference::get_index()
{
	return m_index; 

}
void CEXPR_OperationReference::set(std::size_t index)
{	
	m_index= index;
	m_set= true;
}
double CEXPR_OperationReference::evaluate(CEXPR * p_expr) 
{ 		
	double ret = DBL_MAX;
	if(m_set) {ret =  p_expr->m_operation_list.evaluate(p_expr, m_index);	}
	else { cout << "ref not set\n"; }
	return ret;
}

///
//CEXPR_Operand
///
CEXPR_Operand::CEXPR_Operand()
{	
	m_active_index = 0;
}
void CEXPR_Operand::add_variable_reference (CEXPR_VariableReference variable_reference)
{ 
	mv_variable_reference.push_back(variable_reference);
}
void CEXPR_Operand::add_operation_reference (CEXPR_OperationReference operation_reference)
{ 
	mv_operation_reference.push_back(operation_reference); 
}
bool CEXPR_Operand::link_variable_and_operation_references(CEXPR * p_expr)
{		
	std::vector<CEXPR_VariableReference>::iterator var_ref_iter;
	for(var_ref_iter = mv_variable_reference.begin(); var_ref_iter != mv_variable_reference.end(); ++var_ref_iter) {
		std::size_t index = p_expr->m_variable_list.get_index_from_ID(var_ref_iter->m_ID);
		var_ref_iter->set(index);
	}
	std::vector<CEXPR_OperationReference>::iterator operation_ref_iter;
	for(operation_ref_iter = mv_operation_reference.begin(); operation_ref_iter != mv_operation_reference.end(); ++operation_ref_iter) {
		std::size_t index = p_expr->m_operation_list.get_index_from_ID(operation_ref_iter->m_ID);
		operation_ref_iter->set(index);
	}	
	return true;
}


double CEXPR_Operand::get(CEXPR * p_expr)
{
	double ret = DBL_MAX;
	switch(m_type) {
		case EXPR_operand_type_constant: {
			if(mv_constant.size()>0) { ret= mv_constant[m_active_index]; }
			break;
		}
		case EXPR_operand_type_variable: {
			if(m_active_index< mv_variable_reference.size()) { ret= mv_variable_reference[m_active_index].get(p_expr); }
			else {				
				cout << "mp_expr== NULL\n";
				return DBL_MAX ;
			}			
			break;
		}
		case EXPR_operand_type_operation: {			
			if(m_active_index< mv_operation_reference.size()) { ret= mv_operation_reference[m_active_index].evaluate(p_expr); }
			else {
				cout << "mp_expr== NULL\n";
				return DBL_MAX ;
			}
			break;
		}			
	}
	return ret;
}

///
//string_to_EXPR_operator_type
///
eEXPR_operator_type string_to_EXPR_operator_type(std::string str)
{		
	if(str== "Constant") return EXPR_operator_constant;
	else if(str== "Variable") return EXPR_operator_variable;
	else if((str== "Sin")||(str== "sin")) return EXPR_operator_sin;
	else if((str== "Cos")||(str== "cos")) return EXPR_operator_cos;
	else if((str== "Tan")||(str== "tan")) return EXPR_operator_tan;
	else if(str== "asin") return EXPR_operator_asin;
	else if(str== "acos") return EXPR_operator_acos;
	else if(str== "atan") return EXPR_operator_atan;
	else if(str== "atan2") return EXPR_operator_atan2;
	else if(str== "sinh") return EXPR_operator_sinh;
	else if(str== "cosh") return EXPR_operator_cosh;
	else if(str== "tanh") return EXPR_operator_tanh;	
	else if(str== "log") return EXPR_operator_log;
	else if(str== "log10") return EXPR_operator_log10;
	else if(str== "exp") return EXPR_operator_exp;
	else if(str== "sqrt") return EXPR_operator_sqrt;
	else if(str== "~") return EXPR_operator_ones_compliment;
	else if(str== "!") return EXPR_operator_logical_negation;
	else if(str== "floor") return EXPR_operator_floor;
	else if(str== "ceil") return EXPR_operator_ceil;
	else if(str== "++") return EXPR_operator_postfix_increment;
	else if(str== "--") return EXPR_operator_postfix_decrement;
	else if(str== "+") return EXPR_operator_addition;
	else if(str== "-") return EXPR_operator_subtract;	
	else if(str== "*") return EXPR_operator_multiplication;
	else if(str== "/") return EXPR_operator_division;	
	else if(str== "%") return EXPR_operator_modulus;	
	else if(str== "^") return EXPR_operator_power;//NOTE: xor	
	else if(str== "=") return EXPR_operator_assignment;		
	else if(str== "+=") return EXPR_operator_addition_assignment;		
	else if(str== "-=") return EXPR_operator_subtraction_assignment;		
	else if(str== "*=") return EXPR_operator_multiplication_assignment;		
	else if(str== "/=") return EXPR_operator_division_assignment;		
	else if(str== "%=") return EXPR_operator_modulus_assignment;		
	else if(str== "<<=") return EXPR_operator_left_shift_assignment;	
	else if(str== ">>=") return EXPR_operator_right_shift_assignment;		
	else if(str== "&=") return EXPR_operator_bitwise_AND_assignment;	
	else if(str== "|=") return EXPR_operator_bitwise_inclusive_OR_assignment;	
	else if(str== "^=") return EXPR_operator_bitwise_exclusive_OR_assignment;
	else if(str== "<<") return EXPR_operator_left_shift;	
	else if(str== ">>") return EXPR_operator_right_shift;
	else if(str== "&") return EXPR_operator_bitwise_AND;	
	else if(str== "|") return EXPR_operator_bitwise_inclusive_OR;
	else if(str== "xor") return EXPR_operator_bitwise_exclusive_OR;//NOTE: ^	
	else if(str== "||") return EXPR_operator_logical_OR;	
	else if(str== "&&") return EXPR_operator_logical_AND;
	else if(str== "<") return EXPR_operator_less_than;
	else if(str== "<=") return EXPR_operator_less_than_or_equal_to;
	else if(str== ">") return EXPR_operator_greater_than;
	else if(str== ">=") return EXPR_operator_greater_than_or_equal_to;
	else if(str== "==") return EXPR_operator_equality;
	else if(str== "!=") return EXPR_operator_not_equal;
	//if(str== "zeta") return EXPR_operator_zeta;
	//else if(str== "differentiate") return EXPR_operator_differentiation;
	//else if(str== "intergrate") return EXPR_operator_intergration;

	return EXPR_operator_error;
}

///
//CEXPR_Operation
///
CEXPR_Operation::CEXPR_Operation()
{
	
}

bool CEXPR_Operation::link_operand_references(CEXPR *p_expr)
{
	std::vector<CEXPR_Operand>::iterator operand_iter;
	for(operand_iter = mv_operand.begin(); operand_iter != mv_operand.end(); ++operand_iter) {		
		if(!operand_iter->link_variable_and_operation_references(p_expr)) {
			return false;
		}	
	}
	return true;
}

//complex <double> c1 ( 3.0 , 4.0 );
double CEXPR_Operation::evaluate(CEXPR * p_expr)
{ 	
	double ret= DBL_MAX;
	double left= 0;
	double right= 0;
	std::size_t op_sz = mv_operand.size(); 
	if(op_sz> 0) { left= mv_operand[0].get(p_expr); }			
	if(op_sz> 1) { right = mv_operand[1].get(p_expr); }			
	switch(m_operator_type) {
		//unary operators
		case EXPR_operator_constant: { if(mv_operand.size()> 0) { ret= left; } break; }
		case EXPR_operator_variable: { if(mv_operand.size()> 0) { ret= left; } break; }
		case EXPR_operator_operation: {	if(mv_operand.size()> 0) { ret= left; }	break; }	
		case EXPR_operator_random: { if(mv_operand.size()> 0) {	ret= (double)(rand()%(unsigned long)(left)); } break; }	
		case EXPR_operator_sin: { if(mv_operand.size()> 0) { ret= sin(left); } break; }
		case EXPR_operator_cos: { if(mv_operand.size()> 0) { ret= cos(left); } break; }		
		case EXPR_operator_tan: { if(mv_operand.size()> 0) { ret= tan(left); } break; }				
		case EXPR_operator_asin: { if(mv_operand.size()> 0) { ret= asin(left); } break; }			
		case EXPR_operator_acos:  { if(mv_operand.size()> 0) { ret= acos(left); } break; }			
		case EXPR_operator_atan: { if(mv_operand.size()> 0) { ret= atan(left); } break; }					
		case EXPR_operator_sinh: { if(mv_operand.size()> 0) { ret= sinh(left); } break; }	
		case EXPR_operator_cosh: { if(mv_operand.size()> 0) { ret= cosh(left); } break; }	
		case EXPR_operator_tanh: { if(mv_operand.size()> 0) { ret= tanh(left); } break; }	
		case EXPR_operator_log: { if(mv_operand.size()> 0) { ret= log(left); } break; }			
		case EXPR_operator_log10: { if(mv_operand.size()> 0) { ret= log10(left); } break; }			
		case EXPR_operator_sqrt: { if(mv_operand.size()> 0) { ret= sqrt(left); } break; }			
		case EXPR_operator_exp: { if(mv_operand.size()> 0) { ret= exp(left); } break; }				
		case EXPR_operator_ones_compliment:  { if(mv_operand.size()> 0) { ret= (double)~(unsigned long)(left); } break; }				
		case EXPR_operator_logical_negation:  { if(mv_operand.size()> 0) { ret= !(unsigned long)(left); } break; }			
		case EXPR_operator_floor: { if(mv_operand.size()> 0) { ret= floor(left); } break; }			
		case EXPR_operator_ceil: { if(mv_operand.size()> 0) { ret= ceil(left); } break; }			
		case EXPR_operator_postfix_increment: { if(mv_operand.size()> 0) { ret= left+= 1;	 } break; }			
		case EXPR_operator_postfix_decrement: { if(mv_operand.size()> 0) { ret= left-= 1;	 } break; }		
		//binary operators
		case EXPR_operator_power: { if(mv_operand.size()> 1) { ret= pow(left, right); } break; }			
		case EXPR_operator_multiplication: { if(mv_operand.size()> 1) { ret= (left * right); } break; }			
		case EXPR_operator_division: { if(mv_operand.size()> 1) { ret= (left / right);	} break; }			
		case EXPR_operator_addition: { if(mv_operand.size()> 1) { ret= (left + right);	} break; }			
		case EXPR_operator_subtract: { if(mv_operand.size()> 1) { ret= (left - right);	} break; }	
		case EXPR_operator_modulus: { if(mv_operand.size()> 1) { ret= ((long)left % (long)right);	} break; }	
		case EXPR_operator_atan2: { if(mv_operand.size()> 1) { ret= atan2(left, right); } break; }				
		case EXPR_operator_assignment: { if(mv_operand.size()> 1) { ret= (left = right);	} break; }	
		case EXPR_operator_addition_assignment: { if(mv_operand.size()> 1) { ret= (left += right);	} break; }	
		case EXPR_operator_subtraction_assignment: { if(mv_operand.size()> 1) { ret= (left -= right);	} break; }	
		case EXPR_operator_multiplication_assignment: { if(mv_operand.size()> 1) { ret= (left *= right);	} break; }	
		case EXPR_operator_division_assignment: { if(mv_operand.size()> 1) { ret= (left /= right);	} break; }	
		case EXPR_operator_left_shift: { if(mv_operand.size()> 1) { ret= ((long)left >> (long)right);	} break; }	
		case EXPR_operator_right_shift: { if(mv_operand.size()> 1) { ret= ((long)left << (long)right);	} break; }	
		case EXPR_operator_bitwise_AND: { if(mv_operand.size()> 1) { ret= ((long)left & (long)right);	} break; }	
		case EXPR_operator_bitwise_inclusive_OR: { if(mv_operand.size()> 1) { ret= ((long)left | (long)right);	} break; }	
		case EXPR_operator_bitwise_exclusive_OR: { if(mv_operand.size()> 1) { ret= ((long)left ^ (long)right);	} break; }	
		case EXPR_operator_logical_OR: { if(mv_operand.size()> 1) { ret= (left || right);	} break; }	
		case EXPR_operator_logical_AND: { if(mv_operand.size()> 1) { ret= (left && right);	} break; }	
		case EXPR_operator_less_than: { if(mv_operand.size()> 1) { ret= (left < right);	} break; }	
		case EXPR_operator_less_than_or_equal_to: { if(mv_operand.size()> 1) { ret= (left <= right);	} break; }	
		case EXPR_operator_greater_than: { if(mv_operand.size()> 1) { ret= (left > right);	} break; }	
		case EXPR_operator_greater_than_or_equal_to: { if(mv_operand.size()> 1) { ret= (left >= right);	} break; }	
		case EXPR_operator_equality: { if(mv_operand.size()> 1) { ret= (left = right);	} break; }	
		case EXPR_operator_not_equal: { if(mv_operand.size()> 1) { ret= (left != right);	} break; }	
		
		//case EXPR_operator_modulus_assignment: { if(mv_operand.size()> 1) { ret=  left %= right; } break; }	
		//case EXPR_operator_left_shift_assignment: { if(mv_operand.size()> 1) {  ret= left <<= right;	} break; }	
		//case EXPR_operator_right_shift_assignment: { if(mv_operand.size()> 1) { ret= left >>= right; } break; }
		//case EXPR_operator_bitwise_AND_assignment: { if(mv_operand.size()> 1) { ret= left &= right;	} break; }	
		//case EXPR_operator_bitwise_inclusive_OR_assignment: { if(mv_operand.size()> 1) { ret= left |= right;	} break; }	
		//case EXPR_operator_bitwise_exclusive_OR_assignment: { if(mv_operand.size()> 1) { ret= left ^= right;	} break; }

		case EXPR_operator_solve: {	break; }
		case EXPR_operator_expansion: {	break; }		
		case EXPR_operator_factorisation: {	break; }		
		case EXPR_operator_differentiation: { break; }			
		case EXPR_operator_intergration: { break; }
	}

	return ret;

}
	
std::vector<std::size_t> CEXPR_Operation::get_ref_operation_indices(void)
{
	std::vector<std::size_t> v_indicies;	
	std::vector<CEXPR_Operand>::iterator opd_iter;
	for(opd_iter = mv_operand.begin(); opd_iter < mv_operand.end(); ++opd_iter)	{		
		if(opd_iter->m_type == EXPR_operand_type_operation) 	{
			std::vector<CEXPR_OperationReference>::iterator ref_iter;
			for(ref_iter = opd_iter->mv_operation_reference.begin(); ref_iter <  opd_iter->mv_operation_reference.end(); ++ref_iter) {
				v_indicies.push_back(ref_iter->get_index());
			}
		}
	}
	return v_indicies;
}

std::vector<std::size_t> CEXPR_Operation::get_ref_variable_indices(void)
{
	std::vector<std::size_t> v_indicies;	
	std::vector<CEXPR_Operand>::iterator opd_iter;
	for(opd_iter = mv_operand.begin(); opd_iter < mv_operand.end(); ++opd_iter)	{		
		if(opd_iter->m_type == EXPR_operand_type_variable) 	{
			std::vector<CEXPR_VariableReference>::iterator ref_iter;
			for(ref_iter = opd_iter->mv_variable_reference.begin(); ref_iter <  opd_iter->mv_variable_reference.end(); ++ref_iter) {
				v_indicies.push_back(ref_iter->get_index());
			}
		}
	}
	return v_indicies;
}

std::vector<double> CEXPR_Operation::get_ref_constants(void)
{
	std::vector<double> v_constants;	
	std::vector<CEXPR_Operand>::iterator opd_iter;
	for(opd_iter = mv_operand.begin(); opd_iter < mv_operand.end(); ++opd_iter)	{		
		if(opd_iter->m_type == EXPR_operand_type_constant) 	{
			std::vector<double>::iterator ref_iter;
			for(ref_iter = opd_iter->mv_constant.begin(); ref_iter <  opd_iter->mv_constant.end(); ++ref_iter) {
				v_constants.push_back(*ref_iter);
			}
		}
	}
	return v_constants;	
}

std::vector<CEXPR_Operand_position> CEXPR_Operation::get_operand_positions(void)
{
	std::vector<CEXPR_Operand_position> v_positions;
	std::vector<CEXPR_Operand>::iterator opd_iter;
	for(opd_iter = mv_operand.begin(); opd_iter < mv_operand.end(); ++opd_iter)	{		
		CEXPR_Operand_position pos;
		if(opd_iter->m_type == EXPR_operand_type_constant) 	{ pos.m_type = EXPR_operand_type_constant; }
		else if(opd_iter->m_type == EXPR_operand_type_variable) { pos.m_type = EXPR_operand_type_variable;	}
		else if(opd_iter->m_type == EXPR_operand_type_operation) { pos.m_type = EXPR_operand_type_operation; }
		pos.m_index = std::distance(mv_operand.begin(), opd_iter);
		v_positions.push_back(pos);
	}
	return v_positions;	
}



CEXPR CEXPR_Operation::differentiate(CEXPR * p_expr, std::string WRT)
{
	CEXPR derivative;
	derivative.m_ID = "Derivative";	
	std::vector<std::size_t>::iterator n_iter;
	std::vector<std::size_t> v_oprtn_indicies = get_ref_operation_indices();
	std::vector<std::size_t> v_var_indicies = get_ref_variable_indices();
	std::vector<double> v_constants = get_ref_constants();	
	std::size_t oprtn_count = v_oprtn_indicies.size();
	std::size_t var_count = v_var_indicies.size();
	std::size_t const_count = v_constants.size();
	std::vector<CEXPR_Operand_position> positions = get_operand_positions();
	//std::vector<CEXPR_Operand_position>::iterator pos_iter;

	if(positions.size() == 1)
	{
		if(positions[0].m_type == EXPR_operand_type_constant)
		{
			CEXPR_Operation oprtn_C= create_operation_Const(p_expr, "C", "Constant", 0);
			derivative.m_operation_list.add(oprtn_C);
			derivative.m_active_index = derivative.m_operation_list.size()-1;
		}
		else if(positions[0].m_type == EXPR_operand_type_variable)
		{
			if(m_operator_type == EXPR_operator_sin)
			{		
				CEXPR_Variable var = create_variable(p_expr, v_var_indicies[0]);	
				CEXPR_Operation oprtn= create_operation_Var(p_expr, "Fx", "cos", v_var_indicies[0]);
				derivative.m_variable_list.add(var);	
				derivative.m_operation_list.add(oprtn);
				derivative.m_active_index = derivative.m_operation_list.size()-1;
			}		
			else if(m_operator_type == EXPR_operator_cos)
			{		
				CEXPR_Variable var = create_variable(p_expr, v_var_indicies[0]);	
				CEXPR_Operation oprtn_V= create_operation_Var(p_expr, "Fx0", "sin", v_var_indicies[0]);
				CEXPR_Operation oprtn_CO = create_operation_Const_Oprn(p_expr, "Fx1", -1, "*", "Fx0");
				derivative.m_variable_list.add(var);	
				derivative.m_operation_list.add(oprtn_V);
				derivative.m_operation_list.add(oprtn_CO);
				derivative.m_active_index = derivative.m_operation_list.size()-1;
			}		
			else if(m_operator_type == EXPR_operator_tan)
			{		
				//  d/dx[tan(x)]= (sec(x))^2
				CEXPR_Variable var = create_variable(p_expr, v_var_indicies[0]);	
				CEXPR_Operation oprtn_V= create_operation_Var(p_expr, "Fx0", "cos", v_var_indicies[0]);
				CEXPR_Operation oprtn_OC = create_operation_Oprn_Const(p_expr, "Fx1", "Fx0", "^",2 );
				CEXPR_Operation oprtn_CO = create_operation_Const_Oprn(p_expr, "Fx2", 1, "/", "Fx1");
				derivative.m_variable_list.add(var);	
				derivative.m_operation_list.add(oprtn_V);
				derivative.m_operation_list.add(oprtn_OC);
				derivative.m_operation_list.add(oprtn_CO);
				derivative.m_active_index = derivative.m_operation_list.size()-1;
			}		
			else if(m_operator_type == EXPR_operator_log)
			{		
				CEXPR_Variable var = create_variable(p_expr, v_var_indicies[0]);	
				CEXPR_Operation oprtn= create_operation_Var(p_expr, "Fx", "log", v_var_indicies[0]);
				derivative.m_variable_list.add(var);	
				derivative.m_operation_list.add(oprtn);
				derivative.m_active_index = derivative.m_operation_list.size()-1;
			}		
			else if(m_operator_type == EXPR_operator_log)
			{		
				//  d/dx[log(x)]= 1/x			
			}
			else if(m_operator_type == EXPR_operator_log10)
			{		
				//  d/dx[log10(x)]= 1/(x*log(10) !!
			}
			else if(m_operator_type == EXPR_operator_asin)
			{		
				//  d/dx[asin(x)]= 1/sqrt(1-x^2)
			}
			else if(m_operator_type == EXPR_operator_acos)
			{		
				//  d/dx[asin(x)]= -1/sqrt(1-x^2)
			}
			else if(m_operator_type == EXPR_operator_atan)
			{		
				//  d/dx[asin(x)]= 1/(1+x^2)
			}
		}
	}
	else if(positions.size() == 2)
	{
		if((positions[0].m_type == EXPR_operand_type_constant)&&(positions[1].m_type == EXPR_operand_type_variable))
		{
			if(m_operator_type == EXPR_operator_multiplication)
			{		
				CEXPR_Operation oprtn_C= create_operation_Const(p_expr, "C", "Constant", v_constants[0]);
				derivative.m_operation_list.add(oprtn_C);
				derivative.m_active_index = derivative.m_operation_list.size()-1;
			}
			else if(m_operator_type == EXPR_operator_division)
			{					
				// c/v
			}
			else if(m_operator_type == EXPR_operator_power)
			{			
				CEXPR_Variable var = create_variable(p_expr, v_var_indicies[0]);	
				CEXPR_Operation oprtn_VC = create_operation_Var_Const(p_expr, "Fx0", v_var_indicies[0], "^", v_constants[0]-1);
				CEXPR_Operation oprtn_CO = create_operation_Const_Oprn(p_expr, "Fx1", v_constants[0], "*", "Fx0");
				derivative.m_variable_list.add(var);	
				derivative.m_operation_list.add(oprtn_VC);
				derivative.m_operation_list.add(oprtn_CO);
				derivative.m_active_index = derivative.m_operation_list.size()-1;
			}
		}
		else if((positions[0].m_type == EXPR_operand_type_variable)&&(positions[1].m_type == EXPR_operand_type_constant))
		{
			if(m_operator_type == EXPR_operator_multiplication)
			{		
				CEXPR_Operation oprtn_C= create_operation_Const(p_expr, "C", "Constant", v_constants[0]);
				derivative.m_operation_list.add(oprtn_C);
				derivative.m_active_index = derivative.m_operation_list.size()-1;
			}
			else if(m_operator_type == EXPR_operator_division)
			{					
				// v/c
			}
			else if(m_operator_type == EXPR_operator_power)
			{			
				CEXPR_Variable var = create_variable(p_expr, v_var_indicies[0]);	
				CEXPR_Operation oprtn_CV = create_operation_Const_Var(p_expr, "Fx0", v_constants[0], "^", v_var_indicies[0]);
				CEXPR_Operation oprtn_V = create_operation_Var(p_expr, "Fx1", "log", v_var_indicies[0]);					
				CEXPR_Operation oprtn_OO = create_operation_Oprn_Oprn(p_expr, "Fx2", "Fx0", "*", "Fx1");
				derivative.m_variable_list.add(var);	
				derivative.m_operation_list.add(oprtn_CV);
				derivative.m_operation_list.add(oprtn_V);
				derivative.m_operation_list.add(oprtn_OO);
				derivative.m_active_index = derivative.m_operation_list.size()-1;
			}
		}
		else if((positions[0].m_type == EXPR_operand_type_variable)&&(positions[1].m_type == EXPR_operand_type_operation))
		{
			if(m_operator_type == EXPR_operator_multiplication)
			{		
				int rand_id = rand()%10000;//should  be alpha numeric and check for orininality..
				std::string v_ID;
				//////////////////////////////
				// add variable 
				//////////////////////////////	

				std::vector<std::string> mv_ID;
				CEXPR_Variable var = create_variable(p_expr, v_var_indicies[0]);	
				derivative.m_variable_list.add(var);	

				//////////////////////////////
				// define u
				//////////////////////////////
				std::size_t u = v_var_indicies[0];
				std::string str_var = p_expr->m_variable_list.get_ID_from_index(u);

				//////////////////////////////
				// define DuDx
				//////////////////////////////
				// as the derivative of the variable
				// in this case - always = 1
				// so just use a constant

				//////////////////////////////
				// define v
				//////////////////////////////			
				CEXPR_Operation* p_sub_op = p_expr->m_operation_list.get_operation(0);
				CEXPR_Operation Operation_v  = *p_sub_op;

				std::ostringstream oss_v;
				oss_v << rand_id << "v";
				v_ID = oss_v.str();
				Operation_v.m_ID = v_ID;
				derivative.m_operation_list.add(Operation_v);

				//////////////////////////////
				// define DvDX
				//////////////////////////////
				// as the derivative of v
				//NOTE ID should be DvDx				
				CEXPR expr_v;
				expr_v.m_variable_list.add(var);			
				expr_v.m_operation_list.add(Operation_v);									
				expr_v.m_operation_list.link_operation_references(&expr_v);
				CEXPR expr_DvDx = expr_v.differentiate(str_var);				
				CEXPR_Operation* p_DvDx;		
				int i=0;
				rand_id = rand()%10000;//should  be alpha numeric and check for orininality..
				std::string DvDx_ID;
				for(i=0; i< expr_DvDx.m_operation_list.size(); i++) {
					p_DvDx = expr_DvDx.m_operation_list.get_operation(i);
					std::ostringstream oss;
					oss << rand_id;
					oss << p_DvDx->m_ID;
					DvDx_ID= oss.str();
					p_DvDx->m_ID = DvDx_ID;
					mv_ID.push_back(DvDx_ID);//cache operation IDs 
					derivative.m_operation_list.add(*p_DvDx);
				}
				
				//////////////////////////////
				//product rule	
				//////////////////////////////
				//D[v* O] = O* D[v] + v* D[O]
				
				CEXPR_Operation oprtn_VO1 = create_operation_Var_Oprn(p_expr, "Fx0", u, "*", DvDx_ID);//  = u * dv/dx
				CEXPR_Operation oprtn_OO1 = create_operation_Oprn_Const(p_expr, "Fx1", v_ID, "*", 1);//  = v * du/dx
				CEXPR_Operation oprtn_OO2 = create_operation_Oprn_Oprn(p_expr, "ProductRule", "Fx0", "+", "Fx1");				
				
				derivative.m_operation_list.add(oprtn_VO1);
				derivative.m_operation_list.add(oprtn_OO1);
				derivative.m_operation_list.add(oprtn_OO2);
				derivative.m_active_index = derivative.m_operation_list.size()-1;
			}		
		}
	}	


	if((oprtn_count== 1)&&(var_count== 0)||(const_count== 1))
	{
		CEXPR_Operation* p_sub_op = p_expr->m_operation_list.get_operation(0);

	}


	/*if(v_oprtn_indicies.size()> 0)
	{
		std::string oprtn0_ID = p_expr->m_operation_list.get_ID_from_index(v_oprtn_indicies[0]);
	}*/

	/*for(n_iter = v_oprtn_indicies.begin(); n_iter != v_oprtn_indicies.end(); ++n_iter)
	{
		CEXPR_Operation* p_sub_op = p_expr->m_operation_list.get_operation(*n_iter);

		std::vector<std::size_t> v_var_indicies = p_sub_op->get_ref_variable_indices();
		if(v_var_indicies.size()> 0)
		{
			variable0_ID = p_expr->m_variable_list.get_ID_from_index(v_var_indicies[0]);
		}

		std::vector<double> v_constants = p_sub_op->get_ref_constants();	
		if(v_constants.size()> 0)
		{
			const0 = v_constants[0];
		}

	}*/
	
	

	return derivative;
}


///
//CEXPR_OperationList
///
CEXPR_OperationList::CEXPR_OperationList()
{
	
}
std::size_t CEXPR_OperationList::get_index_from_ID(std::string ID)
{
	std::vector<CEXPR_Operation>::iterator iter;
	for(iter = mv_operation.begin(); iter != mv_operation.end(); ++iter) {
		if(ID == iter->m_ID) { return std::distance(mv_operation.begin(), iter); }
	}
	return UINT_MAX;
}

std::string CEXPR_OperationList::get_ID_from_index(std::size_t index)
{
	if(index< mv_operation.size())
	{
		return mv_operation[index].m_ID;
	}	
	return "bad index";
}

CEXPR_Operation* CEXPR_OperationList::get_operation(std::size_t index)
{
	if(index< mv_operation.size()) {
		return &(mv_operation[index]);
	}

	return NULL;
}

bool CEXPR_OperationList::link_operation_references(CEXPR* p_expr)
{	
	std::vector<CEXPR_Operation>::iterator iter_operation;	
	for(iter_operation = mv_operation.begin(); iter_operation < mv_operation.end(); ++iter_operation) {		
		iter_operation->link_operand_references(p_expr);
	}
	return true;
}


void CEXPR_OperationList::add(CEXPR_Operation operation)
{ 
	std::size_t index= mv_operation.size();
	mv_operation.push_back(operation);
	//mv_operation[index].link_operand_references(mp_expr);
}		
	
double CEXPR_OperationList::evaluate(CEXPR * p_expr, std::size_t index)
{
	double ret = mv_operation[index].evaluate(p_expr);
	if(ret == DBL_MAX)
	{
		cout << "ret == DBL_MAX\n";
	}
	return ret;
}

CEXPR CEXPR_OperationList::differentiate(CEXPR * p_expr, std::size_t index, std::string WRT)
{
	if(index< mv_operation.size())
	{
		return mv_operation[index].differentiate(p_expr, WRT);
	}
	CEXPR NOP;
	return NOP;
}

std::size_t CEXPR_OperationList::size(void)
{
	return mv_operation.size();
}

///
//CEXPR
///
CEXPR::CEXPR()
{
	//m_operation_list.mp_expr = this;
	m_active_index=0;
}
double CEXPR::evaluate()
{
	double ret = m_operation_list.evaluate(this, m_active_index);
	if(ret == DBL_MAX) {
		std::size_t var_sz;
		var_sz = m_variable_list.mv_variable.size();		
		if(var_sz> 0) {		
			ret = m_variable_list.get(var_sz-1);
		}
	}
	return ret;
}
double CEXPR::evaluate_i(std::size_t index)
{
	return m_operation_list.evaluate(this, index);
}

CEXPR CEXPR::differentiate(std::size_t index, std::string WRT)
{
	return m_operation_list.differentiate(this, index, WRT);

}
CEXPR CEXPR::differentiate(std::string WRT)
{
	std::size_t var_sz;
	var_sz = m_operation_list.size();		
	if(var_sz> 0) {		
		return m_operation_list.differentiate(this, var_sz-1, WRT);
	}
	CEXPR dummy;
	return dummy;
}


///
//CMDEXPR
///
bool CMDEXPR::link()
{
	std::vector<CMDEXPRReference>::iterator v_iter;	
	for(v_iter = mv_expr_ref.begin(); v_iter != mv_expr_ref.end(); ++v_iter) {
		//get EXPR indicies from ID
		std::vector<CEXPR>::iterator EXPR_iter;
		for (EXPR_iter =  g_environment.mv_EXPR.begin(); EXPR_iter<  g_environment.mv_EXPR.end(); ++EXPR_iter) {
			if(EXPR_iter->m_ID == v_iter->m_EXPR_ref_name) {
				v_iter->m_EXPR_index = std::distance(g_environment.mv_EXPR.begin(), EXPR_iter);
			}			
		}		
		if(v_iter->m_EXPR_index== UINT_MAX)	{ return false; }
		else {
			//get EXPR variables indices
			std::vector<CMDEXPRVarRef>::iterator v_var_iter;	
			for(v_var_iter = v_iter->mv_EXPR_var_ref.begin(); v_var_iter != v_iter->mv_EXPR_var_ref.end(); ++v_var_iter){
				v_var_iter->m_EXPR_index= g_environment.mv_EXPR[v_iter->m_EXPR_index].m_variable_list.get_index_from_ID(v_var_iter->m_EXPR_ref_name);
			}			
		}
	}	
	return true;
}
osg::Vec2d CMDEXPR::evaluate_2D(osg::Vec2d v)
{
	osg::Vec2d v_out;

	return v_out;
}
osg::Vec3d CMDEXPR::evaluate_3D(osg::Vec3d v)
{
	osg::Vec3d v_out;

	double x=0;
	double y=0;
	double z=0;
	std::string variable_name;
	std::size_t variable_index;
	//set the variables
	std::vector<CMDEXPRReference>::iterator v_iter;	
	for(v_iter = mv_expr_ref.begin(); v_iter != mv_expr_ref.end(); ++v_iter)
	{		
		std::vector<CMDEXPRVarRef>::iterator v_var_iter;	
		for(v_var_iter = v_iter->mv_EXPR_var_ref.begin(); v_var_iter != v_iter->mv_EXPR_var_ref.end(); ++v_var_iter){

			variable_name = v_var_iter->m_EXPR_ref_name;
			variable_index = v_var_iter->m_EXPR_index;

			CEXPR * p_EXPR = &(g_environment.mv_EXPR[v_iter->m_EXPR_index]);

			if((v_var_iter->m_EXPR_ref_name == "x")&&(variable_index!= UINT_MAX))
			{
				p_EXPR->m_variable_list.set_with_str(v_var_iter->m_EXPR_ref_name, v.x());
			}
			if((v_var_iter->m_EXPR_ref_name == "y")&&(variable_index!= UINT_MAX))
			{
				p_EXPR->m_variable_list.set_with_str(v_var_iter->m_EXPR_ref_name, v.y());
			}
			if((v_var_iter->m_EXPR_ref_name == "z")&&(variable_index!= UINT_MAX))
			{
				p_EXPR->m_variable_list.set_with_str(v_var_iter->m_EXPR_ref_name, v.z());
			}			
		}
	}


	int n1= 0;	
	for(v_iter = mv_expr_ref.begin(); v_iter != mv_expr_ref.end(); ++v_iter) {	
		std::size_t index= v_iter->m_EXPR_index;
		if(index < g_environment.mv_EXPR.size()) {
			switch(n1)	{
				case 0: 	{				
					x = g_environment.mv_EXPR[v_iter->m_EXPR_index].evaluate();
					if(x== DBL_MAX) { goto END;	}
					break;
				}
				case 1:	{
					y = g_environment.mv_EXPR[v_iter->m_EXPR_index].evaluate();
					if(y== DBL_MAX) { goto END;	}
					break;
				}
				case 2: 	{
					z = g_environment.mv_EXPR[v_iter->m_EXPR_index].evaluate();
					if(z== DBL_MAX) { goto END;	}
					break;
				}
			}
		}
		n1++;
	}		

	END:
	
	v_out.set(x, y, z);	
	return v_out;
}

std::vector<double> CMDEXPR::evaluate_MD(std::vector<double> v)
{
	std::vector<double> v_out;
  


	return v_out; // (1)
}
///
//CEXPRVectorField
///
CEXPRVectorField::CEXPRVectorField()
{
	m_scale= 1;
}

bool CEXPRVectorField::create(std::vector<osg::Vec3d>* p_v3DPt, unsigned long* count)
{
	//CMDEXPR m_MDEXPR;
	//osg::Vec3d vec = m_MDEXPR.evaluate_3D();	
	//std::size_t index0, index1, index2;
	double x=0;
	double y=0;
	double z=0;
	Vec3d vec_out;
	if(mv_expr_interval.size()>=3)
	{
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
					Vec3d vec_in;
					vec_in.set(x,y,z);
					vec_out= m_MDEXPR.evaluate_3D(vec_in);					
					double A0= m_scale*  vec_out.x();
					double A1= m_scale*  vec_out.y();
					double A2= m_scale*  vec_out.z();
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
	}
	
	return true;
}



CEXPR_Variable create_variable(CEXPR * p_expr, std::size_t index)
{
	CEXPR_Variable var;
	var.m_ID = p_expr->m_variable_list.get_ID_from_index(index);	
	return var;
}

CEXPR_Operand create_constant_operand(double value)
{
	CEXPR_Operand oprnd;
	oprnd.m_type = EXPR_operand_type_constant;
	oprnd.mv_constant.push_back(value);
	return oprnd;
}

CEXPR_Operand create_variable_operand(CEXPR * p_expr, std::size_t index)
{
	CEXPR_Operand oprnd;
	oprnd.m_type = EXPR_operand_type_variable;
	CEXPR_VariableReference variable_reference;
	variable_reference.m_ID = p_expr->m_variable_list.get_ID_from_index(index);
	oprnd.add_variable_reference(variable_reference);
	return oprnd;
}

CEXPR_Operand create_operation_operand(CEXPR * p_expr, std::string operation_ID)
{
	CEXPR_Operand oprnd;
	oprnd.m_type = EXPR_operand_type_operation;
	CEXPR_OperationReference operation_reference;
	operation_reference.m_ID = operation_ID;
	oprnd.mv_operation_reference.push_back(operation_reference);
	return oprnd;
}

CEXPR_Operation create_operation(std::string str_ID, std::string str_oprtr_type)
{
	CEXPR_Operation oprtn;
	oprtn.m_ID = str_ID;
	oprtn.m_operator_type = string_to_EXPR_operator_type(str_oprtr_type);
	return oprtn;
}

CEXPR_Operation create_operation_Const(CEXPR * p_expr, std::string str_ID, std::string str_oprtr_type, double const_value)
{		
	CEXPR_Operand operand = create_constant_operand(const_value);			
	CEXPR_Operation oprtn= create_operation(str_ID, str_oprtr_type);
	oprtn.mv_operand.push_back(operand);	
	return oprtn;
}

CEXPR_Operation create_operation_Var(CEXPR * p_expr, std::string str_ID, std::string str_oprtr_type, std::size_t var_index)
{			
	CEXPR_Operand var_operand= create_variable_operand(p_expr, var_index);			
	CEXPR_Operation oprtn= create_operation(str_ID, str_oprtr_type);				
	oprtn.mv_operand.push_back(var_operand);	
	return oprtn;
}

CEXPR_Operation create_operation_Const_Var(CEXPR * p_expr, std::string str_ID, double const_value, std::string str_oprtr_type, std::size_t var_index)
{	
	CEXPR_Operand const_operand= create_constant_operand(const_value);					
	CEXPR_Operand var_operand= create_variable_operand(p_expr, var_index);			
	CEXPR_Operation oprtn= create_operation(str_ID, str_oprtr_type);				
	oprtn.mv_operand.push_back(const_operand);	
	oprtn.mv_operand.push_back(var_operand);	
	return oprtn;
}

CEXPR_Operation create_operation_Var_Const(CEXPR * p_expr, std::string str_ID, std::size_t var_index, std::string str_oprtr_type, double const_value)
{
	CEXPR_Operand var_operand= create_variable_operand(p_expr, var_index);				
	CEXPR_Operand const_operand= create_constant_operand(const_value);					
	CEXPR_Operation oprtn= create_operation(str_ID, str_oprtr_type);				
	oprtn.mv_operand.push_back(var_operand);	
	oprtn.mv_operand.push_back(const_operand);	
	return oprtn;
}

CEXPR_Operation create_operation_Const_Oprn(CEXPR * p_expr, std::string str_ID, double const_value, std::string str_oprtr_type, std::string str_oprn_ID)
{
	CEXPR_Operand const_operand= create_constant_operand(const_value);					
	CEXPR_Operand oprtn_operand= create_operation_operand(p_expr, str_oprn_ID);			
	CEXPR_Operation oprtn= create_operation(str_ID, str_oprtr_type);				
	oprtn.mv_operand.push_back(const_operand);	
	oprtn.mv_operand.push_back(oprtn_operand);	
	return oprtn;
}

CEXPR_Operation create_operation_Oprn_Const(CEXPR * p_expr, std::string str_ID, std::string str_oprn_ID, std::string str_oprtr_type, double const_value)
{			
	CEXPR_Operand oprtn_operand= create_operation_operand(p_expr, str_oprn_ID);		
	CEXPR_Operand const_operand= create_constant_operand(const_value);			
	CEXPR_Operation oprtn= create_operation(str_ID, str_oprtr_type);				
	oprtn.mv_operand.push_back(oprtn_operand);	
	oprtn.mv_operand.push_back(const_operand);	
	return oprtn;
}

CEXPR_Operation create_operation_Var_Oprn(CEXPR * p_expr, std::string str_ID, std::size_t var_index, std::string str_oprtr_type, std::string str_oprn_ID)
{
	CEXPR_Operand var_operand= create_variable_operand(p_expr, var_index);		
	CEXPR_Operand oprtn_operand= create_operation_operand(p_expr, str_oprn_ID);			
	CEXPR_Operation oprtn= create_operation(str_ID, str_oprtr_type);				
	oprtn.mv_operand.push_back(var_operand);	
	oprtn.mv_operand.push_back(oprtn_operand);	
	return oprtn;
}

CEXPR_Operation create_operation_Oprn_Var(CEXPR * p_expr, std::string str_ID, std::string str_oprn_ID, std::string str_oprtr_type, std::size_t var_index)
{			
	CEXPR_Operand oprtn_operand= create_operation_operand(p_expr, str_oprn_ID);		
	CEXPR_Operand var_operand= create_variable_operand(p_expr, var_index);	
	CEXPR_Operation oprtn= create_operation(str_ID, str_oprtr_type);				
	oprtn.mv_operand.push_back(oprtn_operand);	
	oprtn.mv_operand.push_back(var_operand);	
	return oprtn;
}

CEXPR_Operation create_operation_Oprn_Oprn(CEXPR * p_expr, std::string str_ID, std::string str_oprn1_ID, std::string str_oprtr_type, std::string str_oprn2_ID)
{			
	CEXPR_Operand oprtn_operand1= create_operation_operand(p_expr, str_oprn1_ID);		
	CEXPR_Operand oprtn_operand2= create_operation_operand(p_expr, str_oprn2_ID);		
	CEXPR_Operation oprtn= create_operation(str_ID, str_oprtr_type);				
	oprtn.mv_operand.push_back(oprtn_operand1);	
	oprtn.mv_operand.push_back(oprtn_operand2);	
	return oprtn;
}
