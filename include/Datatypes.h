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
* Constru��o de um compilador para uma linguagem gr�fica que executa scripts de anima��o
*   
* @author Alyson Deives Pereira		Matricula: 416589
* @version 1.0 15/04/2013            
*
*/

/* Header que contem a defini��o de todos os tipos de dados utilizados */


#ifndef	DATATYPES_H
#define DATATYPES_H

#include <string>

using namespace std;

/* Tokens da linguagem GL */
typedef enum: char
{
	/* Identificadores e Constantes*/
	ID,	CONSTANT,
	/* Separadores*/
	COMMA, SEMICOLON, LEFT_PARENS, RIGHT_PARENS,
	/* Operadores aritm�ticos*/
	ASSIGN,	SUM, SUB, MULT, DIV,
	/* Operador L�gicos*/
	AND, OR,NOT,
	/* Comparacao L�gica*/
	EQ,	NEQ, LT, GT, LE, GE,
	/* Palavras reservadas*/
	BEGIN, COLOR, CONST, DECLARE, ELSE, END, FACE,IF, INTEGER, LIGHT, OBJECT, PAUSE, POINT, REAL, ROTTRANS, SCALE, THEN, VAR, WHILE,
	/* Fim de Arquivo*/
	ENDFILE
}Token;

/* Tipo de dado inteiro ou real para identificadores de variaveis e constantes*/
typedef enum{
	TIPO_INTEIRO, TIPO_REAL, TIPO_LOGICO
} Tipo;

/* Classe dos identificadores */
typedef enum{
	CLASSE_VAR, CLASSE_CONST, CLASSE_PONTO, CLASSE_FACE,
	CLASSE_OBJETO, CLASSE_LUZ, CLASSE_COR
} Classe;

/* Registro l�xico */
typedef struct{
	string lexema;
	Token token;
	Tipo tipo;
	short integerValue;
	short fracValue;
}lexicalRegister;

/* Simbolo */
typedef struct {
	string lexema;
	Token token;
	Tipo tipo;
	Classe classe;
	short size;
	short end;
} Symbol;

#endif