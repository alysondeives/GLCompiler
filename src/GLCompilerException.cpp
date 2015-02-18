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

/* Exceções utilizadas */


#include "GLCompilerException.h"

InvalidCharacterException::InvalidCharacterException (){
	mensagem = to_string((long double)linenumber)+":caractere invalido.\n"; //cast realizado pois o VS 2010 não implementa com o tipo int
}

LexemeUnidentifiedException::LexemeUnidentifiedException (){
	mensagem = to_string((long double)linenumber)+":lexema nao identificado ["+lexema+"].";
}

EndOfFileException::EndOfFileException (){
	mensagem = to_string((long double)linenumber)+":fim de arquivo nao esperado.";
}

ExpectedEndOfFileException::ExpectedEndOfFileException (){
	mensagem = to_string((long double)linenumber)+":fim de arquivo esperado.";
}

UnexpectedTokenException::UnexpectedTokenException(){
	mensagem = to_string((long double)linenumber)+":token nao esperado ["+lexema+"].";
}

InvalidNumberOfArgumentsException::InvalidNumberOfArgumentsException(){
	mensagem = "Quantidade de argumentos invalida. Indique o nome do arquivo fonte.";
}

FileNotFoundException::FileNotFoundException(){
	mensagem = "Arquivo nao encontrado.";
}

UndeclaredIdentifierException::UndeclaredIdentifierException(string lex){
	mensagem = to_string((long double)linenumber)+":identificador nao declarado ["+lex+"].";
}

AlreadyDeclaredIdentifierException::AlreadyDeclaredIdentifierException(string lex){
	mensagem = to_string((long double)linenumber)+":identificador ja declarado ["+lex+"].";
}

ClassException::ClassException(string lex){
	mensagem = to_string((long double)linenumber)+":classe de identificador incompativel ["+lex+"].";
}

TypeException::TypeException(){
	mensagem = to_string((long double)linenumber)+":tipos incompativeis.";
}

ComponentColorException::ComponentColorException(){
	mensagem = "Cada componente de cor deve ser um inteiro de 0 a 63.";
}

SymbolNotFoundException::SymbolNotFoundException(){
	mensagem = "Simbolo nao encontrado.";
}

InstructionException::InstructionException(short _ins){
	mensagem =  "Instrucao nao existente: " + to_string((long double)_ins);
}

RegisterException::RegisterException(short _reg){
	mensagem =  "Registrador nao existente: " + to_string((long double)_reg);
}

InvalidInstructionException::InvalidInstructionException(string _ins){
	mensagem =  "Instrucao invalida: " + _ins;
}

InvalidArgumentException::InvalidArgumentException(char * _arg){
	std::string arg = _arg;
	mensagem =  "Argumento invalido: " + arg;
}