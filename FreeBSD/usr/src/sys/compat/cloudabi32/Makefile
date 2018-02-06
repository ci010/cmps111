# $FreeBSD: releng/11.1/sys/compat/cloudabi32/Makefile 304564 2016-08-21 16:01:30Z ed $

all:
	@echo "make sysent only"

sysent: cloudabi32_sysent.c cloudabi32_syscall.h cloudabi32_proto.h \
    cloudabi32_syscalls.c cloudabi32_systrace_args.c

cloudabi32_sysent.c cloudabi32_syscall.h cloudabi32_proto.h \
    cloudabi32_syscalls.c cloudabi32_systrace_args.c: \
    ../../kern/makesyscalls.sh ../../contrib/cloudabi/syscalls32.master \
    syscalls.conf
	sh ../../kern/makesyscalls.sh ../../contrib/cloudabi/syscalls32.master \
	    syscalls.conf
