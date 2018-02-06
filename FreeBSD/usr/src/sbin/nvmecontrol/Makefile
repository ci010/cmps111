# $FreeBSD: releng/11.1/sbin/nvmecontrol/Makefile 319185 2017-05-30 04:17:22Z ngie $

PACKAGE=runtime
PROG=	nvmecontrol
SRCS=	nvmecontrol.c devlist.c firmware.c identify.c logpage.c	\
	perftest.c reset.c nvme_util.c power.c
MAN=	nvmecontrol.8

.PATH:	${SRCTOP}/sys/dev/nvme

.include <bsd.prog.mk>
