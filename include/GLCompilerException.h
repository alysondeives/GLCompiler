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

/* Exceções utilizadas */

#ifndef	EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <iostream>

using namespace std;

extern int linenumber;
extern string lexema;

class GLCompilerException : public exception{
	
public:
	string mensagem;
	void getMessage() { cout<<mensagem<<endl; }
};

class InvalidCharacterException : public GLCompilerException {
public:
	InvalidCharacterException();
};

class LexemeUnidentifiedException : public GLCompilerException {
public:
	LexemeUnidentifiedException();
};

class EndOfFileException : public GLCompilerException {
public:
	EndOfFileException();
};

class ExpectedEndOfFileException : public GLCompilerException {
public:
	ExpectedEndOfFileException();
};

class UnexpectedTokenException : public GLCompilerException {
public:
	UnexpectedTokenException();
};

class InvalidNumberOfArgumentsException : public GLCompilerException {
public:
	InvalidNumberOfArgumentsException();
};

class FileNotFoundException : public GLCompilerException {
public:
	FileNotFoundException(char* filename);
};

class UndeclaredIdentifierException : public GLCompilerException {
public:
	UndeclaredIdentifierException(string);
};

class AlreadyDeclaredIdentifierException : public GLCompilerException {
public:
	AlreadyDeclaredIdentifierException(string);
};

class ClassException : public GLCompilerException {
public:
	ClassException(string);
};

class TypeException : public GLCompilerException {
public:
	TypeException();
};

class ComponentColorException : public GLCompilerException {
public:
	ComponentColorException();
};

class SymbolNotFoundException : public GLCompilerException {
public:
	SymbolNotFoundException();
};

class InstructionException : public GLCompilerException {
public:
	InstructionException(short);
};

class RegisterException : public GLCompilerException {
public:
	RegisterException(short);
};

class InvalidInstructionException : public GLCompilerException {
public:
	InvalidInstructionException(string);
};

class InvalidArgumentException : public GLCompilerException {
	public:
	InvalidArgumentException(char *);
};

#endif