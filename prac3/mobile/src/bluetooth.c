#include <stdio.h>
#include <string.h>
#include "bluetooth.h"
#include "common_hci.h"

void init_bluetooth(void);
static void start_scan(void);
static void stop_scan(void);

// static struct bt_uuid* mobile_bt_uuid = BT_UUID_DECLARE_128(MOBILE_BT_UUID);
static struct bt_uuid_128 base_bt_uuid = BT_UUID_INIT_128(BASE_BT_UUID);
static struct bt_uuid_128 hci_bt_uuid = BT_UUID_INIT_128(HCI_BT_UUID);
static struct bt_uuid_128 val_bt_uuid = BT_UUID_INIT_128(VAL_BT_UUID);

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

// callback for when base subscribes to mobile
static void ccc_cfg_changed(const struct bt_gatt_attr* attr, uint16_t value) {
    return;
}

// define ble gatt services
BT_GATT_SERVICE_DEFINE(svc,
	BT_GATT_PRIMARY_SERVICE(&base_bt_uuid),
	BT_GATT_CHARACTERISTIC(&hci_bt_uuid.uuid,
	        BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
	        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
	        read_cb, write_cb, NULL),
	BT_GATT_CHARACTERISTIC(&val_bt_uuid.uuid,
		BT_GATT_CHRC_NOTIFY,
                BT_GATT_PERM_NONE,
                NULL, NULL, NULL),
        BT_GATT_CCC(ccc_cfg_changed,
                BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

// main bluetooth thread
void mobile_ble_thread(void) {
    init_bluetooth();
    start_scan();
    while (1) {
        k_msleep(1000);
    }
}

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad) {
			 
    char addr_str[BT_ADDR_LE_STR_LEN];
    char msg[100] = {0};

    bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
    // printk("Device found: %s (RSSI %d)\n", addr_str, rssi);
    
    // uint8_t val[6];
    HCI_Msg hci_msg;
    
    //printk("addr: ");
    for (int i = 0; i < 6; i++) {
    	hci_msg.payload.addr[5 - i] = addr->a.val[i];
    	//printk("%x, ", hci_msg.payload.addr[i]);
    }
    //printk("rssi: %d\n", rssi);
    
    sprintf(msg, "Device found: %s (RSSI %d)", addr_str, rssi);
    
    hci_msg.pre = 0xAA;
    hci_msg.len = sizeof(HCI_Msg);
    
    hci_msg.payload.rssi = rssi;
    
    bt_gatt_notify(NULL, &svc.attrs[3], &hci_msg, sizeof(hci_msg));
    memset(&hci_msg, 0, sizeof(HCI_Msg));
    
    printk("%s\r\n", msg);
}

static void start_scan(void) {
    int err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, device_found);
    if (err) {
	printk("Scanning failed to start (err %d)\n", err);
	return;
    }
    printk("Scanning successfully started\n");
}

static void stop_scan(void) {
    int err = bt_le_scan_stop();
    if (err) {
	printk("Scanning failed to stop (err %d)\n", err);
	return;
    }
    printk("Scanning successfully stopped\n");
}

ssize_t read_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
	void *buf, uint16_t len, uint16_t offset) {
    printk("read cb\n\r");
    return 0;
}

ssize_t write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len, uint16_t offset, uint8_t flags) {
    printk("write cb\n\r");
    return 0;
}

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, MOBILE_BT_UUID),
};

void connected(struct bt_conn *conn, uint8_t err) {
    if (err) {
	printk("connection failed (err 0x%02x)\n", err);
    } else {
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
	printk("connected: %s\n", addr);
    }
}

void disconnected(struct bt_conn *conn, uint8_t reason) {
    printk("disconnected (reason 0x%02x)\n", reason);
}

static void bt_ready(int err) {
    char addr_s[BT_ADDR_LE_STR_LEN];
    bt_addr_le_t addr = {0};
    size_t count = 1;

    if (err) {
	printk("bt init failed (err %d)\n", err);
	return;
    }

    printk("bt initialized\n");

    // Start advertising
    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
	printk("adv failed to start (err %d)\n", err);
	return;
    }

    bt_id_get(&addr, &count);
    bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));

    printk("bt started, advertising as %s\n", addr_s);
}

void init_bluetooth(void) {
    printk("starting bt\n");
    
    int err = bt_enable(bt_ready);
    bt_conn_cb_register(&conn_callbacks);
    if (err) {
	printk("bt init failed (err %d)\n", err);
    }
}

