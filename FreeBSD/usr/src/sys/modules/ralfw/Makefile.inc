# $FreeBSD: releng/11.1/sys/modules/ralfw/Makefile.inc 319182 2017-05-30 04:11:12Z ngie $

#
# Common rules for building firmware.  Note this gets auto-included
# by the subdir Makefile's as a consequence of included bsd.kmod.mk.
#
KMOD=	${IMG}fw
_FIRM=	${IMG}.fw

CLEANFILES+=	${_FIRM}

FIRMWS=	${_FIRM}:${KMOD}

${_FIRM}: ${SRCTOP}/sys/contrib/dev/ral/${_FIRM}.uu
	uudecode -p $? > ${.TARGET}
