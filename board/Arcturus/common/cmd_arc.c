/*
 * Command for accessing Arcturus factory environment.
 *
 * Copyright 2013-2017 Arcturus Networks Inc.
 *           https://www.arcturusnetworks.com/products/
 *           by Oleksandr G Zhadan et al.
 *
 * SPDX-License-Identifier: GPL-2.0+ BSD-3-Clause
 *
 */

#include <common.h>
#include <div64.h>
#include <malloc.h>
#include <spi_flash.h>
#include <version.h>

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
	strcpy(smac[1], argv[2]);
	strcpy(smac[2], argv[3]);
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
	flash = spi_flash_probe(CONFIG_ENV_SPI_BUS, CONFIG_ENV_SPI_CS,
				CONFIG_ENV_SPI_MAX_HZ, CONFIG_ENV_SPI_MODE);
	if (!flash) {
		printf("Failed to initialize SPI flash at %u:%u\n", CONFIG_ENV_SPI_BUS, CONFIG_ENV_SPI_CS);
		return 0;
	}

#ifdef CONFIG_FIRM_ADDR1
	if (!spi_flash_read(flash, CONFIG_FIRM_ADDR1, sizeof(smac), smac))
		return 1;
#endif
#ifdef CONFIG_FIRM_ADDR2
	if (!spi_flash_read(flash, CONFIG_FIRM_ADDR2, sizeof(smac), smac))
		return 2;
#endif
#ifdef CONFIG_FIRM_ADDR3
	if (!spi_flash_read(flash, CONFIG_FIRM_ADDR3, sizeof(smac), smac))
		return 3;
#endif
#ifdef CONFIG_FIRM_ADDR4
	if (!spi_flash_read(flash, CONFIG_FIRM_ADDR4, sizeof(smac), smac))
		return 4;
#endif
	return 0;
}

int get_arc_info(void)
{
	int l = read_arc_info();
	char *oldserial = getenv("SERIAL");
	char *oldversion = getenv("VERSION");

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
			setenv("SERIAL", smac[0]);
		}
	}

	if (strcmp(smac[1], "00:00:00:00:00:00") == 0) {
		setenv("ethaddr", NULL);
		setenv("eth1addr", NULL);
		setenv("eth2addr", NULL);
		goto done;
	}

	printf("HWADDR0: ");
	if (smac[1][0] == 0xFF) {
		printf("<not found>\n");
	} else {
		char *ret = getenv("ethaddr");

		if (strcmp(ret, __stringify(CONFIG_ETHADDR)) == 0) {
			setenv("ethaddr", smac[1]);
			printf("%s (factory)\n", smac[1]);
		} else {
			printf("%s\n", ret);
		}
	}

	if (strcmp(smac[2], "00:00:00:00:00:00") == 0) {
		setenv("eth1addr", NULL);
		setenv("eth2addr", NULL);
		goto done;
	}

	printf("HWADDR1: ");
	if (smac[2][0] == 0xFF) {
		printf("<not found>\n");
	} else {
		char *ret = getenv("eth1addr");

		if (strcmp(ret, __stringify(CONFIG_ETH1ADDR)) == 0) {
			setenv("eth1addr", smac[2]);
			printf("%s (factory)\n", smac[2]);
		} else {
			printf("%s\n", ret);
		}
	}

	if (strcmp(smac[3], "00:00:00:00:00:00") == 0) {
		setenv("eth2addr", NULL);
		goto done;
	}

	printf("HWADDR2: ");
	if (smac[3][0] == 0xFF) {
		printf("<not found>\n");
	} else {
		char *ret = getenv("eth2addr");

		if (strcmp(ret, __stringify(CONFIG_ETH2ADDR)) == 0) {
			setenv("eth2addr", smac[3]);
			printf("%s (factory)\n", smac[3]);
		} else {
			printf("%s\n", ret);
		}
	}
done:
	if (oldserial == NULL || oldversion == NULL) {
		if (oldversion == NULL)
			setenv("VERSION", U_BOOT_VERSION);
		saveenv();
	}

	return 0;
}

static int do_arc_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
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
done:
	if (ret == -1)
		return CMD_RET_USAGE;
#else
#warning Have to define at least one of the Firmware Info storage address
	printf("%s: No firmfare info storage address is defined\n", __func__);
#endif

	return ret;
}

U_BOOT_CMD(arc, 6, 1, do_arc_cmd,
	   "Arcturus product command sub-system",
	   "product serial hwaddr0 hwaddr1 hwaddr2    - save Arcturus factory env\n"
	   "info                                      - show Arcturus factory env\n\n");
