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

/* Classe para leitura do arquivo fonte */

#include "../include/FileWriter.h"
#include "../include/GLCompilerException.h"
#include <map>
#include <unordered_map>
#include <sys/stat.h>

using namespace GCX;
using namespace std;

namespace GCX{
	short CS = 0;
	short DS = 0;
	short currentInstruction = 0;
	bool OPT = 0;
	bool files = 0;
}

void FileWriter::abrirArquivo(char *_arquivo){
	inputFile = _arquivo;
	asmFile = inputFile.substr(0,inputFile.find(".gl")) + ".asm";
	exeFile = inputFile.substr(0,inputFile.find(".gl")) + ".exe";

	assembly.open(asmFile, ios::out|ios::binary|ios::ate);
	binary.open(exeFile,std::ofstream::binary);
}

void FileWriter::fecharArquivo(){
	binary.close();
	assembly.close();
}

void FileWriter::deletarArquivo(char * _arquivo){
	std::string inputFile = _arquivo;
	std::string asmFile = inputFile.substr(0,inputFile.find(".gl")) + ".asm";
	std::string exeFile = inputFile.substr(0,inputFile.find(".gl")) + ".exe";
	//Verifica se os arquivos existem e os deleta
	struct stat buf;
	if (stat(exeFile.c_str(), &buf) != -1){
		binary.close();
		remove(exeFile.c_str());
	}
	if (stat(asmFile.c_str(), &buf) != -1){
		assembly.close();
		remove(asmFile.c_str());
	}
}

void FileWriter::salvar(){
	for(short i = 0; i<currentInstruction; i++){
		short size = sizeOf((GCX::Instruction)instructions[i][0]);
		for(short j = 0; j<size;j++){
			binary.write((char*) &instructions[i][j], sizeof(short int));
		}
	}
}

void FileWriter::salvarPeephole(){
	short optCS = 0;
	short insCS = 0;
	int optIns = 0;
	unordered_map<int, int> labels;
	unordered_map<int, int> branches;
	unordered_map<int, int>::iterator it;
	for(int i = 0;i<currentInstruction;i++){
		short size = sizeOf((GCX::Instruction)instructions[i][0]);
		if(!(((instructions[i][0] == STO && instructions[i+1][0] == LOD) || (instructions[i][0] == STOF && instructions[i+1][0] == LODF)) && instructions[i][1] == instructions[i+1][1] && instructions[i][2] == instructions[i+1][2] && !isLabelInstruction(i+1))){
			if(isLabelInstruction(i)){
				labels.insert(std::pair<int,int>(insCS, optCS));
				it = branches.find(insCS);
				if (it != branches.end()){
					int pos = branches.at(insCS);
					if (optInstructions[pos][0] == BZR){
						optInstructions[pos][2] = optCS;
					}
					else if (optInstructions[pos][0] == JMP){
						optInstructions[pos][1] = optCS;
					}
				}
			}
			else if(isOptRetroIns(i)){
				if(instructions[i][0] == BZR){
					branches.insert(std::pair<int, int>((int)instructions[i][2],optIns));
				}
				else if(instructions[i][0] == JMP && instructions[i][1] > insCS){
					branches.insert(std::pair<int, int>((int)instructions[i][1],optIns));
				}	
			}
			for(short j = 0; j<size;j++){
				if (instructions[i][0] == JMP && instructions[i][1] < insCS && j == 1){
					optInstructions[optIns][j] = labels.at((int) instructions[i][1]);
				}
				else{
					optInstructions[optIns][j] = instructions[i][j];
				}
			}
			optIns++;
			optCS+=size;
			insCS +=size;
		}
		else{
			i++;
			insCS += 6;
		}
	}
	for(short i = 0; i<optIns; i++){
		short size = sizeOf((GCX::Instruction)optInstructions[i][0]);
		for(short j = 0; j<size;j++){
			binary.write((char*) &optInstructions[i][j], sizeof(short int));
		}
	}
}

void FileWriter::addInstruction(short _ins, short _op1, short _op2, short _op3){
	instructions[currentInstruction][0] = _ins;
	instructions[currentInstruction][1] = _op1;
	instructions[currentInstruction][2] = _op2;
	instructions[currentInstruction][3] = _op3;
	currentInstruction++;
	CS+=4;
}

void FileWriter::addInstruction(short _ins, short _op1, short _op2){
	instructions[currentInstruction][0] = _ins;
	instructions[currentInstruction][1] = _op1;
	instructions[currentInstruction][2] = _op2;
	currentInstruction++;
	CS+=3;
}

void FileWriter::addInstruction(short _ins, short _op1){
	instructions[currentInstruction][0] = _ins;
	instructions[currentInstruction][1] = _op1;
	currentInstruction++;
	CS+=2;
}

void FileWriter::addInstruction(short _ins){
	instructions[currentInstruction][0] = _ins;
	currentInstruction++;
	CS++;
}

void FileWriter::retroCorrigir(short _instructionNumber, short _desl){
	if (instructions[_instructionNumber][0] == JMP){
		instructions[_instructionNumber][1] = _desl;
	}
	else{
		instructions[_instructionNumber][2] = _desl;
	}
}

void FileWriter::retroCorrigirPeephole(short _instructionNumber, short _desl){
	if (optInstructions[_instructionNumber][0] == JMP){
		optInstructions[_instructionNumber][1] = _desl;
	}
	else{
		optInstructions[_instructionNumber][2] = _desl;
	}
}

/* ADD, MVE, MUL, SUB */
void FileWriter::write(Instruction _ins, IntegerRegister _regA, IntegerRegister _regB){
	if (_ins == ADD || _ins == MVE || _ins == MUL || _ins == SUB){
		addInstruction(_ins, _regA, _regB);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_regA) + ", " + nameOf(_regB)<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + nameOf(_regA) + ", " + nameOf(_regB));
}

/* ADDF, DIV, MVEF, MULF, SUBF */
void FileWriter::write(Instruction _ins, RealRegister _regA, RealRegister _regB){
	if (_ins == ADDF || _ins == DIV || _ins == MVEF || _ins == MULF || _ins == SUBF){
		addInstruction(_ins, _regA, _regB);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_regA) + ", " + nameOf(_regB)<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + nameOf(_regA) + ", " + nameOf(_regB));
}

/*ADI, LDI, LOD, STO */
void FileWriter::write(Instruction _ins, IntegerRegister _regA, short _val){
	if (_ins == ADI || _ins == LDI){
		addInstruction(_ins, _regA, _val);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_regA) + ", #" + to_string (_val) <<endl;
	}
	else if (_ins == LOD || _ins == STO){
		addInstruction(_ins, _regA, _val);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_regA) + ", " + to_string (_val) + "(DS)"<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + nameOf(_regA) + ", #" + to_string (_val));
}

/* ADIF, LDIF */
void FileWriter::write(Instruction _ins, RealRegister _regA, short _intVal, short _fracVal){
	if (_ins == ADIF || _ins == LDIF){
		addInstruction(_ins, _regA, _intVal, _fracVal);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_regA) + ", #" + to_string(((double)_intVal + (double)_fracVal/10000))<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + nameOf(_regA) + ", #" + to_string(((double)_intVal + (double)_fracVal/10000)));
}

/*BNG, BNN, BNP, BNZ, BPS, BZR */
void FileWriter::write(Instruction _ins, IntegerRegister _regA, Label _label){
	if (_ins == BNG || _ins == BNN || _ins == BNP || _ins == BNZ || _ins == BPS || _ins == BZR){
		addInstruction(_ins, _regA, _label.address);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_regA) + ", " + _label.name<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + nameOf(_regA) + ", " + _label.name);
}

/*LODF, STOF */
void FileWriter::write(Instruction _ins, RealRegister _regA, short _desl){
	if (_ins == LODF || _ins == STOF){
		addInstruction(_ins, _regA, _desl);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_regA) + ", " + to_string (_desl) + "(DS)"<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + nameOf(_regA) + ", " + to_string (_desl) + "(DS)");
}

/*BNGF, BNNF, BNPF, BNZF, BPSF, BZRF */
void FileWriter::write(Instruction _ins, RealRegister _regA, Label _label){
	if (_ins == BNGF || _ins == BNNF || _ins == BNPF || _ins == BNZF || _ins == BPSF || _ins == BZRF){
		addInstruction(_ins, _regA, _label.address);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_regA) + ", " + _label.name<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + nameOf(_regA) + ", " + _label.name);
}

/* STI */
void FileWriter::write(Instruction _ins, short _intVal, short _desl){
	if (_ins == STI){
		addInstruction(_ins, _intVal, _desl);
		assembly<<"\t"+nameOf(_ins) + " #" + to_string(_intVal) + ", " + to_string(_desl) + " (DS)"<<endl;
		DS++;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " #" + to_string(_intVal) + ", " + to_string(_desl) + " (DS)");
}

/* STIF */
void FileWriter::write(Instruction _ins, short _intVal, short _fracVal, short _desl){
	if (_ins == STIF){
		addInstruction(_ins, _intVal, _fracVal, _desl);
		assembly<<"\t"+nameOf(_ins) + " #" + to_string(((double)_intVal + (double)_fracVal/10000)) + ", " + to_string(_desl) + " (DS)"<<endl;
		DS += 2;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " #" + to_string(((double)_intVal + (double)_fracVal/10000)) + ", " + to_string(_desl) + " (DS)");
}

/* CNV */
void FileWriter::write(Instruction _ins, RealRegister _regA, IntegerRegister _regB){
	if (_ins == CNV){
		addInstruction(_ins, _regA, _regB);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_regA) + ", " + nameOf(_regB)<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + nameOf(_regA) + ", " + nameOf(_regB));
}

/* ESC */
void FileWriter::write(Instruction _ins, IntegerRegister _regA, RealRegister _regB){
	if (_ins == ESC){
		addInstruction(_ins, _regA, _regB);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_regA) + ", " + nameOf(_regB)<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + nameOf(_regA) + ", " + nameOf(_regB));
}

/* LGT, NEG */
void FileWriter::write(Instruction _ins, IntegerRegister _reg){
	if (_ins == LGT || _ins == NEG){
		addInstruction(_ins, _reg);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_reg)<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + nameOf(_reg));
}

/* NEGF, TME */
void FileWriter::write(Instruction _ins, RealRegister _reg){
	if (_ins == NEGF || _ins == TME){
		addInstruction(_ins, _reg);
		assembly<<"\t"+nameOf(_ins) + " " + nameOf(_reg)<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + nameOf(_reg));
}

/* JMP */
void FileWriter::write(Instruction _ins, Label _label){
	if (_ins == JMP){
		addInstruction(_ins, _label.address);
		assembly<<"\t"+nameOf(_ins) + " " + _label.name<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins) + " " + _label.name);
}

/* HLT, RTR */
void FileWriter::write(Instruction _ins){
	if (_ins == HLT || _ins == RTR){
		addInstruction(_ins);
		assembly<<"\t"+nameOf(_ins)<<endl;
	}
	else throw InvalidInstructionException(nameOf(_ins));
}

void FileWriter::writeLabel(Label _label){
	assembly<<_label.name<<":"<<endl;
	labelInstructions.push_back(currentInstruction);
}

void FileWriter::writeComment(string _str){
	assembly<<_str<<endl;
}

int FileWriter::sizeOf(Instruction _ins){
	if(_ins == ADIF || _ins == LDIF || _ins == STIF) return 4;
	else if (_ins == JMP || _ins == LGT || _ins == NEG || _ins == NEGF || _ins == TME) return 2;
	else if (_ins == HLT || _ins == RTR) return 1;
	else return 3;
}

string FileWriter::nameOf(Instruction _ins){
	switch (_ins){
		case ADD: return "ADD";
		case ADDF: return "ADDF";
		case ADI: return "ADI";
		case ADIF: return "ADIF";
		case BNG: return "BNG";
		case BNGF: return "BNGF";
		case BNN: return "BNN";
		case BNNF: return "BNNF";
		case BNP: return "BNP";
		case BNPF: return "BNPF";
		case BNZ: return "BNZ";
		case BNZF: return "BNZF";
		case BPS: return "BPS";
		case BPSF: return "BPSF";
		case BZR: return "BZR";
		case BZRF: return "BZRF";
		case CNV: return "CNV";
		case DIV: return "DIV";
		case ESC: return "ESC";
		case HLT: return "HLT";
		case JMP: return "JMP";
		case LDI: return "LDI";
		case LDIF: return "LDIF";
		case LGT: return "LGT";
		case LOD: return "LOD";
		case LODF: return "LODF";
		case MVE: return "MVE";
		case MVEF: return "MVEF";
		case MUL: return "MUL";
		case MULF: return "MULF";
		case NEG: return "NEG";
		case NEGF: return "NEGF";
		case RTR: return "RTR";
		case STI: return "STI";
		case STIF: return "STIF";
		case STO: return "STO";
		case STOF: return "STOF";
		case SUB: return "SUB";
		case SUBF: return "SUBF";
		case TME: return "TME";
		default: throw InstructionException((short) _ins);
	}
}

string FileWriter::nameOf(IntegerRegister _reg){
	switch (_reg){
		case A: return "A";
		case B: return "B";
		case C: return "C";
		case D: return "D";
		case E: return "E";
		case F: return "F";
		default: throw RegisterException((short) _reg);
	}
}

string FileWriter::nameOf(RealRegister _reg){
	switch (_reg){
		case AX: return "AX";
		case BX: return "BX";
		case CX: return "CX";
		case DX: return "DX";
		case EX: return "EX";
		case FX: return "FX";
		default: throw RegisterException((short) _reg);
	}
}

bool FileWriter::isBranch(short _ins){
	if (_ins == BNG || _ins == BNN || _ins == BNP || _ins == BNZ || _ins == BPS || _ins == BZR ||
		_ins == BNGF || _ins == BNNF || _ins == BNPF || _ins == BNZF || _ins == BPSF || _ins == BZRF){
			return true;
	}
	else return false;
}

bool FileWriter::isLabelInstruction(short _instructionNumber){
	for(std::list<short>::iterator it = labelInstructions.begin(); it!=labelInstructions.end();it++){
		if(*it == _instructionNumber) return true;
	}
	return false;
}

void FileWriter::addOptRetroIns(short _instructionNumber){
	optRetroInstructions.push_back(_instructionNumber);
}

bool FileWriter::isOptRetroIns(short _instructionNumber){
	for(std::list<short>::iterator it = optRetroInstructions.begin(); it!=optRetroInstructions.end();it++){
		if(*it == _instructionNumber) return true;
	}
	return false;
}