# $FreeBSD: releng/11.1/kerberos5/libexec/kpasswdd/Makefile 319183 2017-05-30 04:13:06Z ngie $

PROG=	kpasswdd
MAN=	kpasswdd.8
CFLAGS+=-I${KRB5DIR}/lib/roken -I${.CURDIR:H:H}/lib/libhdb ${LDAPCFLAGS}
LIBADD=	kadm5srv hdb krb5 roken vers asn1
DPADD=	${LDAPDPADD}
LDADD=	${LDAPLDADD}
LDFLAGS=${LDAPLDFLAGS}

.include <bsd.prog.mk>

.PATH: ${KRB5DIR}/kpasswd
