# $FreeBSD: releng/11.1/sys/modules/sound/driver/csa/Makefile 319182 2017-05-30 04:11:12Z ngie $

.PATH: ${SRCTOP}/sys/dev/sound/pci

KMOD=	snd_csa
SRCS=	device_if.h bus_if.h pci_if.h
SRCS+=	csa.c csapcm.c

.include <bsd.kmod.mk>
