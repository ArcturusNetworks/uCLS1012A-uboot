/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LS1012ARDB_H__
#define __LS1012ARDB_H__

#include "ls1012a_common.h"

#ifndef CONFIG_SECURE_BOOT
#if defined(CONFIG_FSL_LS_PPA)
#define CONFIG_ARMV8_PSCI
#define CONFIG_SYS_LS_PPA_DRAM_BLOCK_MIN_SIZE	(1UL * 1024 * 1024)

#define CONFIG_SYS_LS_PPA_FW_IN_XIP
#ifdef CONFIG_SYS_LS_PPA_FW_IN_XIP
#define CONFIG_SYS_LS_PPA_FW_ADDR	0x40500000
#endif
#endif
#endif

#define CONFIG_DIMM_SLOTS_PER_CTLR	1
#define CONFIG_CHIP_SELECTS_PER_CTRL	1
#define CONFIG_NR_DRAM_BANKS		2

#define CONFIG_CMD_MEMINFO
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x9fffffff

#ifdef CONFIG_FSL_PPFE
#define EMAC1_PHY_ADDR          0x2
#define EMAC2_PHY_ADDR          0x1
#define CONFIG_PHYLIB
#define CONFIG_PHY_REALTEK
#endif
/*
* USB
*/
#define CONFIG_HAS_FSL_XHCI_USB

#ifdef CONFIG_HAS_FSL_XHCI_USB
#define CONFIG_USB_XHCI
#define CONFIG_USB_XHCI_FSL
#define CONFIG_USB_XHCI_DWC3
#define CONFIG_USB_MAX_CONTROLLER_COUNT         1
#define CONFIG_SYS_USB_XHCI_MAX_ROOT_PORTS      2
#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE

#define CONFIG_USB_DWC3
#define CONFIG_USB_DWC3_GADGET

#define CONFIG_USB_GADGET
#define CONFIG_USB_FUNCTION_MASS_STORAGE
#define CONFIG_USB_GADGET_DOWNLOAD
#define CONFIG_USB_GADGET_VBUS_DRAW 2
#define CONFIG_G_DNL_MANUFACTURER "NXP Semiconductor"
#define CONFIG_G_DNL_VENDOR_NUM 0x1234
#define CONFIG_G_DNL_PRODUCT_NUM 0x1234
#define CONFIG_USB_GADGET_DUALSPEED

/* USB Gadget ums command */
#define CONFIG_CMD_USB_MASS_STORAGE
#endif

#define CONFIG_CMD_MEMINFO
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x9fffffff

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"initrd_high=0xffffffff\0"		\
	"verify=no\0"				\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"	\
	"loadaddr=0x80100000\0"			\
	"kernel_addr=0x100000\0"		\
	"ramdisk_addr=0x800000\0"		\
	"ramdisk_size=0x2000000\0"		\
	"fdt_high=0xffffffffffffffff\0"		\
	"initrd_high=0xffffffffffffffff\0"	\
	"kernel_start=0xa00000\0"		\
	"kernel_load=0x96000000\0"		\
	"kernel_size=0x2800000\0"		\
	"console=ttyAMA0,38400n8\0"

#endif /* __LS1012ARDB_H__ */
