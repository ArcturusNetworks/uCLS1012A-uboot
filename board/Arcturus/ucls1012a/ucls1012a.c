// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2018-2024 Arcturus Networks, Inc.
 *           https://www.arcturusnetworks.com/products/ucls1012a/
 */

#include <common.h>
#include <fdt_support.h>
#include <i2c.h>
#include <asm/cache.h>
#include <init.h>
#include <asm/global_data.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/fsl_serdes.h>
#ifdef CONFIG_FSL_LS_PPA
#include <asm/arch/ppa.h>
#endif
#include <asm/arch/mmu.h>
#include <asm/arch/soc.h>
#include <fsl_esdhc.h>
#include <hwconfig.h>
#include <env_internal.h>
#include <fsl_mmdc.h>
#include <netdev.h>
#include <fsl_sec.h>
#include <linux/delay.h>

DECLARE_GLOBAL_DATA_PTR;

extern int get_arc_info(void);
extern int ressurect_arc_info(void);
extern void set_kargs_parts(char *);
#ifdef CONFIG_HX3_HUB_INIT
extern int hx3_hub_init(void);
#endif
char module_rev[4];

#if defined(CX_AUDIO_SUPPORT)
#define MASK_CX_RST	0x10000000
static void reset_cx(void)
{
	struct ccsr_gpio *pgpio = (void *)(GPIO2_BASE_ADDR);

	/* Set direction output for GPIO2_03 (CX2070x reset) */
	setbits_be32(&pgpio->gpdir, MASK_CX_RST);

	/* Pull CX part Reset UP */
	setbits_be32(&pgpio->gpdat, MASK_CX_RST);
	mdelay(50);

	clrbits_be32(&pgpio->gpdat, MASK_CX_RST);
	/* Pull CX part Reset DOWN */
}
#endif

#if defined(I2C_GPIO_EXP)
#define MASK_GPIOEX_RST	0x00000002
static void reset_gpioex(void)
{
	struct ccsr_gpio *pgpio = (void *)(GPIO1_BASE_ADDR);

	/* Set direction output for GPIO1_30 */
	setbits_be32(&pgpio->gpdir, MASK_GPIOEX_RST);

	clrbits_be32(&pgpio->gpdat, MASK_GPIOEX_RST);
	mdelay(50);
	setbits_be32(&pgpio->gpdat, MASK_GPIOEX_RST);
}
#endif

#define MASK_CPU_RST	0x00000001
void reset_misc(void)
{
	struct ccsr_gpio *pgpio = (void *)(GPIO1_BASE_ADDR);

	setbits_be32(&pgpio->gpdir, MASK_CPU_RST);

	/* Pull CPU part Reset UP */
	setbits_be32(&pgpio->gpdat, MASK_CPU_RST);
	mdelay(500);
}

static unsigned char get_board_rev(int m)
{
	if (module_rev[0] == 'R') {
		if (m == 1)
			return module_rev[1];
		if (m == 2)
			return module_rev[2];
	}
	return 'X';
}

int checkboard(void)
{
	struct ccsr_gur __iomem *gur = (void *)(CONFIG_SYS_FSL_GUTS_ADDR);
	unsigned int rcw = gur_in32(&gur->rcwsr[9]);

#if defined(CX_AUDIO_SUPPORT)
	reset_cx();
#endif
#if defined(I2C_GPIO_EXP)
	reset_gpioex();
#endif
	module_rev[3] = (rcw >> 24) & 0xFF;
	module_rev[2] = (rcw >> 16) & 0xFF;
	module_rev[1] = (rcw >> 8)  & 0xFF;
	module_rev[0] = (rcw >> 0)  & 0xFF;

#ifdef CONFIG_SUBTARGET_DONGLE
	printf("Board: uCLS1012A-SOM PD\n\r");
#elif CONFIG_SUBTARGET_SOM120
	printf("Board: uCLS1012A-SOM120 Rev.%c.%c\n\r", get_board_rev(1), get_board_rev(2));
#elif CONFIG_SUBTARGET_SOM2X60
	printf("Board: 006607-CD-MOD-E V%c.%c\n\r", get_board_rev(1), get_board_rev(2));
#elif CONFIG_SUBTARGET_SOM314S
	printf("Board: uCLS1012A-SOM314S Rev.%c.%c\n\r", get_board_rev(1), get_board_rev(2));
#else
	printf("Board: uCLS1012A-SOM Rev.%c.%c\n\r", get_board_rev(1), get_board_rev(2));
#endif
	return 0;
}

int esdhc_status_fixup(void *blob, const char *compat)
{
	char esdhc0_path[] = "/soc/esdhc@1560000";
	char esdhc1_path[] = "/soc/esdhc@1580000";

	do_fixup_by_path(blob, esdhc0_path, "status", "okay",
			 sizeof("okay"), 1);

	do_fixup_by_path(blob, esdhc1_path, "status", "disabled",
			 sizeof("disabled"), 1);
	return 0;
}

#ifdef CONFIG_TFABOOT
int dram_init(void)
{
	gd->ram_size = tfa_get_dram_size();

	if (!gd->ram_size) {
#if defined(CONFIG_SUBTARGET_SOM120) || defined(CONFIG_SUBTARGET_SOM2X60) || defined(CONFIG_SUBTARGET_DONGLE)
		gd->ram_size = SYS_SDRAM_SIZE_512;
#else
		gd->ram_size = SYS_SDRAM_SIZE_1024;
#endif
	}
	return 0;
}
#else
int dram_init(void)
{
	struct fsl_mmdc_info mparam = {
		0x04180000,	/* mdctl */
		0x00030035,	/* mdpdc */
		0x12554000,	/* mdotc */
		0xbabf7954,	/* mdcfg0 */
		0xdb328f64,	/* mdcfg1 */
		0x01ff00db,	/* mdcfg2 */
		0x00001680,	/* mdmisc */
		0x0f3c8000,	/* mdref */
		0x00002000,	/* mdrwd */
		0x00bf1023,	/* mdor */
		0x0000003f,	/* mdasp */
		0x0000022a,	/* mpodtctrl */
		0xa1390003,	/* mpzqhwctrl */
	};
#if defined(CONFIG_SUBTARGET_SOM120) || defined(CONFIG_SUBTARGET_SOM2X60) || defined(CONFIG_SUBTARGET_DONGLE) || defined(CONFIG_BOARD_T1)
		mparam.mdctl = 0x04180000;
		gd->ram_size = SYS_SDRAM_SIZE_512;
#else
		mparam.mdctl = 0x05180000;
		gd->ram_size = SYS_SDRAM_SIZE_1024;
#endif
	mmdc_init(&mparam);

#if !defined(CONFIG_SPL) || defined(CONFIG_SPL_BUILD)
	/* This will break-before-make MMU for DDR */
	update_early_mmu_table();
#endif

	return 0;
}
#endif

int board_early_init_f(void)
{
	fsl_lsch2_early_init_f();

	return 0;
}

int last_stage_init(void)
{
	u8 id8;
	int rc;
	uchar bus = 0;
	struct udevice *dev = NULL;
#if defined(CX_AUDIO_SUPPORT)
	u32 id32;
#endif
	rc = i2c_get_chip_for_busnum(bus, SYS_I2C_VR5100_ADDR, 1, &dev);
	if (rc)
		printf("failed to get device at address 0x%x\n", SYS_I2C_VR5100_ADDR);
	else {
		if (dm_i2c_read(dev, 0, &id8, 1) < 0)
			printf("Error reading i2c VR5100 information!\n");
		else {
			printf("VR5100(@0x%x): ready; ", SYS_I2C_VR5100_ADDR);
			id8 = 0x48;
			if (dm_i2c_write(dev, 0x66, &id8, 1))
				printf("i2c write failed\n");
			printf("5V SWBST enabled\n");
		}
	}

	rc = i2c_get_chip_for_busnum(bus, SYS_I2C_NCT72_ADDR, 1, &dev);
	if (rc)
		printf("failed to get device at address 0x%x\n", SYS_I2C_NCT72_ADDR);
	else {
		if (dm_i2c_read(dev, 0xFE, &id8, 1) < 0)
			printf("Error reading i2c NCT72 information!\n");
		else
			printf("NCT72(@0x%x): ready\n", SYS_I2C_NCT72_ADDR);
	}

#if defined(CX_AUDIO_SUPPORT)
	rc = i2c_get_chip_for_busnum(bus, SYS_I2C_CX_ADDR, 1, &dev);
	if (rc)
		printf("failed to get device at address 0x%x\n", SYS_I2C_CX_ADDR);
	else {
		if (dm_i2c_read(dev, 0x1000, (u8 *)&id32, 1) < 0)
			printf("Error reading i2c CX2070x information!\n");
		else
			printf("CX2070x(@0x%x): ready\n", SYS_I2C_CX_ADDR);
	}
#endif

#ifdef CONFIG_HX3_HUB_INIT
	/* USB hub configuration - needed if defaults are different */
	hx3_hub_init();
#endif

	ressurect_arc_info();
	get_arc_info();

	if (strcmp(CONFIG_LOCALVERSION, "v6") == 0)
		set_kargs_parts((char *)"1550000.spi:");
	else
		set_kargs_parts((char *)"1550000.quadspi:");

#ifdef CONFIG_SUBTARGET_DONGLE
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
	printf("!  CHANGE THE JUMPER on a BONGLE BEFORE PROGRAMMING  !\n\n");
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
#endif

	return 0;
}

int board_init(void)
{
	struct ccsr_cci400 *cci = (struct ccsr_cci400 *)(CONFIG_SYS_IMMR +
					CONFIG_SYS_CCI400_OFFSET);

	/*
	 * Set CCI-400 control override register to enable barrier
	 * transaction
	 */
	if (current_el() == 3)
		out_le32(&cci->ctrl_ord, CCI400_CTRLORD_EN_BARRIER);

#ifdef CONFIG_ENV_IS_NOWHERE
	gd->env_addr = (ulong)&default_environment[0];
#endif

#ifdef CONFIG_FSL_LS_PPA
	ppa_init();
#endif
	return 0;
}

int ft_board_setup(void *blob, struct bd_info *bd)
{
	arch_fixup_fdt(blob);

	ft_cpu_setup(blob, bd);

	return 0;
}
