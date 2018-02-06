# $FreeBSD: releng/11.1/usr.sbin/zic/zic/Makefile 319191 2017-05-30 05:15:09Z ngie $

.PATH: ${SRCTOP}/contrib/tzcode/zic

PROG=	zic
MAN=	zic.8
SRCS=	zic.c ialloc.c scheck.c

CFLAGS+= -DTM_GMTOFF=tm_gmtoff -DTM_ZONE=tm_zone -DSTD_INSPIRED -DPCTS
CFLAGS+= -DHAVE_LONG_DOUBLE -DTZDIR=\"${SHAREDIR}/zoneinfo\" -Demkdir=mkdir
CFLAGS+= -DHAVE_STRERROR -DHAVE_UNISTD_H
CFLAGS+= -I${.CURDIR:H} -I${SRCTOP}/contrib/tzcode/stdtime

WARNS?=	2

.include <bsd.prog.mk>
