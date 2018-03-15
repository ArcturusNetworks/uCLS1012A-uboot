/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/fsl_serdes.h>
#include <asm/arch/ppa.h>
#include <asm/arch/soc.h>
#include <hwconfig.h>
#include <ahci.h>
#include <mmc.h>
#include <scsi.h>
#include <fsl_csu.h>
#include <fsl_esdhc.h>
#include <environment.h>
#include <fsl_mmdc.h>
#include <netdev.h>

DECLARE_GLOBAL_DATA_PTR;

static void set_wait_for_bits_clear(void *ptr, u32 value, u32 bits)
{
	int timeout = 1000;

	out_be32(ptr, value);

	while (in_be32(ptr) & bits) {
		udelay(100);
		timeout--;
	}
	if (timeout <= 0)
		puts("Error: wait for clear timeout.\n");
}

int checkboard(void)
{
	puts("Board: LS1012AFRDM ");

	return 0;
}

void mmdc_init(void)
{
	struct mmdc_p_regs *mmdc =
		(struct mmdc_p_regs *)CONFIG_SYS_FSL_DDR_ADDR;

	/* Set MMDC_MDSCR[CON_REQ] */
	out_be32(&mmdc->mdscr, 0x00008000);

	/* configure timing parms */
	out_be32(&mmdc->mdotc,  0x12554000);
	out_be32(&mmdc->mdcfg0, 0xbabf7954);
	out_be32(&mmdc->mdcfg1, 0xdb328f64);
	out_be32(&mmdc->mdcfg2, 0x01ff00db);

	/* other parms	*/
	out_be32(&mmdc->mdmisc,    0x00001680);
	out_be32(&mmdc->mpmur0,    0x00000800);
	out_be32(&mmdc->mdrwd,     0x00002000);
	out_be32(&mmdc->mpodtctrl, 0x0000022a);

	/* out of reset delays */
	out_be32(&mmdc->mdor,  0x00bf1023);

	/* physical parms */
	out_be32(&mmdc->mdctl, 0x04180000);
	out_be32(&mmdc->mdasp, 0x0000007f);

	/* Enable MMDC */
	out_be32(&mmdc->mdctl, 0x84180000);

	/* dram init sequence: update MRs */
	out_be32(&mmdc->mdscr, 0x00088032);
	out_be32(&mmdc->mdscr, 0x00008033);
	out_be32(&mmdc->mdscr, 0x00048031);
	out_be32(&mmdc->mdscr, 0x19308030);

	/* dram init sequence: ZQCL */
	out_be32(&mmdc->mdscr,	    0x04008040);
	set_wait_for_bits_clear(&mmdc->mpzqhwctrl, 0xa1390003, 0x00010000);

	/* Calibrations now: wr lvl */
	out_be32(&mmdc->mdscr,   0x00848031);
	out_be32(&mmdc->mdscr,   0x00008200);
	set_wait_for_bits_clear(&mmdc->mpwlgcr, 0x00000001, 0x00000001);

	mdelay(1);

	out_be32(&mmdc->mdscr, 0x00048031);
	out_be32(&mmdc->mdscr, 0x00008000);

	mdelay(1);

	/* Calibrations now: Read DQS gating calibration */
	out_be32(&mmdc->mdscr,     0x04008050);
	out_be32(&mmdc->mdscr,     0x00048033);
	out_be32(&mmdc->mppdcmpr2, 0x00000001);
	out_be32(&mmdc->mprddlctl, 0x40404040);
	set_wait_for_bits_clear(&mmdc->mpdgctrl0, 0x10000000, 0x10000000);

	out_be32(&mmdc->mdscr, 0x00008033);

	/* Calibrations now: Read calibration */
	out_be32(&mmdc->mdscr,       0x04008050);
	out_be32(&mmdc->mdscr,       0x00048033);
	out_be32(&mmdc->mppdcmpr2,   0x00000001);
	set_wait_for_bits_clear(&mmdc->mprddlhwctl, 0x00000010, 0x00000010);

	out_be32(&mmdc->mdscr, 0x00008033);

	/* PD, SR */
	out_be32(&mmdc->mdpdc, 0x00030035);
	out_be32(&mmdc->mapsr, 0x00001067);

	/* refresh scheme */
	set_wait_for_bits_clear(&mmdc->mdref, 0x0f3c8000, 0x00000001);

	/* disable CON_REQ */
	out_be32(&mmdc->mdscr, 0x0);
}

int dram_init(void)
{
	mmdc_init();

	gd->ram_size = 0x20000000;

	return 0;
}

int board_early_init_f(void)
{
	fsl_lsch2_early_init_f();

	return 0;
}

int board_init(void)
{
	struct ccsr_cci400 *cci = (struct ccsr_cci400 *)CONFIG_SYS_CCI400_ADDR;
#ifdef CONFIG_FSL_LS_PPA
	u64 ppa_entry;
#endif
	/*
	 * Set CCI-400 control override register to enable barrier
	 * transaction
	 */
	out_le32(&cci->ctrl_ord, CCI400_CTRLORD_EN_BARRIER);

#ifdef CONFIG_ENV_IS_NOWHERE
	gd->env_addr = (ulong)&default_environment[0];
#endif

#ifdef CONFIG_LAYERSCAPE_NS_ACCESS
	enable_layerscape_ns_access();
#endif

#ifdef CONFIG_FSL_LS_PPA
	ppa_init_pre(&ppa_entry);

	if (ppa_entry)
		ppa_init_entry((void *)ppa_entry);
#endif
	return 0;
}

int ft_board_setup(void *blob, bd_t *bd)
{
	u64 base[CONFIG_NR_DRAM_BANKS];
	u64 size[CONFIG_NR_DRAM_BANKS];

	/* fixup DT for the two DDR banks */
	base[0] = gd->bd->bi_dram[0].start;
	size[0] = gd->bd->bi_dram[0].size;
	base[1] = gd->bd->bi_dram[1].start;
	size[1] = gd->bd->bi_dram[1].size;

	fdt_fixup_memory_banks(blob, base, size, 2);
	ft_cpu_setup(blob, bd);

	return 0;
}

void dram_init_banksize(void)
{
	/*
	 * gd->secure_ram tracks the location of secure memory.
	 * It was set as if the memory starts from 0.
	 * The address needs to add the offset of its bank.
	 */
	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
	if (gd->ram_size > CONFIG_SYS_DDR_BLOCK1_SIZE) {
		gd->bd->bi_dram[0].size = CONFIG_SYS_DDR_BLOCK1_SIZE;
		gd->bd->bi_dram[1].start = CONFIG_SYS_DDR_BLOCK2_BASE;
		gd->bd->bi_dram[1].size = gd->ram_size -
			CONFIG_SYS_DDR_BLOCK1_SIZE;
#ifdef CONFIG_SYS_MEM_RESERVE_SECURE
		gd->secure_ram = gd->bd->bi_dram[1].start +
			gd->secure_ram -
			CONFIG_SYS_DDR_BLOCK1_SIZE;
		gd->secure_ram |= MEM_RESERVE_SECURE_MAINTAINED;
#endif
	} else {
		gd->bd->bi_dram[0].size = gd->ram_size;
#ifdef CONFIG_SYS_MEM_RESERVE_SECURE
		gd->secure_ram = gd->bd->bi_dram[0].start + gd->secure_ram;
		gd->secure_ram |= MEM_RESERVE_SECURE_MAINTAINED;
#endif
	}
}
