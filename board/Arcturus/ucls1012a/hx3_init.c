// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2018-2023 Arcturus Networks, Inc.
 *           https://www.arcturusnetworks.com/products/ucls1012a/
 */

#include <common.h>
#include <i2c.h>
#include <asm/io.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define HUB_ADDRESS		0x60
#define MAX_I2C_ATTEMPTS	10
#define HX3_SETTINGS_SIZE	192

/* Cypress HX3 hub settings blob */
const uint8_t hx3_settings[5 + HX3_SETTINGS_SIZE] = { /* Arcturus default */
	'C', 'Y', /* Cypress magic signature */
	0x30, /* I2C speed : 100kHz */
	0xd4, /* Image type: Only settings, no firmware */
	HX3_SETTINGS_SIZE, /* payload size (192) */
	0xb4, 0x04, /* VID */
	0x04, 0x65, /* PID */
	00, 50, /* DID */
	0x00, /* [11] - Reserved */
	0x00, /* [12] - USB3 enable 0x0f - all USB3, 0x0d - all USB3, but DS2 is USB2 only */
	0x32, /* bPwrOn2PwrGood : 100 ms */
	0xdf, /* [14] 4 Downstream ports : DS2 is non-removable (MCU) */
	0xf0, /* [15] individual port power switching */
	0x00, /* [16] */
	0x11, /* BC v1.2 and ghost charging enabled */
	0xf0, /* [18] cdp enabled */
	0xe8, /* [19] embedded hub, overcurrent input is active low */
	0x00, /* [20] reserved */
	0x08, /* [21] USB String descriptors enabled (0x08) / disabled (0x00) */
	0x00, 0x00, /* [22] [23] */
	0x12, 0x00, 0x2c, /* [24] [25] [26] */
	0x66, 0x66, /* [27] [28] USB3.0 TX driver de-emphasis */
	0x69, 0x29, 0x29, 0x29, 0x29, /* [29] [30] [31] [32] [33] TX amplitude */
	0x00, /* [34] Reserved */
	0x06, 0x65, /* [35] USB 2.0 PID */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* Reserved */
	0x04, 0x03, 0x09, 0x04, /* LangID = 0x0409 US English */
	56, 0x03, /* [49] [50] Manufacturer string descriptor */
	'2', 0x00, '0', 0x00, '2', 0x00, '3', 0x00,  /* [51] "2023 Arcturus Networks Inc." */
	' ', 0x00, 'A', 0x00, 'r', 0x00, 'c', 0x00,
       	't', 0x00, 'u', 0x00, 'r', 0x00, 'u', 0x00,
       	's', 0x00, ' ', 0x00, 'N', 0x00, 'e', 0x00,
       	't', 0x00, 'w', 0x00, 'o', 0x00, 'r', 0x00,
       	'k', 0x00, 's', 0x00, ' ', 0x00, 'I', 0x00,
       	'n', 0x00, 'c', 0x00, '.', 0x00,
	42, 0x03, /* Product string descriptor "uCLS1012A CY-HX3 HUB" */
	'u', 0x00, 'C', 0x00, 'L', 0x00, 'S', 0x00,
	'1', 0x00, '0', 0x00, '1', 0x00, '2', 0x00,
	'A', 0x00, ' ', 0x00, 'C', 0x00, 'Y', 0x00,
       	'-', 0x00, 'H', 0x00, 'X', 0x00, '3', 0x00,
       	' ', 0x00, 'H', 0x00, 'U', 0x00, 'B', 0x00,
	4, 0x03, /* Serial string descriptor "1" */
	'1', 0x00,
	0
};

int hx3_hub_init(void) {
	int length, index = 0, i2c_attempts = 0;
	const int settings_size = sizeof(hx3_settings); /* cannot larger than 197 */
	uint8_t *data = (uint8_t *)hx3_settings;
	int rc;
	struct udevice *dev = NULL;
	uchar bus = 0;

	i2c_get_chip_for_busnum(bus, HUB_ADDRESS, 2, &dev);
	rc = i2c_get_chip_for_busnum(bus, HUB_ADDRESS, 2, &dev);
	if (rc) {
//		printf("failed to get USB HUB device at address 0x%x\n", HUB_ADDRESS);
		return -1;
	}
	puts("USB: configuring hub....");

	while(index <= settings_size - 1){
		length = MIN(64, (settings_size - index));

		if (dm_i2c_write(dev, index, data, length)) {
			if(i2c_attempts < 1)
				printf("\nWARNING: I2C error during configuring USB hub slave. retrying...\n");
			if(++i2c_attempts >= MAX_I2C_ATTEMPTS){
				printf("ERROR: Maximum USB hub configuration attempts reached. Exiting now\n");
				return 1;
			}
			continue;
		}
		i2c_attempts = 0; /* reset error count */
		index += length;
		data += length;
	}

	puts("Done!\n");
	return 0;
}
