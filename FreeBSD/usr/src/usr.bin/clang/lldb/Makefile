# $FreeBSD: releng/11.1/usr.bin/clang/lldb/Makefile 310618 2016-12-26 20:36:37Z dim $

.include "${SRCTOP}/lib/clang/lldb.pre.mk"

PACKAGE=	lldb
PROG_CXX=	lldb
# Man page directory
.PATH:		${LLDB_SRCS}/docs

CFLAGS+=	-I${LLDB_SRCS}/include

SRCDIR=		tools/lldb/tools/driver
SRCS+=		Driver.cpp

.include "${SRCTOP}/lib/clang/clang.build.mk"

LIBDEPS+=	lldb
LIBDEPS+=	clang
LIBDEPS+=	llvm

.for lib in ${LIBDEPS}
DPADD+=		${OBJTOP}/lib/clang/lib${lib}/lib${lib}.a
LDADD+=		${OBJTOP}/lib/clang/lib${lib}/lib${lib}.a
.endfor

LIBADD+=	edit
LIBADD+=	panel
LIBADD+=	ncursesw
LIBADD+=	pthread
LIBADD+=	z

.include <bsd.prog.mk>
