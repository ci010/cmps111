# $FreeBSD: releng/11.1/sys/modules/ipfw_nat64/Makefile 319182 2017-05-30 04:11:12Z ngie $

.PATH: ${SRCTOP}/sys/netpfil/ipfw/nat64

KMOD=	ipfw_nat64
SRCS=	ip_fw_nat64.c nat64_translate.c
SRCS+=	nat64lsn.c nat64lsn_control.c
SRCS+=	nat64stl.c nat64stl_control.c
SRCS+=	opt_ipfw.h

.include <bsd.kmod.mk>
