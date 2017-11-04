/**
 * @file	infix_to_postfix.cpp
 * @brief	Funções para conversão de expressão infixa para posfixa
 *
 * @author	Josivan Medeiros da Silva Gois
 * 
 */

#include "infix_to_postfix.h"

bool is_operand( Token t )
{
	return t.type == Token::token_t::OPERAND;
}
bool is_operator( Token t )
{
	return t.type == Token::token_t::OPERATOR;
}
enum
{
	//DEFAULT_OP,
	//PLUS_MINUS,
	//DIV_MULT_MOD,
	EXPONENTIATION = 3
};
short get_precedence( char s )
{
	// Only '+', '-', '*', '%', '/', and '^' (for exponentiation) operators are expected;
	switch( s )
	{
		case '^': 
			return EXPONENTIATION;
			//=============================================================
		case '*':
		case '/':
		case '%':
			return 2;
			//=============================================================
		case '+':
		case '-':
			return 1;
			//=============================================================
		default:
			return 0;
	}
} 
bool has_higher_precedence( Token op1, Token op2 )
{
	// op1 >= op2.
	// Just converts operator into number.
	auto w1 = get_precedence( op1.value[0] );
	auto w2 = get_precedence( op2.value[0] );
	// Special case: '^' >= '^'??? NO!!!
	if ( w1 == w2 and w1 == EXPONENTIATION )
		return false;

	return w1 >= w2;
}

/**
 * @brief	Algoritmo 1 Conversão de expressão no formato infixo para posfixo.
 */
std::vector< Token >
//std::string 
infix2posfix (std::vector< Token > token_list) 
{
	// Armazena a expressão em notação posfixa.
	//std::string postfix ("");
	std::vector< Token > postfix;
	// Stack, to help us convert from infix to postfix.     
	std::stack< Token > st;
	
	// Run through each Token in the incoming expression.
	for (auto it = token_list.begin(); it != token_list.end(); ++it)
	{
		if ( is_operand( *it ) )
		{
			postfix.push_back( *it);
		}/*
		else if ( is_opening_scope( s ) )
		{
			st.push( s ); // goes into the stack, regardless of who is on top.
		}
		else if ( is_closing_scope( s ) )
		{
			// Pops out all symbols until we get to his 'brother'
			while( not st.empty() and not is_opening_scope( st.top() ) )
			{
				auto t = st.top(); // the operand on top.
				st.pop();
				postfix += t; // All operands on the stack goes to the output string.
			}
			// Don't forget to remove the opening scope.
			st.pop();
		}
		*/
		else if ( is_operator( *it ) )
		{
			// top() >= s ??? yes-> goes to output; no-> stay on the stack.
			while(  not st.empty() and has_higher_precedence( st.top(), (*it) ) )
			{
				postfix.push_back( st.top() ); // goes to output.
				st.pop(); // remove the operator
			}
			// The incoming operator always goes into the stack.
			st.push((*it));
		}
	}
	
	// For: processing input.
	// Dump all remaining elements from the stack into the outpu.
	while( not st.empty() )
	{
		postfix.push_back(st.top());
		st.pop();
	}
	return postfix;
}
