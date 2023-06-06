#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/sys/byteorder.h>

#define NAME_LEN 50

#define MIN_SAMPLE 2
#define MAX_SAMPLE 30

#define BASE_BLE_THREAD_PRIORITY 10
#define BASE_BLE_THREAD_STACK 3000

// Custom mobile uuid
#define MOBILE_BT_UUID \
	BT_UUID_128_ENCODE(0xF53FEC48, 0x62FB, 0x4488, 0x9489, 0x9EE24F719121)
	//F53FEC48-62FB-4488-9489-9EE24F719121

// Custom base uuid
#define BASE_BT_UUID \
	BT_UUID_128_ENCODE(0xF53FEC48, 0x62FB, 0x4488, 0x9489, 0x9EE24F719122)
	//F53FEC48-62FB-4488-9489-9EE24F719122
	
// Custom hci uuid
#define HCI_BT_UUID \
	BT_UUID_128_ENCODE(0xF53FEC48, 0x62FB, 0x4488, 0x9489, 0x9EE24F719123)
	//F53FEC48-62FB-4488-9489-9EE24F719123
	
// Custom mobile request uuid - used for writing a request to the mobile node
#define MOBILE_REQ_UUID \
	BT_UUID_128_ENCODE(0xF53FEC48, 0x62FB, 0x4488, 0x9489, 0x9EE24F719123)
	//F53FEC48-62FB-4488-9489-9EE24F719123

// Custom mobile value uuid - used for subscribing to mobile node values
#define MOBILE_VAL_UUID \
	BT_UUID_128_ENCODE(0xF53FEC48, 0x62FB, 0x4488, 0x9489, 0x9EE24F719124)
	//F53FEC48-62FB-4488-9489-9EE24F719124

// function declarations
void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad);
void connected(struct bt_conn *conn, uint8_t err);
void disconnected(struct bt_conn *conn, uint8_t reason);

void base_ble_thread(void);
void start_scan(void);
void stop_scan(void);

#endif
