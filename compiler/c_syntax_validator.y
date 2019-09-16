%{
#include<stdio.h>
#include<stdlib.h>
extern FILE *yyin;
%}

%start PROGRAM
%token DATA_TYPE MAIN SCANF PRINTF WHILE IF ELSE IDENTIFIER CONSTANT STRING

%%

PROGRAM	: DATA_TYPE MAIN '(' ')' BLOCK
	;

BLOCK	: '{' LINES '}'
      	;

LINES	: DATA_TYPE VARIABLES ';' LINES
      	| EXPRESSIONS ';' LINES
	| SCANF ';' LINES
	| PRINTF ';' LINES
	| WHILE '(' BOOL ')' BLOCK  LINES
	| IF '(' BOOL ')' BLOCK  LINES
	| IF '(' BOOL ')' BLOCK ELSE BLOCK LINES
      	|
	;

VAR	: IDENTIFIER
   	| IDENTIFIER ',' VAR
 
	;

BOOL	: EXPR '=' '=' EXPR
     	| EXPR '>' '=' EXPR
	| EXPR '<' '=' EXPR
	| EXPR '>' EXPR
	| EXPR '<' EXPR
	;

EXPRESSIONS : IDENTIFIER '=' EXPR
	    ;

EXPR	: EXPR '+' EXPR
     	| EXPR '-' EXPR
	| EXPR '*' EXPR
	| EXPR '/' EXPR
	| '(' EXPR ')'
        | CONSTANT
	| IDENTIFIER
	; 

VARIABLES : IDENTIFIER '=' CONSTANT
	  | IDENTIFIER '=' CONSTANT ',' VARIABLES
	  | IDENTIFIER
	  ;



%%


int yyerror(char *s)
{
	printf("\n Error\n");
	exit(0);
}

void yywrap()
{
	;
}

void main()
{
	yyparse();
	exit(0);
}
