/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 Advanced Micro Devices, Inc.
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


#include <arch/io.h>

static void sb700_enable_rom(void)
{
	u32 word;
	u32 dword;
	pci_devfn_t dev;

	dev = PCI_DEV(0, 0x14, 0x03);
	/* SB700 LPC Bridge 0:20:3:44h.
	 * BIT6: Port Enable for serial port 0x3f8-0x3ff
	 * BIT29: Port Enable for KBC port 0x60 and 0x64
	 * BIT30: Port Enable for ACPI Micro-Controller port 0x66 and 0x62
	 */
	dword = pci_io_read_config32(dev, 0x44);
	//dword |= (1<<6) | (1<<29) | (1<<30) ;
	/*Turn on all of LPC IO Port decode enable */
	dword = 0xffffffff;
	pci_io_write_config32(dev, 0x44, dword);

	/* SB700 LPC Bridge 0:20:3:48h.
	 * BIT0: Port Enable for SuperIO 0x2E-0x2F
	 * BIT1: Port Enable for SuperIO 0x4E-0x4F
	 * BIT4: Port Enable for LPC ROM Address Arrage2 (0x68-0x6C)
	 * BIT6: Port Enable for RTC IO 0x70-0x73
	 * BIT21: Port Enable for Port 0x80
	 */
	dword = pci_io_read_config32(dev, 0x48);
	dword |= (1<<0) | (1<<1) | (1<<4) | (1<<6) | (1<<21) ;
	pci_io_write_config32(dev, 0x48, dword);

	/* Enable 4MB rom access at 0xFFE00000 - 0xFFFFFFFF */
	/* Set the 4MB enable bits */
	word = pci_io_read_config16(dev, 0x6c);
	word = 0xFFC0;
	pci_io_write_config16(dev, 0x6c, word);
}

static void bootblock_southbridge_init(void)
{
	/* Setup the rom access for 2M */
	sb700_enable_rom();
}
