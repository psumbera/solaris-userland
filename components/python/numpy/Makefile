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

COMPILER =		gcc
BUILD_BITS = 		32_and_64
include ../../../make-rules/shared-macros.mk

COMPONENT_NAME=		numpy
COMPONENT_VERSION=	1.14.2
COMPONENT_SRC=		$(COMPONENT_NAME)-$(COMPONENT_VERSION)
COMPONENT_ARCHIVE=	$(COMPONENT_SRC).zip
COMPONENT_ARCHIVE_HASH=	\
    sha256:facc6f925c3099ac01a1f03758100772560a0b020fb9d70f210404be08006bcb
COMPONENT_ARCHIVE_URL=	$(call pypi_url)
COMPONENT_PROJECT_URL=	http://www.numpy.org/
COMPONENT_BUGDB=	python-mod/numpy
COMPONENT_ANITYA_ID=	2509

TPNO=			42611

# This component is not yet built for Python 3.7
PYTHON_VERSIONS = $(WITHOUT_PYTHON3.7)

BUILD_STYLE= setup.py

# Have not been able to get consistent test results yet, needs more work
TEST_TARGET = $(SKIP_TEST)

include $(WS_MAKE_RULES)/common.mk

COMPONENT_BUILD_ARGS.studio 	+= --compiler=sun$(BITS) --fcompiler=sun$(BITS)
COMPONENT_BUILD_ARGS.gcc	+= --compiler=gnu --fcompiler=gnu95
COMPONENT_BUILD_ARGS 		+= $(COMPONENT_BUILD_ARGS.$(COMPILER)) -lm

studio_XBITS.i386.32 		+= -xarch=sse2

PYTHON_ENV +=   CC="$(CC) $(CC_BITS)"
PYTHON_ENV +=   PATH="$(SPRO_VROOT)/bin:$(PATH)"

PYTHON_SCRIPTS += $(PROTO_DIR)/$(PYTHON_VENDOR_PACKAGES_BASE)/numpy/*.py
PYTHON_SCRIPTS += $(PROTO_DIR)/$(PYTHON_VENDOR_PACKAGES_BASE)/numpy/*/*.py
PYTHON_SCRIPTS += $(PROTO_DIR)/$(PYTHON_VENDOR_PACKAGES_BASE)/numpy/*/*/*.py

# Merge 32-bit & 64-bit versions of header with different SIZEOF values
install: $(BUILD_DIR)/.merged-header
$(BUILD_DIR)/.merged-header:	PYTHON_VERSION=2.7

NUMPYCONFIG_H = $(PYTHON_VENDOR_PACKAGES_BASE)/numpy/core/include/numpy/_numpyconfig.h
$(BUILD_DIR)/.merged-header: $(BUILD_DIR)/$(MACH32)-2.7/.installed $(BUILD_DIR)/$(MACH64)-2.7/.installed
	-$(RM) $(PROTO_DIR)/$(NUMPYCONFIG_H)
	print '#include <sys/isa_defs.h>' > $(PROTO_DIR)/$(NUMPYCONFIG_H)
	$(GDIFF) -D _LP64 \
	    $(BUILD_DIR)/$(MACH32)-2.7/src.solaris-*.32bit-2.7/numpy/core/include/numpy/_numpyconfig.h \
	    $(BUILD_DIR)/$(MACH64)-2.7/src.solaris-*.64bit-2.7/numpy/core/include/numpy/_numpyconfig.h \
	    >> $(PROTO_DIR)/$(NUMPYCONFIG_H) ; \
	diffstatus=$$? ; \
	if (( $$diffstatus != 1 )) ; then exit $$diffstatus ; fi
	touch $@


COMPONENT_TEST_DIR  = $(COMPONENT_DIR)
COMPONENT_TEST_ARGS = -c 'import numpy; numpy.test()'
COMPONENT_TEST_ENV += F77="$(SPRO_VROOT)/bin/f77 -m$(BITS)"
COMPONENT_TEST_ENV += F90="$(SPRO_VROOT)/bin/f90 -m$(BITS)"
COMPONENT_TEST_TRANSFORMS += '-e "s|^\\(Ran [0-9]* tests\\) in .*s|\1|"'
COMPONENT_TEST_TRANSFORMS += '-e "s|$(PROTO_DIR)|\\$$(PROTO_DIR)|g"'

REQUIRED_PACKAGES += library/python/nose
REQUIRED_PACKAGES += system/library
REQUIRED_PACKAGES += system/library/math
