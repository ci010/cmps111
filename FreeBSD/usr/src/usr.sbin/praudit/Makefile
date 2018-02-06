#
# $FreeBSD: releng/11.1/usr.sbin/praudit/Makefile 319119 2017-05-29 10:15:41Z ngie $
#

OPENBSMDIR=${SRCTOP}/contrib/openbsm
.PATH: ${OPENBSMDIR}/bin/praudit

PROG=	praudit
MAN=	praudit.1

WARNS?=	3

LIBADD=	bsm

.include <bsd.prog.mk>
