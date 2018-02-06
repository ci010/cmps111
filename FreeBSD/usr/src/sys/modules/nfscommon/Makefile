# $FreeBSD: releng/11.1/sys/modules/nfscommon/Makefile 319182 2017-05-30 04:11:12Z ngie $

.PATH: ${SRCTOP}/sys/fs/nfs
KMOD=	nfscommon
SRCS=	vnode_if.h \
	nfs_commonacl.c \
	nfs_commonkrpc.c \
	nfs_commonport.c \
	nfs_commonsubs.c \
	opt_inet6.h \
	opt_kgssapi.h \
	opt_nfs.h \
	opt_ufs.h

.include <bsd.kmod.mk>
