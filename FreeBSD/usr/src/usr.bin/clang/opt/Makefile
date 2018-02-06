# $FreeBSD: releng/11.1/usr.bin/clang/opt/Makefile 310618 2016-12-26 20:36:37Z dim $

PROG_CXX=	opt

SRCDIR=		tools/opt
SRCS+=		AnalysisWrappers.cpp
SRCS+=		BreakpointPrinter.cpp
SRCS+=		GraphPrinters.cpp
SRCS+=		NewPMDriver.cpp
SRCS+=		PassPrinters.cpp
SRCS+=		PrintSCC.cpp
SRCS+=		opt.cpp

LIBADD+=	z

.include "../llvm.prog.mk"
