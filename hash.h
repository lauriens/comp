/*
	Etapa 1
	Nomes: Laurien Santin
	Usuário: ~lsantin
*/
#ifndef HASH_H
#define HASH_H
#define HASH_SIZE 997

#define NATUREZA_ESCALAR 01
#define NATUREZA_VETORIAL 02
#define NATUREZA_FUNCIONAL 03
#define DATATYPE_INT 04
#define DATATYPE_FLOAT 05
#define DATATYPE_BOOL 06

#define SYMBOL_LABEL 10
#define SYMBOL_TEMP 11

typedef struct hash_node
{
	int type;
	char *text;
	struct hash_node *next;
} HASH_NODE;

void hashInit();
int hashAddress(char *text);
HASH_NODE* hashInsert(int type, char *text);
HASH_NODE* hashFind(char *text);
void hashPrint();

int isRunning();
int getLineNumber();
void initMe();

HASH_NODE* makeTemp();
HASH_NODE* makeLabel();

#endif
