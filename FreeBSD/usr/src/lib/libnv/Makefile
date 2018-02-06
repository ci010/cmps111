# $FreeBSD: releng/11.1/lib/libnv/Makefile 313538 2017-02-10 07:32:40Z ngie $

PACKAGE=lib${LIB}
SHLIBDIR?= /lib

.include <src.opts.mk>

LIB=	nv
SHLIB_MAJOR= 0

.PATH: ${SRCTOP}/sys/contrib/libnv ${SRCTOP}/sys/sys
CFLAGS+=-I${SRCTOP}/sys -I${.CURDIR}

SRCS=	dnvlist.c
SRCS+=	msgio.c
SRCS+=	nvlist.c
SRCS+=	nvpair.c

.if ${MK_TESTS} != "no"
SUBDIR+=	tests
.endif

.include <bsd.lib.mk>
