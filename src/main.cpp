/**
* ------------------------------------------------------------------
* Pontificia Universidade Catolica de Minas Gerais 
* Curso de Ciencia da Computacao 
* Compiladores
* 
* Trabalho Pratico - GLCompiler
* Parte 1
* Analisador Léxico e Analisador Sintático (Parser)
*
* Objetivo:
* Construção de um compilador para uma linguagem gráfica que executa scripts de animação
*   
* 
* @author Alyson Deives Pereira		Matricula: 416589
* @author Douglas de Loreto Borges	Matricula: 417889
* @author Matheus Lincoln Pereira	Matricula: 415904
* @version 1.0 15/04/2013            
*
*/

/* Programa Principal */

#include <iostream>
#include "Lex.h"
#include "Parser.h"
#include "GLCompilerException.h"
#include "FileWriter.h"
using namespace std;
Parser *parser;

int main (int argc, char* argv[]){
	bool opt = 0;
	
	char * optPar = argv[2];
	char * arquivo = argv[1];
	char * optArg = "-opt";
	try{
		//verifica se o nome do arquivo foi informado
		if(argc < 2){
			throw InvalidNumberOfArgumentsException();
		}
		if (argc == 3 && strcmp(optPar, optArg) == 0){
			opt = 1;
		}
		else{
			throw InvalidArgumentException(optPar);
		}
		parser = new Parser(arquivo, opt);
		parser->run();
		cout<<"Parsing realizado com sucesso"<<endl;
	}
	catch(GLCompilerException e){ //captura exceções lançadas e exibe mensagem
		e.getMessage();
		parser->escritor.deletarArquivo(arquivo);
	}
}