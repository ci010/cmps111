#	@(#)Makefile	8.1 (Berkeley) 5/31/93
# $FreeBSD: releng/11.1/usr.bin/factor/Makefile 319186 2017-05-30 04:27:05Z ngie $

.include <src.opts.mk>

PROG=	factor
SRCS=	factor.c pr_tbl.c
CFLAGS+=-I${SRCTOP}/usr.bin/primes

.if ${MK_OPENSSL} != "no"
CFLAGS+=-DHAVE_OPENSSL
LIBADD+=crypto
.endif

MAN=	factor.6
MLINKS+=factor.6 primes.6
.PATH:	${SRCTOP}/usr.bin/primes

.include <bsd.prog.mk>
