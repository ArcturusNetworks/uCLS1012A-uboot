/*
 * Copyright (C) 2015 Freescale Semiconductor
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LS1012A_COMMON_H
#define __LS1012A_COMMON_H

#define CONFIG_FSL_LAYERSCAPE
#define CONFIG_FSL_LSCH2
#define CONFIG_LS1012A
#define CONFIG_GICV2

#define CONFIG_SYS_HAS_SERDES

#include <asm/arch/config.h>
#define CONFIG_SYS_NO_FLASH

#define CONFIG_SUPPORT_RAW_INITRD

#define CONFIG_DISPLAY_BOARDINFO_LATE

#define CONFIG_SYS_TEXT_BASE		0x40100000

#define CONFIG_SYS_FSL_CLK
#define CONFIG_SYS_CLK_FREQ		100000000
#define CONFIG_DDR_CLK_FREQ		125000000

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_BOARD_EARLY_INIT_F	1

#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_FSL_OCRAM_BASE + 0xfff0)
#define CONFIG_SYS_LOAD_ADDR	(CONFIG_SYS_DDR_SDRAM_BASE + 0x10000000)

#define CONFIG_SYS_DDR_SDRAM_BASE	0x80000000
#define CONFIG_SYS_FSL_DDR_SDRAM_BASE_PHY	0
#define CONFIG_SYS_SDRAM_BASE		CONFIG_SYS_DDR_SDRAM_BASE
#define CONFIG_SYS_DDR_BLOCK2_BASE	0x880000000ULL

/* Generic Timer Definitions */
#define COUNTER_FREQUENCY		25000000	/* 12MHz */

/* CSU */
#define CONFIG_LAYERSCAPE_NS_ACCESS

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 128 * 1024)

/*SPI device */
#ifdef CONFIG_QSPI_BOOT
#define CONFIG_SYS_QE_FW_IN_SPIFLASH
#define CONFIG_SYS_FMAN_FW_ADDR		0x400d0000
#define CONFIG_ENV_SPI_BUS		0
#define CONFIG_ENV_SPI_CS		0
#define CONFIG_ENV_SPI_MAX_HZ		1000000
#define CONFIG_ENV_SPI_MODE		0x03
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH_SPANSION
#define CONFIG_FSL_SPI_INTERFACE
#define CONFIG_SF_DATAFLASH

#define CONFIG_FSL_QSPI
#define QSPI0_AMBA_BASE		0x40000000
#define CONFIG_DM_SPI_FLASH

#define FSL_QSPI_FLASH_SIZE		(1 << 26)
#define FSL_QSPI_FLASH_NUM		2
#define CONFIG_SYS_FSL_QSPI_AHB
#define CONFIG_SYS_FSL_ERRATUM_A009282

/*
 * Environment
 */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SIZE			0x40000          /* 256KB */
#define CONFIG_ENV_OFFSET		0x200000        /* 2MB */
#define CONFIG_ENV_SECT_SIZE		0x40000
#endif

/* I2C */
#if !defined(CONFIG_EMU)
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#endif /* CONFIG_EMU */

#define CONFIG_CONS_INDEX       1
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE     1
#define CONFIG_SYS_NS16550_CLK          (get_bus_freq(0)/2)

#ifdef CONFIG_EMU
#define CONFIG_BAUDRATE			3000
#else
#define CONFIG_BAUDRATE			115200
#endif
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/* Command line configuration */
#define CONFIG_CMD_CACHE
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_ENV
#define CONFIG_CMD_GREPENV
#define CONFIG_CMD_MII
#define CONFIG_CMD_PING
#undef CONFIG_CMD_IMLS

/*  PPFE */
#define CONFIG_FSL_PPFE

#ifdef CONFIG_FSL_PPFE
#define CONFIG_CMD_PFE_COMMANDS
#define CONFIG_UTIL_PE_DISABLED

#endif


#define CONFIG_ARCH_EARLY_INIT_R

#define CONFIG_SYS_HZ			1000

#define CONFIG_HWCONFIG
#define HWCONFIG_BUFFER_SIZE		128

#define CONFIG_DISPLAY_CPUINFO

/* Initial environment variables */
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
	"kernel_load=0xa0000000\0"		\
	"kernel_size=0x2800000\0"		\
	"console=ttyAMA0,38400n8\0"

#ifdef	CONFIG_EMU
#define CONFIG_BOOTARGS		"console=ttyS2,115200 root=/dev/ram0 " \
				"earlycon=uart8250,mmio,0x21d0500,115200n8"
/* Kernel image should be pre-loaded to address kernel_load */
#define CONFIG_BOOTCOMMAND	"bootm $kernel_load"
#else
#define CONFIG_BOOTARGS		"console=ttyS0,115200 root=/dev/ram0 " \
				"earlycon=uart8250,mmio,0x21c0500 quiet lpj=250000"
#define CONFIG_BOOTCOMMAND		"pfe stop; sf probe 0:0; sf read $kernel_load "\
					"$kernel_start $kernel_size && "\
					"bootm $kernel_load"
#endif
#define CONFIG_BOOTDELAY		10

/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */
#ifndef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT		"=> "
#endif
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE /* Boot args buffer */
#define CONFIG_SYS_LONGHELP
#define CONFIG_CMDLINE_EDITING		1
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_MAXARGS		64	/* max command args */

#define CONFIG_PANIC_HANG
#define CONFIG_SYS_BOOTM_LEN   (64 << 20)      /* Increase max gunzip size */

/* Flat Device Tree Definitions */
#define CONFIG_OF_LIBFDT
#define CONFIG_OF_BOARD_SETUP

/* new uImage format support */
#define CONFIG_FIT
#define CONFIG_FIT_VERBOSE	/* enable fit_format_{error,warning}() */
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMDLINE_TAG

/* Used by SCSI, MMC and USB */
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
#define CONFIG_CMD_EXT2

#endif /* __LS1012A_COMMON_H */
