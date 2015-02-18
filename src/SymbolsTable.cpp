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

/* Classe que manipula a tabela de símbolos */

#include "SymbolsTable.h"
#include "GLCompilerException.h"
#include <string>
#include <unordered_map>
#include <iostream>

SymbolsTable::SymbolsTable(){
	this->load();
}

void SymbolsTable::insert(string lexema, Token token){
	Symbol s = {lexema, token, (Tipo) -1, (Classe) -1,-1,-1};
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
}

void SymbolsTable::atualizaClasse (string lexema, Classe classe){
	it = symbolsTable.find(lexema);
	if (it == symbolsTable.end()) throw SymbolNotFoundException();
	else it->second.classe = classe;
}

void SymbolsTable::atualizaTipo (string lexema, Tipo tipo){
	it = symbolsTable.find(lexema);
	if (it == symbolsTable.end()) throw SymbolNotFoundException();
	else it->second.tipo = tipo;
}

short SymbolsTable::getSize(string _lexema){
	it = symbolsTable.find(lexema);
	if (it == symbolsTable.end()) throw SymbolNotFoundException();
	else it->second.size;
}

void SymbolsTable::setSize(string lexema, short _size){
	it = symbolsTable.find(lexema);
	if (it == symbolsTable.end()) throw SymbolNotFoundException();
	else it->second.size = _size;
}

Classe SymbolsTable::getClasse(string lexema){
	it = symbolsTable.find(lexema);
	if (it == symbolsTable.end()) throw SymbolNotFoundException();
	else return it->second.classe;
}

Tipo SymbolsTable::getTipo(string lexema){
	it = symbolsTable.find(lexema);
	if (it == symbolsTable.end()) throw SymbolNotFoundException();
	else return it->second.tipo;
}

short SymbolsTable::getEndereco(string lexema){
	it = symbolsTable.find(lexema);
	if (it == symbolsTable.end()) throw SymbolNotFoundException();
	else return it->second.end;
}

void SymbolsTable::setEndereco(string lexema, short end){
	it = symbolsTable.find(lexema);
	if (it == symbolsTable.end()) throw SymbolNotFoundException();
	else it->second.end = end;
}

bool SymbolsTable::find(string lexema){
	it = symbolsTable.find(lexema);
	if (it != symbolsTable.end())
		return true;
	return false;
}

Token SymbolsTable::getSearchedToken (){
	return it->second.token;
}

void SymbolsTable::print(){
	cout << "Tabela de simbolos contem:"<<endl;
	//C++11  implementation. Não funciona no VS 2010
	/*
	for (auto& x: symbolsTable) {
		std::cout << x.first << " : " << x.second.token << std::endl;
	}
	*/

	for (it = symbolsTable.begin(); it != symbolsTable.end(); ++it ){
		cout << " " << it->first;
		cout << ": " << (int) it->second.end <<endl;
	}
	cout << endl;
}

void SymbolsTable::load(){
	Symbol s;
	s.classe = (Classe) -1;
	s.tipo = (Tipo) -1;

	/* Separadores*/
	s.token = COMMA;
	s.lexema = ",";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));

	s.token = SEMICOLON;
	s.lexema = ";";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));

	s.token = LEFT_PARENS;
	s.lexema = "(";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));

	s.token = RIGHT_PARENS;
	s.lexema = ")";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	/* Operadores aritméticos*/
	s.token = ASSIGN;
	s.lexema = "=";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = SUM;
	s.lexema = "+";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = SUB;
	s.lexema = "-";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = MULT;
	s.lexema = "*";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = DIV;
	s.lexema = "/";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	/* Operador Lógicos*/
	s.token = AND;
	s.lexema = "and";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = OR;
	s.lexema = "or";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = NOT;
	s.lexema = "not";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	/* Comparacao Lógica*/
	s.token = EQ;
	s.lexema = "==";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = NEQ;
	s.lexema = "!=";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = LT;
	s.lexema = "<";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = GT;
	s.lexema = ">";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = LE;
	s.lexema = "<=";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = GE;
	s.lexema = ">=";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	/* Palavras reservadas*/
	s.token = BEGIN;
	s.lexema = "begin";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = COLOR;
	s.lexema = "color";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = CONST;
	s.lexema = "const";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = DECLARE;
	s.lexema = "declare";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = ELSE;
	s.lexema = "else";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = END;
	s.lexema = "end";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = FACE;
	s.lexema = "face";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = IF;
	s.lexema = "if";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = INTEGER;
	s.lexema = "integer";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = LIGHT;
	s.lexema = "light";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = OBJECT;
	s.lexema = "object";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));

	s.token = PAUSE;
	s.lexema = "pause";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));

	s.token = POINT;
	s.lexema = "point";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
	
	s.token = REAL;
	s.lexema = "real";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));

	s.token = ROTTRANS;
	s.lexema = "rottrans";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));

	s.token = SCALE;
	s.lexema = "scale";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));

	s.token = THEN;
	s.lexema = "then";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));

	s.token = VAR;
	s.lexema = "var";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));

	s.token = WHILE;
	s.lexema = "while";
	symbolsTable.insert(std::pair<string,Symbol>(s.lexema, s));
}