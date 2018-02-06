# $FreeBSD: releng/11.1/usr.sbin/bhyveload/Makefile 319119 2017-05-29 10:15:41Z ngie $

PROG=	bhyveload
SRCS=	bhyveload.c
MAN=	bhyveload.8
PACKAGE=	bhyve

LIBADD=	vmmapi

WARNS?=	3

CFLAGS+=-I${SRCTOP}/sys/boot/userboot

.include <bsd.prog.mk>
