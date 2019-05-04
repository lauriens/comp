# Members: Laurien Santin

etapa5: lex.yy.o main.o tac.o hash.o y.tab.o ast.o
	gcc -o etapa5 -g -Wall lex.yy.o main.o hash.o tac.o y.tab.o ast.o
hash.o:	hash.c
	gcc -c hash.c
ast.o: ast.c
	gcc -c ast.c
tac.o:	tac.c
	gcc -c tac.c
main.o: main.c
	gcc -c main.c
y.tab.c: parser.y
	yacc -d -v parser.y
lex.yy.o: lex.yy.c 
	gcc -c lex.yy.c 
lex.yy.c: scanner.l y.tab.c
	lex scanner.l
y.tab.o: y.tab.c
	gcc -c y.tab.c
clean:
	rm lex.yy.c lex.yy.o main.o hash.o y.tab.h y.tab.c y.tab.o etapa5 ast.o tac.o
