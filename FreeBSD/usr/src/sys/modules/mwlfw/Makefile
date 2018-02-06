# $FreeBSD: releng/11.1/sys/modules/mwlfw/Makefile 319182 2017-05-30 04:11:12Z ngie $

KMOD=	mw88W8363fw
FIRMWS=	mw88W8363.fw:mw88W8363fw mwlboot.fw:mwlboot

CLEANFILES+= mw88W8363.fw mwlboot.fw

mw88W8363.fw: ${SRCTOP}/sys/contrib/dev/mwl/mw88W8363.fw.uu
	uudecode -p $? > ${.TARGET}

mwlboot.fw: ${SRCTOP}/sys/contrib/dev/mwl/mwlboot.fw.uu
	uudecode -p $? > ${.TARGET}

.include <bsd.kmod.mk>
