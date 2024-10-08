#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright (c) 2015, 2019, Oracle and/or its affiliates. All rights reserved.
#
BUILD_BITS= 64
# Must use gcc because of 21090206
COMPILER= gcc

include ../../../make-rules/shared-macros.mk

COMPONENT_NAME=		php
COMPONENT_VERSION=	7.1.32
UL_PHP_MINOR_VERSION=	7.1
COMPONENT_PROJECT_URL=	http://php.net/
COMPONENT_ARCHIVE=	$(COMPONENT_SRC).tar.xz
COMPONENT_ARCHIVE_HASH=\
    sha256:7f38b5bdaae3184d325a8c70e86c010afcc33651d15faafe277a0db6d2ea2741
COMPONENT_ARCHIVE_URL=	http://php.net/get/$(COMPONENT_ARCHIVE)/from/this/mirror
COMPONENT_ANITYA_ID=	3627

TPNO=			53983
TPNO_IMAP=		32942

include $(WS_MAKE_RULES)/common.mk
include $(PHP_TOP_DIR)/php.mk

# NOTES: See http://php.net/manual/en/install.unix.apache2.php
# The contents of files/php7.1.conf would normally go into Apache's httpd.conf
# but we put it in a separate file for our convenience.
# See http://php.net/manual/en/configuration.file.php
# The contents of files/extensions.ini would normally go into php.ini file
# but is in a separate file for our convenience.
# Both of these files are hand crafted.

# need GCC_ROOT/bin for configure, usr/sbin for dtrace 
export PATH=$(GCC_ROOT)/bin:$(USRBINDIR):$(USRSBINDIR):$(GNUBIN)

# We patch configure.in to allow building just modules (no SAPI's), e.g. intl.
# Since we are sharing the source, regenerate configure here.
COMPONENT_PREP_ACTION += (cd $(SOURCE_DIR) ; autoreconf) ;

include enable-extensions.mk

CONFIGURE_OPTIONS += --enable-dtrace

# Without this, the build seems to interrogate GCC for potential library
# search path information and then embed it into the RUNPATH of selected ELF
# files it creates.  This includes GCC private directories
CONFIGURE_OPTIONS += --disable-rpath

ETCPHPDIR= $(ETCDIR)/php/$(UL_PHP_MINOR_VERSION)
VARPHPDIR= $(VARDIR)/php/$(UL_PHP_MINOR_VERSION)
# .conf files go under /etc, not /usr
CONFIGURE_OPTIONS +=	--sysconfdir=$(ETCPHPDIR)
# var doesn't go under /usr either!
CONFIGURE_OPTIONS +=	--localstatedir=$(VARPHPDIR)
# Nor does php.ini and its friends.
CONFIGURE_OPTIONS +=	--with-config-file-path=$(ETCPHPDIR)
CONFIGURE_OPTIONS +=	--with-config-file-scan-dir=$(ETCPHPDIR)/conf.d
# Nor does PEAR (default: $PREFIX/lib/php)
CONFIGURE_OPTIONS +=	--with-pear=$(VARPHPDIR)/pear
# No debug php's.  They won't build anyways.
CONFIGURE_OPTIONS +=	--disable-phpdbg
CONFIGURE_OPTIONS +=	--disable-phpdbg-webhelper
CONFIGURE_OPTIONS +=	--disable-phpdbg-debug
# if debug is desired do this
# https://bugs.php.net/bugs-generating-backtrace.php
# CONFIGURE_OPTIONS +=    --enable-debug

# icu's switch to g++
# 21090206 icu should build C++ to C++11
# causes this in php forcing php to switch also
# 23003672 PHP fails all or most tests
CXXFLAGS += -std=c++11
CONFIGURE_OPTIONS += CXXFLAGS="$(CXXFLAGS)"

# Use openldap
# This is good enough to fool configure
CPPFLAGS += -I$(USRINCDIR)/openldap
LDFLAGS += -lldap_r
# but ensure "make" gets the point
COMPONENT_BUILD_ARGS += LDAP_SHARED_LIBADD="-lldap_r"

# Disable SSLv2 and SSLv3 for security reasons
CFLAGS += -DOPENSSL_NO_SSL2
CFLAGS += -DOPENSSL_NO_SSL3

PROTOUSRPHPVERDIR = $(PROTO_DIR)/$(CONFIGURE_PREFIX)

# Adapted from PHP 5.3 Makefile.
# Creates alternate versions of the scripts needed for building extensions
# separately, since the generated versions of these scripts hardcode
# configuration for a PHP that is installed on the system.
COMPONENT_POST_INSTALL_ACTION += \
	( set -e; \
	mkdir -p proto-scripts; \
	cat $(BUILD_DIR_64)/scripts/phpize | \
	 sed -e "s,^\#!/bin/sh,\#!$(CONFIG_SHELL)," | \
	 sed -e "s%^prefix=.*%prefix=$(PROTOUSRPHPVERDIR)%" | \
	 sed -e "s%^includedir=.*%includedir=$(PROTOUSRPHPVERDIR)/include/php%" | \
	 sed -e "s%^phpdir=.*%phpdir=$(SOURCE_DIR)%"  | \
	 sed -e "s%phpdir/phpize.m4%phpdir/scripts/phpize.m4%" | \
	 sed -e "s%$PHP_AUTOCONF   || exit 1%$PHP_AUTOCONF -f   || exit 1%" | \
	 sed -e "s%$PHP_AUTOHEADER || exit 1%$PHP_AUTOHEADER -f || exit 1%" | \
	 sed -e 's%(cd "$$phpdir" && cp $$FILES_BUILD "$$builddir"/build)%(cd "$$phpdir/build" \&\& cp $$FILES_BUILD "$$builddir"/build)%' | \
	 sed -e "s%^builddir=.*%builddir=$(BUILD_DIR_64)%" \
	 > proto-scripts/phpize-proto ; \
	chmod 755 proto-scripts/phpize-proto ) ; \
	( set -e; \
	 cat $(BUILD_DIR_64)/scripts/php-config | \
	 sed -e "s,^\#! /bin/sh,\#!$(CONFIG_SHELL)," | \
	 sed -e "s%^prefix=.*%prefix=$(PROTOUSRPHPVERDIR)%" | \
	 sed -e "s%^include_dir=.*%include_dir=$(PROTOUSRPHPVERDIR)/include/php%" \
	 > proto-scripts/php-config-proto ; \
	chmod 755 proto-scripts/php-config-proto ) ;

# sapi/fpm/tests/002.phpt leaves php-fpm running
COMPONENT_PRE_TEST_ACTION += (pkill -u `id -u` php-fpm; exit 0);
# test failures; keep going
COMPONENT_TEST_ARGS += -k -i
# different results for different architectures
COMPONENT_TEST_MASTER = $(COMPONENT_TEST_RESULTS_DIR)/results-$(MACH)-$(BITS).master
COMPONENT_TEST_TRANSFORMS += \
    '-e "/^make/d" ' \
    '-e "/^TEST RESULT SUMMARY$$/,\$$p" ' \
    '-e "/.*/d" '

CLEAN_PATHS += proto-scripts

# test results are unstable between runs but still useful for humans.
$(SKIP_TEST_AT_TOP_LEVEL)

clobber::
	cd ../uw-imap; gmake clobber

# Add directory for generate check
GENERATE_PROTO_DIRS += files^files

REQUIRED_PACKAGES += developer/icu
REQUIRED_PACKAGES += library/libxml2
REQUIRED_PACKAGES += library/security/openssl
REQUIRED_PACKAGES += system/dtrace
REQUIRED_PACKAGES += system/library/gcc/gcc-c-runtime
REQUIRED_PACKAGES += system/library/gcc/gcc-c++-runtime
REQUIRED_PACKAGES += system/library/math
REQUIRED_PACKAGES += system/management/snmp/net-snmp
REQUIRED_PACKAGES += system/network/ldap/openldap
REQUIRED_PACKAGES += text/gnu-sed
