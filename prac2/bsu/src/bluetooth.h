#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/sys/byteorder.h>

#define NAME_LEN 50

#define MIN_SAMPLE 2
#define MAX_SAMPLE 30

// Custom ahu uuid
#define AHU_BT_UUID \
	BT_UUID_128_ENCODE(0xF53FEC48, 0x62FB, 0x4488, 0x9489, 0x9EE24F719121)
	//F53FEC48-62FB-4488-9489-9EE24F719121

// Custom bsu uuid
#define BSU_BT_UUID \
	BT_UUID_128_ENCODE(0xF53FEC48, 0x62FB, 0x4488, 0x9489, 0x9EE24F719122)
	//F53FEC48-62FB-4488-9489-9EE24F719122
	
// Custom bsu hci uuid
#define BSU_HCI_BT_UUID \
	BT_UUID_128_ENCODE(0xF53FEC48, 0x62FB, 0x4488, 0x9489, 0x9EE24F719123)
	//F53FEC48-62FB-4488-9489-9EE24F719123
	
// Custom ahu request uuid - used for writing a request to the ahu
#define AHU_REQ_UUID \
	BT_UUID_128_ENCODE(0xF53FEC48, 0x62FB, 0x4488, 0x9489, 0x9EE24F719123)
	//F53FEC48-62FB-4488-9489-9EE24F719123

// Custom ahu value uuid - used for subscribing to ahu values
#define AHU_VAL_UUID \
	BT_UUID_128_ENCODE(0xF53FEC48, 0x62FB, 0x4488, 0x9489, 0x9EE24F719124)
	//F53FEC48-62FB-4488-9489-9EE24F719124

// function declarations
void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad);
void connected(struct bt_conn *conn, uint8_t err);
void disconnected(struct bt_conn *conn, uint8_t reason);
void start_scan(void);
void stop_scan(void);
void init_bluetooth(void);

void handle_device(int device_id);

void sampling_on(const struct shell* shell, int sample_time);
void sampling_off(void);
void invalid_cmd(void);

#endif
