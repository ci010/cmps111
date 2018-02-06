#	$OpenBSD: Makefile,v 1.13 2014/05/12 19:11:19 espie Exp $
# $FreeBSD: releng/11.1/usr.bin/m4/Makefile 319186 2017-05-30 04:27:05Z ngie $

# -DEXTENDED 
# 	if you want the paste & spaste macros.

.include <src.opts.mk>

PROG=	m4
CFLAGS+=-DEXTENDED -I${.CURDIR} -I${SRCTOP}/lib/libopenbsd
LIBADD=	y l m openbsd

NO_WMISSING_VARIABLE_DECLARATIONS=

SRCS=	eval.c expr.c look.c main.c misc.c gnum4.c trace.c parser.y tokenizer.l

WARNS=	3

tokenizer.o: parser.h

CLEANFILES+=	parser.c parser.h tokenizer.o

.if ${MK_TESTS} != "no"
SUBDIR+= tests
.endif

.include <bsd.prog.mk>
