// Laurien Santin

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

AST_NODE* astInsert (int type, HASH_NODE* symbol, AST_NODE* ch0, AST_NODE* ch1, AST_NODE* ch2, AST_NODE* ch3)
{
	AST_NODE* newNode = NULL;
	
	newNode = calloc(1, sizeof(AST_NODE));
	
	newNode->type = type;
	newNode->lineNumber = getLineNumber();
	newNode->symbol = symbol;
	newNode->children[0] = ch0;
	newNode->children[1] = ch1;
	newNode->children[2] = ch2;
	newNode->children[3] = ch3;
	
	return newNode;
}

void printTree(AST_NODE* node)
{
	int i;
	if (node == 0)
		return;
	switch(node->type)
	{
		case AST_PROGRAM:		printTree(node->children[0]);
						break;
		case AST_PROG_ELEMENTS:		printTree(node->children[0]);
						printTree(node->children[1]);
						break;
		case AST_FUNC_DECLARATION:	printTree(node->children[0]);
						if(node->symbol != 0)
							fprintf(output, " %s d ", node->symbol->text);
						printTree(node->children[1]);
						fprintf(output, " b\n\t");
						printTree(node->children[2]);
						fprintf(output, "\n");
						break;
		case AST_VAR_DECLARATION:	printTree(node->children[0]);
						if (node->symbol != 0)
							fprintf(output, " %s = ", node->symbol->text);
						printTree(node->children[1]);
						fprintf(output, ";\n");
						break;
		case AST_VECTOR_DECLARATION:	printTree(node->children[0]);
						if(node->symbol != 0)
							fprintf(output, " %s q ", node->symbol->text);
						printTree(node->children[1]);
						fprintf(output, "p;\n");
						break;
		case AST_VEC_DEC_INIT:		printTree(node->children[0]);
						if(node->symbol != 0)
							fprintf(output, " %s q ", node->symbol->text);
						printTree(node->children[1]);
						fprintf(output, "p : ");
						printTree(node->children[2]);
						fprintf(output, ";\n");
						break;
		case AST_SYMBOL:		if (node->symbol != 0)
							fprintf(output, "%s", node->symbol->text);
						break;
		case AST_LIT_LIST:		printTree(node->children[0]);
						if(node->children[1] != 0)
						{
							fprintf(output, " ");
							printTree(node->children[1]);
						}	
						break;
		case AST_LITERAL:		printTree(node->children[0]);
						break;
		case AST_CMD_LIST:		printTree(node->children[0]);
						fprintf(output, ";\n");
						if (node->children[1])
						{
							printTree(node->children[1]);
							fprintf (output, ";\n");
						}
						break;
		case AST_PRINT_LIST:		if (node->symbol != 0)
						{
							fprintf(output, "%s", node->symbol->text);
							if (node->children[0] != 0)
							{
								fprintf(output, ", ");
								printTree(node->children[0]);
							}
						}
						else
						{
							printTree(node->children[0]);
							if (node->children[1] != 0)
							{
								fprintf(output, ", ");
								printTree(node->children[1]);
							}
						}
						break;
		case AST_PARAM_LIST:		printTree(node->children[0]);
						if (node->symbol != 0)
							fprintf(output, " %s", node->symbol->text);
						if (node->children[1] != 0)
						{
							fprintf(output, ", ");
							printTree(node->children[1]);
						}
						break;
		case AST_ARG_LIST:		printTree(node->children[0]);
						if(node->children[1])
						{
							fprintf(output, ", ");
							printTree(node->children[1]);
						}
						break;
		case AST_VAR_ATTRIBUTION:	if(node->symbol != 0)
						{
							fprintf (output, "%s", node->symbol->text);
							fprintf (output, " = ");
							printTree (node->children[0]);
							
						}
						break;
		case AST_VECTOR_ATTRIBUTION:	if(node->symbol != 0)
						{
							fprintf(output, "%s q ", node->symbol->text);
							printTree(node->children[1]);
							fprintf(output, " p = ");
							printTree(node->children[2]);
							
						}
						break;
		case AST_IF:			fprintf(output, "if ");
						printTree(node->children[0]);
						fprintf(output, " then\n\t");
						printTree(node->children[1]);
						break;
		case AST_IF_ELSE:		fprintf(output, "if ");
						printTree(node->children[0]);
						fprintf(output, " then\n\t");
						printTree(node->children[1]);
						fprintf(output, "\nelse\n\t");
						printTree(node->children[2]);
						break;
		case AST_WHILE:			fprintf(output, "while ");
						printTree(node->children[0]);
						fprintf(output, "\n");
						printTree(node->children[1]);
						break;
		case AST_READ:			if (node->symbol != 0)
							fprintf(output, "read %s", node->symbol->text);
						break;
		case AST_PRINT:			fprintf(output, "print ");
						printTree(node->children[0]);
						break;
		case AST_RETURN:		fprintf(output, "return ");
						printTree(node->children[0]);
						break;
		case AST_CMD_BLOCK:		fprintf(output, "{\n\t");
						printTree(node->children[0]);
						fprintf(output, "}");
						break;
		case AST_SUM:			printTree(node->children[0]);
						fprintf(output, " + ");
						printTree(node->children[1]);
						break;
		case AST_SUB:			printTree(node->children[0]);
						fprintf(output, " - ");
						printTree(node->children[1]);
						break;
		case AST_MUL:			printTree(node->children[0]);
						fprintf(output, " * ");
						printTree(node->children[1]);
						break;
		case AST_DIV:			printTree(node->children[0]);
						fprintf(output, " / ");
						printTree(node->children[1]);
						break;
		case AST_LT:			printTree(node->children[0]);
						fprintf(output, " < ");
						printTree(node->children[1]);
						break;
		case AST_GT:			printTree(node->children[0]);
						fprintf(output, " > ");
						printTree(node->children[1]);
						break;
		case AST_LE:			printTree(node->children[0]);
						fprintf(output, " <= ");
						printTree(node->children[1]);
						break;
		case AST_GE:			printTree(node->children[0]);
						fprintf(output, " >= ");
						printTree(node->children[1]);
						break;
		case AST_EQ:			printTree(node->children[0]);
						fprintf(output, " == ");
						printTree(node->children[1]);
						break;
		case AST_OR:			printTree(node->children[0]);
						fprintf(output, " or ");
						printTree(node->children[1]);
						break;
		case AST_AND:			printTree(node->children[0]);
						fprintf(output, " and ");
						printTree(node->children[1]);
						break;
		case AST_NOT:			fprintf(output, "not ");
						printTree(node->children[0]);
						break;
		case AST_EXPRESSION_BRACKETS:	fprintf(output, "d");
						printTree(node->children[0]);
						fprintf(output, "b");
						break;
		case AST_FUNC_CALL:		if(node->symbol != 0)
						{
							fprintf(output, "%s d ", node->symbol->text);
							printTree(node->children[0]);
							fprintf(output, " b");
						}
						break;
		case AST_CHAR:			fprintf(output, "char");
						break;
		case AST_INT:			fprintf(output, "int");
						break;
		case AST_FLOAT:			fprintf(output, "float");
						break;
		case AST_LIT_INT:		if(node->symbol != 0)
						{
							fprintf(output, "%s ", node->symbol->text);
						}
						break;
	}
	/*if(node->symbol != 0)
		fprintf(stderr, "%s \n", node->symbol->text);
	else
		fprintf(stderr, "\n");
		
	for(i = 0; i < MAX_CHILDREN; i++)
		printTree(node->children[i], level + 1);*/
}

void printCode(AST_NODE* root)
{
	
}
