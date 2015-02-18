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
* @author Douglas de Loreto Borges	Matricula: 417889
* @author Matheus Lincoln Pereira	Matricula: 415904
* @version 1.0 15/04/2013            
*
*/

/* Classe que manipula a tabela de s�mbolos */

#ifndef	SYMBOLSTABLE_H
#define SYMBOLSTABLE_H

#include <string>
#include <unordered_map>
#include "Datatypes.h"

using namespace std;

class SymbolsTable{

	private:
	unordered_map<string, Symbol> symbolsTable;

	public:
	SymbolsTable ();
	Token searchedToken;
	unordered_map<string,Symbol>::iterator it;
	void insert(string, Token);
	void atualizaClasse (string, Classe);
	void atualizaTipo (string, Tipo);
	Classe getClasse (string);
	Tipo getTipo (string);
	void setEndereco(string, short);
	short getEndereco(string);
	void load();
	void print();
	bool find(string);
	Token getSearchedToken ();
	short getSize (string);
	void setSize (string, short);
	
};

#endif