%{ 
	#include <stdio.h>
	#include <stdlib.h>
	#include "hash.h"
	#include "y.tab.h"
	int yylex(void); 
	void yyerror(char *); 
	extern FILE *yyin;
	
	#define SYMBOL_LIT_INT 1
	#define SYMBOL_LIT_FLOAT 2
	#define SYMBOL_LIT_CHAR 3
	#define SYMBOL_LIT_STRING 4
	#define SYMBOL_IDENTIFIER 5

%} 

%union { HASH_NODE* symbol; };

%token 	KW_CHAR
%token 	KW_INT 
%token 	KW_FLOAT
%token 	KW_IF 
%token 	KW_THEN 
%token 	KW_ELSE 
%token 	KW_WHILE 
%token 	KW_READ 
%token 	KW_RETURN 
%token 	KW_PRINT 
%token 	OPERATOR_LE 
%token 	OPERATOR_GE 
%token 	OPERATOR_EQ 
%token 	OPERATOR_OR 
%token 	OPERATOR_AND 
%token 	OPERATOR_NOT 
%token<symbol>	TK_IDENTIFIER 
%token<symbol>	LIT_INTEGER 
%token<symbol> 	LIT_FLOAT 
%token<symbol>	LIT_CHAR 
%token<symbol>	LIT_STRING 
%token 	TOKEN_ERROR

%nonassoc LOWER_THAN_ELSE
%nonassoc KW_ELSE

%left 	'<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_OR OPERATOR_AND 
%left	'+' '-'
%left	'*' '/'
%left	OPERATOR_NOT
 
%% 
program:
	progrElements
	;

progrElements:
	funcDeclaration progrElements
	| varDeclaration progrElements
	| {printf("Success!\n");}
	;

name:
	TK_IDENTIFIER
	;

vector:
	name 'q' expression 'p'
	;

type:
	KW_CHAR
	| KW_INT
	| KW_FLOAT
	;
	
literal:
	LIT_INTEGER
	| LIT_FLOAT
	| LIT_CHAR
	;
	
	
funcDeclaration:
	type name 'd' parameterList 'b' cmd
	;

cmdBlock:
	'{' cmdList '}'
	;
	
cmdList:
	cmd ';' cmdList
	|
	;

cmd:
	attribution
	| KW_IF expression KW_THEN cmd %prec LOWER_THAN_ELSE
	| KW_IF expression KW_THEN cmd KW_ELSE cmd
	| KW_WHILE expression cmd
	| KW_READ name
	| KW_PRINT printList
	| KW_RETURN expression
	| cmdBlock
	| 
	;

printList:
	LIT_STRING ',' printList
	| expression ',' printList
	| LIT_STRING
	| expression
	;

parameterList:
	type name ',' parameterList
	| type name
	|
	;
	
litList:
	literal
	| literal litList
	;

argList:
	expression ',' argList
	| expression
	;
		
varDeclaration:
	type name '=' expression ';'
	| type name 'q' LIT_INTEGER 'p' ';'
	| type name 'q' LIT_INTEGER 'p' ':' litList ';'
	;
	
attribution:
	name '=' expression
	| vector '=' expression
	;

expression:
	literal
	| name
	| vector
	| expression '+' expression
	| expression '-' expression
	| expression '*' expression
	| expression '/' expression
	| expression '<' expression
	| expression '>' expression
	| expression OPERATOR_LE expression
	| expression OPERATOR_GE expression
	| expression OPERATOR_EQ expression
	| expression OPERATOR_OR expression
	| expression OPERATOR_AND expression
	| OPERATOR_NOT expression
	| 'd' expression 'b'
	| name 'd' argList 'b'
        ; 

%% 

int yylex();
extern FILE *yyin;

extern int isRunning();
extern void initMe();

void yyerror(char *s) { 
	extern int lineNumber;
	fprintf(stderr, "line %d: %s\n", lineNumber, s);
	exit (3);
} 
