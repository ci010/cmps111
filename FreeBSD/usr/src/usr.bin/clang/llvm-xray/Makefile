# $FreeBSD: releng/11.1/usr.bin/clang/llvm-xray/Makefile 311310 2017-01-04 21:47:39Z dim $

PROG_CXX=	llvm-xray
MAN=

SRCDIR=		tools/llvm-xray
SRCS+=		llvm-xray.cc
SRCS+=		xray-extract.cc
SRCS+=		xray-registry.cc

LIBADD+=	z

.include "../llvm.prog.mk"
