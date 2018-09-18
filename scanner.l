%{
	/*
		Etapa 1
		Nomes: Laurien Santin e Rodrigo Oliveira
		Usuário: ~lsantin
	*/
	#include "hash.h"
	#include "y.tab.h"
	int lineNumber;
	int running = 0;
%}
digito		[0-9]
letra		[a|c|[e-o]|[r-z]|[A-Z]]
caracterEspecial [-|,|;|:|b|d|p|q|{|}|+|*|/|<|>|=|!|&|$|#]

%x COMMENT


%%

"char" 				{return KW_CHAR;}
"int" 				{return KW_INT;}
"float"				{return KW_FLOAT;}
"if" 				{return KW_IF;}
"then" 				{return KW_THEN;}
"else" 				{return KW_ELSE;}
"while"				{return KW_WHILE;}
"read" 				{return KW_READ;}
"return"			{return KW_RETURN;}
"print"				{return KW_PRINT;}

"or"				{return OPERATOR_OR;}
"and"				{return OPERATOR_AND;}
"not"				{return OPERATOR_NOT;}

"<="				{return OPERATOR_LE;}
">="				{return OPERATOR_GE;}
"=="				{return OPERATOR_EQ;}

{caracterEspecial}		{return yytext[0];}

"\n"				{lineNumber++;}

{digito}+			{yylval.symbol = hashInsert(LIT_INTEGER, yytext); return LIT_INTEGER;}
[0-9]+"\."[0-9]+		{yylval.symbol = hashInsert(LIT_FLOAT, yytext); return LIT_FLOAT;}
[a|c|e-o|r-z|A-Z|_][a|c|e-o|r-z|A-Z|0-9|_]* {yylval.symbol = hashInsert(TK_IDENTIFIER, yytext); return TK_IDENTIFIER;}

\"([^(\\.)\"]|\\.)*\"		{yylval.symbol = hashInsert(LIT_STRING, yytext); return LIT_STRING;}
"'"("\\".|[^'\n])"'"		{yylval.symbol = hashInsert(LIT_CHAR, yytext); return LIT_CHAR;}

[ \t]

"//".*
"/*" 				{BEGIN (COMMENT);}
<COMMENT>\n 			{lineNumber++;}		
<COMMENT>.
<COMMENT>"*/" 			{BEGIN (INITIAL);}			

.				{return TOKEN_ERROR;}

%%

void initMe()
{
	hashInit();
	running = 1;
	lineNumber = 1;
}

int isRunning()
{
	return running;
}

int getLineNumber()
{
	return lineNumber;
}

int yywrap()
{	
	return 1;
}

