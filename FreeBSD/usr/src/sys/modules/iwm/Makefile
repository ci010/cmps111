# $FreeBSD: releng/11.1/sys/modules/iwm/Makefile 319182 2017-05-30 04:11:12Z ngie $

.PATH:  ${SRCTOP}/sys/dev/iwm

KMOD=	if_iwm
# Main driver
SRCS=	if_iwm.c if_iwm_binding.c if_iwm_util.c if_iwm_phy_db.c
SRCS+=	if_iwm_mac_ctxt.c if_iwm_phy_ctxt.c if_iwm_time_event.c
SRCS+=	if_iwm_power.c if_iwm_scan.c if_iwm_led.c
# bus layer
SRCS+=	if_iwm_pcie_trans.c
SRCS+=	device_if.h bus_if.h pci_if.h opt_wlan.h

CFLAGS+= -DIWM_DEBUG

.include <bsd.kmod.mk>
