  100 FOR PN%=0 TO 63
  110   *FX 159,0,66
  120   *FX 159,0,64
  130   *FX 159,0,64
  140   A%=159:X%=0:Y%=64+PN%*4 MOD 16:R%=USR(&FFF4)
  150   A%=159:X%=0:Y%=64+PN% DIV 4:R%=USR(&FFF4) 
  160   IF FNREADBYTE <> &28 OR FNREADBYTE <> &43 OR FNREADBYTE <> &29 THEN NEXT:END
  170 C$="(C)"+FNREADSTRING
  180 T$=FNREADSTRING
  200 PRINT ~PN%+&C0;": ";T$;" ";C$ 
  210 NEXT:END
 1000 DEF FNREADBYTE
 1010 *FX 159,0,16
 1020 A%=158:R%=USR(&FFF4)
 1030 = ((R% DIV &10000)-1) AND &FF
 1100 DEF FNREADSTRING
 1110 R$=""
 1120 REPEAT:C=FNREADBYTE
 1130   IF C=0 THEN UNTIL TRUE:=R$
 1140 R$=R$+CHR$(C)
 1150 UNTIL FALSE
