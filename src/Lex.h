/**
* ----------------------------------------------------------------
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
* @author Douglas de Loreto Borges	Matricula: 417889
* @author Matheus Lincoln Pereira	Matricula: 415904
* @version 1.0 15/04/2013            
*
*/

/* Classe que realiza a análise léxica */

#ifndef	LEX_H
#define LEX_H

#include <string>
#include <iostream>
#include "FileReader.h"
#include "Datatypes.h"
#include "SymbolsTable.h"

using namespace std;

class Lex{

	private:
	FileReader arquivo;
	lexicalRegister lexReg;
	
	bool isDigit(char);
	bool isLetter(char);
	bool isValid(char);
	bool isValidInLexeme(char);
	bool isDelimiter(char);
	bool isBlank(char);
	bool isNewLine(char);

	public:
	Lex (char*);
	lexicalRegister getToken();
	bool fimDeArquivo();
};

#endif
