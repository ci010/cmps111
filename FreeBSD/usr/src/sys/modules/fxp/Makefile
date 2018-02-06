# $FreeBSD: releng/11.1/sys/modules/fxp/Makefile 319182 2017-05-30 04:11:12Z ngie $

.PATH: ${SRCTOP}/sys/dev/fxp

KMOD=	if_fxp
SRCS=	device_if.h bus_if.h if_fxp.c inphy.c miibus_if.h miidevs.h pci_if.h

.include <bsd.kmod.mk>
