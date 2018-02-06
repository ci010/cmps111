# Makefile for syscall tables
#
# $FreeBSD: releng/11.1/sys/compat/svr4/Makefile 315062 2017-03-11 08:39:24Z dchagin $

all:
	@echo "make sysent only"

sysent:  svr4_sysent.c svr4_syscall.h svr4_proto.h

svr4_syscallnames.c svr4_sysent.c svr4_syscall.h svr4_proto.h: \
		../../kern/makesyscalls.sh syscalls.master syscalls.conf
	sh ../../kern/makesyscalls.sh syscalls.master syscalls.conf

