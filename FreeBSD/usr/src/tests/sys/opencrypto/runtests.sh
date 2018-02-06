#!/bin/sh -
#
# Copyright (c) 2014 The FreeBSD Foundation
# All rights reserved.
#
# This software was developed by John-Mark Gurney under
# the sponsorship from the FreeBSD Foundation.
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# $FreeBSD: releng/11.1/tests/sys/opencrypto/runtests.sh 275732 2014-12-12 19:56:36Z jmg $
#

set -e

if [ ! -d /usr/local/share/nist-kat ]; then
	echo 'Skipping, nist-kat package not installed for test vectors.'
	exit 0
fi

if kldload aesni 2>/dev/null; then
	unloadaesni=1
fi

if kldload cryptodev 2>/dev/null; then
	unloadcdev=1
fi

# Run software crypto test
oldcdas=$(sysctl -e kern.cryptodevallowsoft)
sysctl kern.cryptodevallowsoft=1

python $(dirname $0)/cryptotest.py

sysctl "$oldcdas"

if [ x"$unloadcdev" = x"1" ]; then
	kldunload cryptodev
fi
if [ x"$unloadaesni" = x"1" ]; then
	kldunload aesni
fi
