/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LS1012ARDB_H__
#define __LS1012ARDB_H__

#include "ls1012a_common.h"

#if defined(CONFIG_FSL_LS_PPA)
#define CONFIG_ARMV8_PSCI
#define CONFIG_SYS_LS_PPA_DRAM_BLOCK_MIN_SIZE		(1UL * 1024 * 1024)

#define CONFIG_SYS_LS_PPA_FW_IN_XIP
#ifdef CONFIG_SYS_LS_PPA_FW_IN_XIP
#define	CONFIG_SYS_LS_PPA_FW_ADDR	0x40500000
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

/*  MMC  */
#if  !defined(CONFIG_EMU)
#define CONFIG_MMC
#ifdef CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_ESDHC_TWO_CONTROLLERS_SUPPORT
#define CONFIG_FSL_ESDHC_1_NON_REMOVABLE_CARD
#define CONFIG_SYS_FSL_MMC_HAS_CAPBLT_VS33
#define CONFIG_GENERIC_MMC
#endif
#endif /* CONFIG_EMU */

/* SATA */
#if  !defined(CONFIG_EMU)
#define CONFIG_LIBATA
#define CONFIG_SCSI_AHCI
#define CONFIG_SCSI_AHCI_PLAT
#define CONFIG_CMD_SCSI
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_SYS_SATA				AHCI_BASE_ADDR

#define CONFIG_SYS_SCSI_MAX_SCSI_ID		1
#define CONFIG_SYS_SCSI_MAX_LUN			1
#define CONFIG_SYS_SCSI_MAX_DEVICE		(CONFIG_SYS_SCSI_MAX_SCSI_ID * \
						CONFIG_SYS_SCSI_MAX_LUN)

#define CONFIG_PCI		/* Enable PCI/PCIE */
#define CONFIG_PCIE1		/* PCIE controller 1 */
#define CONFIG_PCIE_LAYERSCAPE	/* Use common FSL Layerscape PCIe code */
#define FSL_PCIE_COMPAT "fsl,ls1043a-pcie"

#define CONFIG_SYS_PCI_64BIT

#define CONFIG_SYS_PCIE_CFG0_PHYS_OFF	0x00000000
#define CONFIG_SYS_PCIE_CFG0_SIZE	0x00001000	/* 4k */
#define CONFIG_SYS_PCIE_CFG1_PHYS_OFF	0x00001000
#define CONFIG_SYS_PCIE_CFG1_SIZE	0x00001000	/* 4k */

#define CONFIG_SYS_PCIE_IO_BUS		0x00000000
#define CONFIG_SYS_PCIE_IO_PHYS_OFF	0x00010000
#define CONFIG_SYS_PCIE_IO_SIZE		0x00010000	/* 64k */

#define CONFIG_SYS_PCIE_MEM_BUS         0x08000000
#define CONFIG_SYS_PCIE_MEM_PHYS_OFF    0x04000000
#define CONFIG_SYS_PCIE_MEM_SIZE        0x80000000      /* 128M */

#define CONFIG_NET_MULTI
#define CONFIG_PCI_PNP
#define CONFIG_E1000
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_CMD_PCI
#endif

/*
 * I2C IO expander
 */

#define I2C_MUX_IO1_ADDR	0x24
#define __SW_BOOT_MASK		0xFC
#define __SW_BOOT_EMU		0x10
#define __SW_BOOT_BANK1		0x00
#define __SW_BOOT_BANK2		0x01
#define __SW_REV_MASK		0x07
#define __SW_REV_A		0xF8
#define __SW_REV_B		0xF0

/*
 *I2C RESET expander
 */
#define CONFIG_SYS_I2C_RESET_IO_EXPANDER	0x25
#define __PHY_MASK		0xF9
#define __PHY_ETH2_MASK		0xFB
#define __PHY_ETH1_MASK		0xFD

#define CONFIG_CMD_MEMINFO
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x9fffffff

#define CONFIG_FSL_CAAM			/* Enable CAAM */

#include <asm/fsl_secure_boot.h>

#endif /* __LS1012ARDB_H__ */
