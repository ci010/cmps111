# $FreeBSD: releng/11.1/lib/libclang_rt/ubsan_standalone_cxx/Makefile 310618 2016-12-26 20:36:37Z dim $

.include <bsd.init.mk>

LIB=		clang_rt.ubsan_standalone_cxx-${CRTARCH}

CXXFLAGS+=	-frtti

SRCS+=		ubsan/ubsan_handlers_cxx.cc
SRCS+=		ubsan/ubsan_type_hash.cc
SRCS+=		ubsan/ubsan_type_hash_itanium.cc
SRCS+=		ubsan/ubsan_type_hash_win.cc

.include <bsd.lib.mk>
