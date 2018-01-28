
myshell : myshell.c lex.yy.c
	cc -g -O0 -Wall -Wextra -std=c99 -o myshell myshell.c lex.yy.c -lfl

lex.yy.c : lex.l
	flex lex.l

clean :
	rm myshell.o lex.yy.o lex.yy.c myshell

test :
	./myshell