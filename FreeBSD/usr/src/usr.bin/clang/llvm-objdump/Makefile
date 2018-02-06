# $FreeBSD: releng/11.1/usr.bin/clang/llvm-objdump/Makefile 316423 2017-04-02 17:24:58Z dim $

PROG_CXX=	llvm-objdump
MAN=

SRCDIR=		tools/llvm-objdump
SRCS+=		COFFDump.cpp
SRCS+=		ELFDump.cpp
SRCS+=		MachODump.cpp
SRCS+=		WasmDump.cpp
SRCS+=		llvm-objdump.cpp

LIBADD+=	z

.include "../llvm.prog.mk"
