# $FreeBSD: releng/11.1/sys/modules/i2c/iicbus/Makefile 319182 2017-05-30 04:11:12Z ngie $

.PATH:		${SRCTOP}/sys/dev/iicbus
KMOD		= iicbus
SRCS		= device_if.h bus_if.h iicbus_if.h iicbus_if.c \
		  iiconf.h iiconf.c iicbus.h iicbus.c

.include <bsd.kmod.mk>
