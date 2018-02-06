# $FreeBSD: releng/11.1/lib/libclang_rt/asan_cxx/Makefile 310618 2016-12-26 20:36:37Z dim $

.include <bsd.init.mk>

LIB=		clang_rt.asan_cxx-${CRTARCH}

CXXFLAGS+=	-frtti

SRCS+=		asan/asan_new_delete.cc
SRCS+=		ubsan/ubsan_handlers_cxx.cc
SRCS+=		ubsan/ubsan_type_hash.cc
SRCS+=		ubsan/ubsan_type_hash_itanium.cc
SRCS+=		ubsan/ubsan_type_hash_win.cc

.include <bsd.lib.mk>
