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

#ifndef	FILEREADER_H
#define FILEREADER_H

#include <string>
#include <iostream>
#include <fstream>

/* Classe para leitura do arquivo fonte */
class FileReader{

	private:
	std::ifstream file; /* Objeto para a leitura de arquivo */
	char buffer[1];		/* Buffer que armazena os caracteres lidos */

	public:
	bool fimArquivo;			/* Flag que indica que chegou no fim do arquivo */
	char getChar();				/* Lê um caracter do arquivo */
	void fecharArquivo();		/* Fecha o arquivo */
	void abrirArquivo(char *);	/* Abre o arquivo para leitura */
	void returnChar();			/* Volta um caracter na leitura do arquivo */
};

#endif
