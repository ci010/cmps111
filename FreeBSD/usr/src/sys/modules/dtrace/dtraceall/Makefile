# $FreeBSD: releng/11.1/sys/modules/dtrace/dtraceall/Makefile 319182 2017-05-30 04:11:12Z ngie $

SYSDIR?=	${SRCTOP}/sys

KMOD=		dtraceall
SRCS=		dtraceall.c opt_compat.h opt_nfs.h

CFLAGS+=	-I${SYSDIR}

.if !defined(KERNBUILDDIR)
.if ${MACHINE_ARCH} == "amd64"
opt_compat.h:
	echo "#define COMPAT_FREEBSD32 1" >> ${.TARGET}
.endif
.endif

.include <bsd.kmod.mk>

CFLAGS+=	-include ${SYSDIR}/cddl/compat/opensolaris/sys/debug_compat.h
