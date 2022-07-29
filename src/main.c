/*
 * Copyright (c) 2022 James B Greer.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>

void main(void)
{
	printk("Hello World! %s\n", CONFIG_BOARD);
}
