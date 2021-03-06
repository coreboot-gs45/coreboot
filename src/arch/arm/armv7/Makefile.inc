################################################################################
##
## This file is part of the coreboot project.
##
## Copyright (C) 2013 The ChromiumOS Authors
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
###############################################################################

armv7_flags = -mthumb -I$(src)/arch/arm/include/armv7/ -D__COREBOOT_ARM_ARCH__=7
armv7-a_flags = -march=armv7-a $(armv7_flags)
armv7-m_flags = -march=armv7-m $(armv7_flags)

armv7_asm_flags = -Wa,-mthumb -Wa,-mimplicit-it=always -Wa,-mno-warn-deprecated
armv7-a_asm_flags = $(armv7-a_flags) $(armv7_asm_flags)
armv7-m_asm_flags = $(armv7-m_flags) $(armv7_asm_flags)

###############################################################################
# bootblock
###############################################################################

ifeq ($(CONFIG_ARCH_BOOTBLOCK_ARMV7),y)
bootblock-c-ccopts += $(armv7-a_flags)
bootblock-S-ccopts += $(armv7-a_asm_flags)

ifneq ($(CONFIG_ARM_BOOTBLOCK_CUSTOM),y)
bootblock-y += bootblock.S
bootblock-$(CONFIG_BOOTBLOCK_SIMPLE) += bootblock_simple.c
endif

bootblock-y += cache.c
bootblock-y += cpu.S
bootblock-$(CONFIG_BOOTBLOCK_CONSOLE) += exception.c
bootblock-$(CONFIG_BOOTBLOCK_CONSOLE) += exception_asm.S
bootblock-y += mmu.c

else ifeq ($(CONFIG_ARCH_BOOTBLOCK_ARMV7_M),y)
bootblock-c-ccopts += $(armv7-m_flags)
bootblock-S-ccopts += $(armv7-m_asm_flags)

ifneq ($(CONFIG_ARM_BOOTBLOCK_CUSTOM),y)
bootblock-y += bootblock_m.S
bootblock-y += bootblock_simple.c
endif
bootblock-y += exception_m.c
bootblock-y += cache_m.c

endif # CONFIG_ARCH_BOOTBLOCK_ARMV7

bootblock-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

################################################################################
## verification stage
################################################################################

ifeq ($(CONFIG_ARCH_VERSTAGE_ARMV7),y)
libverstage-c-ccopts += $(armv7-a_flags)
libverstage-S-ccopts += $(armv7-a_asm_flags)
verstage-c-ccopts += $(armv7-a_flags)
verstage-S-ccopts += $(armv7-a_asm_flags)

verstage-y += cache.c
verstage-y += cpu.S
verstage-y += exception.c
verstage-y += exception_asm.S
verstage-y += mmu.c

else ifeq ($(CONFIG_ARCH_VERSTAGE_ARMV7_M),y)
libverstage-c-ccopts += $(armv7-m_flags)
libverstage-S-ccopts += $(armv7-m_asm_flags)
verstage-c-ccopts += $(armv7-m_flags)
verstage-S-ccopts += $(armv7-m_asm_flags)

endif # CONFIG_ARCH_VERSTAGE_ARMV7_M

verstage-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

################################################################################
## ROM stage
################################################################################
ifeq ($(CONFIG_ARCH_ROMSTAGE_ARMV7),y)
romstage-y += cache.c
romstage-y += cpu.S
romstage-y += exception.c
romstage-y += exception_asm.S
romstage-y += mmu.c
romstage-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

romstage-c-ccopts += $(armv7-a_flags)
romstage-S-ccopts += $(armv7-a_asm_flags)

rmodules_arm-c-ccopts += $(armv7-a_flags)
rmodules_arm-S-ccopts += $(armv7-a_asm_flags)

endif # CONFIG_ARCH_ROMSTAGE_ARMV7

###############################################################################
# ramstage
###############################################################################

ifeq ($(CONFIG_ARCH_RAMSTAGE_ARMV7),y)

ramstage-y += cache.c
ramstage-y += cpu.S
ramstage-y += exception.c
ramstage-y += exception_asm.S
ramstage-y += mmu.c
ramstage-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

ramstage-c-ccopts += $(armv7-a_flags)
ramstage-S-ccopts += $(armv7-a_asm_flags)

# All rmodule code is armv7 if ramstage is armv7.
rmodules_arm-c-ccopts += $(armv7-a_flags)
rmodules_arm-S-ccopts += $(armv7-a_asm_flags)
endif # CONFIG_ARCH_RAMSTAGE_ARMV7
