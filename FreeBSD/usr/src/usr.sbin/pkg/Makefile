# $FreeBSD: releng/11.1/usr.sbin/pkg/Makefile 319119 2017-05-29 10:15:41Z ngie $

PROG=	pkg
SRCS=	pkg.c dns_utils.c config.c
MAN=	pkg.7

CFLAGS+=-I${SRCTOP}/contrib/libucl/include
.PATH:	${SRCTOP}/contrib/libucl/include
LIBADD=	archive fetch ucl sbuf crypto ssl

.include <bsd.prog.mk>
