# $FreeBSD: releng/11.1/sys/boot/efi/loader/arch/amd64/Makefile.inc 306738 2016-10-05 22:04:22Z kib $

SRCS+=	amd64_tramp.S \
	start.S \
	framebuffer.c \
	elf64_freebsd.c \
	trap.c \
	exc.S

.PATH:	${.CURDIR}/../../i386/libi386
SRCS+=	nullconsole.c \
	comconsole.c

CFLAGS+=	-fPIC
LDFLAGS+=	-Wl,-znocombreloc
