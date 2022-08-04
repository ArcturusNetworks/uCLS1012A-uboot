// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2017-2022 Arcturus Networks, Inc.
 *           https://www.arcturusnetworks.com/products/
 *           by Oleksandr G Zhadan et al.
 */

#include <config.h>
#include <common.h>
#include <env_internal.h>

#define ENVN1(b) #b
#define ENVN(b) ENVN1(b)

#ifndef CONFIG_SYS_FLASH_BASE
#define CONFIG_SYS_FLASH_BASE 0
#endif

#define GETSETENV(e, pn, s) \
	e = env_get(ENVN1(part##pn##base)); \
	if (e != NULL) { \
		pb[s] = (simple_strtoul(e, NULL, 0) - CONFIG_SYS_FLASH_BASE); \
		e = env_get(ENVN1(part##pn##size)); \
		if (e != NULL) { \
			ps[s] = simple_strtoul(e, NULL, 0); \
			s++; \
		} else \
			printf("env " ENVN1(part##pn##size) " not found\n"); \
	} else \
		printf("env " ENVN1(part##pn##base) " not found\n")

static unsigned int pb[MAX_PARTS_NUM];
static unsigned int ps[MAX_PARTS_NUM];

const char pn[MAX_PARTS_NUM][4] = {
#ifdef ADDRESS_PART0
	ENVN(0),
#endif
#ifdef ADDRESS_PART1
	ENVN(1),
#endif
#ifdef ADDRESS_PART2
	ENVN(2),
#endif
#ifdef ADDRESS_PART3
	ENVN(3),
#endif
#ifdef ADDRESS_PARTB
	ENVN(B),
#endif
#ifdef ADDRESS_PARTE
	ENVN(E)
#endif
};

static char args_mtdparts[256];

void set_kargs_parts(char *pname)
{
	int count = 0, i;
	char *tmp;

#ifdef ADDRESS_PART0
	GETSETENV(tmp, 0, count);
#endif
#ifdef ADDRESS_PART1
	GETSETENV(tmp, 1, count);
#endif
#ifdef ADDRESS_PART2
	GETSETENV(tmp, 2, count);
#endif
#ifdef ADDRESS_PART3
	GETSETENV(tmp, 3, count);
#endif
#ifdef ADDRESS_PARTB
	GETSETENV(tmp, B, count);
#endif
#ifdef ADDRESS_PARTE
	GETSETENV(tmp, E, count);
#endif

	strcpy(args_mtdparts, "mtdparts=");
	strcat(args_mtdparts, pname);
	for ( i = 0; i < count; i++) {
		char p[32];

		sprintf(p, "%d@%d(%s),", ps[i], pb[i], pn[i]);
		strcat(args_mtdparts, p);
	}

	*strrchr(args_mtdparts, ',') = ' ';
	env_set("kargs_parts", args_mtdparts);
}
