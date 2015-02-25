	STIF #-10.000000, 0 (DS)
	STIF #10.000000, 2 (DS)
	STIF #10.000000, 4 (DS)
	STIF #-10.000000, 6 (DS)
	STIF #-10.000000, 8 (DS)
	STIF #10.000000, 10 (DS)
	STIF #10.000000, 12 (DS)
	STIF #-10.000000, 14 (DS)
	STIF #10.000000, 16 (DS)
	STIF #10.000000, 18 (DS)
	STIF #10.000000, 20 (DS)
	STIF #10.000000, 22 (DS)
	STIF #-10.000000, 24 (DS)
	STIF #10.000000, 26 (DS)
	STIF #-10.000000, 28 (DS)
	STIF #-10.000000, 30 (DS)
	STIF #-10.000000, 32 (DS)
	STIF #-10.000000, 34 (DS)
	STIF #10.000000, 36 (DS)
	STIF #-10.000000, 38 (DS)
	STIF #-10.000000, 40 (DS)
	STIF #10.000000, 42 (DS)
	STIF #10.000000, 44 (DS)
	STIF #-10.000000, 46 (DS)
	STI #15, 48 (DS)
	STI #20, 49 (DS)
	STI #20, 50 (DS)
	STI #40, 51 (DS)
	STI #60, 52 (DS)
	STI #30, 53 (DS)
	STI #60, 54 (DS)
	STI #4, 55 (DS)
	STI #48, 56 (DS)
	STI #0, 57 (DS)
	STI #6, 58 (DS)
	STI #12, 59 (DS)
	STI #18, 60 (DS)
	STI #4, 61 (DS)
	STI #48, 62 (DS)
	STI #18, 63 (DS)
	STI #12, 64 (DS)
	STI #36, 65 (DS)
	STI #42, 66 (DS)
	STI #4, 67 (DS)
	STI #48, 68 (DS)
	STI #42, 69 (DS)
	STI #36, 70 (DS)
	STI #30, 71 (DS)
	STI #24, 72 (DS)
	STI #4, 73 (DS)
	STI #48, 74 (DS)
	STI #24, 75 (DS)
	STI #30, 76 (DS)
	STI #6, 77 (DS)
	STI #0, 78 (DS)
	STI #4, 79 (DS)
	STI #48, 80 (DS)
	STI #24, 81 (DS)
	STI #0, 82 (DS)
	STI #18, 83 (DS)
	STI #42, 84 (DS)
	STI #4, 85 (DS)
	STI #48, 86 (DS)
	STI #36, 87 (DS)
	STI #12, 88 (DS)
	STI #6, 89 (DS)
	STI #30, 90 (DS)
	STIF #200.000000, 91 (DS)
	STIF #200.000000, 93 (DS)
	STIF #200.000000, 95 (DS)
	STI #6, 97 (DS)
	STIF #1.000000, 98 (DS)
	STI #55, 100 (DS)
	STI #61, 101 (DS)
	STI #67, 102 (DS)
	STI #73, 103 (DS)
	STI #79, 104 (DS)
	STI #85, 105 (DS)
	STIF #5.500000, 106 (DS)
	STI #500, 108 (DS)
************************** Corpo Programa ******
	LDI A, #91
	LGT A
************************** Expressao ******
	LODF AX, 106(DS)
	LDI A, #97
	ESC A, AX
************************** Atribuicao ******
************************** Expressao ******
	STI #1, 110 (DS)
	LOD A, 110(DS)
	STO A, 109(DS)
************************** Expressao ******
	STI #0, 110 (DS)
	LOD B, 110(DS)
	CNV AX, B
************************** Expressao ******
	STI #0, 110 (DS)
	LOD B, 110(DS)
	CNV BX, B
************************** Expressao ******
	STI #0, 110 (DS)
	LOD B, 110(DS)
	CNV CX, B
************************** Expressao ******
	STI #0, 110 (DS)
	LOD B, 110(DS)
	CNV DX, B
************************** Expressao ******
	STI #0, 110 (DS)
	LOD B, 110(DS)
	CNV EX, B
************************** Expressao ******
	STI #0, 110 (DS)
	LOD B, 110(DS)
	CNV FX, B
	LDI A, #97
	RTR
************************** Enquanto ******
ROT1:
************************** Expressao ******
	LOD A, 109(DS)
	LOD B, 108(DS)
	SUB A, B
	BNG A, ROT2
	LDI A, #0
	JMP ROT3
ROT2:
	LDI A, #1
ROT3:
	STO A, 110(DS)
	LOD A, 110(DS)
	BZR A, ROT4
************************** Expressao ******
	STIF #0.010000, 110 (DS)
	LODF AX, 110(DS)
	LOD B, 109(DS)
	CNV BX, B
	MULF AX, BX
	STOF AX, 114(DS)
	LODF AX, 114(DS)
************************** Expressao ******
	STIF #0.100000, 110 (DS)
	LODF BX, 110(DS)
************************** Expressao ******
	STI #0, 110 (DS)
	LOD B, 110(DS)
	CNV CX, B
************************** Expressao ******
	STI #0, 110 (DS)
	LOD B, 110(DS)
	CNV DX, B
************************** Expressao ******
	STI #0, 110 (DS)
	LOD B, 110(DS)
	CNV EX, B
************************** Expressao ******
	STI #0, 110 (DS)
	LOD B, 110(DS)
	CNV FX, B
	LDI A, #97
	RTR
************************** Atribuicao ******
************************** Expressao ******
	STI #1, 110 (DS)
	LOD A, 109(DS)
	LOD B, 110(DS)
	ADD A, B
	STO A, 112(DS)
	LOD A, 112(DS)
	STO A, 109(DS)
************************** Expressao ******
	STIF #0.050000, 110 (DS)
	LODF AX, 110(DS)
	TME AX
	JMP ROT1
ROT4:
	HLT
