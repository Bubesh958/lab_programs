%{
#include<stdio.h>
#include<stdlib.h>
#include "y.tab.h"
#ifdef debug
#define AA printf(" %s \n",yytext)
#else
#define AA ; 
#endif
%}

%%

\"[a-zA-Z0-9]*\" {  AA; return STRING;}


scanf\(\"(.)*\"(.)*\) { AA; return SCANF;}

printf\(\"(.)*\"(.)*\) { AA; return PRINTF;}

main { AA; return MAIN;}

[0-9]+ { AA; return CONSTANT;}

(int|float|char) { AA; return DATA_TYPE;}

if { AA; return IF;}

else { AA; return ELSE;}

while { AA; return WHILE; }


[_a-zA-Z][_a-zA-Z0-9]* {  AA; return IDENTIFIER; }

[ \t\n] ;

. {  AA; return  yytext[0]; }
%%


