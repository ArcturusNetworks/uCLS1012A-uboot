/*
 * (C) Copyright 2018
 * Oleksandr Zhadan, Arcturus Networks Inc., <www.arcturusnetworks.com>.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <stdio.h>
#include <stdint.h>

uint64_t swap_uint64(void *ptr)
{
	uint64_t x = (uint64_t)ptr;

	x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
	x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
	x = (x & 0x00FF00FF00FF00FF) << 8 | (x & 0xFF00FF00FF00FF00) >> 8;

	return x;
}

int main(int argc, char *argv[])
{
	FILE *in, *out;
	uint64_t xy, yx;
	int ret = -1;

	if ((in = fopen(argv[1], "r")) != NULL) {
		if ((out = fopen(argv[2], "w")) != NULL) {
			while (fread(&xy, 1, 8, in)) {
				yx = swap_uint64((void *)xy);
				fwrite(&yx, 1, 8, out);
			}
			ret = 0;
			fclose(out);
		}
	}

	return ret;
}
