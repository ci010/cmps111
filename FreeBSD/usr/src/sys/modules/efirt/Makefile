# $FreeBSD: releng/11.1/sys/modules/efirt/Makefile 319182 2017-05-30 04:11:12Z ngie $

.PATH: ${SRCTOP}/sys/${MACHINE}/${MACHINE}
.PATH: ${SRCTOP}/sys/dev/efidev

KMOD=	efirt
SRCS=	efirt.c efidev.c
SRCS+=  device_if.h bus_if.h

.include <bsd.kmod.mk>
