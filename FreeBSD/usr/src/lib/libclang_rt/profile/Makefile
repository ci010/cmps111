# $FreeBSD: releng/11.1/lib/libclang_rt/profile/Makefile 314821 2017-03-07 02:01:02Z ngie $

.include <bsd.init.mk>

LIB=		clang_rt.profile-${CRTARCH}

# This is needed for --coverage
CFLAGS+=	-fvisibility=default

SRCS+=		profile/GCDAProfiling.c
SRCS+=		profile/InstrProfiling.c
SRCS+=		profile/InstrProfilingBuffer.c
SRCS+=		profile/InstrProfilingFile.c
SRCS+=		profile/InstrProfilingMerge.c
SRCS+=		profile/InstrProfilingMergeFile.c
SRCS+=		profile/InstrProfilingPlatformDarwin.c
SRCS+=		profile/InstrProfilingPlatformLinux.c
SRCS+=		profile/InstrProfilingPlatformOther.c
SRCS+=		profile/InstrProfilingRuntime.cc
SRCS+=		profile/InstrProfilingUtil.c
SRCS+=		profile/InstrProfilingValue.c
SRCS+=		profile/InstrProfilingWriter.c

.include <bsd.lib.mk>
