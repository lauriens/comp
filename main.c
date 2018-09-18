#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "y.tab.h"

int main(int argc, char** argv)
{
	int token;
	extern FILE *yyin;
	
	if (argc < 2)
	{
		fprintf (stderr, "Forneca o nome do arquivo a ser analisado.\n");
		exit (1);
	}
	if (!(yyin = fopen(argv[1], "r")))
	{
		fprintf(stderr, "Erro na abertura do arquivo.\n");
		exit(2);
	}
	
	initMe();
	yyparse();
	//hashPrint();
	return 0;
}

