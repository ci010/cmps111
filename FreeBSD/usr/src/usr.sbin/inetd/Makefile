#	@(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: releng/11.1/usr.sbin/inetd/Makefile 313203 2017-02-04 15:49:50Z ngie $

.include <src.opts.mk>

PROG=	inetd
MAN=	inetd.8
MLINKS=	inetd.8 inetd.conf.5
SRCS=	inetd.c builtins.c

WARNS?=	3
CFLAGS+= -DLOGIN_CAP
#CFLAGS+= -DSANITY_CHECK

.if ${MK_INET6_SUPPORT} != "no"
CFLAGS+= -DINET6
.endif

LIBADD=	util

.if ${MK_TCP_WRAPPERS} != "no"
CFLAGS+=	-DLIBWRAP
LIBADD+=	wrap
.endif

# XXX for src/release/picobsd
.if !defined(RELEASE_CRUNCH)
CFLAGS+= -DIPSEC
LIBADD+=	ipsec
.endif

.include <bsd.prog.mk>
