# $FreeBSD: releng/11.1/sys/modules/dtrace/dtrace_test/Makefile 319182 2017-05-30 04:11:12Z ngie $

SYSDIR?=	${SRCTOP}/sys

.PATH:		${SYSDIR}/cddl/dev/dtrace

KMOD=		dtrace_test
SRCS=		dtrace_test.c

SRCS+=		vnode_if.h

CFLAGS+=	-I${SYSDIR}

CFLAGS+=	-D_KERNEL

.include <bsd.kmod.mk>

CFLAGS+=	-include ${SYSDIR}/cddl/compat/opensolaris/sys/debug_compat.h
