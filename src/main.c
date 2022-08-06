/*
 * Copyright (c) 2022 James B Greer.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <bluetooth/scan.h>


/* advertisement scan filter sets pointer to this fn to handle matched entries */
static void scan_filter_match(struct bt_scan_device_info *device_info,
	struct bt_scan_filter_match *filter_match, bool connectable)
{
	char addr[BT_ADDR_LE_STR_LEN];

	if (device_info != NULL) {
		if (device_info->recv_info != NULL) {
			bt_addr_le_to_str(device_info->recv_info->addr, addr, sizeof(addr));

			printk("address:%s,sid:%d,rssi:%d,tx_power:%d,adv_type:%d,adv_props:%d,interval:%d,primary_phy:%d,secondary_phy:%d\n",
				addr,
				device_info->recv_info->sid, device_info->recv_info->rssi,device_info->recv_info->tx_power, 
				device_info->recv_info->adv_type, device_info->recv_info->adv_props, device_info->recv_info->interval, 
				device_info->recv_info->primary_phy, device_info->recv_info->secondary_phy);
		}

	};
	return;
}

/* magic macro to create a callback struct with ptrs to fns for cases */
BT_SCAN_CB_INIT(scan_cb, scan_filter_match, NULL, NULL, NULL);


/* create struct with manufacturer data to be matched */
static uint8_t mfg_data_array[] = { 0x4C, 0x00 };
static struct bt_scan_manufacturer_data mfg_data = {
	.data = mfg_data_array,
	.data_len = 2
};


void main(void)
{
	int err;

	/* enable use of bluetooth */
	err = bt_enable(NULL);
	if (err) {
		printk("bt_enable failed %d\n", err);
		return;
	}
	printk("bt_enable succeeded\n");

	/* initialize ability to scan for advertisements */
	bt_scan_init(NULL);
	if (err) {
		printk("bt_scan_init failed %d\n", err);
		return;
	}
	printk("bt_scan_init succeeded\n");

	/* register a callback struct for scans - includes pointers to fns */
	bt_scan_cb_register(&scan_cb);
	
	/* add an advertisment filter */
	err = bt_scan_filter_add(BT_SCAN_FILTER_TYPE_MANUFACTURER_DATA, &mfg_data);
	if (err) {
		printk("bt_scan_filter_add failed %d\n", err);
		return;
	}
	printk("bt_scan_filter_add succeeded\n");

	/* enable use of filter */
	err = bt_scan_filter_enable(BT_SCAN_MANUFACTURER_DATA_FILTER, false);
	if (err) {
		printk("bt_scan_filter_enable failed %d\n", err);
		return;
	}
	printk("bt_scan_filter_enable succeeded\n");

	/* start scanning */
	err = bt_scan_start(BT_SCAN_TYPE_SCAN_PASSIVE);
	if (err) {
		printk("bt_scan_start failed %d\n", err);
		return;
	}

	printk("Start NCS BLE Filtered Advertisement Scanner\n");
	return;
}
