// Laurien Santin

%{ 
	#include <stdio.h>
	#include <stdlib.h>
	#include "tac.h"
	#include "ast.h"
	#include "hash.h" 
	
	int yylex(void); 
	void yyerror(char *); 
	extern FILE *yyin;
	
	#define SYMBOL_LIT_INT 1
	#define SYMBOL_LIT_FLOAT 2
	#define SYMBOL_LIT_CHAR 3
	#define SYMBOL_LIT_STRING 4
	#define SYMBOL_IDENTIFIER 5

%} 

%union { HASH_NODE* symbol; 
	 AST_NODE* tree;
	 };

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

%type<tree> program
%type<tree> progrElements
%type<tree> type
%type<tree> literal
%type<tree> funcDeclaration

%type<tree> cmdList
%type<tree> cmd
%type<tree> printList
%type<tree> argList
%type<tree> parameterList
%type<tree> litList
%type<tree> varDeclaration

%type<tree> attribution
%type<tree> expression
 
%% 
program:
	progrElements									{$$ = astInsert(AST_PROGRAM, 0, $1, 0, 0, 0); TAC_NODE* teste = generateCode($$); printTAC(teste); exit(0);}
	;

progrElements:
	funcDeclaration progrElements							{$$ = astInsert(AST_PROG_ELEMENTS, 0, $1, $2, 0, 0);}
	| varDeclaration progrElements							{$$ = astInsert(AST_PROG_ELEMENTS, 0, $1, $2, 0, 0);}
	| {printf("Success!\n");}							{$$ = 0;}
	;

type:
	KW_CHAR										{$$ = astInsert(AST_CHAR, 0, 0, 0, 0, 0);}
	| KW_INT									{$$ = astInsert(AST_INT, 0, 0, 0, 0, 0);}
	| KW_FLOAT									{$$ = astInsert(AST_FLOAT, 0, 0, 0, 0, 0);}
	;
	
literal:
	LIT_INTEGER									{$$ = astInsert(AST_SYMBOL, $1, 0, 0, 0, 0);}
	| LIT_FLOAT									{$$ = astInsert(AST_SYMBOL, $1, 0, 0, 0, 0);}
	| LIT_CHAR									{$$ = astInsert(AST_SYMBOL, $1, 0, 0, 0, 0);}
	;
	
	
funcDeclaration:
	type TK_IDENTIFIER 'd' parameterList 'b' cmd					{$$ = astInsert(AST_FUNC_DECLARATION, $2, $1, $4, $6, 0);}
	;

	
cmdList:
	cmd ';' cmdList									{$$ = astInsert(AST_CMD_LIST, 0, $1, $3, 0, 0);}
	|										{$$ = 0;}
	;

cmd:
	attribution									{$$ = $1;}
	| KW_IF expression KW_THEN cmd %prec LOWER_THAN_ELSE				{$$ = astInsert(AST_IF, 0, $2, $4, 0, 0);}
	| KW_IF expression KW_THEN cmd KW_ELSE cmd					{$$ = astInsert(AST_IF_ELSE, 0, $2, $4, $6, 0);}
	| KW_WHILE expression cmd							{$$ = astInsert(AST_WHILE, 0, $2, $3, 0, 0);}
	| KW_READ TK_IDENTIFIER								{$$ = astInsert(AST_READ, $2, 0, 0, 0, 0);}
	| KW_PRINT printList								{$$ = astInsert(AST_PRINT, 0, $2, 0, 0, 0);}
	| KW_RETURN expression								{$$ = astInsert(AST_RETURN, 0, $2, 0, 0, 0);}
	| '{' cmdList '}'								{$$ = astInsert(AST_CMD_BLOCK, 0, $2, 0, 0, 0);}
	| 										{$$ = 0;}
	;

printList:
	LIT_STRING ',' printList							{$$ = astInsert(AST_PRINT_LIST, $1, $3, 0, 0, 0);}
	| expression ',' printList							{$$ = astInsert(AST_PRINT_LIST, 0, $1, $3, 0, 0);}
	| LIT_STRING									{$$ = astInsert(AST_PRINT_LIST, $1, 0, 0, 0, 0);}
	| expression									{$$ = astInsert(AST_PRINT_LIST, 0, $1, 0, 0, 0);}
	;

parameterList:
	type TK_IDENTIFIER ',' parameterList						{$$ = astInsert(AST_PARAM_LIST, $2, $1, $4, 0, 0);}
	| type TK_IDENTIFIER								{$$ = astInsert(AST_PARAM_LIST, $2, $1, 0, 0, 0);}
	|										{$$ = 0;}
	;
	
litList:
	literal										{$$ = astInsert(AST_LIT_LIST, 0, $1, 0, 0, 0);}
	| literal litList								{$$ = astInsert(AST_LIT_LIST, 0, $1, $2, 0, 0);}
	;

argList:
	expression ',' argList								{$$ = astInsert(AST_ARG_LIST, 0, $1, $3, 0, 0);}
	| expression									{$$ = astInsert(AST_ARG_LIST, 0, $1, 0, 0, 0);}
	|										{$$ = 0;}
	;
		
varDeclaration:
	type TK_IDENTIFIER '=' expression ';'						{$$ = astInsert(AST_VAR_DECLARATION, $2, $1, $4, 0, 0);}
	| type TK_IDENTIFIER 'q' LIT_INTEGER 'p' ';'					{$$ = astInsert(AST_VECTOR_DECLARATION, $2, $1, astInsert(AST_SYMBOL, $4, 0, 0, 0, 0), 0, 0);}
	| type TK_IDENTIFIER 'q' LIT_INTEGER 'p' ':' litList ';'			{$$ = astInsert(AST_VEC_DEC_INIT, $2, $1, astInsert(AST_SYMBOL, $4, 0, 0, 0, 0), $7, 0);}
	;
	
attribution:
	TK_IDENTIFIER '=' expression							{$$ = astInsert(AST_VAR_ATTRIBUTION, $1, $3, 0, 0, 0);}
	| TK_IDENTIFIER 'q' expression 'p' '=' expression				{$$ = astInsert(AST_VECTOR_ATTRIBUTION, $1, $3, $6, 0, 0);}
	;

expression:
	literal										{$$ = $1;}
	| TK_IDENTIFIER									{$$ = astInsert(AST_SYMBOL, $1, 0, 0, 0, 0);}
	| TK_IDENTIFIER 'q' expression 'p'						{$$ = astInsert(AST_VECTOR, $1, $3, 0, 0, 0);}
	| expression '+' expression							{$$ = astInsert(AST_SUM, 0, $1, $3, 0, 0);}
	| expression '-' expression							{$$ = astInsert(AST_SUB, 0, $1, $3, 0, 0);}
	| expression '*' expression							{$$ = astInsert(AST_MUL, 0, $1, $3, 0, 0);}
	| expression '/' expression							{$$ = astInsert(AST_DIV, 0, $1, $3, 0, 0);}
	| expression '<' expression							{$$ = astInsert(AST_LT, 0, $1, $3, 0, 0);}
	| expression '>' expression							{$$ = astInsert(AST_GT, 0, $1, $3, 0, 0);}
	| expression OPERATOR_LE expression						{$$ = astInsert(AST_LE, 0, $1, $3, 0, 0);}
	| expression OPERATOR_GE expression						{$$ = astInsert(AST_GE, 0, $1, $3, 0, 0);}
	| expression OPERATOR_EQ expression						{$$ = astInsert(AST_EQ, 0, $1, $3, 0, 0);}
	| expression OPERATOR_OR expression						{$$ = astInsert(AST_OR, 0, $1, $3, 0, 0);}
	| expression OPERATOR_AND expression						{$$ = astInsert(AST_AND, 0, $1, $3, 0, 0);}
	| OPERATOR_NOT expression							{$$ = astInsert(AST_NOT, 0, $2, 0, 0, 0);}
	| 'd' expression 'b'								{$$ = astInsert(AST_EXPRESSION_BRACKETS, 0, $2, 0, 0, 0);}
	| TK_IDENTIFIER 'd' argList 'b'							{$$ = astInsert(AST_FUNC_CALL, $1, $3, 0, 0, 0);}
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
