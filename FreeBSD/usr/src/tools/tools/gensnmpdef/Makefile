# $FreeBSD: releng/11.1/tools/tools/gensnmpdef/Makefile 312101 2017-01-14 00:39:58Z ngie $

.PATH:	${SRCTOP}/contrib/bsnmp/gensnmpdef

PROG=	gensnmpdef

SRCS=	gensnmpdef.c

MAN=	gensnmpdef.1

LOCALBASE?=	/usr/local

BINDIR=	${LOCALBASE}/bin

MANDIR=	${LOCALBASE}/man/man

CFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib

LDADD+=	-lsmi

WARNS?=	6

.include <bsd.prog.mk>
