#	@(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.1/usr.sbin/syslogd/Makefile 314823 2017-03-07 02:02:23Z ngie $

.include <src.opts.mk>

.PATH: ${SRCTOP}/usr.bin/wall

PROG=	syslogd
MAN=	syslog.conf.5 syslogd.8
SRCS=	syslogd.c ttymsg.c

LIBADD=	util

WARNS?= 3

.if ${MK_INET6_SUPPORT} != "no"
CFLAGS+= -DINET6
.endif

CFLAGS+= -I${SRCTOP}/usr.bin/wall

.include <bsd.prog.mk>
