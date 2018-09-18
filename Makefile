# Members: Laurien Santin e Rodrigo Oliveira

etapa2: lex.yy.o main.o hash.o y.tab.o
	gcc -o etapa2 lex.yy.o main.o hash.o y.tab.o

hash.o:	hash.c
	gcc -c hash.c

y.tab.c: parser.y
	yacc -d -v parser.y
lex.yy.o: lex.yy.c 
	gcc -c lex.yy.c 
lex.yy.c: scanner.l y.tab.c
	lex scanner.l
y.tab.o: y.tab.c
	gcc -c y.tab.c
main.o: main.c
	gcc -c main.c
clean:
	rm lex.yy.c lex.yy.o main.o hash.o y.tab.h y.tab.c y.tab.o etapa2
