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

/* Classe que realiza a analise sintática */

#include "GLCompilerException.h"
#include "Parser.h"
#include "FileWriter.h"

Parser::Parser(char * _arquivo, bool _opt){
	lex = new Lex(_arquivo);
	escritor.abrirArquivo(_arquivo);
	currentLabel = 1;
	opt = _opt;
}

void Parser::run(){
	currentToken = lex->getToken();
	this->start();
	if(!lex->fimDeArquivo()){
		throw ExpectedEndOfFileException();
	}

	if (opt == 0){
		escritor.salvar();
	}
	else {
		escritor.salvarPeephole();
	}
	escritor.fecharArquivo();
}

void Parser::matchToken(Token _token){
	if(currentToken.token == _token){
		//armazena o token corrente
		previousToken = currentToken;

		//cout<<currentToken.lexema<<endl; //Imprime token corrente (teste)
		currentToken = lex->getToken(); //recebe próximo token
	}
	else if (currentToken.token == ENDFILE){
		throw EndOfFileException();
	}
	else{
		throw UnexpectedTokenException();
	}
}

void Parser::start(){
	matchToken(DECLARE);
	while(currentToken.token == POINT || currentToken.token == LIGHT ||	currentToken.token == COLOR ||
		currentToken.token == FACE || currentToken.token == OBJECT || currentToken.token == VAR || currentToken.token == CONST){
			declaration();
	}
	escritor.writeComment("************************** Corpo Programa ******");
	baseEndTemp = GCX::DS;
	commandBlock();
	matchToken(ENDFILE);
	escritor.write(GCX::Instruction::HLT);
}

void Parser::declaration(){
	Classe classe;		//Variaveis do ET
	Tipo tipo;			//Variaveis do ET
	string constLexema; //Variaveis do ET
	short sinal;		//Variaveis do ET

	switch (currentToken.token){
	case POINT:
		classe = CLASSE_PONTO; //Regra 1 do ET
		matchToken(POINT);
		do{
			matchToken (ID);
			unicityAlreadyDeclared(classe);			// Regra 10 do ET. Verifica se o identificador ja foi declarado
			setEndereco(previousToken.lexema);		// Regra 60 do ET.
			matchToken (ASSIGN);
			matchToken (LEFT_PARENS);
			for(int i=1;i<=3;i++){
				sinal = 1;							//Regra 61 do ET
				if(currentToken.token == SUM){
					matchToken(SUM);
				}
				else if (currentToken.token == SUB){
					matchToken(SUB);
					sinal = -1;						//Regra 62 do ET.
				}
				matchToken (CONSTANT);
				escritor.write(GCX::Instruction::STIF, sinal * previousToken.integerValue, sinal * previousToken.fracValue, GCX::DS); // Regra 63 do ET.
				if(i<=2) matchToken (COMMA);
				else matchToken (RIGHT_PARENS); 
			}
			matchToken (SEMICOLON);
		} while (currentToken.token == ID);
		break;
	case LIGHT:
		classe = CLASSE_LUZ; //Regra 2 do ET
		matchToken(LIGHT);
		do{
			matchToken (ID);
			unicityAlreadyDeclared(classe);		// Regra 10 do ET. Verifica se o identificador ja foi declarado
			setEndereco(previousToken.lexema);
			matchToken (ASSIGN);
			matchToken (LEFT_PARENS);
			for(int i = 1;i<=3;i++){
				sinal = 1;						//Regra 61 do ET.
				if(currentToken.token == SUM){
					matchToken(SUM);
				}
				else if (currentToken.token == SUB){
					matchToken(SUB);
					sinal = -1;					//Regra 62 do ET.
				}
				matchToken (CONSTANT);
				escritor.write(GCX::Instruction::STIF, sinal * previousToken.integerValue, sinal * previousToken.fracValue, GCX::DS); //Regra 63 do ET.
				if (i<=2) matchToken (COMMA);
				else matchToken (RIGHT_PARENS);
			}
			matchToken (SEMICOLON);
		} while (currentToken.token == ID);
		break;
	case COLOR:
		classe = CLASSE_COR; //Regra 3 do ET
		matchToken(COLOR);
		do{
			matchToken (ID);
			unicityAlreadyDeclared(classe);						// Regra 10 do ET. Verifica se o identificador ja foi declarado
			setEndereco(previousToken.lexema);					//Regra 60 do ET
			matchToken (ASSIGN);
			for(int i=1;i<=7;i++){
				matchToken (CONSTANT);
				constTypeVerify(previousToken.tipo, TIPO_INTEIRO);		// Regra 18 do ET. Verifica se o tipo do identificador é inteiro
				if (i>=2){
					componentColorVerify(previousToken.integerValue);	// Regra 19 do ET. Verifica se o valor do identificador está entre 0 e 63
				}
				escritor.write(GCX::Instruction::STI, previousToken.integerValue, GCX::DS);	//Regra 64 do ET
				if(i<=6) matchToken (COMMA);
				else matchToken (SEMICOLON);
			}
		} while (currentToken.token == ID);
		break;
	case FACE:
		classe = CLASSE_FACE; //Regra 4 do ET
		matchToken(FACE);
		do{
			short endCor;
			list<short> endPonto;
			matchToken(ID);
			unicityAlreadyDeclared(classe);		// Regra 10 do ET. Verifica se o identificador ja foi declarado
			setEndereco(previousToken.lexema);	//Regra 60 do ET
			matchToken (ASSIGN);
			matchToken (ID);
			unicityNotDeclared();			// Regra 53 do ET. Verifica se o identificador nao foi declarado
			classVerify(CLASSE_COR);		// Regra 11 do ET.
			endCor = symbolsTable->getEndereco(previousToken.lexema);	//Regra 65 do ET.
			for(int i=0;i<3;i++){
				matchToken (COMMA);
				matchToken (ID);
				unicityNotDeclared();			// Regra 53 do ET. Verifica se o identificador nao foi declarado
				classVerify(CLASSE_PONTO);		// Regra 12 do ET.
				endPonto.push_back(symbolsTable->getEndereco(previousToken.lexema));	//Regra 66 do ET.
			}
			while (currentToken.token == COMMA){
				matchToken(COMMA);
				matchToken(ID);
				unicityNotDeclared();			// Regra 53 do ET. Verifica se o identificador nao foi declarado
				classVerify(CLASSE_PONTO);		// Regra 15 do ET.
				endPonto.push_back(symbolsTable->getEndereco(previousToken.lexema));	//Regra 66 do ET.
			}
			matchToken(SEMICOLON);
			escritor.write(GCX::Instruction::STI, endPonto.size(), GCX::DS);			//Regra 67 do ET.
			escritor.write(GCX::Instruction::STI, endCor, GCX::DS);						//Regra 67 do ET.
			for(std::list<short>::iterator it = endPonto.begin(); it != endPonto.end(); ++it){
				escritor.write(GCX::Instruction::STI, *it, GCX::DS);					//Regra 67 do ET.
			}
		}while(currentToken.token == ID);
		break;
	case OBJECT:
		classe = CLASSE_OBJETO; //Regra 5 do ET
		matchToken(OBJECT);
		do{
			list<short> endFace;
			matchToken(ID);
			unicityAlreadyDeclared(classe);		// Regra 10 do ET. Verifica se o identificador ja foi declarado
			setEndereco(previousToken.lexema);	//Regra 60 do ET
			matchToken (ASSIGN);
			matchToken (ID);
			unicityNotDeclared();			// Regra 53 do ET. Verifica se o identificador nao foi declarado
			classVerify(CLASSE_FACE);		// Regra 16 do ET.
			endFace.push_back(symbolsTable->getEndereco(previousToken.lexema));
			while (currentToken.token == COMMA){
				matchToken(COMMA);
				matchToken(ID);
				unicityNotDeclared();		// Regra 53 do ET. Verifica se o identificador nao foi declarado
				classVerify(CLASSE_FACE);	// Regra 17 do ET.
				endFace.push_back(symbolsTable->getEndereco(previousToken.lexema));		//Regra 68 do ET.
			}
			matchToken(SEMICOLON);
			escritor.write(GCX::Instruction::STI, endFace.size(), GCX::DS);				//Regra 69 do ET.
			escritor.write(GCX::Instruction::STIF, 1, 0, GCX::DS);						//Regra 69 do ET.
			for(std::list<short>::iterator it = endFace.begin(); it != endFace.end(); ++it){
				escritor.write(GCX::Instruction::STI, *it, GCX::DS);					//Regra 69 do ET.
			}
		}while(currentToken.token == ID);
		break;
	case VAR:
		classe = CLASSE_VAR; //Regra 6 do ET
		matchToken(VAR);
		do{
			if(currentToken.token == INTEGER){
				tipo = TIPO_INTEIRO;	//Regra 8 do ET
				matchToken(INTEGER);
			}
			else{
				tipo = TIPO_REAL;		//Regra 9 do ET
				matchToken(REAL);
			}
			matchToken(ID);
			unicityAlreadyDeclared(classe);			// Regra 10 do ET. Verifica se o identificador ja foi declarado
			setType(tipo);							// Regra 25 do ET. Verifica o tipo do identificador
			setEndereco(previousToken.lexema);		// Regra 60 do ET
			if (tipo == TIPO_INTEIRO) GCX::DS++;
			else GCX::DS+=2;
			while (currentToken.token == COMMA){
				matchToken(COMMA);
				matchToken(ID);
				unicityAlreadyDeclared(classe);		// Regra 10 do ET. Verifica se o identificador ja foi declarado
				setType(tipo);						// Regra 26 do ET. Verifica o tipo do identificador
				setEndereco(previousToken.lexema);	// Regra 60 do ET
				if (tipo == TIPO_INTEIRO) GCX::DS++;
				else GCX::DS+=2;
			}
			matchToken(SEMICOLON);
		}while(currentToken.token == INTEGER || currentToken.token == REAL);
		break;
	case CONST:
		classe = CLASSE_CONST; //Regra 7 do ET
		matchToken(CONST);
		do{
			matchToken(ID);
			constLexema = previousToken.lexema;
			unicityAlreadyDeclared(classe);							// Regra 10 do ET. Verifica se o identificador ja foi declarado
			setEndereco(previousToken.lexema);						//Regra 60 do ET.
			matchToken(ASSIGN);
			sinal = 1;												//Regra 61 do ET.
			if (currentToken.token == SUM){
				matchToken(SUM);
			}
			else if (currentToken.token == SUB){
				matchToken(SUB);
				sinal = -1;											//Regra 62 do ET.
			}
			matchToken(CONSTANT);
			setType(constLexema, previousToken.tipo);				// Regra 27 do ET. Seta o tipo do identificador
			if (previousToken.tipo == TIPO_INTEIRO){
				escritor.write(GCX::Instruction::STI, sinal*previousToken.integerValue, GCX::DS); //Regra 70 do ET
			}
			else{
				escritor.write(GCX::Instruction::STIF, sinal*previousToken.integerValue, sinal*previousToken.fracValue, GCX::DS);	//Regra 70 do ET
			}matchToken(SEMICOLON);
		}while(currentToken.token == ID);
		break;
	}//fim switch
} // fim declaration

void Parser::commandBlock(){
	matchToken(BEGIN);
	while(currentToken.token == ID ||
		currentToken.token == WHILE ||
		currentToken.token == IF ||
		currentToken.token == SCALE ||
		currentToken.token == PAUSE ||
		currentToken.token == LIGHT ||
		currentToken.token == ROTTRANS){
			command();
	}
	matchToken(END);
}

void Parser::command(){
	Tipo expTipo;
	string idLexema;
	short endereco;
	GCX::Label rotInicio, rotFim, rotFalso;
	short instructionNumber, instructionNumberRotFalso, instructionNumberRotFim;
	switch (currentToken.token){
	case ID:
		matchToken(ID);
		escritor.writeComment("************************** Atribuicao ******");
		idLexema = previousToken.lexema;
		unicityNotDeclared();			// Regra 53 do ET. Verifica se o identificador nao foi declarado
		classVerify(CLASSE_VAR);		// Regra 11 do ET.
		matchToken(ASSIGN);
		exp(expTipo,endereco);
		if(expTipo == TIPO_LOGICO) throw TypeException();	//Regra 29 do ET
		else if (symbolsTable->getTipo(idLexema) == TIPO_INTEIRO && expTipo == TIPO_REAL) throw TypeException();
		matchToken(SEMICOLON);
		if(expTipo == TIPO_INTEIRO){
			escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::A, endereco);
		}
		else {
			escritor.write(GCX::Instruction::LODF, GCX::RealRegister::AX, endereco);
		}
		if (symbolsTable->getTipo(idLexema) == TIPO_REAL){
			if (expTipo == TIPO_INTEIRO){
				escritor.write(GCX::Instruction::CNV, GCX::RealRegister::AX, GCX::IntegerRegister::A);
			}
			escritor.write(GCX::Instruction::STOF, GCX::RealRegister::AX, symbolsTable->getEndereco(idLexema));
		}
		else{
			escritor.write(GCX::Instruction::STO, GCX::IntegerRegister::A, symbolsTable->getEndereco(idLexema));
		}
		break;
	case WHILE:
		matchToken(WHILE);
		escritor.writeComment("************************** Enquanto ******");
		rotInicio = novoRot();
		rotInicio.address = GCX::CS;
		escritor.writeLabel(rotInicio);
		exp(expTipo, endereco);
		rotFim = novoRot();
		if(expTipo != TIPO_LOGICO){
			throw TypeException();	//Regra 30 do ET
		}
		else{
			escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::A, endereco);
			instructionNumber = GCX::currentInstruction;
			escritor.addOptRetroIns(GCX::currentInstruction);
			escritor.write(GCX::Instruction::BZR, GCX::IntegerRegister::A, rotFim);
		}
		if (currentToken.token == BEGIN) commandBlock();
		else command();

		escritor.write(GCX::Instruction::JMP, rotInicio);
		escritor.writeLabel(rotFim);
		escritor.retroCorrigir(instructionNumber, GCX::CS);
		escritor.addOptRetroIns(GCX::currentInstruction);
		break;
	case IF:
		matchToken(IF);
		exp(expTipo, endereco);
		if(expTipo != TIPO_LOGICO) throw TypeException();	//Regra 30 do ET
		escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::A, endereco);
		rotFalso = novoRot();
		instructionNumberRotFalso = GCX::currentInstruction;
		escritor.addOptRetroIns(GCX::currentInstruction);
		escritor.write(GCX::Instruction::BZR, GCX::IntegerRegister::A, rotFalso);
		matchToken(THEN);
		if (currentToken.token == BEGIN)
			commandBlock();
		else command();
		if(currentToken.token == ELSE){
			matchToken(ELSE);

			instructionNumberRotFim = GCX::currentInstruction;
			escritor.addOptRetroIns(GCX::currentInstruction);
			rotFim = novoRot();
			escritor.write(GCX::Instruction::JMP, rotFim);
            escritor.writeLabel(rotFalso);
			escritor.retroCorrigir(instructionNumberRotFalso, GCX::CS);

			if (currentToken.token == BEGIN)
				commandBlock();
			else 
				command();

			escritor.writeLabel(rotFim);
			escritor.retroCorrigir(instructionNumberRotFim, GCX::CS);
		}
		else{
			escritor.writeLabel(rotFalso);
			escritor.retroCorrigir(instructionNumberRotFalso, GCX::CS);
		}
		break;
	case SCALE:
		matchToken(SCALE);
		matchToken(ID);
		unicityNotDeclared();		// Regra 53 do ET. Verifica se o identificador nao foi declarado
		classVerify(CLASSE_OBJETO);	// Regra 31 do ET.
		idLexema = previousToken.lexema; //salvar o lexema do objeto
		matchToken(COMMA);
		exp(expTipo, endereco);
		if (expTipo != TIPO_INTEIRO && expTipo != TIPO_REAL) throw TypeException();	//Regra 32 do ET.
		if (expTipo == TIPO_REAL){
			escritor.write (GCX::Instruction::LODF, GCX::RealRegister::AX, endereco);
		}
		else{
			escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::B, endereco);
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::AX, GCX::IntegerRegister::B);
		}
		escritor.write(GCX::Instruction::LDI, GCX::IntegerRegister::A, symbolsTable->getEndereco(idLexema));
		escritor.write(GCX::Instruction::ESC, GCX::IntegerRegister::A, GCX::RealRegister::AX);
		matchToken(SEMICOLON);
		break;
	case PAUSE:
		matchToken(PAUSE);
		exp(expTipo, endereco);
		if (expTipo == TIPO_LOGICO) throw TypeException();	//Regra 32 do ET.
		if (expTipo == TIPO_REAL){
			escritor.write (GCX::Instruction::LODF, GCX::RealRegister::AX, endereco);
		}
		else{
			escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::B, endereco);
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::AX, GCX::IntegerRegister::B);
		}
		escritor.write(GCX::Instruction::TME, GCX::RealRegister::AX);
		matchToken(SEMICOLON);
		break;
	case LIGHT:
		matchToken(LIGHT);
		matchToken(ID);
		unicityNotDeclared();		// Regra 53 do ET. Verifica se o identificador nao foi declarado
		classVerify(CLASSE_LUZ);	// Regra 33 do ET.
		escritor.write(GCX::Instruction::LDI, GCX::IntegerRegister::A, symbolsTable->getEndereco(previousToken.lexema));
		escritor.write(GCX::Instruction::LGT, GCX::IntegerRegister::A);
		matchToken(SEMICOLON);
		break;
	case ROTTRANS:
		Tipo exp1Tipo, exp2Tipo, exp3Tipo, exp4Tipo, exp5Tipo, exp6Tipo;
		short endereco1, endereco2, endereco3, endereco4, endereco5, endereco6;
		matchToken(ROTTRANS);
		matchToken(ID);
		unicityNotDeclared();		// Regra 53 do ET. Verifica se o identificador nao foi declarado
		classVerify(CLASSE_OBJETO);	// Regra 31 do ET.
		idLexema = previousToken.lexema;
		matchToken(COMMA);
		exp(exp1Tipo, endereco1);
		if (exp1Tipo == TIPO_LOGICO) throw TypeException();		//Regra 34 do ET.
		if(exp1Tipo == TIPO_REAL){
			escritor.write(GCX::Instruction::LODF, GCX::RealRegister::AX, endereco1);
		}
		else{
			escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::B, endereco1);
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::AX, GCX::IntegerRegister::B);
		}
		matchToken(COMMA);
		exp(exp2Tipo, endereco2);
		if (exp2Tipo == TIPO_LOGICO) throw TypeException();		//Regra 35 do ET.
		if(exp2Tipo == TIPO_REAL){
			escritor.write(GCX::Instruction::LODF, GCX::RealRegister::BX, endereco2);
		}
		else{
			escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::B, endereco2);
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::BX, GCX::IntegerRegister::B);
		}
		matchToken(COMMA);
		exp(exp3Tipo, endereco3);
		if (exp3Tipo == TIPO_LOGICO) throw TypeException();		//Regra 36 do ET.
		if(exp3Tipo == TIPO_REAL){
			escritor.write(GCX::Instruction::LODF, GCX::RealRegister::CX, endereco3);
		}
		else{
			escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::B, endereco3);
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::CX, GCX::IntegerRegister::B);
		}
		matchToken(COMMA);
		exp(exp4Tipo, endereco4);
		if (exp4Tipo == TIPO_LOGICO) throw TypeException();		//Regra 37 do ET.
		if(exp4Tipo == TIPO_REAL){
			escritor.write(GCX::Instruction::LODF, GCX::RealRegister::DX, endereco4);
		}
		else{
			escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::B, endereco4);
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::DX, GCX::IntegerRegister::B);
		}
		matchToken(COMMA);
		exp(exp5Tipo, endereco5);
		if (exp5Tipo == TIPO_LOGICO) throw TypeException();		//Regra 38 do ET.
		if(exp5Tipo == TIPO_REAL){
			escritor.write(GCX::Instruction::LODF, GCX::RealRegister::EX, endereco5);
		}
		else{
			escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::B, endereco5);
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::EX, GCX::IntegerRegister::B);
		}
		matchToken(COMMA);
		exp(exp6Tipo, endereco6);
		if (exp6Tipo == TIPO_LOGICO) throw TypeException();		//Regra 39 do ET.
		if(exp6Tipo == TIPO_REAL){
			escritor.write(GCX::Instruction::LODF, GCX::RealRegister::FX, endereco6);
		}
		else{
			escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::B, endereco6);
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::FX, GCX::IntegerRegister::B);
		}
		matchToken(SEMICOLON);
		escritor.write(GCX::Instruction::LDI, GCX::IntegerRegister::A, symbolsTable->getEndereco(idLexema));
		escritor.write(GCX::Instruction::RTR);
		break;
	}//end switch
}//end command

void Parser::logicComparation(){
	switch (currentToken.token){
	case EQ:
		matchToken(EQ);
		break;
	case NEQ:
		matchToken(NEQ);
		break;
	case LT:
		matchToken(LT);
		break;
	case GT:
		matchToken(GT);
		break;
	case LE:
		matchToken(LE);
		break;
	default:
		matchToken(GE);
		break;
	}//end switch
}//end logicComparation

void Parser::exp(Tipo &_tipo, short &_endereco){
	Tipo tipo1;
	short endereco1;
	escritor.writeComment("************************** Expressao ******");
	tempReset(); //Reseta base de temporarios
	expSum(_tipo, _endereco);											//VERIFICAR NECESSIDADE DA REGRA 41 do ET.
	if (currentToken.token == EQ || currentToken.token == NEQ || currentToken.token == LT || currentToken.token == GT || currentToken.token == LE || currentToken.token == GE){
		logicComparation();
		Token op = previousToken.token;
		//_tipo = TIPO_LOGICO;								// Regra 42 do ET.
		expSum(tipo1, endereco1);
		if (tipo1 == TIPO_LOGICO) throw TypeException();	// Regra 43 do ET.

		if (_tipo == TIPO_INTEIRO) escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::A, _endereco);
		else escritor.write(GCX::Instruction::LODF, GCX::RealRegister::AX, _endereco);

		if (tipo1 == TIPO_INTEIRO) escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::B, endereco1);
		else escritor.write(GCX::Instruction::LODF, GCX::RealRegister::AX, endereco1);

		if (_tipo == TIPO_INTEIRO && tipo1 == TIPO_REAL){
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::AX, GCX::IntegerRegister::A);
			_tipo = TIPO_REAL;
		}
		else if (tipo1 == TIPO_INTEIRO && _tipo == TIPO_REAL){
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::BX, GCX::IntegerRegister::B);
			tipo1 = TIPO_REAL;
		}

		if(_tipo == TIPO_INTEIRO){
			escritor.write(GCX::Instruction::SUB, GCX::IntegerRegister::A, GCX::IntegerRegister::B);
		}
		else{
			escritor.write(GCX::Instruction::SUBF, GCX::RealRegister::AX, GCX::RealRegister::BX);
		}

		GCX::Label rotVerdadeiro = novoRot();
		rotVerdadeiro.address = GCX::CS + 8;
		switch (op){
		case EQ:
			if(_tipo == TIPO_INTEIRO) escritor.write(GCX::Instruction::BZR, GCX::IntegerRegister::A, rotVerdadeiro);
			else escritor.write(GCX::Instruction::BZRF, GCX::RealRegister::AX, rotVerdadeiro);
			break;
		case NEQ:
			if(_tipo == TIPO_INTEIRO) escritor.write(GCX::Instruction::BNZ, GCX::IntegerRegister::A, rotVerdadeiro);
			else escritor.write(GCX::Instruction::BNZF, GCX::RealRegister::AX, rotVerdadeiro);
			break;
		case LT:
			if(_tipo == TIPO_INTEIRO) escritor.write(GCX::Instruction::BNG, GCX::IntegerRegister::A, rotVerdadeiro);
			else escritor.write(GCX::Instruction::BNGF, GCX::RealRegister::AX, rotVerdadeiro);
			break;
		case GT:
			if(_tipo == TIPO_INTEIRO) escritor.write(GCX::Instruction::BPS, GCX::IntegerRegister::A, rotVerdadeiro);
			else escritor.write(GCX::Instruction::BPSF, GCX::RealRegister::AX, rotVerdadeiro);
			break;
		case LE:
			if(_tipo == TIPO_INTEIRO) escritor.write(GCX::Instruction::BNP, GCX::IntegerRegister::A, rotVerdadeiro);
			else escritor.write(GCX::Instruction::BNPF, GCX::RealRegister::AX, rotVerdadeiro);
			break;
		case GE:
			if(_tipo == TIPO_INTEIRO) escritor.write(GCX::Instruction::BNN, GCX::IntegerRegister::A, rotVerdadeiro);
			else escritor.write(GCX::Instruction::BNNF, GCX::RealRegister::AX, rotVerdadeiro);
			break;
		}
		escritor.write(GCX::Instruction::LDI, GCX::IntegerRegister::A, 0);
		GCX::Label rotFim = novoRot();
		rotFim.address = GCX::CS + 5;
		escritor.write(GCX::Instruction::JMP, rotFim);
		escritor.writeLabel(rotVerdadeiro);
		escritor.write(GCX::Instruction::LDI, GCX::IntegerRegister::A, 1);
		escritor.writeLabel(rotFim);

		_endereco = novoTemp(1);
		_tipo = TIPO_LOGICO;
		escritor.write(GCX::Instruction::STO, GCX::IntegerRegister::A, _endereco);
	}//end if
}// end exp

void Parser::expSum(Tipo &_tipo, short &_endereco){
	Tipo tipo1;
	short endereco1;
	Token op=SUM, op1;
	if (currentToken.token == SUM){
		matchToken(SUM);
	}
	else if (currentToken.token == SUB){
		matchToken(SUB);
		op = previousToken.token;
	}
	expProduct(_tipo, _endereco);											//VERIFICAR NECESSIDADE DA REGRA 44 do ET.
	while(currentToken.token == SUM || currentToken.token == SUB || currentToken.token == OR){
		if(currentToken.token == SUM)
			matchToken(SUM);
		else if (currentToken.token == SUB)
			matchToken(SUB);
		else
			matchToken(OR);
		op1 = previousToken.token;
		expProduct(tipo1, endereco1);
		if (op1 == OR && !(_tipo == TIPO_LOGICO && tipo1 == TIPO_LOGICO)) throw TypeException();							//Regra 45 do ET.
		else if ((op1 == SUM || op1 == SUB) && (_tipo == TIPO_LOGICO || tipo1 == TIPO_LOGICO)) throw TypeException();		//Regra 45 do ET.

		if(_tipo == TIPO_INTEIRO) escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::A, _endereco);
		else escritor.write(GCX::Instruction::LODF, GCX::RealRegister::AX, _endereco);

		if(op == SUB){
			if (_tipo == TIPO_INTEIRO) escritor.write(GCX::Instruction::NEG, GCX::IntegerRegister::A);
			else escritor.write(GCX::Instruction::NEGF, GCX::RealRegister::AX);
		}

		if(tipo1 == TIPO_INTEIRO) escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::B, endereco1);
		else escritor.write(GCX::Instruction::LODF, GCX::RealRegister::BX, endereco1);

		if (_tipo == TIPO_INTEIRO && tipo1 == TIPO_REAL){
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::AX, GCX::IntegerRegister::A);
			_tipo = TIPO_REAL;
		}
		else if (_tipo == TIPO_REAL && tipo1 == TIPO_INTEIRO){
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::BX, GCX::IntegerRegister::B);
			tipo1 = TIPO_REAL;
		}
		switch (op1){
		case SUM:
			if (_tipo == TIPO_INTEIRO){
				escritor.write(GCX::Instruction::ADD, GCX::IntegerRegister::A, GCX::IntegerRegister::B);
			}
			else {
				escritor.write(GCX::Instruction::ADDF, GCX::RealRegister::AX, GCX::RealRegister::BX);
			}
			break;
		case SUB:
			if (_tipo == TIPO_INTEIRO){
				escritor.write(GCX::Instruction::SUB, GCX::IntegerRegister::A, GCX::IntegerRegister::B);
			}
			else {
				escritor.write(GCX::Instruction::SUBF, GCX::RealRegister::AX, GCX::RealRegister::BX);
			}
			break;
		case OR:
			GCX::Label rotFalso = novoRot();
			rotFalso.address = GCX::CS + 6;
			escritor.write(GCX::Instruction::BZR, GCX::IntegerRegister::B, rotFalso);
			escritor.write(GCX::Instruction::LDI, GCX::IntegerRegister::A, 1);
			escritor.writeLabel(rotFalso);
			break;
		}
		if(_tipo == TIPO_INTEIRO || _tipo == TIPO_LOGICO){
			_endereco = novoTemp(1);
			escritor.write(GCX::Instruction::STO, GCX::IntegerRegister::A, _endereco);
		}
		else{
			_endereco = novoTemp(2);
			escritor.write(GCX::Instruction::STOF, GCX::RealRegister::AX, _endereco);
		}
	}//end while
}//end expSum

void Parser::expProduct(Tipo &_tipo, short &_endereco){
	Tipo tipo1;
	short endereco1;
	expValue(_tipo, _endereco);																				//VERIFICAR NECESSIDADE DA REGRA 46 do ET.
	while(currentToken.token == MULT || currentToken.token == DIV || currentToken.token == AND){
		Token op;
		if(currentToken.token == MULT)
			matchToken(MULT);
		else if (currentToken.token == DIV)
			matchToken(DIV);
		else
			matchToken(AND);
		op = previousToken.token;
		expValue(tipo1, endereco1);
		if (op == AND && !(_tipo == TIPO_LOGICO && tipo1 == TIPO_LOGICO)) throw TypeException();						//Regra 46 do ET.
		else if ((op == MULT || op == DIV) && (_tipo == TIPO_LOGICO || tipo1 == TIPO_LOGICO)) throw TypeException();	//Regra 46 do ET.

		if(_tipo == TIPO_INTEIRO) escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::A, _endereco);
		else escritor.write(GCX::Instruction::LODF, GCX::RealRegister::AX, _endereco);

		if(tipo1 == TIPO_INTEIRO) escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::B, endereco1);
		else escritor.write(GCX::Instruction::LODF, GCX::RealRegister::BX, endereco1);

		if (_tipo == TIPO_INTEIRO && tipo1 == TIPO_REAL){
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::AX, GCX::IntegerRegister::A);
			_tipo = TIPO_REAL;
		}
		else if (_tipo == TIPO_REAL && tipo1 == TIPO_INTEIRO){
			escritor.write(GCX::Instruction::CNV, GCX::RealRegister::BX, GCX::IntegerRegister::B);
			tipo1 = TIPO_REAL;
		}
		switch (op){
		case MULT:
			if (_tipo == TIPO_INTEIRO){
				escritor.write(GCX::Instruction::MUL, GCX::IntegerRegister::A, GCX::IntegerRegister::B);
			}
			else {
				escritor.write(GCX::Instruction::MULF, GCX::RealRegister::AX, GCX::RealRegister::BX);
			}
			break;
		case DIV:
			if (_tipo == TIPO_INTEIRO){
				escritor.write(GCX::Instruction::CNV, GCX::RealRegister::AX, GCX::IntegerRegister::A);
				escritor.write(GCX::Instruction::CNV, GCX::RealRegister::BX, GCX::IntegerRegister::B);
				_tipo = TIPO_REAL;
			}
			escritor.write(GCX::Instruction::DIV, GCX::RealRegister::AX, GCX::RealRegister::BX);
			break;
		case AND:
			escritor.write(GCX::Instruction::MUL, GCX::IntegerRegister::A, GCX::IntegerRegister::B);
			break;
		}
		if(_tipo == TIPO_INTEIRO){
			_endereco = novoTemp(1);
			escritor.write(GCX::Instruction::STO, GCX::IntegerRegister::A, _endereco);
		}
		else{
			_endereco = novoTemp(2);
			escritor.write(GCX::Instruction::STOF, GCX::RealRegister::AX, _endereco);
		}
	}//fim while
}//fim expProduct

void Parser::expValue(Tipo &_tipo, short &_endereco){
	Tipo tipo1;
	short endereco1;
	switch (currentToken.token){
	case NOT:
		matchToken(NOT);
		_tipo = TIPO_LOGICO;								//Regra 48 do ET
		expValue(tipo1, endereco1);
		if(tipo1 != TIPO_LOGICO) throw TypeException();		//Regra 49 do ET
		_endereco = novoTemp(1);
		escritor.write(GCX::Instruction::LOD, GCX::IntegerRegister::A, endereco1);
		escritor.write(GCX::Instruction::NEG, GCX::IntegerRegister::A);
		escritor.write(GCX::Instruction::ADI, GCX::IntegerRegister::A, 1);
		escritor.write(GCX::Instruction::STO, GCX::IntegerRegister::A, _endereco);
		break;
	case LEFT_PARENS:
		matchToken(LEFT_PARENS);
		exp(_tipo, _endereco);								//VERIFICAR NECESSIDADE DA REGRA 50
		matchToken(RIGHT_PARENS);
		break;
	case ID:
		matchToken(ID);
		unicityNotDeclared();											//Regra 53 do ET. Verifica se o identificador nao foi declarado
		if(symbolsTable->getClasse(previousToken.lexema) != CLASSE_VAR && symbolsTable->getClasse(previousToken.lexema) != CLASSE_CONST ){
			throw TypeException();
		}
		else{
			_tipo = symbolsTable->getTipo(previousToken.lexema);			//Regra 51 do ET
			_endereco = symbolsTable->getEndereco(previousToken.lexema);	//Regra 71 do ET.
		}
		break;
	default:
		matchToken(CONSTANT);
		_tipo = previousToken.tipo;								//Regra 52 do ET.
		if(_tipo == TIPO_INTEIRO){
			_endereco = novoTemp(1);
			escritor.write(GCX::Instruction::STI, previousToken.integerValue, _endereco);
		}
		else{
			_endereco = novoTemp(2);
			escritor.write(GCX::Instruction::STIF, previousToken.integerValue, previousToken.fracValue, _endereco);
		}
		break;
	}//fim switch
}//fim expValue

void Parser::unicityAlreadyDeclared(Classe _classe){
	Classe classe = symbolsTable->getClasse(previousToken.lexema);
	if( classe == (Classe) -1){
		symbolsTable->atualizaClasse(previousToken.lexema, _classe);
	}
	else{
		throw AlreadyDeclaredIdentifierException(previousToken.lexema);
	}
}

void Parser::unicityNotDeclared(){
	if( symbolsTable->getClasse(previousToken.lexema) == (Classe) -1){
		throw UndeclaredIdentifierException(previousToken.lexema);
	}
}

void Parser::setType(Tipo _tipo){
	symbolsTable->atualizaTipo(previousToken.lexema, _tipo);
}

void Parser::setType(string _lexema, Tipo _tipo){
	symbolsTable->atualizaTipo(_lexema, _tipo);
}

void Parser::classVerify(Classe _classe){
	if(symbolsTable->getClasse(previousToken.lexema) != _classe){
		throw ClassException(previousToken.lexema);
	}
}

void Parser::typeVerify(Tipo _tipo){
	if(symbolsTable->getTipo(previousToken.lexema) != _tipo){
		throw TypeException();
	}
}

void Parser::constTypeVerify(Tipo _tipoConst, Tipo _tipo){
	if(_tipoConst != _tipo){
		throw TypeException();
	}
}

void Parser::componentColorVerify(int _value){
	if(_value <0 || _value>63){
		throw ComponentColorException();
	}
}

inline void Parser::setEndereco(string _lexema){
	symbolsTable->setEndereco(_lexema, GCX::DS);
}

inline short Parser::novoTemp(short size){
	short end = GCX::DS;
	GCX::DS+=size;
	return end;
}

void Parser::tempReset(){
	GCX::DS = baseEndTemp;
}

GCX::Label Parser::novoRot(){
	string rot = "ROT" + to_string(currentLabel);
	currentLabel++;
	GCX::Label label = {rot, -1};
	return label;
}

