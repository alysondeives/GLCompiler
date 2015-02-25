/**
* ------------------------------------------------------------------
* Pontificia Universidade Catolica de Minas Gerais 
* Curso de Ciencia da Computacao 
* Compiladores
* 
* Trabalho Pratico - GLCompiler
*
*
* Objetivo:
* Construção de um compilador para uma linguagem gráfica que executa scripts de animação
*   
* @author Alyson Deives Pereira		Matricula: 416589
* @version 1.0 15/04/2013            
*
*/

/* Programa Principal */

#include <iostream>
#include "../include/Lex.h"
#include "../include/Parser.h"
#include "../include/GLCompilerException.h"
#include "../include/FileWriter.h"
using namespace std;
Parser *parser;

int main (int argc, char* argv[]){
	bool opt = 0;
	
	try{
		//verifica se o nome do arquivo foi informado
		if(argc < 2){
			throw InvalidNumberOfArgumentsException();
		}
		if (argc == 3){
			if(strcmp(argv[2], "-opt") == 0){
				opt = 1;
			}
			else{
				throw InvalidArgumentException(argv[2]);
			}
		}
		parser = new Parser(argv[1], opt);
		parser->run();
		cout<<"Parsing realizado com sucesso"<<endl;
	}
	catch(GLCompilerException e){ //captura exceções lançadas e exibe mensagem
		e.getMessage();
		parser->escritor.deletarArquivo(argv[1]);
	}
}