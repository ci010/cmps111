wc : wc.o
	gcc -o wc wc.o

wc.o : wc.c
	gcc -c wc.c

test:
	wc test-empty.in
	wc test-1w.in
	wc test-2w.in

.PHONY: test