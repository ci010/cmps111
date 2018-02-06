# $FreeBSD: releng/11.1/sys/modules/tpm/Makefile 319182 2017-05-30 04:11:12Z ngie $

.PATH:	${SRCTOP}/sys/dev/tpm

KMOD=	tpm

SRCS=	tpm.c bus_if.h device_if.h
#Bus specific stuff.
SRCS+=	tpm_isa.c tpm_acpi.c isa_if.h opt_acpi.h acpi_if.h

.include <bsd.kmod.mk>
