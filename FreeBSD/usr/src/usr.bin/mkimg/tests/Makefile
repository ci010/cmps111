# $FreeBSD: releng/11.1/usr.bin/mkimg/tests/Makefile 319560 2017-06-04 02:21:38Z ngie $

PACKAGE=	tests

_REBASE_SCRIPT=	mkimg_test
ATF_TESTS_SH=	${_REBASE_SCRIPT}

SOURCES!=	cd ${.CURDIR}; echo *.uu
${PACKAGE}FILES+=		${SOURCES:S,.gz.uu,,g}

${${PACKAGE}FILES}:
	uudecode -p ${.CURDIR}/${.TARGET}.gz.uu | gunzip -c > ${.TARGET}

CLEANFILES+=	${${PACKAGE}FILES}}

rebase: .PHONY
	(cd ${.CURDIR}; atf-sh ${_REBASE_SCRIPT}.sh rebase)

.include <bsd.test.mk>
