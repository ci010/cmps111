# $FreeBSD: releng/11.1/usr.sbin/wpa/hostapd_cli/Makefile 319190 2017-05-30 04:45:11Z ngie $

.include "../Makefile.inc"

.PATH.c:${HOSTAPD_DISTDIR}

PROG=	hostapd_cli
SRCS=	common.c edit.c eloop.c hostapd_cli.c os_unix.c wpa_ctrl.c wpa_debug.c

CFLAGS+= -DCONFIG_CTRL_IFACE
CFLAGS+= -DCONFIG_CTRL_IFACE_UNIX

LIBADD+=	util

MAN=	hostapd_cli.8

.include <bsd.prog.mk>
