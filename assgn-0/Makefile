wc : wc.o
	cc -o wc wc.o

wc.o : wc.c
	cc -c wc.c

test:
	wc test-empty.in
	wc test-1w.in
	wc test-2w.in

.PHONY: test