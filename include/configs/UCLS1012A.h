/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2019-2020 Arcturus Networks Inc.
 *           http://www.arcturusnetworks.com/products/ucls1012a/
 */

#ifndef __UCLS1012A_H__
#define __UCLS1012A_H__

#include "ls1012a_common.h"

/* Board Revs */
#define BOARD_REV_1_4			0x14
#define BOARD_REV_1_6			0x16
#define CONFIG_VERSION_VARIABLE

/* I2C & Co. */
#undef CONFIG_SYS_I2C_MXC_I2C2	 /* disable I2C bus 2 */

#define SYS_I2C_VR5100_ADDR	0x08
#define SYS_I2C_CX_ADDR		0x14
#define SYS_I2C_NCT72_ADDR	0x4C

/* DDR */
#define CONFIG_DIMM_SLOTS_PER_CTLR	1
#define CONFIG_CHIP_SELECTS_PER_CTRL	1
#define SYS_SDRAM_SIZE_512		0x20000000
#define SYS_SDRAM_SIZE_1024		0x40000000
#define CONFIG_SYS_SDRAM_SIZE		SYS_SDRAM_SIZE_1024
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

#ifndef CONFIG_SPL_BUILD
#undef BOOT_TARGET_DEVICES
#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(USB, usb, 0) \
	func(DHCP, dhcp, na)
#endif

/* ENV */
#define CONFIG_SYS_FSL_QSPI_BASE	0x40000000
#define CONFIG_ENV_ADDR			(CONFIG_SYS_FSL_QSPI_BASE + \
					CONFIG_ENV_OFFSET)
#undef CONFIG_ENV_OFFSET
#define CONFIG_ENV_OFFSET		0x200000

#undef FSL_QSPI_FLASH_SIZE
#ifdef CONFIG_SPI_FLASH_128M
#define FSL_QSPI_FLASH_SIZE		SZ_128M
#else
#define FSL_QSPI_FLASH_SIZE		SZ_64M
#endif

#undef CONFIG_ENV_SECT_SIZE
#define CONFIG_ENV_SECT_SIZE		0x10000 /*  64 KB */
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE			0x40000 /* 128 KB */

#if defined(CONFIG_SYS_REDUNDAND_ENVIRONMENT)
#define CONFIG_ENV_ADDR_REDUND		(CONFIG_ENV_ADDR + CONFIG_ENV_SIZE)
#define CONFIG_ENV_SIZE_REDUND		(CONFIG_ENV_SIZE)
#endif

/*  MMC  */
#ifdef CONFIG_MMC
#define CONFIG_SYS_FSL_MMC_HAS_CAPBLT_VS33
#endif

#define CONFIG_PCIE1		/* PCIE controller 1 */

#define CONFIG_PCI_SCAN_SHOW

#undef CONFIG_BOOTDELAY
#ifdef CONFIG_SUBTARGET_DONGLE
#define CONFIG_BOOTDELAY		0
#else
#define CONFIG_BOOTDELAY		3
#endif

#define CONFIG_HOSTNAME		"UCLS1012A"
#define CONFIG_BOOTFILE		"part0-000000.itb"
#define CONFIG_UBOOTPATH	u-boot.bin	/* U-Boot image on TFTP server */

/*
 * Partitions Configuration
 */
#define ADDRESS_PARTB	0x100000
#define SIZE_PARTB	0xC0000
#define ADDRESS_PARTE	CONFIG_ENV_OFFSET	/* 0x200000 */
#define SIZE_PARTE	CONFIG_ENV_SIZE		/*  0x40000 */
#define ADDRESS_RCW	0
#define SIZE_RCW	0x20000
#define ADDRESS_PFE	0x20000
#define SIZE_PFE	0x20000

#ifdef CONFIG_SUBTARGET_SOM
#define ADDRESS_PART0 0x00A00000
#define SIZE_PART0    0x07600000
#define MAX_PARTS_NUM 3

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS				\
	"verify=no\0"						\
	"loadaddr=0x80100000\0"					\
	"kernel_addr=0x100000\0"				\
	"kernel_start=0x1000000\0"				\
	"kernel_load=0xa0000000\0"				\
	"kernel_size=" __stringify(SIZE_PART0) "\0"		\
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
	"rcwbase=" __stringify(ADDRESS_RCW) "\0"		\
	"rcwsize=" __stringify(SIZE_RCW) "\0"			\
	"pfebase=" __stringify(ADDRESS_PFE) "\0"		\
	"pfesize=" __stringify(SIZE_PFE) "\0"			\
	"fdt_high=0xffffffffffffffff\0"				\
	"initrd_high=0xffffffffffffffff\0"			\
	"kargs_rootdev=root=/dev/mtdblock1\0"			\
	"kargs_misc=rootfstype=cramfs\0"			\
	"kargs_misc2=quiet lpj=250000\0"			\
	"kernel_load=0x96000000\0"				\
	"kargs=setenv bootargs console=$console,$baudrate "	\
		"$kargs_rootdev $kargs_misc $kargs_parts "	\
		"$kargs_misc2\0"				\
	"uboot= " __stringify(CONFIG_UBOOTPATH) "\0"		\
	"ubootaddr=0x01000000\0"				\
	"itest=tftp $kernel_load part0-000000.itb; "		\
		"pfe stop; "					\
		"run kargs; "					\
		"bootm $kernel_load\0"				\
	"iprogram=tftp part0-000000.itb; sf probe 0:0; "	\
		"sf erase $part0base +$part0size; "		\
		"sf write $loadaddr $part0base $filesize\0"	\
	"program0=sf probe 0:0; "				\
		"sf erase $part0base +$part0size; "		\
		"sf write $loadaddr $part0base $filesize\0"	\
	"program_rcw=sf probe 0:0; "				\
		"sf protect unlock $rcwbase $rcwsize; "		\
		"sf erase $rcwbase $rcwsize; "			\
		"sf write $loadaddr $rcwbase $filesize; "	\
		"sf protect lock $rcwbase $rcwsize\0"		\
	"program_pfe=sf probe 0:0; "				\
		"sf protect unlock $pfebase $pfesize; "		\
		"sf erase $pfebase $pfesize; "			\
		"sf write $loadaddr $pfebase $filesize; "	\
		"sf protect lock $pfebase $pfesize\0"		\
	"program_uboot=sf probe 0:0; "				\
		"sf erase $partBbase +$partBsize; "		\
		"sf write $loadaddr $partBbase $filesize\0"	\
	"console=ttyS0,115200n8\0"

#else
#ifdef CONFIG_SUBTARGET_DONGLE

#define ADDRESS_PART1 0x01200000
#define SIZE_PART2    0x00080000
#define SIZE_PART3    0x00800000

#ifdef CONFIG_SPI_FLASH_128M
	#define SIZE_PART1    0x06580000
	#define ADDRESS_PART2 0x07780000
	#define ADDRESS_PART3 0x07800000
#else
	#define SIZE_PART1    0x02580000
	#define ADDRESS_PART2 0x03780000
	#define ADDRESS_PART3 0x03800000
#endif

#define ADDRESS_PART0 0x00A00000
#define SIZE_PART0    0x00800000
#define MAX_PARTS_NUM 6

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS			\
	"bootcmd=echo; echo B$ run erase_all; echo; "	\
		"echo Run erase_all only to recover pre-program modules; echo; echo;" \
		"echo B$ run program_all; echo;"	\
		"echo Remove dongle, reset the module and press ESC; echo;"	\
		"echo B$ env default -a; "			\
		"echo B$ saveenv; "				\
		"echo B$ arc product <serial> <hw0> <hw1> 00:00:00:00:00:00; echo; "	\
		"echo Reset the module; "			\
		"echo \0"					\
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
	"rcwbase=" __stringify(ADDRESS_RCW) "\0"		\
	"rcwsize=" __stringify(SIZE_RCW) "\0"			\
	"pfebase=" __stringify(ADDRESS_PFE) "\0"		\
	"pfesize=" __stringify(SIZE_PFE) "\0"			\
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
		"sf write $loadaddr $part0base $filesize\0"	\
	"program0=if run dl_part0; then sf probe 0:0; "		\
		"sf erase $part0base +$part0size; "		\
		"sf write $loadaddr $part0base $filesize; "	\
		"else echo TFTP ERROR; fi\0"			\
	"program1=if run dl_part1; then sf probe 0:0; "		\
		"sf erase $part1base +$part1size; "		\
		"sf write $loadaddr $part1base $filesize;"	\
		"else echo TFTP ERROR; fi\0"			\
	"program2=if run dl_part2; then sf probe 0:0; "		\
		"sf erase $part2base +$part2size; "		\
		"sf write $loadaddr $part2base $filesize; "	\
		"else echo TFTP ERROR; fi\0"			\
	"program3=if run dl_part3; then sf probe 0:0; "		\
		"sf erase $part3base +$part3size; "		\
		"sf write $loadaddr $part3base $filesize; "	\
		"else echo TFTP ERROR; fi\0"			\
	"format3=sf probe 0:0; "				\
		"sf erase $part3base +$part3size \0"		\
	"program_rcw=if run dl_rcw; then sf probe 0:0; "	\
		"sf protect unlock $rcwbase $rcwsize; "		\
		"sf erase $rcwbase $rcwsize; "			\
		"sf write $loadaddr $rcwbase $filesize; "	\
		"else echo TFTP ERROR; fi\0"			\
	"program_pfe=if run dl_pfe; then sf probe 0:0; "	\
		"sf protect unlock $pfebase $pfesize; "		\
		"sf erase $pfebase $pfesize; "			\
		"sf write $loadaddr $pfebase $filesize; "	\
		"else echo TFTP ERROR; fi\0"			\
	"program_uboot=if run dl_uboot; then sf probe 0:0; "	\
		"sf erase $partBbase +$partBsize; "		\
		"sf write $loadaddr $partBbase $filesize; "	\
		"else echo TFTP ERROR; fi\0"			\
	"fn_rcw=ucls1012a_201904/rcw.bin\0"			\
	"fn_pfe=ucls1012a_201904/pfe.bin\0"			\
	"fn_uboot=ucls1012a_201904/u-boot.bin\0"		\
	"fn_part0=ucls1012a_201904/part0-000000.itb\0"		\
	"fn_part1=ucls1012a_201904/part1-000000.bin\0"		\
	"fn_part2=ucls1012a_201904/part2-000000.bin\0"		\
	"fn_part3=ucls1012a_201904/part3-000000.bin\0"		\
	"dl_rcw=tftp $fn_rcw\0"					\
	"dl_pfe=tftp $fn_pfe\0"					\
	"dl_uboot=tftp $fn_uboot\0"				\
	"dl_part0=tftp $fn_part0\0"				\
	"dl_part1=tftp $fn_part1\0"				\
	"dl_part2=tftp $fn_part2\0"				\
	"dl_part3=tftp $fn_part3\0"				\
	"erase_all=sf probe; sf erase 0 +0x8000000\0"		\
	"erase_all_64=sf probe; sf erase 0 +0x4000000\0"	\
	"program_all=run program_uboot; "			\
		"run program1; "				\
		"run program2; "				\
		"run format3; "					\
		"run program0; sleep 3; "			\
		"run program_pfe; sleep 1;"			\
		"run program_rcw; sleep 1\0"			\
	"program_all_64=run part64; run fn64; "			\
		"run program_all\0"				\
	"fn64=setenv fn_rcw $fn_rcw.64; "			\
		"setenv fn_uboot $fn_uboot.64; "		\
		"setenv fn_part0 $fn_part0.64; "		\
		"setenv fn_part1 $fn_part1.64; "		\
		"setenv fn_part2 $fn_part2.64\0"		\
	"part64=setenv part1size 0x2580000; "			\
		"setenv part2base 0x3780000; "			\
		"setenv part3base 0x380000\0" 			\
	"console=ttyS0,115200n8\0"

#else

#define ADDRESS_PART1 0x01200000
#define SIZE_PART2    0x00080000
#define SIZE_PART3    0x00800000

#ifdef CONFIG_SPI_FLASH_128M
	#define SIZE_PART1    0x06580000
	#define ADDRESS_PART2 0x07780000
	#define ADDRESS_PART3 0x07800000
#else
	#define SIZE_PART1    0x02580000
	#define ADDRESS_PART2 0x03780000
	#define ADDRESS_PART3 0x03800000
#endif

#define ADDRESS_PART0 0x00A00000
#define SIZE_PART0    0x00800000
#define MAX_PARTS_NUM 6

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS				\
	"verify=no\0"						\
	"loadaddr=0x80100000\0"					\
	"kernel_load=0xa0000000\0"				\
	"kernel_size=" __stringify(SIZE_PART0) "\0"		\
	"ethaddr=" __stringify(CONFIG_ETHADDR) "\0"		\
	"eth1addr=" __stringify(CONFIG_ETH1ADDR) "\0"		\
	"initrd_high=0xffffffff\0"				\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"			\
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
	"rcwbase=" __stringify(ADDRESS_RCW) "\0"		\
	"rcwsize=" __stringify(SIZE_RCW) "\0"			\
	"pfebase=" __stringify(ADDRESS_PFE) "\0"		\
	"pfesize=" __stringify(SIZE_PFE) "\0"			\
	"fdt_high=0xffffffffffffffff\0"				\
	"initrd_high=0xffffffffffffffff\0"			\
	"kargs_rootdev=root=/dev/mtdblock1\0"			\
	"kargs_misc=rootfstype=cramfs\0"			\
	"kargs_misc2=quiet lpj=250000\0"			\
	"kargs=setenv bootargs console=$console,$baudrate "	\
		"$kargs_rootdev $kargs_misc $kargs_parts "	\
		"$kargs_misc2\0 "				\
	"uboot= " __stringify(CONFIG_UBOOTPATH) "\0"		\
	"ubootaddr=0x01000000\0"				\
	"itest=tftp $kernel_load part0-000000.itb; "		\
		"pfe stop; "					\
		"run kargs; "					\
		"bootm $kernel_load\0"				\
	"iprogram=tftp part0-000000.itb; sf probe 0:0; "	\
		"sf erase $part0base +$filesize; "		\
		"sf write $loadaddr $part0base $filesize\0"	\
	"program0=sf probe 0:0; "				\
		"sf erase $part0base +$part0size; "		\
		"sf write $loadaddr $part0base $filesize\0"	\
	"program1=sf probe 0:0; "				\
		"sf erase $part1base +$part1size; "		\
		"sf write $loadaddr $part1base $filesize\0"	\
	"program2=sf probe 0:0; "				\
		"sf erase $part2base +$part2size; "		\
		"sf write $loadaddr $part2base $filesize\0"	\
	"program3=sf probe 0:0; "				\
		"sf erase $part3base +$part3size; "		\
		"sf write $loadaddr $part3base $filesize\0"	\
	"format3=sf probe 0:0; "				\
		"sf erase $part3base +$part3size\0"		\
	"program_rcw=sf probe 0:0; "				\
		"sf protect unlock $rcwbase $rcwsize; "		\
		"sf erase $rcwbase $rcwsize; "			\
		"sf write $loadaddr $rcwbase $filesize; "	\
		"sf protect lock $rcwbase $rcwsize\0"		\
	"program_pfe=sf probe 0:0; "				\
		"sf protect unlock $pfebase $pfesize; "		\
		"sf erase $pfebase $pfesize; "			\
		"sf write $loadaddr $pfebase $filesize; "	\
		"sf protect lock $pfebase $pfesize\0"		\
	"program_uboot=sf probe 0:0; "				\
		"sf erase $partBbase +$partBsize; "		\
		"sf write $loadaddr $partBbase $filesize\0"	\
	"program_uboot_big=sf probe 0:0; "			\
		"sf erase 0 +$filesize; "			\
		"sf write $loadaddr 0 $filesize\0"		\
	"console=ttyS0,115200n8\0"

#endif /* CONFIG_SUBTARGET_DONGLE */
#endif /* CONFIG_SUBTARGET_SOM */

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND	"pfe stop; sf probe 0:0; "\
				"sf read $kernel_load $part0base $part0size; " \
				"run kargs; "\
				"bootm $kernel_load\0 "

#include <asm/fsl_secure_boot.h>
#endif /* __UCLS1012A_H__ */
