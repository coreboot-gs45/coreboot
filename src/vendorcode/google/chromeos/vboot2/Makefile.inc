##
## This file is part of the coreboot project.
##
## Copyright (C) 2014 The ChromiumOS Authors.  All rights reserved.
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
##

libverstage-generic-ccopts += -D__PRE_RAM__ -D__VERSTAGE__

bootblock-y += ../vboot_common.c
verstage-y += ../vboot_common.c
romstage-y += ../vboot_common.c
ramstage-y += ../vboot_common.c

bootblock-y += verstub.c
libverstage-y += verstub.c
bootblock-y += common.c
libverstage-y += verstage.c
verstage-y += common.c
ifeq (${CONFIG_VBOOT2_MOCK_SECDATA},y)
libverstage-y += secdata_mock.c
else
libverstage-y += antirollback.c
endif
romstage-y += vboot_handoff.c common.c

verstage-y += verstage.ld

VB_FIRMWARE_ARCH := $(ARCHDIR-$(ARCH-VERSTAGE-y))
VB2_LIB = $(obj)/external/vboot_reference/vboot_fw20.a
VBOOT_CFLAGS += $(patsubst -I%,-I$(top)/%,$(filter-out -include $(src)/include/kconfig.h, $(CPPFLAGS_verstage)))
VBOOT_CFLAGS += $(libverstage-c-ccopts)
VBOOT_CFLAGS += -include $(top)/src/include/kconfig.h -Wno-missing-prototypes
VBOOT_CFLAGS += -DVBOOT_DEBUG

$(VB2_LIB): $(obj)/config.h
	@printf "    MAKE       $(subst $(obj)/,,$(@))\n"
	$(Q)FIRMWARE_ARCH=$(VB_FIRMWARE_ARCH) \
	CC="$(CC_verstage)" \
	CFLAGS="$(VBOOT_CFLAGS)" VBOOT2="y" \
	$(MAKE) -C $(VB_SOURCE) \
		BUILD=$(top)/$(dir $(VB2_LIB)) \
		V=$(V) \
		fwlib20

libverstage-srcs += $(VB2_LIB)

ifeq ($(CONFIG_SEPARATE_VERSTAGE),y)
cbfs-files-y += $(call strip_quotes,$(CONFIG_CBFS_PREFIX))/verstage
fallback/verstage-file = $(objcbfs)/verstage.elf
fallback/verstage-type = stage
fallback/verstage-compression = none
else
ifeq ($(VBOOT_STARTS_IN_BOOTBLOCK),y)
bootblock-srcs += $(objgenerated)/libverstage.a
else
romstage-srcs += $(objgenerated)/libverstage.a
endif
endif # CONFIG_SEPARATE_VERSTAGE
