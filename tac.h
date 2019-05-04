// Laurien Santin

#ifndef TAC_H
#define TAC_H

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_VECTOR 2
#define TAC_SUM 3
#define TAC_SUB 4
#define TAC_MUL 5
#define TAC_DIV 6
#define TAC_GE 7
#define TAC_LE 8
#define TAC_GT 9
#define TAC_LT 10
#define TAC_EQ 11
#define TAC_AND 12
#define TAC_OR 13
#define TAC_NOT 14
#define TAC_FUNC_CALL 15
#define TAC_MOVE 16
#define TAC_VECTOR_ATTRIBUTION 17
#define TAC_VAR_DEC 18
#define TAC_VEC_DEC_INIT 19
#define TAC_VECTOR_DEC 20
#define TAC_PARAM 21
#define TAC_PRINT_LIST 22
#define TAC_PRINT 23
#define TAC_IF 24
#define TAC_LABEL 25
#define TAC_JMP 26
#define TAC_READ 27
#define TAC_RETURN 28
#define TAC_BEGIN 29
#define TAC_END 30
#define TAC_CHAR 31
#define TAC_INT 32
#define TAC_FLOAT 33
#define TAC_WHILE 34
#define TAC_IF_ELSE 35
#define TAC_ARGS 36

typedef struct tacNode
{
	int type;
	HASH_NODE *result;
	HASH_NODE *op0;
	HASH_NODE *op1;
	struct tacNode *prev;
} TAC_NODE;

TAC_NODE* tacCreate(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2);
TAC_NODE* generateCode(AST_NODE* node);
TAC_NODE* tacJoin(TAC_NODE* tac1, TAC_NODE* tac2);
void printTAC(TAC_NODE* tac);
void printBackwards(TAC_NODE* list);

#endif
