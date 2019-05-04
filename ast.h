#ifndef AST_H
#define AST_H

#include <stdio.h>

#define MAX_CHILDREN 4

#define AST_PROGRAM 1
#define AST_PROG_ELEMENTS 2
#define AST_VAR_DECLARATION 3
#define AST_VECTOR_DECLARATION 4
#define AST_VEC_DEC_INIT 5
#define AST_SYMBOL 6
#define AST_LIT_LIST 9
#define AST_LIT_INT 10
#define AST_LIT_FLOAT 11
#define AST_LIT_CHAR 12
#define AST_LITERAL 13
#define AST_VECTOR 15
#define AST_SUM 16
#define AST_SUB 17
#define AST_MUL 18
#define AST_DIV 19
#define AST_LT 20
#define AST_GT 21
#define AST_EQ 22
#define AST_LE 23
#define AST_GE 24
#define AST_OR 25
#define AST_AND 26
#define AST_NOT 27
#define AST_EXPRESSION_BRACKETS 28
#define AST_FUNC_CALL 29
#define AST_FUNC_DECLARATION 30
#define AST_PARAM_LIST 31
#define AST_ATTRIBUTION 32
#define AST_IF 33
#define AST_IF_ELSE 34
#define AST_WHILE 35
#define AST_READ 36
#define AST_RETURN 37
#define AST_PRINT 38
#define AST_CMD_BLOCK 39
#define AST_VAR_ATTRIBUTION 40
#define AST_VECTOR_ATTRIBUTION 41
#define AST_PRINT_LIST 42
#define AST_CMD_LIST 43
#define AST_ARG_LIST 44
#define AST_CHAR 45
#define AST_INT 46
#define AST_FLOAT 47



/*
#define AST_FUNC_DECLARATION 12

#define AST_CMD_LIST 16
#define AST_CMD 17
#define AST_PRINT_LIST 18
#define AST_PARAMETER_LIST 19
#define AST_ARG_LIST 21

#define AST_NAME 22
#define AST_VECTOR 23
#define AST_TYPE 24
#define AST_LITERAL 25

#define AST_ATTRIBUTION 30
#define AST_CMD_BLOCK 31
#define AST_EXPRESSION 32
#define AST_IF 33
#define AST_WHILE 34
#define AST_READ 35
#define AST_PRINT 36
#define AST_RETURN 37
#define AST_SUM 38
#define AST_SUB 39
#define AST_MUL 40
#define AST_DIV 41
#define AST_LT 42
#define AST_GT 43
#define AST_LE 44
#define AST_GE 45
#define AST_EQ 46
#define AST_OR 47
#define AST_AND 48
#define AST_NOT 49
#define AST_EXP_BRACKETS 50
#define AST_FUNC_CALLING 51*/

typedef struct astNode
{
	int type;
	int lineNumber;
	HASH_NODE* symbol;
	struct astNode* children[MAX_CHILDREN];
	
} AST_NODE;

FILE* output;


AST_NODE* astInsert(int type, HASH_NODE* symbol, AST_NODE* ch0, AST_NODE* ch1, AST_NODE* ch2, AST_NODE* ch3);

void printTree(AST_NODE* node);

void printCode(AST_NODE* node);

#endif
