# $FreeBSD: releng/11.1/usr.bin/mkcsmapper/Makefile.inc 319186 2017-05-30 04:27:05Z ngie $

.include <bsd.compiler.mk>

SRCS+=	lex.l yacc.y
CFLAGS+= -I${.CURDIR} -I${SRCTOP}/usr.bin/mkcsmapper \
	 -I${SRCTOP}/lib/libc/iconv \
	 -I${SRCTOP}/lib/libiconv_modules/mapper_std
CFLAGS.gcc+= --param max-inline-insns-single=64
