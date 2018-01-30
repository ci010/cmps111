
myshell : myshell.c lex.yy.c
	cc -g -O0 -Wall -Wextra -std=c99 -o myshell myshell.c lex.yy.c -lfl

lex.yy.c : lex.l
	flex lex.l

clean :
	rm lex.yy.c myshell myshell.core

test :
	./myshell