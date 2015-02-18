     STIF  #-10.0000 , 0(DS)
     STIF  #10.0000 , 2(DS)
     STIF  #10.0000 , 4(DS)
     STIF  #-10.0000 , 6(DS)
     STIF  #-10.0000 , 8(DS)
     STIF  #10.0000 , 10(DS)
     STIF  #10.0000 , 12(DS)
     STIF  #-10.0000 , 14(DS)
     STIF  #10.0000 , 16(DS)
     STIF  #10.0000 , 18(DS)
     STIF  #10.0000 , 20(DS)
     STIF  #10.0000 , 22(DS)
     STIF  #-10.0000 , 24(DS)
     STIF  #10.0000 , 26(DS)
     STIF  #-10.0000 , 28(DS)
     STIF  #-10.0000 , 30(DS)
     STIF  #-10.0000 , 32(DS)
     STIF  #-10.0000 , 34(DS)
     STIF  #10.0000 , 36(DS)
     STIF  #-10.0000 , 38(DS)
     STIF  #-10.0000 , 40(DS)
     STIF  #10.0000 , 42(DS)
     STIF  #10.0000 , 44(DS)
     STIF  #-10.0000 , 46(DS)
     STI   #15 , 48(DS)
     STI   #20 , 49(DS)
     STI   #20 , 50(DS)
     STI   #40 , 51(DS)
     STI   #60 , 52(DS)
     STI   #30 , 53(DS)
     STI   #60 , 54(DS)
     STI   #4 , 55(DS)
     STI   #48 , 56(DS)
     STI   #0 , 57(DS)
     STI   #6 , 58(DS)
     STI   #12 , 59(DS)
     STI   #18 , 60(DS)
     STI   #4 , 61(DS)
     STI   #48 , 62(DS)
     STI   #18 , 63(DS)
     STI   #12 , 64(DS)
     STI   #36 , 65(DS)
     STI   #42 , 66(DS)
     STI   #4 , 67(DS)
     STI   #48 , 68(DS)
     STI   #42 , 69(DS)
     STI   #36 , 70(DS)
     STI   #30 , 71(DS)
     STI   #24 , 72(DS)
     STI   #4 , 73(DS)
     STI   #48 , 74(DS)
     STI   #24 , 75(DS)
     STI   #30 , 76(DS)
     STI   #6 , 77(DS)
     STI   #0 , 78(DS)
     STI   #4 , 79(DS)
     STI   #48 , 80(DS)
     STI   #24 , 81(DS)
     STI   #0 , 82(DS)
     STI   #18 , 83(DS)
     STI   #42 , 84(DS)
     STI   #4 , 85(DS)
     STI   #48 , 86(DS)
     STI   #36 , 87(DS)
     STI   #12 , 88(DS)
     STI   #6 , 89(DS)
     STI   #30 , 90(DS)
     STIF  #200.0000 , 91(DS)
     STIF  #200.0000 , 93(DS)
     STIF  #200.0000 , 95(DS)
     STI   #6 , 97(DS)
     STIF  #1.0000 , 98(DS)
     STI   #55 , 100(DS)
     STI   #61 , 101(DS)
     STI   #67 , 102(DS)
     STI   #73 , 103(DS)
     STI   #79 , 104(DS)
     STI   #85 , 105(DS)
     STIF  #5.5000 , 106(DS)
     STI   #500 , 108(DS)
************************** Corpo Programa ******
     LDI   A , #91
     LGT   A
************************** Expressao ******
     LODF  A , 106(DS)
     LDI   A , #97
     ESC   A , A
************************** Atribuicao ******
************************** Expressao ******
     STI   #1 , 110(DS)
     LOD   A , 110(DS)
     STO   A , 109(DS)
************************** Expressao ******
     STI   #0 , 110(DS)
************************** Expressao ******
     STI   #0 , 111(DS)
************************** Expressao ******
     STI   #0 , 112(DS)
************************** Expressao ******
     STI   #0 , 113(DS)
************************** Expressao ******
     STI   #0 , 114(DS)
************************** Expressao ******
     STI   #0 , 115(DS)
     LOD   A , 110(DS)
     CNV   A , A
     LOD   B , 111(DS)
     CNV   B , B
     LOD   C , 112(DS)
     CNV   C , C
     LOD   D , 113(DS)
     CNV   D , D
     LOD   E , 114(DS)
     CNV   E , E
     LOD   F , 115(DS)
     CNV   F , F
     LDI   A , #97
     RTR 
************************** Enquanto ******
ROT1:
************************** Expressao ******
************************** Expressao ******
     LOD   A , 109(DS)
     LOD   B , 108(DS)
     SUB   A , B
     BNG   A , ROT2
     LDI   A , #0
     JMP   ROT3
ROT2:
     LDI   A , #1
ROT3:
     STO   A , 110(DS)
     LOD   A , 110(DS)
     BZR   A , ROT4
************************** Expressao ******
     STIF  #0.0100 , 110(DS)
     LODF  A , 110(DS)
     LOD   B , 109(DS)
     CNV   B , B
     MULF  A , B
     STOF  A , 112(DS)
************************** Expressao ******
     STIF  #0.1000 , 114(DS)
************************** Expressao ******
     STI   #0 , 116(DS)
************************** Expressao ******
     STI   #0 , 117(DS)
************************** Expressao ******
     STI   #0 , 118(DS)
************************** Expressao ******
     STI   #0 , 119(DS)
     LODF  A , 112(DS)
     LODF  B , 114(DS)
     LOD   C , 116(DS)
     CNV   C , C
     LOD   D , 117(DS)
     CNV   D , D
     LOD   E , 118(DS)
     CNV   E , E
     LOD   F , 119(DS)
     CNV   F , F
     LDI   A , #97
     RTR 
************************** Atribuicao ******
************************** Expressao ******
     STI   #1 , 110(DS)
     LOD   A , 109(DS)
     LOD   B , 110(DS)
     ADD   A , B
     STO   A , 111(DS)
     LOD   A , 111(DS)
     STO   A , 109(DS)
************************** Expressao ******
     STIF  #0.0500 , 110(DS)
     LODF  A , 110(DS)
     TME   A
     JMP   ROT1
ROT4:
     HLT 
