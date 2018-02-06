/* $FreeBSD: releng/11.1/lib/libelftc/elftc_version.c 320751 2017-07-06 18:30:52Z emaste $ */

#include <sys/types.h>
#include <libelftc.h>

const char *
elftc_version(void)
{
	return "elftoolchain r3561M";
}
