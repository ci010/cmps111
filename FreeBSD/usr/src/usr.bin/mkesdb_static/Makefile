# $FreeBSD: releng/11.1/usr.bin/mkesdb_static/Makefile 319186 2017-05-30 04:27:05Z ngie $

.PATH: ${SRCTOP}/lib/libc/iconv ${SRCTOP}/usr.bin/mkesdb

PROG=	mkesdb_static
SRCS=	citrus_bcs.c citrus_db_factory.c citrus_db_hash.c \
	citrus_lookup_factory.c
MAN=
NO_SHARED= yes
NO_WMISSING_VARIABLE_DECLARATIONS=

build-tools: mkesdb_static

.include "${SRCTOP}/usr.bin/mkesdb/Makefile.inc"
.include <bsd.prog.mk>

${PROG}: ${BUILD_TOOLS_META}
