# $FreeBSD: releng/11.1/sys/modules/ucycom/Makefile 319182 2017-05-30 04:11:12Z ngie $

S=	${SRCTOP}/sys
.PATH: $S/dev/usb

KMOD=	ucycom
SRCS=	ucycom.c opt_usb.h device_if.h bus_if.h usbdevs.h

.include <bsd.kmod.mk>
