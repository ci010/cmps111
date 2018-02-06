# $FreeBSD: releng/11.1/usr.sbin/ntp/ntptime/Makefile 319191 2017-05-30 05:15:09Z ngie $

.PATH: ${SRCTOP}/contrib/ntp/util

PROG=	ntptime
MAN=

CFLAGS+= -I${SRCTOP}/contrib/ntp/include \
	-I${SRCTOP}/contrib/ntp/lib/isc/include/ \
	-I${SRCTOP}/contrib/ntp/lib/isc/unix/include/ \
	-I${SRCTOP}/contrib/ntp/lib/isc/pthreads/include \
	-I${.CURDIR:H}

LIBADD=	ntp pthread

.include <bsd.prog.mk>
