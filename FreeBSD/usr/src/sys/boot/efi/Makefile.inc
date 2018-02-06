# $FreeBSD: releng/11.1/sys/boot/efi/Makefile.inc 317140 2017-04-19 14:07:35Z andrew $

BINDIR?=	/boot

.if ${MACHINE_CPUARCH} == "i386"
CFLAGS+=        -march=i386
CFLAGS+=	-mno-aes
.endif

# Options used when building app-specific efi components
# See conf/kern.mk for the correct set of these
CFLAGS+=	-ffreestanding -Wformat -msoft-float ${CFLAGS_NO_SIMD}
LDFLAGS+=	-nostdlib

.if ${MACHINE_CPUARCH} == "amd64"
CFLAGS+=	-fshort-wchar
CFLAGS+=	-mno-red-zone
CFLAGS+=	-mno-aes
.endif

.if ${MACHINE_CPUARCH} == "aarch64"
CFLAGS+=	-fshort-wchar
CFLAGS+=	-fPIC
.endif

.if ${MACHINE_CPUARCH} == "arm"
CFLAGS+=	-fPIC
.endif

.include "../Makefile.inc"
