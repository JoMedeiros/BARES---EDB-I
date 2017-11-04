#ifndef _INFIX_TO_POSTFIX_H_
#define _INFIX_TO_POSTFIX_H_ 

#include "parser.h"
#include <stack>

bool is_operand( Token t );
bool is_operator( Token t );
short get_precedence( char s );
bool has_higher_precedence( Token op1, Token op2 );
std::vector< Token > infix2posfix (std::vector< Token > token_list);

#endif