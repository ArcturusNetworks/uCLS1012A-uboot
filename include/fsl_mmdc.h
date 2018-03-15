/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef FSL_MMDC_H
#define FSL_MMDC_H

/* MMDC Registers */
struct mmdc_p_regs {
	u32 mdctl;
	u32 mdpdc;
	u32 mdotc;
	u32 mdcfg0;
	u32 mdcfg1;
	u32 mdcfg2;
	u32 mdmisc;
	u32 mdscr;
	u32 mdref;
	u32 res1[2];
	u32 mdrwd;
	u32 mdor;
	u32 mdmrr;
	u32 mdcfg3lp;
	u32 mdmr4;
	u32 mdasp;
	u32 res2[239];
	u32 maarcr;
	u32 mapsr;
	u32 maexidr0;
	u32 maexidr1;
	u32 madpcr0;
	u32 madpcr1;
	u32 madpsr0;
	u32 madpsr1;
	u32 madpsr2;
	u32 madpsr3;
	u32 madpsr4;
	u32 madpsr5;
	u32 masbs0;
	u32 masbs1;
	u32 res3[2];
	u32 magenp;
	u32 res4[239];
	u32 mpzqhwctrl;
	u32 mpzqswctrl;
	u32 mpwlgcr;
	u32 mpwldectrl0;
	u32 mpwldectrl1;
	u32 mpwldlst;
	u32 mpodtctrl;
	u32 mprddqby0dl;
	u32 mprddqby1dl;
	u32 mprddqby2dl;
	u32 mprddqby3dl;
	u32 res5[4];
	u32 mpdgctrl0;
	u32 mpdgctrl1;
	u32 mpdgdlst0;
	u32 mprddlctl;
	u32 mprddlst;
	u32 mpwrdlctl;
	u32 mpwrdlst;
	u32 mpsdctrl;
	u32 mpzqlp2ctl;
	u32 mprddlhwctl;
	u32 mpwrdlhwctl;
	u32 mprddlhwst0;
	u32 mprddlhwst1;
	u32 mpwrdlhwst0;
	u32 mpwrdlhwst1;
	u32 mpwlhwerr;
	u32 mpdghwst0;
	u32 mpdghwst1;
	u32 mpdghwst2;
	u32 mpdghwst3;
	u32 mppdcmpr1;
	u32 mppdcmpr2;
	u32 mpswdar0;
	u32 mpswdrdr0;
	u32 mpswdrdr1;
	u32 mpswdrdr2;
	u32 mpswdrdr3;
	u32 mpswdrdr4;
	u32 mpswdrdr5;
	u32 mpswdrdr6;
	u32 mpswdrdr7;
	u32 mpmur0;
	u32 mpwrcadl;
	u32 mpdccr;
};

#endif /* FSL_MMDC_H */
