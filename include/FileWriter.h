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

#ifndef	FILEWRITER_H
#define FILEWRITER_H

#include <string>
#include <stdio.h>
#include <fstream>
#include <list>

namespace GCX{
	#define MEM_SIZE 32768

	extern short CS;
	extern short DS;
	extern short currentInstruction;
	extern bool OPT;
	extern bool files;

	/* Instrucoes GCX */
	typedef enum:short{
		ADD=1, ADDF, ADI, ADIF,
		BNG, BNGF, BNN, BNNF, BNP, BNPF,BNZ, BNZF,BPS, BPSF, BZR, BZRF,
		CNV, DIV, ESC, HLT, JMP, LDI, LDIF, LGT, LOD, LODF,MVE, MVEF, 
		MUL, MULF, NEG, NEGF, RTR, STI, STIF, STO, STOF, SUB, SUBF, TME
	} Instruction;

	/* Registradores para Inteiros*/
	typedef enum:short{
		A=1, B, C, D, E, F
	} IntegerRegister;

	/* Registradores para Reais*/
	typedef enum:short{
		AX=1, BX, CX, DX, EX, FX
	} RealRegister;

	typedef struct{
		std::string name;
		short address;
	} Label;
}

/* Classe para escrita do arquivo assembly e binário */
class FileWriter{

	private:
	std::ofstream assembly;		/* Objeto para a escrita do arquivo assembly*/
	std::ofstream binary;		/* Objeto para a escrita do arquivo binario*/
	short instructions [MEM_SIZE][4];
	short optInstructions [MEM_SIZE][4];
	std::list<short> labelInstructions;
	std::list<short> optRetroInstructions;
	std::string inputFile;
	std::string asmFile;
	std::string exeFile;

	public:
	void abrirArquivo (char *);
	void fecharArquivo();
	void deletarArquivo(char *);
	void addInstruction(short, short, short, short);
	void addInstruction(short, short, short);
	void addInstruction(short, short);
	void addInstruction(short);
	void write (GCX::Instruction, GCX::IntegerRegister, GCX::IntegerRegister); //Ex: ADD
	void write (GCX::Instruction, GCX::IntegerRegister, GCX::RealRegister); //Ex: ESC
	void write (GCX::Instruction, GCX::RealRegister, GCX::RealRegister); //Ex: ADDF
	void write (GCX::Instruction, GCX::RealRegister, GCX::IntegerRegister); //Ex: CNV
	void write (GCX::Instruction, GCX::IntegerRegister, short); //Ex: ADI
	void write (GCX::Instruction, GCX::IntegerRegister, GCX::Label); //Ex: BNG
	void write (GCX::Instruction, GCX::RealRegister, short, short); //Ex: ADIF
	void write (GCX::Instruction, GCX::RealRegister, short); //Ex LODF
	void write (GCX::Instruction, GCX::RealRegister, GCX::Label); //Ex BNGF
	void write (GCX::Instruction, short, short,short); //Ex: STIF
	void write (GCX::Instruction, short, short); //Ex: STI
	void write (GCX::Instruction, GCX::Label); //Ex: JMP
	void write (GCX::Instruction, GCX::IntegerRegister); //Ex: LGT
	void write (GCX::Instruction, GCX::RealRegister); //Ex:NEGF
	void write (GCX::Instruction); //Ex: HLT, RTR
	void writeLabel (GCX::Label);
	void writeComment(std::string);
	void retroCorrigir(short, short);
	void retroCorrigirPeephole(short, short);
	int sizeOf(GCX::Instruction);
	std::string nameOf(GCX::Instruction);
	std::string nameOf(GCX::IntegerRegister);
	std::string nameOf(GCX::RealRegister);
	void salvar();
	void salvarPeephole();
	bool isBranch(short);
	bool isLabelInstruction(short);
	void addOptRetroIns(short);
	bool isOptRetroIns(short);
};

#endif