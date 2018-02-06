# $FreeBSD: releng/11.1/sys/modules/usb/runfw/Makefile 319182 2017-05-30 04:11:12Z ngie $

KMOD=	runfw
FIRMWS=	run.fw:runfw:1

CLEANFILES=	run.fw

run.fw: ${SRCTOP}/sys/contrib/dev/run/rt2870.fw.uu
	uudecode -p $? > ${.TARGET}

.include <bsd.kmod.mk>
