// Laurien Santin

#include <stdio.h>
#include <stdlib.h>
#include "tac.h"
#include "hash.h"

TAC_NODE* makeOp(int type, TAC_NODE* op1, TAC_NODE* op2);

TAC_NODE* tacCreate(int type, HASH_NODE* res, HASH_NODE* op0, HASH_NODE* op1)
{
	TAC_NODE* newTAC;
	
	newTAC = (TAC_NODE*) calloc(1, sizeof(TAC_NODE));
	newTAC->type = type;
	newTAC->result = res;
	newTAC->op0 = op0;
	newTAC->op1 = op1;
	newTAC->prev = 0;
	
	return newTAC;
}

void printTAC(TAC_NODE* tac)
{
	if(tac != 0)
	{
		printTAC(tac->prev);
		switch(tac->type)
		{
			case TAC_SYMBOL:					break;
			case TAC_VECTOR:					fprintf(stderr, "TAC_VECTOR "); break;
			case TAC_SUM:						fprintf(stderr, "TAC_SUM "); break;
			case TAC_SUB:						fprintf(stderr, "TAC_SUB "); break;
			case TAC_MUL:						fprintf(stderr, "TAC_MUL "); break;
			case TAC_DIV:						fprintf(stderr, "TAC_DIV "); break;
			case TAC_GE:						fprintf(stderr, "TAC_GE "); break;
			case TAC_LE:						fprintf(stderr, "TAC_LE "); break;
			case TAC_GT:						fprintf(stderr, "TAC_GT "); break;
			case TAC_LT:						fprintf(stderr, "TAC_LT "); break;
			case TAC_EQ:						fprintf(stderr, "TAC_EQ "); break;
			case TAC_AND:						fprintf(stderr, "TAC_AND "); break;
			case TAC_OR:						fprintf(stderr, "TAC_OR "); break;
			case TAC_NOT:						fprintf(stderr, "TAC_NOT "); break;
			case TAC_FUNC_CALL:					fprintf(stderr, "TAC_FUNC_CALL "); break;
			case TAC_MOVE:						fprintf(stderr, "TAC_MOVE "); break;
			case TAC_VECTOR_ATTRIBUTION:				fprintf(stderr, "TAC_VECTOR_ATTRIBUTION "); break;
			case TAC_VAR_DEC:					fprintf(stderr, "TAC_VAR_DEC "); break;
			case TAC_VEC_DEC_INIT:					fprintf(stderr, "TAC_VEC_DEC_INIT "); break;
			case TAC_VECTOR_DEC:					fprintf(stderr, "TAC_VECTOR_DEC "); break;
			case TAC_PARAM:						fprintf(stderr, "TAC_PARAM "); break;
			case TAC_PRINT_LIST:					fprintf(stderr, "TAC_PRINT_LIST "); break;
			case TAC_PRINT:						fprintf(stderr, "TAC_PRINT "); break;
			case TAC_IF:						fprintf(stderr, "TAC_IF "); break;
			case TAC_IF_ELSE:					fprintf(stderr, "TAC_IF_ELSE "); break;
			case TAC_LABEL:						fprintf(stderr, "TAC_LABEL "); break;
			case TAC_JMP:						fprintf(stderr, "TAC_JMP "); break;
			case TAC_READ:						fprintf(stderr, "TAC_READ "); break;
			case TAC_RETURN:					fprintf(stderr, "TAC_RETURN "); break;
			case TAC_BEGIN:						fprintf(stderr, "TAC_BEGIN "); break;
			case TAC_END:						fprintf(stderr, "TAC_END "); break;
			case TAC_CHAR:						fprintf(stderr, "TAC_CHAR "); break;
			case TAC_INT:						fprintf(stderr, "TAC_INT "); break;
			case TAC_FLOAT:						fprintf(stderr, "TAC_FLOAT "); break;
			case TAC_ARGS:						fprintf(stderr, "TAC_ARGS "); break;
			default: 						fprintf(stderr, "TAC UNKNOWN %d", tac->type); break;
		}
		if(tac->result /*&& tac->type != TAC_SYMBOL*/)
			fprintf(stderr, "result: %s ", tac->result->text);
		if (tac->op0)
			fprintf(stderr, "op0: %s ", tac->op0->text);
		if (tac->op1)
			fprintf(stderr, "op1: %s ", tac->op1->text);
		fprintf(stderr, "\n");
	}
}

void printBackwards(TAC_NODE* list)
{
	if (list != 0)
	{
		printBackwards(list->prev);
		printTAC(list);
	}
}

TAC_NODE* tacJoin(TAC_NODE* op1, TAC_NODE* op2)
{
	TAC_NODE* aux;
	
	if (op1 == 0)
		return op2;
	if (op2 == 0)
		return op1;
	
	for(aux = op2; aux->prev; aux = aux->prev);
	
	aux->prev = op1;
	return op2;
}

TAC_NODE* makeVector(HASH_NODE* name, TAC_NODE* index)
{
	HASH_NODE* tempName = makeTemp();
	TAC_NODE* newTac = tacCreate(TAC_VECTOR, tempName, name, index ? index->result : 0);
	return tacJoin(index, newTac);
}

TAC_NODE* makeOp(int type, TAC_NODE* op0, TAC_NODE* op1)
{
	return tacJoin(tacJoin(op0,op1), tacCreate(type, makeTemp(), op0 ? op0->result : 0, op1 ? op1->result : 0));
}

TAC_NODE* makeNot(TAC_NODE* exp)
{
	TAC_NODE* newTac = tacCreate(TAC_NOT, exp ? exp->result : 0, 0, 0);
	
	return tacJoin(exp, newTac);
}

TAC_NODE* makeFuncCall(HASH_NODE* symbol, TAC_NODE* arg)
{
	TAC_NODE* newTac;
	HASH_NODE* temp = makeTemp();
	
	newTac = tacCreate(TAC_FUNC_CALL, temp, symbol, arg ? arg->result : 0);
	
	return tacJoin(arg, newTac);
}

TAC_NODE* makeVectorAttrib(HASH_NODE* symbol, TAC_NODE* index, TAC_NODE* result)
{
	TAC_NODE* newTac = tacCreate(TAC_VECTOR_ATTRIBUTION, symbol, index ? index->result : 0, result ? result->result : 0);
	
	return tacJoin(tacJoin(index, result), newTac);
}

TAC_NODE* makeVarDeclaration(HASH_NODE* symbol, TAC_NODE* type, TAC_NODE* expression)
{
	TAC_NODE* newTac = tacCreate(TAC_VAR_DEC, symbol, expression ? expression->result : 0, 0);
	
	return tacJoin(tacJoin(expression, newTac), type);
}

TAC_NODE* makeVectorDeclaration(HASH_NODE* symbol, HASH_NODE* index, TAC_NODE* result, TAC_NODE* type)
{
	TAC_NODE* newTac = tacCreate(TAC_VEC_DEC_INIT, symbol, index, result ? result->result : 0);
	
	return tacJoin(tacJoin(result, newTac), type);
}

TAC_NODE* makeParam(HASH_NODE* symbol, TAC_NODE* param, TAC_NODE* next)
{
	TAC_NODE* newTac = tacCreate(TAC_PARAM, symbol, param ? param->result : 0, 0);
	
	return tacJoin(newTac, next);
}

TAC_NODE* makeIf(TAC_NODE* op0, TAC_NODE* op1)
{
	TAC_NODE* newTac;
	HASH_NODE* newLabel = makeLabel();
	TAC_NODE* labelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);
	
	newTac = tacCreate(TAC_IF, op0 ? op0->result : 0, newLabel, 0);
	
	return tacJoin(tacJoin(tacJoin(op0, newTac), op1), labelTac);	
}

TAC_NODE* makeIfElse(TAC_NODE* op0, TAC_NODE* op1, TAC_NODE* op2)
{
	TAC_NODE* newTac;

	HASH_NODE* ifLabel = makeLabel();
	TAC_NODE* ifTac = tacCreate(TAC_LABEL, ifLabel, 0, 0);
	
	newTac = tacCreate(TAC_IF_ELSE, op0 ? op0->result : 0, ifLabel, 0);
	
	return tacJoin(tacJoin(tacJoin(tacJoin(op0, newTac), op1), ifTac), op2);
}

TAC_NODE* makeWhile(TAC_NODE* expression, TAC_NODE* cmd)
{
	HASH_NODE* beginLabel = makeLabel();
	HASH_NODE* endLabel = makeLabel();
	
	TAC_NODE* loopBegin = tacCreate(TAC_LABEL, beginLabel, 0, 0);
	TAC_NODE* loopEnd = tacCreate(TAC_LABEL, endLabel, 0, 0);
	TAC_NODE* jmpBegin = tacCreate(TAC_JMP, beginLabel,0, 0);
	TAC_NODE* jmpEnd = tacCreate(TAC_JMP, expression ? expression->result : 0, endLabel, 0);
	
	return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(loopBegin, expression), jmpEnd), cmd), jmpBegin), loopEnd);
	
}

TAC_NODE* makeFuncDeclaration(HASH_NODE* name, TAC_NODE* param, TAC_NODE* cmd)
{
	TAC_NODE* beginFunc = tacCreate(TAC_BEGIN, name, 0, 0);
	TAC_NODE* endFunc = tacCreate(TAC_END, name, 0, 0);
	
	return tacJoin(tacJoin(tacJoin(beginFunc, param), cmd), endFunc);
}

TAC_NODE* makeVectorDec(HASH_NODE* name, TAC_NODE* type, TAC_NODE* size)
{
	TAC_NODE* newTac = tacCreate(TAC_VECTOR_DEC, name, size ? size->result : 0, 0);
	
	return tacJoin(tacJoin(size, newTac), type);
}

TAC_NODE* makeArgs(TAC_NODE* exp, TAC_NODE* next)
{
	TAC_NODE* newTac;

	if(exp == 0 && next == 0)
		return 0;
	else
		newTac = tacCreate(TAC_ARGS, exp ? exp->result : 0, 0, 0);
	
	return tacJoin(next, newTac);
}

TAC_NODE* makePrintList(TAC_NODE* arg1, TAC_NODE* next)
{
	TAC_NODE* newTac;
	TAC_NODE* aux;
	
	if(arg1 != 0)
		if(arg1->type != TAC_PRINT_LIST)
		{
			aux = tacCreate(TAC_PRINT_LIST, arg1->result, 0, 0);
			return tacJoin(next, aux);
		}
		else if (arg1->type == TAC_PRINT_LIST)
			return tacJoin(next,arg1);

	//newTac = tacCreate(TAC_PRINT_LIST, arg1->result, 0, 0);
	return 0;
}

TAC_NODE* generateCode(AST_NODE* node)
{
	int i;
	TAC_NODE* result[MAX_CHILDREN];
	
	for(i = 0; i < MAX_CHILDREN; i++)
		result[i] = 0;
	
	if(node != 0)
	{
		for(i = 0; i < MAX_CHILDREN; i++)
		{
			result[i] = generateCode(node->children[i]);
			//printTAC(result[i]);
		}
			
		
		switch(node->type)
		{
			case AST_SYMBOL:			if(node->symbol != 0)
									return tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
								break;
			case AST_VECTOR:			if(node->symbol != 0)
									return makeVector(node->symbol, result[0]);
								break;
			case AST_SUM:				return makeOp(TAC_SUM, result[0], result[1]);
			case AST_SUB:				return makeOp(TAC_SUB, result[0], result[1]);
			case AST_MUL:				return makeOp(TAC_MUL, result[0], result[1]);
			case AST_DIV:				return makeOp(TAC_DIV, result[0], result[1]);
			case AST_GE:				return makeOp(TAC_GE, result[0], result[1]);
			case AST_LE:				return makeOp(TAC_LE, result[0], result[1]);
			case AST_GT:				return makeOp(TAC_GT, result[0], result[1]);
			case AST_LT:				return makeOp(TAC_LT, result[0], result[1]);
			case AST_EQ:				return makeOp(TAC_EQ, result[0], result[1]);
			case AST_AND:				return makeOp(TAC_AND, result[0], result[1]);
			case AST_OR:				return makeOp(TAC_OR, result[0], result[1]);
			case AST_NOT: 				return makeNot(result[0]);
			case AST_EXPRESSION_BRACKETS:		return result[0];
			case AST_FUNC_CALL:			if (node->symbol != 0)
									return makeFuncCall(node->symbol, result[0]);
								break;
			case AST_VAR_ATTRIBUTION:		if(node->symbol != 0)
									return tacJoin(result[0], tacCreate(TAC_MOVE, node->symbol, result[0] ? result[0]->result : 0, 0));
								break;
			case AST_VECTOR_ATTRIBUTION:		if (node->symbol != 0)
									return makeVectorAttrib(node->symbol, result[0], result[1]);
								break;
			case AST_VAR_DECLARATION:		if (node->symbol != 0)
									return makeVarDeclaration(node->symbol, result[0], result[1]);
									//return tacCreate(TAC_VAR_DEC, node->symbol, node->children[1] ? node->children[1]->symbol : 0, 0);
								break;
			case AST_VECTOR_DECLARATION:		if (node->symbol != 0)
									return makeVectorDec(node->symbol, result[0], result[1]);
								break;
			case AST_VEC_DEC_INIT:			if (node->symbol != 0)
									return  makeVectorDeclaration(node->symbol, node->children[1] ? node->children[1]->symbol : 0, result[2] ? result[2] : 0, result[0]);
								break;
			case AST_ARG_LIST:			return makeArgs(result[0], result[1]);
			case AST_LIT_LIST:			return tacJoin(result[0], result[1]);
			case AST_PARAM_LIST:			if(node->symbol != 0)
									return tacJoin(result[0], makeParam(node->symbol, result[0], result[1]));
								break;
			case AST_PRINT_LIST:			//return makePrintList(result[0], result[1]);
								return tacJoin(tacJoin(result[0],tacCreate(TAC_PRINT_LIST,node->symbol,0,0)),result[1]);
								return tacJoin(result[0]->type == TAC_PRINT_LIST ? result[0] : tacCreate(TAC_PRINT_LIST, result[0]->result, 0, 0), result[1] ? tacCreate(TAC_PRINT_LIST, result[1]->result, 0, 0) : 0);
								if (node->symbol != 0)
									return tacJoin(tacCreate(TAC_PRINT_LIST, node->symbol, result[0] ? result[0]->result : 0, 0), result[0]);
								else
									return tacJoin(result[0], result[1]);
			case AST_IF:				return makeIf(result[0], result[1]);
			case AST_IF_ELSE:			return makeIfElse(result[0], result[1], result[2]);
			case AST_WHILE:				return makeWhile(result[0], result[1]);
			case AST_READ:				if(node->symbol != 0)
									return tacCreate(TAC_READ, node->symbol, 0, 0);
			case AST_PRINT:				return tacJoin(result[0], tacCreate(TAC_PRINT, result[0] ? result[0]->result : 0, 0, 0));
			case AST_RETURN:			return tacCreate(TAC_RETURN, result[0] ? result[0]->result : 0, 0, 0);
			case AST_CMD_BLOCK:			return result[0];
			case AST_CMD_LIST:			return tacJoin(result[0], result[1]);
			case AST_FUNC_DECLARATION:		if (node->symbol != 0)
									return makeFuncDeclaration(node->symbol, result[1] ? result[1] : 0, result[2] ? result[2] : 0);
								break;
			case AST_CHAR:				return tacCreate(TAC_CHAR, 0, 0, 0);
								break;
			case AST_INT:				return tacCreate(TAC_INT, 0, 0, 0);
								break;
			case AST_FLOAT:				return tacCreate(TAC_FLOAT, 0, 0, 0);
								break;
			/*case AST_PROG_ELEMENTS:			return tacJoin(result[0], result[1]);
								break;
			case AST_PROGRAM:			return result[0];
								break;*/
			case AST_LIT_INT:			return tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
								break;
			default:				return tacJoin(tacJoin(tacJoin(result[0], result[1]), result[2]), result[3]);
								break;
		}
	}
	else
		return 0;	
}



