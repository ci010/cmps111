# $FreeBSD: releng/11.1/sys/modules/cxgb/Makefile 319182 2017-05-30 04:11:12Z ngie $

SYSDIR?=${SRCTOP}/sys
.include "${SYSDIR}/conf/kern.opts.mk"

SUBDIR= cxgb
SUBDIR+= cxgb_t3fw
SUBDIR+= ${_tom}
SUBDIR+= ${_iw_cxgb}

.if ${MACHINE_CPUARCH} == "amd64" || ${MACHINE_CPUARCH} == "i386"
_tom=		tom
.if ${MK_OFED} != "no" || defined(ALL_MODULES)
_iw_cxgb=	iw_cxgb
.endif
.endif

.include <bsd.subdir.mk>
