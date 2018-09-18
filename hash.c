/*
	Etapa 1
	Nome: Laurien Santin e Rodrigo Oliveira
	Usuário: ~lsantin
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

HASH_NODE* Table[HASH_SIZE];

void hashInit()
{
	int i;
	
	for (i = 0; i < HASH_SIZE; i++)
		Table[i] = 0;
}

int hashAddress(char *text)
{
	int i;
	int address = 1;
	for (i = 0; i < strlen(text); i++)
		address = (address * text[i]) % HASH_SIZE + 1;
	return address - 1;
}

HASH_NODE* hashInsert(int type, char *text)
{
	HASH_NODE* newNode;
	int address = hashAddress(text);
	
	if ((newNode = hashFind(text)) != NULL)
		return newNode;
	else
	{
		newNode = (HASH_NODE*) calloc (1, sizeof(HASH_NODE));
		newNode->text = malloc (strlen(text) + 1);
		
		strcpy(newNode->text, text);
		newNode->type = type;
		newNode->next = Table[address];
		Table[address] = newNode;
		
		return newNode;
	}
}

HASH_NODE* hashFind(char *text)
{
	int address = hashAddress(text);
	HASH_NODE* node = Table[address];
	
	for(node; node; node = node->next)
		if (node->text == text)
			return node;
	return NULL;
}

void hashPrint()
{
	int i = 0;
	HASH_NODE* node;
	
	for (i; i < HASH_SIZE; i++)
	{
		node = Table[i];
		printf("Address: %d\n", i);
		for(node; node; node = node->next)
			printf("\tType: %d\tText: %s", node->type, node->text);
	}
}
