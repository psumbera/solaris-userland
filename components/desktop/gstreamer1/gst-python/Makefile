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
# Copyright (c) 2016, 2019, Oracle and/or its affiliates. All rights reserved.
#

BUILD_BITS =		32_and_64
# All components using gobject-introspection must be built with gcc as
# gobject-introspection does not produce correct results when using Studio.
COMPILER =		gcc
include ../../../../make-rules/shared-macros.mk

COMPONENT_NAME=		gst-python
COMPONENT_VERSION=	1.10.3
COMPONENT_PROJECT_URL=	https://gstreamer.freedesktop.org/
COMPONENT_ARCHIVE=	$(COMPONENT_SRC).tar.xz
COMPONENT_ARCHIVE_HASH= \
	sha256:bdfa2d06dfe0ce68f638b04fed6890db506416c1dcf1279e83458269d719a4e8
COMPONENT_ARCHIVE_URL=	https://gstreamer.freedesktop.org/src/$(COMPONENT_NAME)/$(COMPONENT_ARCHIVE)
COMPONENT_SIG_URL=	$(COMPONENT_ARCHIVE_URL).asc
COMPONENT_BUGDB=	gnome/multimedia
COMPONENT_ANITYA_ID=	3881

TPNO=			33712

# Requires pygobject-3 which is currently only built for Python 2.7
PYTHON_VERSIONS= 2.7
PYTHON_VERSION = 2.7

include $(WS_MAKE_RULES)/common.mk

# Use the build area of gstreamer
GST_BASE = $(WS_COMPONENTS)/desktop/gstreamer1/gstreamer/build/prototype/$(MACH)
PKG_CONFIG_PATHS += $(GST_BASE)$(USRLIB)/pkgconfig/

CONFIGURE_OPTIONS += --disable-static

# The build process modifies the shebang line to use the version of Python
# automatically found; to avoid using the current system default, one is
# explicitly specified here.
CONFIGURE_ENV += PYTHON=$(PYTHON)

# Ensure Python modules are installed into vendor package directories.
CONFIGURE_ENV += am_cv_python_pythondir=$(PYTHON_VENDOR_PACKAGES)
CONFIGURE_ENV += am_cv_python_pyexecdir=$(PYTHON_VENDOR_PACKAGES)

CONFIGURE_OPTIONS.64 += --with-pygi-overrides-dir=$(PYTHON_VENDOR_PACKAGES_BASE)/gi/overrides/64

# for the tests
unexport SHELLOPTS
COMPONENT_TEST_MASTER = $(COMPONENT_TEST_RESULTS_DIR)/results-all.master

REQUIRED_PACKAGES += library/desktop/gstreamer-1
REQUIRED_PACKAGES += library/glib2
REQUIRED_PACKAGES += library/python/pygobject-3-27
REQUIRED_PACKAGES += runtime/python-27
