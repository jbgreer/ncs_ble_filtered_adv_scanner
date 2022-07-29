/*
 * Copyright (c) 2022 James B Greer.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <bluetooth/scan.h>


static void scan_filter_match(struct bt_scan_device_info *device_info,
	struct bt_scan_filter_match *filter_match, bool connectable)
{

	printk("scan_filter_match\n");
	return;
}

static void scan_filter_no_match(struct bt_scan_device_info *device_info,
	struct bt_scan_filter_match *filter_match, bool connectable)
{
	/* TODO disable in production 
	printk("scan_filter_no_match\n");
	*/
	return;
}

BT_SCAN_CB_INIT(scan_cb, scan_filter_match, scan_filter_no_match, NULL, NULL);

static const uint8_t mfg_data_array[] = { 0x41, 0x01 };
struct bt_scan_manufacturer_data mfg_data = {
	.data = mfg_data_array,
	.data_len = 2
};

void main(void)
{
	int err;

	err = bt_enable(NULL);
	if (err) {
		printk("bt_enable failed %d\n", err);
		return;
	}
	printk("bt_enable succeeded\n");

	bt_scan_init(NULL);
	if (err) {
		printk("bt_scan_init failed %d\n", err);
		return;
	}
	printk("bt_scan_init succeeded\n");

	bt_scan_cb_register(&scan_cb);
	
	/* add filter */
	err = bt_scan_filter_add(BT_SCAN_FILTER_TYPE_MANUFACTURER_DATA, &mfg_data);
	if (err) {
		printk("bt_scan_filter_add failed %d\n", err);
		return;
	}
	printk("bt_scan_filter_add succeeded\n");

	err = bt_scan_filter_enable(BT_SCAN_MANUFACTURER_DATA_FILTER, false);
	if (err) {
		printk("bt_scan_filter_enable failed %d\n", err);
		return;
	}
	printk("bt_scan_filter_enable succeeded\n");

	err = bt_scan_start(BT_SCAN_TYPE_SCAN_PASSIVE);
	if (err) {
		printk("bt_scan_start failed %d\n", err);
		return;
	}

	printk("Start NCS BLE Filtered Advertisement Scanner\n");
	return;
}
