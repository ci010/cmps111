#	@(#)Makefile	8.1 (Berkeley) 6/4/93
# $FreeBSD: releng/11.1/libexec/tftpd/Makefile 313231 2017-02-04 16:55:46Z ngie $

.include <src.opts.mk>

PROG=	tftpd
MAN=	tftpd.8
SRCS=	tftp-file.c tftp-io.c tftp-options.c tftp-transfer.c tftp-utils.c
SRCS+=	tftpd.c
WFORMAT=0

.if ${MK_TCP_WRAPPERS} != "no"
CFLAGS+=	-DLIBWRAP
LIBADD=	wrap
.endif

.include <bsd.prog.mk>
