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
# Copyright (c) 2011, 2019, Oracle and/or its affiliates. All rights reserved.
#
BUILD_BITS= 64
COMPILER= gcc
include ../../make-rules/shared-macros.mk

# Upcoming squid version update will resolve GCC 9 build issues.
GCC_ROOT= /usr/gcc/7

PATH=$(USRBINDIR):$(GNUBIN):$(USRSBINDIR):$(USRDIR)/perl5/bin

COMPONENT_NAME=         squid
COMPONENT_VERSION=      4.8
TPNO=			52929
COMPONENT_PROJECT_URL=  http://www.squid-cache.org/
COMPONENT_ARCHIVE=      $(COMPONENT_SRC).tar.xz
COMPONENT_ARCHIVE_HASH= sha256:78cdb324d93341d36d09d5f791060f6e8aaa5ff3179f7c949cd910d023a86210
COMPONENT_ARCHIVE_URL=  $(COMPONENT_PROJECT_URL)Versions/v4/$(COMPONENT_ARCHIVE)
COMPONENT_SIG_URL=    $(COMPONENT_ARCHIVE_URL).asc
COMPONENT_ANITYA_ID=	4880

# Enable adiheap and adistack security extensions
ADIHEAP_MODE =	$(ADIHEAP_ENABLE)
ADISTACK_MODE =	$(ADISTACK_ENABLE)

TEST_TARGET= $(TEST_64)
include $(WS_MAKE_RULES)/common.mk

CFLAGS += -I$(USRINCDIR)/kerberosv5
CFLAGS += $(CPP_LARGEFILES)

CONFIGURE_LIBDIR.64 =	$(CONFIGURE_PREFIX)/lib

# Squid has own prefix
CONFIGURE_PREFIX =	$(USRDIR)/squid

CONFIGURE_OPTIONS += --libexecdir=$(CONFIGURE_PREFIX)/libexec
CONFIGURE_OPTIONS += --localstatedir=$(VARDIR)/squid
CONFIGURE_OPTIONS += --with-pidfile=/system/volatile/squid.pid
CONFIGURE_OPTIONS += --sharedstatedir=$(VARDIR)/squid
CONFIGURE_OPTIONS += --sysconfdir=$(ETCDIR)/squid
CONFIGURE_OPTIONS += --mandir=$(CONFIGURE_PREFIX)/man
CONFIGURE_OPTIONS += --sbindir=$(CONFIGURE_PREFIX)/sbin
CONFIGURE_OPTIONS += --enable-arp-acl
CONFIGURE_OPTIONS += --enable-auth-basic='DB,NCSA,NIS,LDAP,PAM,getpwnam,POP3,SMB,SMB_LM,SASL'
CONFIGURE_OPTIONS += --enable-cache-digests
CONFIGURE_OPTIONS += --enable-carp
CONFIGURE_OPTIONS += --enable-coss-aio-ops
CONFIGURE_OPTIONS += --enable-delay-pools
CONFIGURE_OPTIONS += --enable-auth-digest='file,LDAP'
CONFIGURE_OPTIONS += --enable-external-acl-helpers='file_userip,unix_group,LDAP_group,wbinfo_group'
CONFIGURE_OPTIONS += --enable-follow-x-forwarded-for
CONFIGURE_OPTIONS += --enable-forward-log
CONFIGURE_OPTIONS += --enable-forw-via-db
CONFIGURE_OPTIONS += --enable-htcp
CONFIGURE_OPTIONS += --enable-icmp
CONFIGURE_OPTIONS += --enable-large-cache-files
CONFIGURE_OPTIONS += --enable-multicast-miss
CONFIGURE_OPTIONS += --enable-auth-negotiate='kerberos'
CONFIGURE_OPTIONS += --enable-auth-ntlm='SMB_LM,fake'
CONFIGURE_OPTIONS += --enable-ntlm-fail-open
CONFIGURE_OPTIONS += --enable-removal-policies='heap,lru'
CONFIGURE_OPTIONS += --enable-snmp
CONFIGURE_OPTIONS += --enable-storeio='aufs,diskd,ufs'
CONFIGURE_OPTIONS += --enable-x-accelerator-vary
CONFIGURE_OPTIONS += --with-aio
CONFIGURE_OPTIONS += --with-aufs-threads=8
CONFIGURE_OPTIONS += --with-build-environment=POSIX_V6_LP64_OFF64
# Due to the current licensing of squid and openssl, gnutls/nettle
# must be enabled and used instead of openssl for tls and/or other
# crypto support.
CONFIGURE_OPTIONS += --with-gnutls
CONFIGURE_OPTIONS += --with-nettle
CONFIGURE_OPTIONS += --with-pthreads
CONFIGURE_OPTIONS += --disable-arch-native
CONFIGURE_OPTIONS += PERL=$(PERL)

# Appends the following line to the end of the squid.conf configuration
# file. This line is necessary for running squid for testing. 
# This starts up squid as user webservd. 
COMPONENT_POST_INSTALL_ACTION += \
	(cd $(PROTO_DIR)/etc/squid ; \
 	echo '\ncache_effective_user webservd' >> squid.conf)

# Adjust '#!/usr/bin/perl'
PERL_SCRIPTS= $(PROTOUSRDIR)/squid/libexec/helper-mux.pl

# Clean test-suite directory to get the same results regardless of
# how many times 'gmake test' command was called (some tests are
# skipped in the second and further runs if not cleaned).
COMPONENT_PRE_TEST_ACTION += (cd $(@D)/test-suite; gmake clean);

COMPONENT_TEST_TRANSFORMS += \
	'-n ' \
	'-e "/=====/p" ' \
	'-e "/TOTAL/p" ' \
	'-e "/SKIP/p" ' \
	'-e "/PASS/p" ' \
	'-e "/FAIL/p" ' \
	'-e "/Can not open file/d" ' \
	'-e "/Testsuite summary/p" ' \
	'-E -e "/^(\# )?ERROR/p" '

REQUIRED_PACKAGES += library/gnutls-3
REQUIRED_PACKAGES += library/libtool/libltdl
REQUIRED_PACKAGES += library/security/nettle
REQUIRED_PACKAGES += $(PERL_PKG)
REQUIRED_PACKAGES += security/kerberos-5
REQUIRED_PACKAGES += library/libxml2
REQUIRED_PACKAGES += library/expat
REQUIRED_PACKAGES += shell/ksh93
REQUIRED_PACKAGES += system/core-os
REQUIRED_PACKAGES += system/library/gcc/gcc-c-runtime
REQUIRED_PACKAGES += system/library/gcc/gcc-c++-runtime
REQUIRED_PACKAGES += system/library/math
REQUIRED_PACKAGES += system/library/pam-core
REQUIRED_PACKAGES += system/library/security/crypto
REQUIRED_PACKAGES += system/library/security/libsasl2
REQUIRED_PACKAGES += system/network
REQUIRED_PACKAGES += system/network/ldap/openldap
