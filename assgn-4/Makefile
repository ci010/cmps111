all : fat_fuse fatcli utest

fat_fuse : fat_fuse.c fat.h
	cc fat_fuse.c -o fat_fuse `pkgconf fuse --cflags --libs`

fatcli: fatcli.c fat.h
	cc fatcli.c -o fatcli

utest : 
	cc utest.c -o utest
	./utest
	rm ./utest 

test : 
	./fatcli -f fatimg 1024
	mkdir -p fatdisk
	sudo ./fat_fuse fatdisk fatimg

stop :
	sudo umount ./fatdisk

clean :
	sudo umount ./fatdisk
	rm -r ./fatdisk
	rm fatimg
	rm fat_fuse
	rm *.core

.PHONY : test stop all utest clean
