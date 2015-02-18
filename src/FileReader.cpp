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
* @author Matheus Lincoln Pereira		Matricula: 415904
* @version 1.0 15/04/2013            
*
*/

/* Classe para leitura do arquivo fonte */

#include "FileReader.h"
#include "GLCompilerException.h"

int linenumber = 1; /*armazena a linha atual que está sendo lida*/

void FileReader::abrirArquivo(char* arquivo){
	file.open(arquivo, ios::in|ios::binary|ios::ate);
	if (file.is_open())	{
		file.seekg (0, ios::beg);
		fimArquivo = 0;
	}
	else{
		throw FileNotFoundException();
	}
}

void FileReader::fecharArquivo(){
	if (file.is_open()){
		file.close();
	}
	else cout << "Não foi possível fechar o arquivo"<<endl;
}

char FileReader::getChar(){
	if(!file.eof()){							
		file.read(buffer, 1);					
		if(!file.eof()){						
			if(buffer[0] == 10) linenumber++;	//incrementa numero de linhas se o caracter lido for quebra de linha 
			return buffer[0];					
		}
		else{									
			fimArquivo = 1;
			return EOF; //retorna caracter de fim de arquivo
		}
	}
	else{
		fimArquivo = 1; //caracter de fim de arquivo 
	}
}

void FileReader::returnChar(){
	if(!file.eof()){
		int position = file.tellg();
		file.seekg (-1, ios::cur);
	}
}

