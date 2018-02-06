# $FreeBSD: releng/11.1/usr.bin/minigzip/Makefile 319186 2017-05-30 04:27:05Z ngie $

SRCDIR=	${SRCTOP}/contrib/zlib/test
.PATH:	${SRCDIR}

PROG=	minigzip

WARNS?=	5
CFLAGS+=-DUSE_MMAP
LIBADD=	z

.include <bsd.prog.mk>
