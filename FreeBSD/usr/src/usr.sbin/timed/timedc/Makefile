#	@(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.1/usr.sbin/timed/timedc/Makefile 319119 2017-05-29 10:15:41Z ngie $

.PATH: ${.CURDIR:H}/timed

PROG=	timedc
MAN=	timedc.8
SRCS=	cmds.c cmdtab.c timedc.c byteorder.c measure.c cksum.c
BINOWN=	root
BINMODE= 4555

WARNS?=	1

.include "../../Makefile.inc"
.include <bsd.prog.mk>
