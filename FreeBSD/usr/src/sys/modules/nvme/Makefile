# $FreeBSD: releng/11.1/sys/modules/nvme/Makefile 319182 2017-05-30 04:11:12Z ngie $

.PATH: ${SRCTOP}/sys/dev/nvme

KMOD = nvme

SRCS =	nvme.c			\
	nvme_ctrlr.c		\
	nvme_ctrlr_cmd.c	\
	nvme_ns.c		\
	nvme_ns_cmd.c		\
	nvme_qpair.c		\
	nvme_sysctl.c		\
	nvme_test.c		\
	nvme_util.c		\
				\
	bus_if.h		\
	device_if.h		\
	opt_cam.h		\
	pci_if.h

.include <bsd.kmod.mk>
