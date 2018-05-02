/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __UCLS1012A_H__
#define __UCLS1012A_H__

#define CONFIG_VERSION_VARIABLE

#include "ls1012a_common.h"

/* I2C & Co. */

#undef CONFIG_SYS_I2C_MXC_I2C2	 /* disable I2C bus 2 */

#define CONFIG_SYS_I2C_VR5100_ADDR	0x08
#define CONFIG_SYS_I2C_CX_ADDR		0x14
#define CONFIG_SYS_I2C_NCT72_ADDR	0x4C

#define CONFIG_DTT_ADM1021	1	/* ADM1021 temp sensor support	*/
#define CONFIG_SYS_DTT_BUS_NUM	0	/* The I2C bus for DTT		*/
#define CONFIG_DTT_SENSORS	{ 0, 1 }	/* Sensor index	*/
/*
 * ADM1021/NCT72 temp sensor configuration (see dtt/adm1021.c for details).
 * there will be one entry in this array for each two (dummy) sensors in
 * CONFIG_DTT_SENSORS.
 *
 * For uCP1020 module:
 * - only one ADM1021/NCT72
 * - i2c addr 0x41
 * - conversion rate 0x02 = 0.25 conversions/second
 * - ALERT output disabled
 * - local temp sensor enabled, min set to 0 deg, max set to 85 deg
 * - remote temp sensor enabled, min set to 0 deg, max set to 85 deg
 */
#define CONFIG_SYS_DTT_ADM1021	{ { CONFIG_SYS_I2C_NCT72_ADDR, \
					 0x02, 0, 1, 0, 85, 1, 0, 85} }

#define CONFIG_CMD_DTT

/* DDR */
#define CONFIG_DIMM_SLOTS_PER_CTLR	1
#define CONFIG_CHIP_SELECTS_PER_CTRL	1
#define CONFIG_NR_DRAM_BANKS		2
#define CONFIG_SYS_SDRAM_SIZE		0x40000000
#define CONFIG_CHIP_SELECTS_PER_CTRL	1
#define CONFIG_CMD_MEMINFO
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x9fffffff

/* Network PFE */
#define CONFIG_ETHADDR		00:06:3B:FF:FF:FF
#define CONFIG_ETH1ADDR		00:06:3B:FF:FF:FE
#define CONFIG_ETH2ADDR		00:06:3B:FF:FF:FD
#define CONFIG_IPADDR		192.168.1.81
#define CONFIG_IPADDR1		192.168.1.82
#define CONFIG_IPADDR2		192.168.1.83
#define CONFIG_SERVERIP		192.168.1.80
#define CONFIG_GATEWAYIP	192.168.1.1
#define CONFIG_NETMASK		255.255.255.0

#ifdef CONFIG_SUBTARGET_FRDM
#define EMAC1_PHY_ADDR          0x2
#define EMAC2_PHY_ADDR          0x1
#define CONFIG_PHY_REALTEK
#else
#define EMAC1_PHY_ADDR          0x4
#define EMAC2_PHY_ADDR          0x5
#define CONFIG_PHY_ATHEROS
#endif

#define CONFIG_PHYLIB

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
#define CONFIG_MMC
#ifdef CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_MMC_HAS_CAPBLT_VS33
#define CONFIG_GENERIC_MMC
#define CONFIG_DOS_PARTITION
#endif

/* SATA */
#define CONFIG_LIBATA
#define	CONFIG_SCSI
#define CONFIG_SCSI_AHCI
#define CONFIG_SCSI_AHCI_PLAT
#define CONFIG_CMD_SCSI
#define CONFIG_DOS_PARTITION
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_SYS_SATA				AHCI_BASE_ADDR

#define CONFIG_SYS_SCSI_MAX_SCSI_ID		1
#define CONFIG_SYS_SCSI_MAX_LUN			1
#define CONFIG_SYS_SCSI_MAX_DEVICE		(CONFIG_SYS_SCSI_MAX_SCSI_ID * \
						CONFIG_SYS_SCSI_MAX_LUN)
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
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_CMD_PCI

#define CONFIG_CMD_MEMINFO
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x9fffffff

#undef CONFIG_BOOTDELAY
#define CONFIG_BOOTDELAY		3

#define CONFIG_HOSTNAME		UCLS1012A
#define CONFIG_BOOTFILE		"part0-000000.itb"
#define CONFIG_UBOOTPATH	u-boot.bin /* U-Boot image on TFTP server */

#undef FSL_QSPI_FLASH_SIZE
#ifdef CONFIG_SPI_FLASH_128M
#define FSL_QSPI_FLASH_SIZE		SZ_128M
#else
#define FSL_QSPI_FLASH_SIZE		SZ_64M
#endif

#ifndef CONFIG_SPI_FLASH_STMICRO
#define CONFIG_SPI_FLASH_STMICRO
#endif

#ifndef CONFIG_SPI_FLASH_SPANSION
#define CONFIG_SPI_FLASH_SPANSION
#endif

#define CONFIG_SYS_FSL_QSPI_AHB

#define CONFIG_LAST_STAGE_INIT

#undef CONFIG_BOOTARGS /* CONFIG_BOOTARGS was defined in ls1012a_common.h */
/* CONFIG_SYS_TEXT_BASE defined in ls1012a_common.h */

#if defined(CONFIG_SYS_REDUNDAND_ENVIRONMENT)
/* Address and size of Redundant Environment Sector	*/
#define CONFIG_ENV_ADDR_REDUND	(CONFIG_ENV_ADDR + CONFIG_ENV_SIZE)
#define CONFIG_ENV_SIZE_REDUND	CONFIG_ENV_SIZE
#endif

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"pfe stop; "	\
	"run kargs; "	\
	"sf probe 0:0; " \
	"sf read $kernel_load $part0base $image0size && bootm $kernel_load "

/* #define CONFIG_SYS_FSL_ERRATUM_A009282 */

/*
 * Partitions Configuration
 */
#ifdef CONFIG_SUBTARGET_SOM
#define ADDRESS_PART0 0x00A00000
#define SIZE_PART0    0x07600000
#define MAX_PARTS_NUM 1

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS				\
	"ethaddr=" __stringify(CONFIG_ETHADDR) "\0"		\
	"eth1addr=" __stringify(CONFIG_ETH1ADDR) "\0"		\
	"initrd_high=0xffffffff\0"				\
	"verify=no\0"						\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"			\
	"loadaddr=0x80100000\0"					\
	"part0base=" __stringify(ADDRESS_PART0) "\0"		\
	"part0size=" __stringify(SIZE_PART0) "\0"		\
	"partBbase=" __stringify(ADDRESS_PARTB) "\0"		\
	"partBsize=" __stringify(SIZE_PARTB) "\0"		\
	"partEbase=" __stringify(ADDRESS_PARTE) "\0"		\
	"partEsize=" __stringify(SIZE_PARTE) "\0"		\
	"image0size=" __stringify(SIZE_PART0) "\0"		\
	"fdt_high=0xffffffffffffffff\0"				\
	"initrd_high=0xffffffffffffffff\0"			\
	"kargs_rootdev=root=/dev/mtdblock1\0"			\
	"kargs_misc=rootfstype=cramfs\0"			\
	"kernel_load=0x96000000\0"				\
	"kargs=setenv bootargs console=$console,$baudrate "	\
		"$kargs_rootdev $kargs_misc $kargs_parts\0"	\
	"uboot= " __stringify(CONFIG_UBOOTPATH) "\0"		\
	"ubootaddr=0x01000000\0"				\
	"itest=tftp $kernel_load part0-000000.itb; "		\
		"pfe stop; "					\
		"run kargs; "					\
		"bootm $kernel_load\0"				\
	"iprogram=tftp part0-000000.itb; sf probe 0:0; "	\
		"sf erase $part0base +$filesize; "		\
		"sf write $loadaddr $part0base $filesize; "	\
		"setenv image0size 0x$filesize; "		\
		"saveenv\0"					\
	"program0=sf probe 0:0; "				\
		"sf erase $part0base +$image0size; "		\
		"sf write $loadaddr $part0base $image0size\0"	\
	"program_rcw=sf probe 0:0; "				\
		"sf protect unlock 0 0x40000; "			\
		"sf erase 0 +0x40000; "				\
		"sf write $loadaddr 0 $filesize; "		\
		"sf protect lock 0 0x40000\0"			\
	"program_uboot=sf probe 0:0; "				\
		"sf erase $partBbase +$filesize; "		\
		"sf write $loadaddr $partBbase $filesize\0"	\
	"console=ttyS0,115200n8\0"

#else

#define ADDRESS_PART0 0x00A00000
#define SIZE_PART0    0x00800000
#define ADDRESS_PART1 0x01200000
#define SIZE_PART1    0x02580000
#define ADDRESS_PART2 0x03780000
#define SIZE_PART2    0x00080000
#define ADDRESS_PART3 0x03800000
#ifdef CONFIG_SPI_FLASH_128M
#define SIZE_PART3    0x04800000
#else
#define SIZE_PART3    0x00800000
#endif

#define ADDRESS_PARTB 0x100000
#define SIZE_PARTB    0x100000
#define ADDRESS_PARTE 0x200000	/* CONFIG_ENV_OFFSET from ls1012a_common.h */
#define SIZE_PARTE    0x40000	/* CONFIG_ENV_SIZE from ls1012a_common.h */
#define MAX_PARTS_NUM 6

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS				\
	"ethaddr=" __stringify(CONFIG_ETHADDR) "\0"		\
	"eth1addr=" __stringify(CONFIG_ETH1ADDR) "\0"		\
	"initrd_high=0xffffffff\0"				\
	"verify=no\0"						\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"			\
	"loadaddr=0x80100000\0"					\
	"part0base=" __stringify(ADDRESS_PART0) "\0"		\
	"part0size=" __stringify(SIZE_PART0) "\0"		\
	"image0size=" __stringify(SIZE_PART0) "\0"		\
	"part1base=" __stringify(ADDRESS_PART1) "\0"		\
	"part1size=" __stringify(SIZE_PART1) "\0"		\
	"part2base=" __stringify(ADDRESS_PART2) "\0"		\
	"part2size=" __stringify(SIZE_PART2) "\0"		\
	"part3base=" __stringify(ADDRESS_PART3) "\0"		\
	"part3size=" __stringify(SIZE_PART3) "\0"		\
	"partBbase=" __stringify(ADDRESS_PARTB) "\0"		\
	"partBsize=" __stringify(SIZE_PARTB) "\0"		\
	"partEbase=" __stringify(ADDRESS_PARTE) "\0"		\
	"partEsize=" __stringify(SIZE_PARTE) "\0"		\
	"fdt_high=0xffffffffffffffff\0"				\
	"initrd_high=0xffffffffffffffff\0"			\
	"kargs_rootdev=root=/dev/mtdblock1\0"			\
	"kargs_misc=rootfstype=cramfs\0"			\
	"kernel_load=0x96000000\0"				\
	"kargs=setenv bootargs console=$console,$baudrate "	\
		"$kargs_rootdev $kargs_misc $kargs_parts\0"	\
	"uboot= " __stringify(CONFIG_UBOOTPATH) "\0"		\
	"ubootaddr=0x01000000\0"				\
	"itest=tftp $kernel_load part0-000000.itb; "		\
		"pfe stop; "					\
		"run kargs; "					\
		"bootm $kernel_load\0"				\
	"iprogram=tftp part0-000000.itb; sf probe 0:0; "	\
		"sf erase $part0base +$filesize; "		\
		"sf write $loadaddr $part0base $filesize; "	\
		"setenv image0size 0x$filesize; "		\
		"saveenv\0"					\
	"program0=sf probe 0:0; "				\
		"sf erase $part0base +$image0size; "		\
		"sf write $loadaddr $part0base $image0size\0"	\
	"program1=sf probe 0:0; "				\
		"sf erase $part1base +$filesize; "		\
		"sf write $loadaddr $part1base $filesize\0"	\
	"program2=sf probe 0:0; "				\
		"sf erase $part2base +$filesize; "		\
		"sf write $loadaddr $part2base $filesize\0"	\
	"program3=sf probe 0:0; "				\
		"sf erase $part3base +$part3size; "		\
		"sf write $loadaddr $part3base $filesize\0"	\
	"program_rcw=sf probe 0:0; "				\
		"sf protect unlock 0 0x40000; "			\
		"sf erase 0 +0x40000; "				\
		"sf write $loadaddr 0 $filesize; "		\
		"sf protect lock 0 0x40000\0"			\
	"program_uboot=sf probe 0:0; "				\
		"sf erase $partBbase +$filesize; "		\
		"sf write $loadaddr $partBbase $filesize\0"	\
	"console=ttyS0,115200n8\0"

#endif /* CONFIG_SUBTARGET_SOM */

#endif /* __UCLS1012A_H__ */
