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
* @version 1.0 15/04/2013            
*
*/

/* Classe que realiza a análise léxica */

#include <fstream>
#include "../include/Lex.h"
#include "../include/GLCompilerException.h"
#include "../include/FileReader.h"

using namespace std;

string lexema = "";
SymbolsTable *symbolsTable;

Lex::Lex(char* _arquivo){
	symbolsTable = new SymbolsTable();
	arquivo.abrirArquivo(_arquivo);
}


lexicalRegister Lex::getToken(){
	lexema = "";
	string integerValue = "0";
	string fracValue = "0";
	Tipo tipo = TIPO_INTEIRO;
	int estado = 0;
	char c;
	bool TokenConst = false; //boolean para verificar se o token é constante
	while(estado != 2){
		if (estado != 2){
			c = arquivo.getChar();
		}
		if(c == EOF && (estado == 7 || estado == 8 )){
			throw EndOfFileException();
		}
		else if (c==EOF && lexema != ""){
			estado = 2;
		}
		else if(arquivo.fimArquivo){
			//throw EndOfFileException(); removido por enquanto. Fim de arquivo é o Token ENDFILE
			estado = 2;
			lexReg.lexema = "eof";
			lexReg.token = ENDFILE;
			lexReg.integerValue = -1; // Registro lexico não possui valor inteiro
			lexReg.fracValue = -1; //Registro lexico nao possui valor fracionario
			lexReg.tipo = (Tipo) -1; //Registro lexico nao possui tipo
			return lexReg;
		}
		if(!this->isValid(c)){
			throw InvalidCharacterException();
		}		
		switch (estado){
		case 0:
			if (this->isBlank(c) || this->isNewLine(c)){
				estado = 0;
			}
			else if(this->isLetter(c)){
				lexema = c;
				estado = 1;
			}
			else if(c == '*' || c == '+' || c =='-' || c == ';' || c == ',' || c == '(' || c == ')'){
				lexema = c;
				estado = 2;
			}
			else if (this->isDigit(c)){
				lexema = c;
				estado = 3;
			}
			else if (c == '.'){
				lexema = c;
				estado = 4;
			}
			else if (c == '>' || c == '<' || c=='='){
				lexema = c;
				estado = 5;
			}
			else if (c == '/'){
				lexema = c;
				estado = 6;
			}
			else if (c == '!'){
				lexema = c;
				estado = 9;
			}
			else if(!this->isValidInLexeme(c)){
				lexema=c;
				throw LexemeUnidentifiedException();
			}
			break;
		case 1:
			if(this->isLetter(c) || this->isDigit(c)){
				lexema += c;
				estado = 1;
			}
			else if(!this->isValidInLexeme(c)){
				lexema += c;
				throw LexemeUnidentifiedException();
			}
			else{
				estado = 2;
				arquivo.returnChar();
			}
			break;
		case 3:
			if (this->isDigit(c)){
				lexema += c;
				estado = 10;
			}
			else if (c == '.'){
				integerValue = lexema; //armazena o valor acumulado da parte inteira
				lexema += c;
				estado = 13;
			}
			else{
				TokenConst = true;
				arquivo.returnChar();
				integerValue = lexema;
				estado = 2;
			}
			break;
		case 4:
			if (this->isDigit(c)){
				lexema += c;
				estado = 14;
			}
			else{ 
				//ERRO - Lexema não identificado
				throw LexemeUnidentifiedException();
			}
			break;
		case 5:
			if(c == '='){
				lexema += c;
				estado = 2;
			}
			else{
				arquivo.returnChar();
				estado = 2;
			}
			break;
		case 6:
			if(c == '*'){
				lexema = "";
				estado = 7;
			}
			else{
				estado = 2;
				arquivo.returnChar();
			}
			break;
		case 7:
			if (c == '*'){
				estado = 8;
			}
			else if (this->isValid(c)){
				estado = 7;
			}
			else{
				//ERRO - Lexema não identificado
				throw LexemeUnidentifiedException();
			}
			break;
		case 8:
			if (c == '/'){
				estado = 0;
			}
			else if (c != '/' && c!= '*' && this->isValid(c)){
				estado = 7;
			}
			else if (c == '*'){
				estado = 8;
			}
			else{
				//ERRO - Lexema não identificado
				throw LexemeUnidentifiedException();
			}
			break;
		case 9:
			if(c == '='){
				lexema += c;
				estado = 2;
			}
			else{
				//ERRO - Lexema não identificado
				throw LexemeUnidentifiedException();
			}
			break;
		case 10:
			if (this->isDigit(c)){
				lexema += c;
				estado = 11;
			}
			else if (c == '.'){
				integerValue = lexema; //armazena o valor acumulado da parte inteira
				lexema += c;
				estado = 13;
			}
			else{
				TokenConst = true;
				arquivo.returnChar();
				integerValue = lexema;
				estado = 2;
			}
			break;
		case 11:
			if (this->isDigit(c)){
				lexema += c;
				estado = 12;
			}
			else if (c == '.'){
				integerValue = lexema; //armazena o valor acumulado da parte inteira
				lexema += c;
				estado = 13;
			}
			else{
				TokenConst = true;
				arquivo.returnChar();
				integerValue = lexema;
				estado = 2;
			}
			break;
		case 12:
			if (c == '.'){
				integerValue = lexema; //armazena o valor acumulado da parte inteira
				lexema += c;
				estado = 13;
			}
			else if(!this->isDigit(c)){
				TokenConst = true;
				arquivo.returnChar();
				integerValue = lexema;
				estado = 2;
			}
			else{
				//ERRO - Lexema não identificado
				lexema += c;
				throw LexemeUnidentifiedException();
			}
			break;
		case 13:
			if (this->isDigit(c)){
				lexema += c;
				estado = 14;
			}
			else if(c != '.') {
				TokenConst = true;
				arquivo.returnChar();
				tipo = TIPO_REAL;
				estado = 2;
			}
			else{
				//ERRO - Lexema não identificado
				lexema += c;
				throw LexemeUnidentifiedException();
			}
			break;
		case 14:
			if (this->isDigit(c)){
				lexema += c;
				estado = 15;
			}
			else if(c != '.') {
				TokenConst = true;
				arquivo.returnChar();
				tipo = TIPO_REAL;
				unsigned pos = lexema.find(".");
				fracValue = lexema.substr(pos+1)+"000";
				estado = 2;
			}
			else{
				//ERRO - Lexema não identificado
				lexema += c;
				throw LexemeUnidentifiedException();
			}
			break;
		case 15:
			if (this->isDigit(c)){
				lexema += c;
				estado = 16;
			}
			else if(c != '.') {
				TokenConst = true;
				arquivo.returnChar();
				tipo = TIPO_REAL;
				unsigned pos = lexema.find(".");
				fracValue = lexema.substr(pos+1)+"00";
				estado = 2;
			}
			else{
				//ERRO - Lexema não identificado
				lexema += c;
				throw LexemeUnidentifiedException();
			}
			break;
		case 16:
			if (this->isDigit(c)){
				lexema += c;
				estado = 17;
			}
			else if(c != '.') {
				TokenConst = true;
				arquivo.returnChar();
				tipo = TIPO_REAL;
				unsigned pos = lexema.find(".");
				fracValue = lexema.substr(pos+1)+"0";
				estado = 2;
			}
			else{
				//ERRO - Lexema não identificado
				lexema += c;
				throw LexemeUnidentifiedException();
			}
			break;
		case 17:
			if(c != '.' && !this->isDigit(c)) {
				TokenConst = true;
				arquivo.returnChar();
				tipo = TIPO_REAL;
				unsigned pos = lexema.find(".");
				fracValue = lexema.substr(pos+1);
				estado = 2;
			}
			else{
				//ERRO - Lexema não identificado
				lexema += c;
				throw LexemeUnidentifiedException();
			}
			break;
		default:
			break;
		}//fim switch
	}//fim while
	if (!TokenConst){
		if(symbolsTable->find(lexema)){
			lexReg.lexema = lexema;
			lexReg.token = symbolsTable->getSearchedToken();
			lexReg.integerValue = -1; // Registro lexico não possui valor inteiro
			lexReg.fracValue = -1; //Registro lexico nao possui valor fracionario
			lexReg.tipo = (Tipo) -1; //Registro lexico nao possui tipo
		}
		else{
			symbolsTable->insert(lexema,ID);
			lexReg.lexema = lexema;
			lexReg.token = ID;
			lexReg.integerValue = -1; // Registro lexico não possui valor inteiro
			lexReg.fracValue = -1; //Registro lexico nao possui valor fracionario
			lexReg.tipo = (Tipo) -1; //Registro lexico nao possui tipo
		}
	}
	else{
		lexReg.lexema = lexema;
		lexReg.token = CONSTANT;
		lexReg.integerValue = stoi(integerValue);
		lexReg.fracValue = stoi(fracValue);
		lexReg.tipo = tipo;
	}
	return lexReg;
}

bool Lex::isDigit(char c){
	if(c>=48 && c<=57)
		return true;
	else
		return false;
}

bool Lex::isLetter(char c){
	if(c>=65 && c<=90 || c>=97 && c<=122)
		return true;
	else
		return false;
}

bool Lex::isBlank(char c){
	if(c==9 || c== 13 || c==32){
		return true;
	}
	else{
		return false;
	}
}

bool Lex::isNewLine(char c){
	if(c==10){
		return true;
	}
	else{
		return false;
	}
}


bool Lex::isValid(char c){
	if(c==9 || c==10 || c==13 ||	//quebras de linha e tab
		c>=32 && c<= 34 ||			//espaço, ! e "
		c>=39 && c<=63 ||			//' ( ) * + , - . / 0 1 2 3 4  5 6 7 8 9 : ; < = > ? 
		c>= 65 && c<=91 ||			//A a Z [
		c==93 ||					//]	
		c>=97 && c<= 123 ||			//a A z {
		c==125 ||					//}
		c==EOF){						//fim de arquivo
			return true;
	}
	else return false;
}

bool Lex::isValidInLexeme(char c){
		if(c==9 || c==10 || c==13 ||	//quebras de linha e tab
		c>=32 && c<= 34 ||			//espaço, ! e "
		c>=40 && c<=57 ||			//( ) * + , - . / 0 1 2 3 4  5 6 7 8 9
		c>=59 && c<=62 ||			//; < = >
		c>= 65 && c<=90 ||			//A a Z
		c>=97 && c<= 122){			//a A z
			return true;
	}
	else return false;
	}

bool Lex::fimDeArquivo(){
	return arquivo.fimArquivo;
}


