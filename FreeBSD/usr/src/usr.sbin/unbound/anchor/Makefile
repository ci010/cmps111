# $FreeBSD: releng/11.1/usr.sbin/unbound/anchor/Makefile 319191 2017-05-30 05:15:09Z ngie $

# Vendor sources and generated files
LDNSDIR= ${SRCTOP}/contrib/ldns
UNBOUNDDIR= ${SRCTOP}/contrib/unbound
EXPATDIR= ${SRCTOP}/contrib/expat

.PATH: ${UNBOUNDDIR} ${UNBOUNDDIR}/smallapp ${UNBOUNDDIR}/doc

PROG=	unbound-anchor
SRCS=	unbound-anchor.c
CFLAGS+=	-I${UNBOUNDDIR} -I${LDNSDIR} -I${EXPATDIR}/lib
LIBADD=	unbound bsdxml ssl crypto pthread
MAN=	unbound-anchor.8

.include <bsd.prog.mk>
