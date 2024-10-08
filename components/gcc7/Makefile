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
# Copyright (c) 2014, 2019, Oracle and/or its affiliates. All rights reserved.
#
export PARFAIT_BUILD=no
BUILD_BITS= 64
COMPILER= gcc
include ../../make-rules/shared-macros.mk

COMPONENT_NAME=		gcc
COMPONENT_VERSION=	7.3.0
COMPONENT_PROJECT_URL=	https://gcc.gnu.org/
COMPONENT_ARCHIVE_HASH= \
    sha256:fa06e455ca198ddc11ea4ddf2a394cf7cfb66aa7e0ab98cc1184189f1d405870
COMPONENT_ARCHIVE_URL=	https://ftp.gnu.org/gnu/gcc/$(COMPONENT_SRC)/$(COMPONENT_ARCHIVE)
COMPONENT_SIG_URL=	$(COMPONENT_ARCHIVE_URL).sig
COMPONENT_BUGDB=        utility/gnu-compiler
COMPONENT_ANITYA_ID=	6502

TPNO=			40886

# Use gcc's default flags instead of userland's.
CFLAGS=
CPPFLAGS=
CXXFLAGS=

include $(WS_MAKE_RULES)/common.mk

PYTHON_VERSION = 3.7

# The GNU compiler wants the GNU utilities.
PATH=$(GNUBIN):$(USRBINDIR):$(USRDIR)/perl5/bin

PARCH =         $(MACH:i386=x86_64-pc)
GNU_ARCH =      $(PARCH:sparc=sparcv9-sun)-solaris$(SOLARIS_VERSION)

PKG_MACROS +=   GNU_ARCH="$(GNU_ARCH)"

VERSION_NUMBERS = $(subst ., ,$(COMPONENT_VERSION))
GCC_VERSION=$(word 1,$(VERSION_NUMBERS)).$(word 2,$(VERSION_NUMBERS))
GCC_MAJOR_VERSION=$(word 1,$(VERSION_NUMBERS))
PKG_MACROS +=	GCC_MAJOR_VERSION=$(GCC_MAJOR_VERSION)
PKG_MACROS +=	GCC_VERSION=$(GCC_VERSION)
PKG_MACROS +=	GCC_BASEDIR=$(CONFIGURE_PREFIX)

# direct binding causes various testsuite failures
LD_B_DIRECT=

# /usr/lib/ld/map.noexdata destroys Intel
LD_MAP_NOEXDATA.i386=

# /usr/lib/ld/map.noexbss destroys SPARC
LD_MAP_NOEXBSS.sparc=

# for some reason the fixincludes target fails with bash on Solaris.
CONFIG_SHELL = /bin/sh
MAKESHELL = /bin/sh
COMPONENT_BUILD_ENV += SHELL=$(CONFIG_SHELL)
COMPONENT_BUILD_ENV += MAKESHELL=$(MAKESHELL)

CONFIGURE_PREFIX =	/usr/gcc/$(GCC_MAJOR_VERSION)
CONFIGURE_INFODIR =	$(CONFIGURE_PREFIX)/share/info

# We are going to build the 64-bit version of the compiler and tools, but we
# want it to install in the normal .../{bin|lib}/... locations.  GCC will
# take care of building and putting 32/64 bit objects in the right place
# under there.
CONFIGURE_BINDIR.64 = $(CONFIGURE_PREFIX)/bin
CONFIGURE_LIBDIR.64 = $(CONFIGURE_PREFIX)/lib

CONFIGURE_OPTIONS +=	--infodir=$(CONFIGURE_INFODIR)
CONFIGURE_OPTIONS +=	--libexecdir=$(CONFIGURE_PREFIX)/lib
CONFIGURE_OPTIONS +=	--enable-languages="c,c++,fortran,objc"
CONFIGURE_OPTIONS +=	--enable-shared
CONFIGURE_OPTIONS +=	--enable-initfini-array
CONFIGURE_OPTIONS +=	--disable-rpath
CONFIGURE_OPTIONS +=	--with-system-zlib
CONFIGURE_OPTIONS +=	--with-build-config=no
CONFIGURE_OPTIONS +=	--with-gmp-include=$(shell pkg-config --variable=includedir libgmp)
CONFIGURE_OPTIONS +=	--with-mpfr-include=$(shell pkg-config --variable=includedir libmpfr)
CONFIGURE_OPTIONS +=	--without-gnu-ld --with-ld=$(USRBINDIR)/ld
CONFIGURE_OPTIONS +=	--with-gnu-as --with-as=$(GNUBIN)/as

# If the compiler used to build matches the compiler being built, there is no
# need for a 3 stage build.
ifneq ($(shell $(CC) --version | grep $(COMPONENT_VERSION)),)
CONFIGURE_OPTIONS +=    --disable-bootstrap
else
COMPONENT_BUILD_TARGETS=bootstrap
endif

CONFIGURE_OPTIONS +=	BOOT_CFLAGS='-g -O2'

# This is the target and it must be last
CONFIGURE_OPTIONS +=    $(GNU_ARCH)


CONFIGURE_ENV     += PYTHON="$(PYTHON)"

# compile python modules
COMPONENT_POST_INSTALL_ACTION = \
	( gfind $(PROTO_DIR)$(CONFIGURE_PREFIX) -name '*.py' | \
	  xargs -n 1 dirname | sort -u | \
	  xargs $(PYTHON) -m compileall )


COMPONENT_PRE_TEST_ENV += TCL_LIBRARY="$(USRLIBDIR)/tcl8.5"

# We don't have DejaGNU in S11.3.
ifneq ($(OS_VERSION), 11.3)
  COMPONENT_PRE_TEST_ENV += DEJAGNULIBS="$(USRSHAREDIR)/dejagnu"
endif

#
# Run the tests and generate a summary report, then output the summary
# report into the results file. Note that list of reported tests is sorted
# to allow parallel test run.
#
# To ensure that all tests that are expected to pass actually
# pass, we have to increase the stacksize limit to at least
# 16MB. Otherwise we'll get spurious failures in the test
# harness (gcc.c-torture/compile/limits-exprparen.c and others).
# With the soft stacksize limit set to 16384 we get reasonably good
# test results.
#
COMPONENT_PRE_TEST_ACTION += \
	(cd $(COMPONENT_TEST_DIR) ; \
	 ulimit -Ss 16385 ; \
	 $(ENV) $(COMPONENT_PRE_TEST_ENV) \
	        $(GMAKE) -k -i $(JOBS:%=-j%) check check-target ; \
	 $(FIND) . -name  '*.sum' | while read f; do \
	        gsed -e '1,/^Running target unix/p' -e  'd' $f > $f.2; \
	        gsed -e '/^Running target unix/,/Summary ===$/p' -e  'd' $f | grep  '^.*: ' | sort -k 2 >> $f.2; \
	        gsed -e '/Summary ===$/,$p' -e  'd' $f >> $f.2; \
	        mv $f.2 $f; done; \
	 $(GMAKE) mail-report.log)

COMPONENT_TEST_CMD = $(CAT)
COMPONENT_TEST_TARGETS = mail-report.log

# Master test results are different between x86 and SPARC.
COMPONENT_TEST_MASTER = \
	$(COMPONENT_TEST_RESULTS_DIR)/results-$(MACH).master

REQUIRED_PACKAGES += developer/test/dejagnu
REQUIRED_PACKAGES += library/gmp
REQUIRED_PACKAGES += library/isl
REQUIRED_PACKAGES += library/mpc
REQUIRED_PACKAGES += library/mpfr
REQUIRED_PACKAGES += library/zlib
REQUIRED_PACKAGES += runtime/tcl-8
REQUIRED_PACKAGES += shell/bash
REQUIRED_PACKAGES += shell/ksh93
REQUIRED_PACKAGES += system/library/math
REQUIRED_PACKAGES += system/library/gcc/gcc-c-runtime
REQUIRED_PACKAGES += system/library/gcc/gcc-c++-runtime
