#!/bin/sh
# $FreeBSD: releng/11.1/tests/sys/geom/class/uzip/conf.sh 293443 2016-01-08 21:38:26Z ngie $

class="uzip"
base=`basename $0`

uzip_test_cleanup()
{
	if [ -n "$mntpoint" ]; then
		umount $mntpoint
		rmdir $mntpoint
	fi
	geom_test_cleanup
}
trap uzip_test_cleanup ABRT EXIT INT TERM

. `dirname $0`/../geom_subr.sh

# NOTE: make sure $TMPDIR has been set by geom_subr.sh if unset [by kyua, etc]
mntpoint=$(mktemp -d tmp.XXXXXX) || exit
