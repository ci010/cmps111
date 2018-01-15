wc : wc.o
	cc -o wc wc.o

wc.o : wc.c
	cc -c wc.c

clean:
	rm wc wc.o

test:
	./wc $(wildcard *.in)

.PHONY: test