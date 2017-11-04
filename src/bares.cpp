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
			std::cout << ">>> Expression SUCCESSFULLY parsed!\n";

		 // Recuperar a lista de tokens.
		auto lista = my_parser.get_tokens();
		std::cout << ">>> Tokens: { ";
		std::copy( lista.begin(), lista.end(),
				std::ostream_iterator< Token >(std::cout, " ") );
		std::cout << "}\n";
	}

	std::cout << "\n>>> Normal exiting...\n";

	std::cout << "\nLista de Tokens:\n";
	for (Token & t : my_parser.get_tokens())
	{
		std::cout << t.value;
	}

	return EXIT_SUCCESS;
}