# $FreeBSD: releng/11.1/sys/boot/forth/Makefile.inc 281009 2015-04-03 02:27:40Z jkim $

FILES+=	beastie.4th
FILES+=	brand.4th
FILES+=	brand-fbsd.4th
FILES+=	check-password.4th
FILES+=	color.4th
FILES+=	delay.4th
FILES+=	frames.4th
FILES+=	loader.4th
FILES+=	loader.conf
FILES+=	loader.help
FILES+=	logo-beastie.4th
FILES+=	logo-beastiebw.4th
FILES+=	logo-fbsdbw.4th
FILES+=	logo-orb.4th
FILES+=	logo-orbbw.4th
FILES+=	menu.4th
FILES+=	menu-commands.4th
FILES+=	menusets.4th
FILES+=	screen.4th
FILES+=	shortcuts.4th
FILES+=	support.4th
FILES+=	version.4th
FILESDIR_loader.conf=	/boot/defaults