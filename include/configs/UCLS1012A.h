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
#ifdef CONFIG_SUBTARGET_DONGLE
#define FSL_QSPI_FLASH_SIZE		SZ_64M
#else
#define FSL_QSPI_FLASH_SIZE		SZ_128M
#endif
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

#if defined(CONFIG_SUBTARGET_SOM) || defined(CONFIG_SUBTARGET_SOM120)

#ifdef CONFIG_SPI_FLASH_128M
#define ADDRESS_PART0 0x00A00000
#define SIZE_PART0    0x00800000
#define ADDRESS_PART1 0x01200000
#define SIZE_PART1    0x02E00000
#define ADDRESS_PART2 0x04000000
#define SIZE_PART2    0x04000000
#else
#define ADDRESS_PART0 0x00A00000
#define SIZE_PART0    0x00800000
#define ADDRESS_PART1 0x01200000
#define SIZE_PART1    0x00E00000
#define ADDRESS_PART2 0x02000000
#define SIZE_PART2    0x02000000
#endif

#define MAX_PARTS_NUM 5

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
	"part0base=" __stringify(ADDRESS_PART0) "\0"		\
	"part0size=" __stringify(SIZE_PART0) "\0"		\
	"part1base=" __stringify(ADDRESS_PART1) "\0"		\
	"part1size=" __stringify(SIZE_PART1) "\0"		\
	"part2base=" __stringify(ADDRESS_PART2) "\0"		\
	"part2size=" __stringify(SIZE_PART2) "\0"		\
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
	"kargs_misc=rootfstype=cramfs,squashfs,jffs2\0"		\
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
	"program0=sf probe 0:0;"				\
		"sf erase $part0base +$part0size; "		\
		"sf write $loadaddr $part0base $filesize\0"	\
	"program1=sf probe 0:0;"				\
		"sf erase $part1base +$part1size; "		\
		"sf write $loadaddr $part1base $filesize\0"	\
	"format2=sf probe 0:0;"					\
		"sf erase $part2base +$part2size\0"		\
	"program_firmware=sf probe 0:0; "			\
		"sf erase $rcwbase +$filesize; "		\
		"sf write $loadaddr $rcwbase $filesize\0"	\
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
		"echo B$ run <GRXVP or MBARXSC or SOM>; echo;"	\
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
	"kargs_misc=rootfstype=cramfs,squashfs,jffs2\0"		\
	"kernel_load=0x96000000\0"				\
	"kargs=setenv bootargs console=$console,$baudrate "	\
		"$kargs_rootdev $kargs_misc $kargs_parts\0"	\
	"uboot= " __stringify(CONFIG_UBOOTPATH) "\0"		\
	"ubootaddr=0x01000000\0"				\
	"itest=tftp $kernel_load part0-000000.itb; "		\
		"pfe stop; "					\
		"run kargs; "					\
		"bootm $kernel_load\0"				\
	"program0=sf probe 0:0; "				\
		"sf erase $part0base +$part0size; "		\
		"sf write $loadaddr $part0base $filesize\0 "	\
	"program1=sf probe 0:0; "				\
		"sf erase $part1base +$part1size; "		\
		"sf write $loadaddr $part1base $filesize\0"	\
	"program2=sf probe 0:0; "				\
		"sf erase $part2base +$part2size; "		\
		"sf write $loadaddr $part2base $filesize\0 "	\
	"program3=sf probe 0:0; "				\
		"sf erase $part3base +$part3size; "		\
		"sf write $loadaddr $part3base $filesize\0 "	\
	"format3=sf probe 0:0; "				\
		"sf erase $part3base +$part3size \0"		\
	"program_rcw=sf probe 0:0; "	\
		"sf protect unlock $rcwbase $rcwsize; "		\
		"sf erase $rcwbase $rcwsize; "			\
		"sf write $loadaddr $rcwbase $filesize\0"	\
	"program_firmware=sf probe 0:0; "	\
		"sf erase $rcwbase +$filesize; "		\
		"sf write $loadaddr $rcwbase $filesize\0"	\
	"program_pfe=sf probe 0:0; "	\
		"sf protect unlock $pfebase $pfesize; "		\
		"sf erase $pfebase $pfesize; "			\
		"sf write $loadaddr $pfebase $filesize\0 "	\
	"program_uboot=sf probe 0:0; "	\
		"sf erase $partBbase +$partBsize; "		\
		"sf write $loadaddr $partBbase $filesize\0 "	\
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
	"erase_all=echo ""; echo !!!!!!!!!!!!!!!!!!!!!!!!!!;"	\
		"echo !  Erasing whole flash... !;"		\
		"echo !!!!!!!!!!!!!!!!!!!!!!!!!!;"		\
		"sf probe; sf erase 0 +0x8000000\0"		\
	"erase_all_64=echo ""; echo !!!!!!!!!!!!!!!!!!!!!!!!!!;"\
		"echo !  Erasing whole flash... !;"		\
		"echo !!!!!!!!!!!!!!!!!!!!!!!!!!;"		\
		"sf probe; sf erase 0 +0x4000000\0"		\
	"console=ttyS0,115200n8\0"				\
	"prog_dev=usb\0"						\
	"USBB=echo $PROD:firmware/boot.bin;"			\
	   "load usb $USBID $loadaddr firmware/boot-firmware.bin; &&" \
	   "run program_firmware; &&"				\
	   "load usb $USBID $loadaddr firmware/boot.bin; &&"	\
	   "run program_uboot\0"				\
	"USB0=echo $PROD:firmware/part0-000000.itb;"		\
	   "load usb $USBID $loadaddr firmware/part0-000000.itb; &&" \
	   "run program0\0"					\
	"USB1=echo $PROD:firmware/part1-00000a.bin;"		\
	   "load usb $USBID $loadaddr firmware/part1-00000s.bin; &&" \
	   "run program1\0"					\
	"USB2=echo $PROD:firmware/part2-00000a.bin;"		\
	   "load usb $USBID $loadaddr firmware/part2-00000s.bin; &&" \
	   "run program2\0"					\
	"TFTPB=echo $PDIR/boot.bin;"			\
	   "tftp $PDIR/boot-firmware.bin; &&" 			\
	   "run program_firmware; &&"				\
	   "tftp $PDIR/boot.bin; &&"				\
	   "run program_uboot\0"				\
	"TFTP0=echo $PDIR/part0-000000.itb;"		\
	   "tftp $PDIR/part0-000000.itb; &&"			\
	   "run program0\0"					\
	"TFTP1=echo $PDIR/part1-00000a.bin;"		\
	   "tftp $PDIR/part1-00000s.bin; &&" 			\
	   "run program1\0"					\
	"TFTP2=echo $PDIR/part2-00000a.bin;"		\
	   "tftp $PDIR/part2-00000s.bin; &&" 			\
	   "run program2\0"					\
	"GRXVP=setenv PROD GRXVP;"				\
		"setenv PDIR GRXVP;"				\
		"setenv USBID 0:1;"				\
		"run program_all\0"				\
	"MBARXSC=setenv PROD MBARXSC;"				\
		"setenv PDIR MBARXSC;"				\
		"setenv USBID 0:2;"				\
		"run program_all\0"				\
	"SOM=setenv PROD uCls1012a-SOM;"			\
		"setenv PDIR UCLS1012A_SOM;"			\
		"setenv USBID 0:2;"				\
		"run program_all\0"				\
	"SOM120=setenv PROD uCls1012a-SOM120;"			\
		"setenv PDIR UCLS1012A_SOM120;"			\
		"setenv USBID 0:2;"				\
		"run program_all\0"				\
	"program_all_usb=echo !!!!!!!!!!!!!!!!!!!!!!!!!!;"	\
		"echo !   Program from USB     !;"		\
		"echo !!!!!!!!!!!!!!!!!!!!!!!!!!;"		\
		"run USBB;"					\
		"run USB0;"					\
		"run USB1;"					\
		"run USB2;\0"					\
	"program_all_tftp=echo !!!!!!!!!!!!!!!!!!!!!!!!!!;"	\
		"echo !   Program from TFTP    !;"		\
		"echo !!!!!!!!!!!!!!!!!!!!!!!!!!;"		\
		"run TFTPB;"					\
		"run TFTP0;"					\
		"run TFTP1;"					\
		"run TFTP2;\0"					\
	"program_all=echo Start;"				\
		"if test $prog_dev = tftp; then "		\
			"run program_all_tftp;"			\
		"else "						\
			"usb start;"				\
			"run program_all_usb;"			\
		"fi; \0"					\
	"console=ttyS0,115200n8\0"

#else

#define ADDRESS_PART1 0x01200000
#define SIZE_PART2    0x00080000
#define SIZE_PART3    0x00800000

#ifdef CONFIG_SPI_FLASH_128M
	#define SIZE_PART1    0x05580000
	#define ADDRESS_PART4 0x06780000
	#define SIZE_PART4    0x01000000
	#define ADDRESS_PART2 0x07780000
	#define ADDRESS_PART3 0x07800000
#else
	#define SIZE_PART1    0x01D80000
	#define ADDRESS_PART4 0x02F80000
	#define SIZE_PART4    0x00800000
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
	"part4base=" __stringify(ADDRESS_PART4) "\0"		\
	"part4size=" __stringify(SIZE_PART4) "\0"		\
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
	"kargs_misc=rootfstype=cramfs,squashfs,jffs2\0"		\
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
