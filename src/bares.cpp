/**
 * @file	bares.cpp
 * @brief	Implementação do programa BARES (Basic ARithmetic Expression Evaluator based on Stacks) para avaliar e calcular expressões aritméticas
 *
 * @author	Josivan Medeiros da Silva Gois
 * @since	04/11/2017
 * @date	04/11/2017
 *
 */

#include <iostream>
#include <iomanip>
#include <vector>

#include "parser.h"
#include "infix_to_postfix.h"

typedef short int required_int_type;

std::vector<std::string> expressions;

void print_msg( const Parser::ParserResult & result, std::string str )
{
	std::string error_indicator( str.size()+1, ' ');

	// Have we got a parsing error?
	error_indicator[result.at_col] = '^';
	switch ( result.type )
	{
		case Parser::ParserResult::UNEXPECTED_END_OF_EXPRESSION:
			std::cout << ">>> Unexpected end of input at column (" << result.at_col << ")!\n";
			break;
		case Parser::ParserResult::ILL_FORMED_INTEGER:
			std::cout << ">>> Ill formed integer at column (" << result.at_col << ")!\n";
			break;
		case Parser::ParserResult::MISSING_TERM:
			std::cout << ">>> Missing <term> at column (" << result.at_col << ")!\n";
			break;
		case Parser::ParserResult::EXTRANEOUS_SYMBOL:
			std::cout << ">>> Extraneous symbol after valid expression found at column (" << result.at_col << ")!\n";
			break;
		case Parser::ParserResult::INTEGER_OUT_OF_RANGE:
			std::cout << ">>> Integer constant out of range beginning at column (" << result.at_col << ")!\n";
			break;
		default:
			std::cout << ">>> Unhandled error found!\n";
			break;
	}

	std::cout << "\"" << str << "\"\n";
	std::cout << " " << error_indicator << std::endl;
}

required_int_type execute_operator( required_int_type term1, required_int_type term2, char op) 
{
	required_int_type result = 0;
	switch( op )
	{
		case '+':  
			result = term1 + term2;
			break;
		case '-':
			result = term1 - term2;
			break;
		case '*':
			result = term1 * term2;
			break;
		case '^':
			result = pow( term1, term2 );
			break;
		case '/':
			if ( term2 == 0 ) throw std::runtime_error(" Division by zero " );                    
			result = term1 / term2;                    
			break;
		case '%':  
			if ( term2 == 0 ) throw std::runtime_error(" Division by zero " );
			result = term1 % term2;
			break;
	}
	return result;
}

required_int_type evaluate_postfix( std::vector<Token> postfix )
{
	// Stack of numbers.
	std::stack< required_int_type > st;
	// Process each symbol in the postfix experssion.     
	for( Token & t : postfix )
	{         
		if ( is_operand( t ) )         
		{             
			// Each operand always goes into the stack. 
			auto temp = std::stoi(t.value, nullptr);
			st.push( temp );
		}
		else if ( is_operator( t ) )         
		{
			// Pops out the two terms of the binary operation...
			auto term2 = st.top(); st.pop();
			// Invert the order first!
			auto term1 = st.top(); st.pop();
			// ... executes the operation, and...
			auto r = execute_operator( term1, term2, t.value[0] );             
			// ... pushes the result back onto the stack.             
			st.push( r );         
		}     
	}     
	// If everything goes smoothly, the result should be on the top of the stack.     
	return st.top(); 
}

int main ( /*int argc, char const *argv[]*/ )
{
	/* @TODO	Verificar se o programa foi executado com parâmetros ou stream */

	// Lendos os dados de entrada
	std::string temp;
	while ( std::getline(std::cin, temp) )
	{
		expressions.push_back(temp);
	}

	Parser my_parser; // Instancia um parser.
	// Tentar analisar cada expressão da lista.
	for( const auto & expr : expressions )
	{
		// Fazer o parsing desta expressão.
		auto result = my_parser.parse( expr );
		// Preparar cabeçalho da saida.
		std::cout << std::setfill('=') << std::setw(80) << "\n";
		std::cout << std::setfill(' ') << ">>> Parsing \"" << expr << "\"\n";
		// Se deu pau, imprimir a mensagem adequada.
		if ( result.type != Parser::ParserResult::PARSER_OK )
			print_msg( result, expr );
		else
		{
			std::cout << ">>> Expression SUCCESSFULLY parsed!\n";
			std::cout << "\nLista de Tokens posfixo:\n";
			std::vector< Token > postfix = infix2posfix( my_parser.get_tokens() );

			for (Token & t : postfix)
			{
				std::cout << t.value;
			}
			std::cout << "\n>>>>Resposta: <<<<<<<\n";
			auto answer = evaluate_postfix(postfix);
			std::cout << "===" << answer << "===\n\n";
		}

		 // Recuperar a lista de tokens.
		auto lista = my_parser.get_tokens();
		std::cout << ">>> Tokens: { ";
		std::copy( lista.begin(), lista.end(),
				std::ostream_iterator< Token >(std::cout, " ") );
		std::cout << "}\n";
	}

	std::cout << "\n>>> Normal exiting...\n";

	return EXIT_SUCCESS;
}
