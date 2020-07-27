// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2018-2020 Arcturus Networks, Inc.
 *           https://www.arcturusnetworks.com/products/ucls1012a/
 */

#include <common.h>
#include <i2c.h>
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
#include <env.h>
#include <fsl_mmdc.h>
#include <netdev.h>
#include <fsl_sec.h>

DECLARE_GLOBAL_DATA_PTR;

extern int get_arc_info(void);
extern void set_kargs_parts(char *);
#ifdef CONFIG_HX3_HUB_INIT
extern int hx3_hub_init(void);
#endif
extern void read_board_rev(void);
extern char module_rev[4];

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
	mdelay(50);
}

#define MASK_CPU_RST	0x00000003
void reset_misc(void)
{
	struct ccsr_gpio *pgpio = (void *)(GPIO1_BASE_ADDR);

	/* Set direction output for GPIO1_30 (LS1012A rev1.2 cpu reset ) */
	/* Set direction output for GPIO1_31 (LS1012A rev1.4 cpu reset) */
	setbits_be32(&pgpio->gpdir, MASK_CPU_RST);

	/* Pull CPU part Reset UP */
	setbits_be32(&pgpio->gpdat, MASK_CPU_RST);
	mdelay(500);
}

static inline unsigned char get_board_rev(int m)
{
	read_board_rev();

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
#ifdef CONFIG_SUBTARGET_DONGLE
	printf("Board: uCLS1012A-SOM PD\n\r");
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
#ifdef CONFIG_TARGET_LS1012AFRWY
	int board_rev;
#endif
	gd->ram_size = tfa_get_dram_size();

	if (!gd->ram_size) {
#ifdef CONFIG_TARGET_LS1012AFRWY
		board_rev = get_board_version();

		if (board_rev & BOARD_REV_C)
			gd->ram_size = SYS_SDRAM_SIZE_1024;
		else
			gd->ram_size = SYS_SDRAM_SIZE_512;
#else
		gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
#endif
	}
	return 0;
}
#else
int dram_init(void)
{
	struct fsl_mmdc_info mparam = {
		0x05180000,	/* mdctl */
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
	gd->ram_size = SYS_SDRAM_SIZE_1024;

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
	u32 id32;

	reset_cx();

	/* Initialize i2c */
	i2c_set_bus_num(0);

	if (i2c_read(SYS_I2C_VR5100_ADDR, 0x00, 1, &id8, 1) < 0)
		printf("Error reading i2c VR5100 information!\n");
	else {
		printf("VR5100(0x%x): ready; ", id8);
		id8 = 0x48;
		i2c_write(SYS_I2C_VR5100_ADDR, 0x66, 1, &id8, 1);
		printf("5V SWBST enabled\n");
	}
	if (i2c_read(SYS_I2C_NCT72_ADDR, 0xFE, 1, &id8, 1) < 0)
		printf("Error reading i2c NCT72 information!\n");
	else
		printf("NCT72(0x%x): ready\n", id8);
	if (i2c_read(SYS_I2C_CX_ADDR, 0x1000, 1, (u8 *)&id32, 1) < 0)
		printf("Error reading i2c CX2070x information!\n");
	else
		printf("CX2070x(0x%x): ready\n", id32);

#ifdef CONFIG_HX3_HUB_INIT
	/* USB hub configuration - needed if defaults are different */
	hx3_hub_init();
#endif

	get_arc_info();
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

#ifdef CONFIG_FSL_CAAM
	sec_init();
#endif

#ifdef CONFIG_FSL_LS_PPA
	ppa_init();
#endif
	return 0;
}

int ft_board_setup(void *blob, bd_t *bd)
{
	arch_fixup_fdt(blob);

	ft_cpu_setup(blob, bd);

	return 0;
}
