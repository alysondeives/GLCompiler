/**
* ------------------------------------------------------------------
* Pontificia Universidade Catolica de Minas Gerais 
* Curso de Ciencia da Computacao 
* Compiladores
* 
* Trabalho Pratico - GLCompiler
* Parte 1
* Analisador L�xico e Analisador Sint�tico (Parser)
*
* Objetivo:
* Constru��o de um compilador para uma linguagem gr�fica que executa scripts de anima��o
*   
* 
* @author Alyson Deives Pereira		Matricula: 416589
* @author Douglas de Loreto Borges	Matricula: 417889
* @author Matheus Lincoln Pereira	Matricula: 415904
* @version 1.0 15/04/2013            
*
*/

/* Gram�tica LL1 correspondente a linguagem GL

START -> declare DECLARATION* COMMAND_BLOCK ENDFILE
DECLARATION -> 	point {id = "(" [+|-] num, [+|-] num, [+|-] num ")";}+ |
		light {id = "("[+|-] num, [+|-] num, [+|-] num ")";}+ |
		color {id = num,num,num,num,num,num;}+ |
		face {id = id,id,id,id {,id}*;}+ |
		object {id = id {,id}*;}+ |
		var { (integer | real) id {,id}*;}+ |
		const {id = [+|-] num;}+

COMMAND_BLOCK -> begin COMMAND* end

COMMAND ->	id = EXP; |
		while EXP ( COMMAND_BLOCK | COMMAND ) |
		if EXP then ( COMMAND_BLOCK | COMMAND ) [else ( COMMAND_BLOCK | COMMAND )] |
		scale id, EXP; |
		pause EXP; |
		light id; |
		rottrans id, EXP, EXP, EXP,EXP, EXP, EXP;

LOGIC_COMPARATION -> == | != | < | > | <= | >=

EXP -> EXP_SUM [LOGIC_COMPARATION EXP_SUM]
EXP_SUM -> [+|-] EXP_PRODUCT {(+|-|or) EXP_PRODUCT}*
EXP_PRODUCT -> EXP_VALUE {(*|/|and) EXP_VALUE}*
EXP_VALUE -> not EXP_VALUE | "(" EXP ")" | id | const
*/


#ifndef	PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
#include "Lex.h"
#include "SymbolsTable.h"
#include "FileWriter.h"


using namespace std;

extern SymbolsTable *symbolsTable;			/* Tabela de S�mbolos */


/* Classe que realiza a analise sint�tica */
class Parser{
private:
	lexicalRegister previousToken;				/* Registro L�xico anterior */
	lexicalRegister currentToken;				/* Registro L�xico corrente */
	Lex *lex;									/* Objeto do analisador l�xico */
	short baseEndTemp;							/* Endereco para a base dos temporarios */
	short endTemp;								/* Valor corrente de endere�o para um temporario */
	long double currentLabel;
	bool opt;

public:
	Parser(char*, bool);							/* Construtor padr�o */
	FileWriter escritor;						/* Gerador do c�digo assembly e bin�rio GCX*/
	void start();							/* Procedimento da regra inicial */
	void declaration();						/* Procedimento da regra de declara��es */
	void commandBlock();					/* Procedimento da regra de bloco de comandos */
	void command();							/* Procedimento da regra de comandos */
	void logicComparation();				/* Procedimento da regra de compara��o l�gica */
	void exp(Tipo &, short &);				/* Procedimento da regra de express�es */
	void expSum(Tipo &, short &);			/* Procedimento da regra de express�es no nivel da soma */	
	void expProduct(Tipo &, short &);		/* Procedimento da regra de express�es no nivel da multiplica��o */	
	void expValue(Tipo &, short &);			/* Procedimento da regra de express�es no nivel de valores */	
	void run();								/* Inicia o processo de parsing */
	void matchToken(Token);					/* M�todo Casa Token */
	void unicityAlreadyDeclared (Classe);	/* Verifica se identificador j� foi declarado */
	void unicityNotDeclared ();
	void setType(Tipo);
	void setType(string, Tipo);
	void classVerify(Classe);
	void typeVerify(Tipo);
	void constTypeVerify(Tipo, Tipo);
	void componentColorVerify(int);
	void setEndereco(string);				/* Seta endere�o dos identificadores */
	short novoTemp(short);						/* Obtem novo endereco para temporario */
	void tempReset();						/* Reseta o contador de temporarios para a basede temporarios*/
	GCX::Label novoRot();
};

#endif