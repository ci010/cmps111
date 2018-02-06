# $FreeBSD: releng/11.1/usr.sbin/ntp/libopts/Makefile 319191 2017-05-30 05:15:09Z ngie $

.PATH: ${SRCTOP}/contrib/ntp/sntp/libopts

LIB=	opts
INTERNALLIB=

SRCS=	libopts.c

CFLAGS+= -I${SRCTOP}/contrib/ntp/include \
	-I${SRCTOP}/contrib/ntp/sntp/libopts \
	-I${.CURDIR:H}

.include <bsd.lib.mk>
