// SPDX-License-Identifier: GPL-2.0+
/*
 * Command for accessing Arcturus factory environment.
 *
 * Copyright 2013-2024 Arcturus Networks Inc.
 *           https://www.arcturusnetworks.com/products/
 *           by Oleksandr G Zhadan et al.
 */

#include <common.h>
#include <div64.h>
#include <malloc.h>
#include <spi_flash.h>
#include <version.h>
#include <env.h>
#include <command.h>
#include <linux/ctype.h>

#include <asm/io.h>

#ifndef CONFIG_SF_DEFAULT_SPEED
#   define CONFIG_SF_DEFAULT_SPEED	1000000
#endif
#ifndef CONFIG_SF_DEFAULT_MODE
#   define CONFIG_SF_DEFAULT_MODE	SPI_MODE0
#endif
#ifndef CONFIG_SF_DEFAULT_CS
#   define CONFIG_SF_DEFAULT_CS		0
#endif
#ifndef CONFIG_SF_DEFAULT_BUS
#   define CONFIG_SF_DEFAULT_BUS	0
#endif

#define MAX_SERIAL_SIZE 15
#define MAX_HWADDR_SIZE 17

static struct spi_flash *flash;
static char smac[4][18];
extern char module_rev[4];
const char prefix[8] = "00:06:3";
const char mac_00[18] = "00:00:00:00:00:00";

static int ishwaddr(char *hwaddr)
{
	if (strlen(hwaddr) == MAX_HWADDR_SIZE)
		if (hwaddr[2] == ':' &&
		    hwaddr[5] == ':' &&
		    hwaddr[8] == ':' &&
		    hwaddr[11] == ':' &&
		    hwaddr[14] == ':')
			return 0;
	return -1;
}

static int isallFF(char *hwaddr)
{
	if (toupper(hwaddr[0]) == 'F' &&
	    toupper(hwaddr[1]) == 'F' &&
	    toupper(hwaddr[3]) == 'F' &&
	    toupper(hwaddr[4]) == 'F' &&
	    toupper(hwaddr[6]) == 'F' &&
	    toupper(hwaddr[7]) == 'F' &&
	    toupper(hwaddr[9]) == 'F' &&
	    toupper(hwaddr[10]) == 'F' &&
	    toupper(hwaddr[12]) == 'F' &&
	    toupper(hwaddr[13]) == 'F' &&
	    toupper(hwaddr[15]) == 'F' &&
	    toupper(hwaddr[16]) == 'F')
			return 1;
	return 0;
}

int set_arc_product(int argc, char *const argv[])
{
	int err = 1;
	char *mystrerr = "ERROR: Failed to save factory info in spi location";

	if (argc != 5)
		return -1;

	/* Check serial number */
	if (strlen(argv[1]) != MAX_SERIAL_SIZE)
		return -1;

	/* Check HWaddrs */
	if (ishwaddr(argv[2]) || ishwaddr(argv[3]) || ishwaddr(argv[4]))
		return -1;

	strcpy(smac[0], argv[1]);
	if (isallFF(argv[2]))
		strcpy(smac[1], mac_00);
	else
		strcpy(smac[1], argv[2]);
	if (isallFF(argv[3]))
		strcpy(smac[2], mac_00);
	else
		strcpy(smac[2], argv[3]);
	if (isallFF(argv[4]))
		strcpy(smac[3], mac_00);
	else
		strcpy(smac[3], argv[4]);

	flash = spi_flash_probe(CONFIG_ENV_SPI_BUS, CONFIG_ENV_SPI_CS,
				CONFIG_ENV_SPI_MAX_HZ, CONFIG_ENV_SPI_MODE);
	if (!flash) {
		printf("Failed to initialize SPI flash\n");
		return -1;
	}

	/* Save factory defaults */
#ifdef CONFIG_FIRM_ADDR1
	if (spi_flash_write(flash, CONFIG_FIRM_ADDR1, sizeof(smac), smac)) {
		printf("%s: %s [1]\n", __func__, mystrerr);
	} else {
		err = 0;
	}
#endif
#ifdef CONFIG_FIRM_ADDR2
	if (spi_flash_write(flash, CONFIG_FIRM_ADDR2, sizeof(smac), smac)) {
		printf("%s: %s [2]\n", __func__, mystrerr);
	} else  {
		err = 0;
	}
#endif
#ifdef CONFIG_FIRM_ADDR3
	if (spi_flash_write(flash, CONFIG_FIRM_ADDR3, sizeof(smac), smac)) {
		printf("%s: %s [3]\n", __func__, mystrerr);
	} else {
		err = 0;
	}
#endif
#ifdef CONFIG_FIRM_ADDR4
	if (spi_flash_write(flash, CONFIG_FIRM_ADDR4, sizeof(smac), smac)) {
		printf("%s: %s [4]\n", __func__, mystrerr);
	} else {
		err = 0;
	}
#endif
	if (err)
		return -2;

	return err;
}

static int read_arc_info(void)
{
	int ret = 0;

	flash = spi_flash_probe(CONFIG_ENV_SPI_BUS, CONFIG_ENV_SPI_CS,
				CONFIG_ENV_SPI_MAX_HZ, CONFIG_ENV_SPI_MODE);
	if (!flash) {
		printf("Failed to initialize SPI flash at %u:%u\n", CONFIG_ENV_SPI_BUS, CONFIG_ENV_SPI_CS);
		return ret;
	}

#ifdef CONFIG_FIRM_ADDR1
	if (!spi_flash_read(flash, CONFIG_FIRM_ADDR1, sizeof(smac), smac)) {
		if (smac[0][0] != 0xFF)
			return 1;
		else
			ret = -1;
	}
#endif
#ifdef CONFIG_FIRM_ADDR2
	if (!spi_flash_read(flash, CONFIG_FIRM_ADDR2, sizeof(smac), smac)) {
		if (smac[0][0] != 0xFF)
			return 2;
		else
			ret = -2;
	}
#endif
#ifdef CONFIG_FIRM_ADDR3
	if (!spi_flash_read(flash, CONFIG_FIRM_ADDR3, sizeof(smac), smac)) {
		if (smac[0][0] != 0xFF)
			return 3;
		else
			ret = -3;
	}
#endif
#ifdef CONFIG_FIRM_ADDR4
	if (!spi_flash_read(flash, CONFIG_FIRM_ADDR4, sizeof(smac), smac)) {
		if (smac[0][0] != 0xFF)
			return 4;
		else
			ret = -4;
	}
#endif
	return ret;
}

int ressurect_arc_info(void)
{
	char *tmpv;
#ifdef CONFIG_ETHADDR2SERIAL
	char key[4];
#endif
	char *mystrerr = "ERROR: Failed to save factory info in spi location";
	int err = read_arc_info();
	int found = 0;

	if (err == 0) {
		printf("%s: failed to read factory info\n", __func__);
		return -1;
	}

	if (smac[1][0] == 0xFF) {
		tmpv = env_get("ethaddr");
		if ((tmpv != NULL) && (strcmp(tmpv, __stringify(CONFIG_ETHADDR)) != 0)) {
			if (tmpv[0] == prefix[0] && tmpv[1] == prefix[1] &&
			    tmpv[2] == prefix[2] && tmpv[3] == prefix[3] &&
			    tmpv[4] == prefix[4] && tmpv[5] == prefix[5] &&
			    tmpv[6] == prefix[6] &&
			    (tmpv[7] == 'b' || tmpv[7] == 'B')) {
#ifdef CONFIG_ETHADDR2SERIAL
				key[0] = tmpv[13];
				key[1] = tmpv[15];
				key[2] = tmpv[16];
				key[3] = 0;
#endif
				strcpy(smac[1], tmpv);
				found = 1;
			}
		}
	}

	if (!found) {
		return -2;
	}

	if (smac[2][0] == 0xFF) {
		tmpv = env_get("eth1addr");
		if ((tmpv != NULL) && (strcmp(tmpv, __stringify(CONFIG_ETH1ADDR)) != 0)) {
			if (tmpv[0] == prefix[0] && tmpv[1] == prefix[1] &&
			    tmpv[2] == prefix[2] && tmpv[3] == prefix[3] &&
			    tmpv[4] == prefix[4] && tmpv[5] == prefix[5] &&
			    tmpv[6] == prefix[6] &&
			    (tmpv[7] == 'b' || tmpv[7] == 'B')) {
				strcpy(smac[2], tmpv);
			}
		} else {
			strcpy(smac[2], "00:00:00:00:00:00");
		}
	}
	if (smac[3][0] == 0xFF) {
		tmpv = env_get("ath2addr");
		if ((tmpv != NULL) && (strcmp(tmpv, __stringify(CONFIG_ETH2ADDR)) != 0)) {
			if (tmpv[0] == prefix[0] && tmpv[1] == prefix[1] &&
			    tmpv[2] == prefix[2] && tmpv[3] == prefix[3] &&
			    tmpv[4] == prefix[4] && tmpv[5] == prefix[5] &&
			    tmpv[6] == prefix[6] &&
			    (tmpv[7] == 'b' || tmpv[7] == 'B')) {
				strcpy(smac[3], tmpv);
			}
		} else {
			strcpy(smac[3], "00:00:00:00:00:00");
		}
	}

	if (smac[0][0] == 0xFF) {
		tmpv = env_get("SERIAL");
		if (tmpv != NULL) {
#ifdef CONFIG_ETHADDR2SERIAL
			if (key[0] == tmpv[12] && key[1] == tmpv[13] && key[2] == tmpv[14]) {
				strncpy(smac[0], tmpv, 15);
				smac[0][15] = 0;
			}
#else
			strncpy(smac[0], tmpv, 15);
			smac[0][15] = 0;
#endif
		}
	}

	/* Ressurect factory defaults */
#ifdef CONFIG_FIRM_ADDR1
	if (spi_flash_write(flash, CONFIG_FIRM_ADDR1, sizeof(smac), smac)) {
		printf("%s: %s [1]\n", __func__, mystrerr);
	} else {
		err = 0;
	}
#endif
#ifdef CONFIG_FIRM_ADDR2
	if (spi_flash_write(flash, CONFIG_FIRM_ADDR2, sizeof(smac), smac)) {
		printf("%s: %s [2]\n", __func__, mystrerr);
	} else  {
		err = 0;
	}
#endif
#ifdef CONFIG_FIRM_ADDR3
	if (spi_flash_write(flash, CONFIG_FIRM_ADDR3, sizeof(smac), smac)) {
		printf("%s: %s [3]\n", __func__, mystrerr);
	} else {
		err = 0;
	}
#endif
#ifdef CONFIG_FIRM_ADDR4
	if (spi_flash_write(flash, CONFIG_FIRM_ADDR4, sizeof(smac), smac)) {
		printf("%s: %s [4]\n", __func__, mystrerr);
	} else {
		err = 0;
	}
#endif
	if (err)
		return -2;

	printf("ARCINFO: ressurected\n");

	return err;
}

int get_arc_info(void)
{
	int l = read_arc_info();
	char *oldserial = env_get("SERIAL");
	char *oldversion = env_get("VERSION");

	if (oldversion != NULL)
		if (strcmp(oldversion, U_BOOT_VERSION) != 0)
			oldversion = NULL;

	if (l > 1)
		printf("%s: using factory info region %d\n", __func__, l);
	else if (l == 0) {
		printf("%s: failed to read factory info\n", __func__);
		return -2;
	}

	if (smac[0][0] != 0) {
		printf("SERIAL:  ");
		if (smac[0][0] == 0xFF) {
			printf("<not found>\n");
		} else {
			printf("%s\n", smac[0]);
			env_set("SERIAL", smac[0]);
		}
	}

	if (strcmp(smac[1], "00:00:00:00:00:00") == 0) {
		env_set("ethaddr", NULL);
		env_set("eth1addr", NULL);
		env_set("eth2addr", NULL);
		goto done;
	}

	printf("HWADDR0: ");
	if (smac[1][0] == 0xFF) {
		printf("<not found>\n");
	} else {
		char *ret = env_get("ethaddr");

		if (strcmp(ret, __stringify(CONFIG_ETHADDR)) == 0) {
			env_set("ethaddr", smac[1]);
			printf("%s (factory)\n", smac[1]);
		} else {
			printf("%s\n", ret);
		}
	}

	if (strcmp(smac[2], "00:00:00:00:00:00") == 0) {
		env_set("eth1addr", NULL);
		env_set("eth2addr", NULL);
		goto done;
	}

	printf("HWADDR1: ");
	if (smac[2][0] == 0xFF) {
		printf("<not found>\n");
	} else {
		char *ret = env_get("eth1addr");

		if (strcmp(ret, __stringify(CONFIG_ETH1ADDR)) == 0) {
			env_set("eth1addr", smac[2]);
			printf("%s (factory)\n", smac[2]);
		} else {
			printf("%s\n", ret);
		}
	}

	if (strcmp(smac[3], "00:00:00:00:00:00") == 0) {
		env_set("eth2addr", NULL);
		goto done;
	}

	printf("HWADDR2: ");
	if (smac[3][0] == 0xFF) {
		printf("<not found>\n");
	} else {
		char *ret = env_get("eth2addr");

		if (strcmp(ret, __stringify(CONFIG_ETH2ADDR)) == 0) {
			env_set("eth2addr", smac[3]);
			printf("%s (factory)\n", smac[3]);
		} else {
			printf("%s\n", ret);
		}
	}
done:
	if (oldserial == NULL || oldversion == NULL) {
		if (oldversion == NULL) {
			env_set("VERSION", U_BOOT_VERSION);
			env_set("board_rev", module_rev);
		}
		env_save();
	}

	return 0;
}

static int do_arc_cmd(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	const char *cmd;
	int ret = -1;

	cmd = argv[1];
	--argc;
	++argv;

#if defined(CONFIG_FIRM_ADDR1) || defined(CONFIG_FIRM_ADDR2) || \
    defined(CONFIG_FIRM_ADDR3) || defined(CONFIG_FIRM_ADDR4)
	if (strcmp(cmd, "product") == 0) {
		ret = set_arc_product(argc, argv);
		goto done;
	}
	if (strcmp(cmd, "info") == 0) {
		ret = get_arc_info();
		goto done;
	}
	if (strcmp(cmd, "ressurect") == 0) {
		ret = ressurect_arc_info();
		goto done;
	}
done:
	if (ret == -1)
		return CMD_RET_USAGE;
#else
#warning Have to define at least one of the Firmware Info storage address
	printf("%s: No firmfare info storage address is defined\n", __func__);
#endif

	return ret;
}

U_BOOT_CMD(arc,   6,   1,     do_arc_cmd,
	   "Arcturus product command sub-system",
	   "product serial hwaddr0 hwaddr1 hwaddr2    - save Arcturus factory env\n"
	   "info                                      - show Arcturus factory env\n\n"
	);
