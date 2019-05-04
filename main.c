#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

int main(int argc, char** argv)
{
	int token;
	extern FILE *yyin;
	FILE *output;
	
	if (argc < 3)
	{
		fprintf (stderr, "Forneca o nome do arquivo a ser analisado e do arquivo a ser gerado.\n");
		exit (1);
	}
	if (!(yyin = fopen(argv[1], "r")))
	{
		fprintf(stderr, "Erro na abertura do arquivo.\n");
		exit(2);
	}
	if (!(output = fopen(argv[2], "w+")))
	{
		fprintf(stderr, "Erro na abertura do arquivo de saída.\n");
		exit (2);
	}
	
	initMe();
	yyparse();
	fclose(output);
	return 0;
}

