# $FreeBSD: releng/11.1/lib/libsbuf/Makefile 313538 2017-02-10 07:32:40Z ngie $

PACKAGE=lib${LIB}
LIB=		sbuf
SHLIBDIR?= 	/lib
SRCS=		subr_prf.c subr_sbuf.c

SHLIB_MAJOR	= 6

SYMBOL_MAPS=    ${.CURDIR}/Symbol.map
VERSION_DEF=	${.CURDIR}/Version.def

.PATH:	${SRCTOP}/sys/kern

.include <bsd.lib.mk>
