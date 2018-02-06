# $FreeBSD: releng/11.1/sys/modules/geom/geom_label/Makefile 319182 2017-05-30 04:11:12Z ngie $

.PATH: ${SRCTOP}/sys/geom/label

KMOD=	geom_label
SRCS=	g_label.c
SRCS+=	g_label_disk_ident.c
SRCS+=	g_label_ext2fs.c
SRCS+=	g_label_gpt.c
SRCS+=	g_label_iso9660.c
SRCS+=	g_label_msdosfs.c
SRCS+=	g_label_ntfs.c
SRCS+=	g_label_reiserfs.c
SRCS+=	g_label_ufs.c
SRCS+=	opt_geom.h

opt_geom.h:
	echo "#define GEOM_LABEL 1" > ${.TARGET}

.include <bsd.kmod.mk>
