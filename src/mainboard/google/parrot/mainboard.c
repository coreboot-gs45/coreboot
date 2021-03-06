/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007-2009 coresystems GmbH
 * Copyright (C) 2011 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <types.h>
#include <string.h>
#include <device/device.h>
#include <device/pci_def.h>
#include <device/pci_ops.h>
#include <console/console.h>
#include <drivers/intel/gma/int15.h>
#include <pc80/mc146818rtc.h>
#include <arch/acpi.h>
#include <arch/io.h>
#include <arch/interrupt.h>
#include <boot/coreboot_tables.h>
#include "onboard.h"
#include "ec.h"
#include <southbridge/intel/bd82x6x/pch.h>
#include <smbios.h>
#include <device/pci.h>
#include <ec/compal/ene932/ec.h>

void mainboard_suspend_resume(void)
{
	/* Call SMM finalize() handlers before resume */
	outb(0xcb, 0xb2);

	/* Enable ACPI mode before OS resume */
	outb(0xe1, 0xb2);
}



static void mainboard_init(device_t dev)
{
	/* Initialize the Embedded Controller */
	parrot_ec_init();
}

static int parrot_onboard_smbios_data(device_t dev, int *handle,
				     unsigned long *current)
{
	int len = 0;
	u8 hardware_version = parrot_rev();
	if (hardware_version < 0x2) {		/* DVT vs PVT */
		len += smbios_write_type41(
			current, handle,
			PARROT_TRACKPAD_NAME,		/* name */
			PARROT_TRACKPAD_IRQ_DVT,	/* instance */
			0,				/* segment */
			PARROT_TRACKPAD_I2C_ADDR,	/* bus */
			0,				/* device */
			0);				/* function */
	} else {
		len += smbios_write_type41(
			current, handle,
			PARROT_TRACKPAD_NAME,		/* name */
			PARROT_TRACKPAD_IRQ_PVT,	/* instance */
			0,				/* segment */
			PARROT_TRACKPAD_I2C_ADDR,	/* bus */
			0,				/* device */
			0);				/* function */
	}

	return len;
}

// mainboard_enable is executed as first thing after
// enumerate_buses().

static void mainboard_enable(device_t dev)
{
	dev->ops->init = mainboard_init;
	dev->ops->get_smbios_data = parrot_onboard_smbios_data;
	install_intel_vga_int15_handler(GMA_INT15_ACTIVE_LFP_EDP, GMA_INT15_PANEL_FIT_DEFAULT, GMA_INT15_BOOT_DISPLAY_DEFAULT, 0);
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
