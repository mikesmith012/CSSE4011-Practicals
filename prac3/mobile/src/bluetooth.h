#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include <stddef.h>
#include <errno.h>

#define MOBILE_BLE_THREAD_PRIORITY 10
#define MOBILE_BLE_THREAD_STACK 1000


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
	//F53FEC48-62FB-4488-9489-9EE24F719122
	
// Custom value uuid
#define VAL_BT_UUID \
	BT_UUID_128_ENCODE(0xF53FEC48, 0x62FB, 0x4488, 0x9489, 0x9EE24F719124)
        //F53FEC48-62FB-4488-9489-9EE24F719124


	
void connected(struct bt_conn *conn, uint8_t err);
void disconnected(struct bt_conn *conn, uint8_t reason);
ssize_t read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
ssize_t write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

void mobile_ble_thread(void);

#endif

