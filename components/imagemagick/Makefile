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
BUILD_BITS= 64_and_32
COMPILER=gcc
include ../../make-rules/shared-macros.mk

COMPONENT_NAME=			ImageMagick
COMPONENT_VERSION=		6.9.10
COMPONENT_MAJOR_VERSION=	$(shell echo $(COMPONENT_VERSION) | $(GSED) -e 's/\([0-9]\+\)\.[0-9]\+\.[0-9]\+/\1/')
COMPONENT_SUBVERSION=		68
HUMAN_VERSION=			$(COMPONENT_VERSION)-$(COMPONENT_SUBVERSION)
COMPONENT_PROJECT_URL=		https://www.imagemagick.org/
COMPONENT_SRC=			$(COMPONENT_NAME)-$(COMPONENT_VERSION)-$(COMPONENT_SUBVERSION)
COMPONENT_ARCHIVE=		$(COMPONENT_SRC).tar.gz
COMPONENT_ARCHIVE_HASH=	\
	sha256:bf6541ea8c6689d9d657065568e359fda75f773b42e6da085a188e857dac82d4
COMPONENT_ARCHIVE_URL=	$(COMPONENT_PROJECT_URL)/download/$(COMPONENT_ARCHIVE)
COMPONENT_SIG_URL=	$(COMPONENT_ARCHIVE_URL).asc
COMPONENT_BUGDB=	utility/imagemagick
COMPONENT_ANITYA_ID=	1372

TPNO=			54691

# Enable adiheap security extension.
# adistack fails with libgcc exception unwinding code.
ADIHEAP_MODE =	$(ADIHEAP_ENABLE)
ADISTACK_MODE =	$(ADISTACK_DISABLE)

include $(WS_MAKE_RULES)/common.mk

PATH=$(SPRO_VROOT)/bin:$(USRBINDIR):$(GNUBIN)

ifeq   ($(strip $(PARFAIT_BUILD)),yes)
CFLAGS += -U__i386__ -U__x86_64__
CXXFLAGS += -U__i386__ -U__x86_64__
endif

# IPS_COMPONENT_VERSION is by default set to $(COMPONENT_VERSION) but in
# this case we need to replace the '-' by '.'.
IPS_COMPONENT_VERSION=	$(COMPONENT_VERSION).$(COMPONENT_SUBVERSION)

# To get the Perl modules built it is necessary to copy the PerlMagick
# directory to the build directory
COMPONENT_PRE_CONFIGURE_ACTION += ($(CP) -R $(SOURCE_DIR)/PerlMagick $(@D))

# Don't use rpath, or relink!
COMPONENT_POST_CONFIGURE_ACTION = \
        (cd $(BUILD_DIR_$(BITS)) ; \
        $(GSED) -i \
	-e 's|^hardcode_libdir_flag_spec=.*|hardcode_libdir_flag_spec=""|g' \
	-e 's|need_relink=yes|need_relink=not_on_solaris|' \
	libtool)

LIBS += $(LIBSOCKET) $(LIBNSL)
CONFIGURE_ENV += LIBS="$(LIBS)"

# Prevent older versions of libMagick{Core,Wand}-6.Q16.so from the build
# machine from being found ahead of new versions from the proto area.
LD_OPTIONS_SO=		-L. -L../../magick/.libs
COMPONENT_BUILD_ENV +=	LD_OPTIONS_SO="$(LD_OPTIONS_SO)"

# To remove the compiler complaints about __restrict keyword,
# -D_magickcore_restrict should be used.
CPPFLAGS_USER += -D_magickcore_restrict=
CONFIGURE_ENV += CPPFLAGS_USER=$(CPPFLAGS_USER)

CONFIGURE_OPTIONS+= --with-modules
CONFIGURE_OPTIONS+= --enable-shared
CONFIGURE_OPTIONS+= --disable-static
CONFIGURE_OPTIONS+= --without-gvc
CONFIGURE_OPTIONS+= --disable-openmp
CONFIGURE_OPTIONS+= --without-openexr
CONFIGURE_OPTIONS+= --without-openjp2 # tests do not pass
# Using option 'with-fontconfig=no' so that the fonts/fonts path specified in
# configuration file 'type-ghostscript.xml' is being used.
CONFIGURE_OPTIONS+= --with-fontconfig=no
# Configure option 'with-gs-font-dir' is required because some build systems
# might not have ghostscript fonts installed.
CONFIGURE_OPTIONS+= --with-gs-font-dir="/usr/share/ghostscript/fonts"
CONFIGURE_OPTIONS+= --x-libraries=$(CONFIGURE_LIBDIR.$(BITS)) 
CONFIGURE_OPTIONS.32+= --without-perl
CONFIGURE_OPTIONS.64+= --with-perl=$(PERL)
CONFIGURE_OPTIONS.64+= --with-perl-options="INSTALLDIRS=vendor"

# Needed for "gmake test" to work successfully.  If SHELLOPTS is exported (as
# it is by the Userland Makefiles), then all shell options get exported to
# child invocations of bash, which results in test failures due to nounset and
# xtrace being set unexpectedly, and errors such as "$1: unbound variable" and
# diffs failing due to script tracing in output files.
unexport SHELLOPTS

# Tests hang waiting for input unless stdin is redirected.
COMPONENT_TEST_TARGETS = check < /dev/null

# Master test transforms
COMPONENT_TEST_TRANSFORMS += \
	'-n '\
	'-e "/TOTAL/p" ' \
	'-e "/SKIP/p" ' \
	'-e "/PASS/p" ' \
	'-e "/FAIL/p" ' \
	'-e "/ERROR/p" ' 

PKG_MACROS += COMPONENT_MAJOR_VERSION=$(COMPONENT_MAJOR_VERSION)
PKG_MACROS += COMPONENT_SUBVERSION=$(COMPONENT_SUBVERSION)
PKG_MACROS += PERL_PKG=$(PERL_PKG)

REQUIRED_PACKAGES += compress/bzip2
REQUIRED_PACKAGES += compress/xz
REQUIRED_PACKAGES += image/library/libjpeg
REQUIRED_PACKAGES += image/library/libpng14
REQUIRED_PACKAGES += image/library/libtiff
REQUIRED_PACKAGES += image/library/libwebp
REQUIRED_PACKAGES += library/desktop/cairo
REQUIRED_PACKAGES += library/desktop/pango
REQUIRED_PACKAGES += library/fftw-3
REQUIRED_PACKAGES += library/glib2
REQUIRED_PACKAGES += library/lcms2
REQUIRED_PACKAGES += library/libtool/libltdl
REQUIRED_PACKAGES += library/libxml2
REQUIRED_PACKAGES += library/zlib
REQUIRED_PACKAGES += print/filter/ghostscript/fonts/gnu-gs-fonts-std
REQUIRED_PACKAGES += shell/ksh93
REQUIRED_PACKAGES += system/library/c++-runtime
REQUIRED_PACKAGES += system/library/fontconfig
REQUIRED_PACKAGES += system/library/freetype-2
REQUIRED_PACKAGES += system/library/gcc/gcc-c++-runtime
REQUIRED_PACKAGES += system/library/gcc/gcc-c-runtime
REQUIRED_PACKAGES += system/library/math
REQUIRED_PACKAGES += system/library/openmp
REQUIRED_PACKAGES += x11/library/libx11
REQUIRED_PACKAGES += x11/library/libxext
