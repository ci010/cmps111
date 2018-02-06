#	@(#)Makefile	8.2 (Berkeley) 3/21/94
# $FreeBSD: releng/11.1/sys/kern/Makefile 315062 2017-03-11 08:39:24Z dchagin $

# Makefile for init_sysent

all:
	@echo "make sysent only"

sysent: init_sysent.c syscalls.c ../sys/syscall.h ../sys/syscall.mk \
../sys/sysproto.h

init_sysent.c syscalls.c systrace_args.c ../sys/syscall.h \
../sys/syscall.mk ../sys/sysproto.h: makesyscalls.sh syscalls.master \
capabilities.conf
	sh makesyscalls.sh syscalls.master
