#include "bluetooth.h"
#include "common_hci.h"
#include "uart.h"
#include "list.h"

#include <string.h>
#include <stdio.h>

void init_bluetooth(void);

void handle_device(int device_id);
void sampling_on(const struct shell* shell, int sample_time);
void sampling_off(void);
void invalid_cmd(void);

LOG_MODULE_REGISTER(ble);

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

static struct bt_conn *default_conn;

static struct bt_uuid* mobile_bt_uuid = BT_UUID_DECLARE_128(MOBILE_BT_UUID);
static struct bt_uuid_128 hci_bt_uuid = BT_UUID_INIT_128(HCI_BT_UUID);

static struct bt_uuid_128 mobile_req_uuid = BT_UUID_INIT_128(MOBILE_REQ_UUID);
static struct bt_uuid_128 mobile_val_uuid = BT_UUID_INIT_128(MOBILE_VAL_UUID);

static struct bt_gatt_discover_params discover_params;
static struct bt_gatt_subscribe_params subscribe_params;

static struct bt_uuid_128 uuid = BT_UUID_INIT_128(0);

extern struct shell* sh;
extern struct k_msgq addr_msgq;

static uint16_t write_hdl;
static int subscribed;

extern int is_mobile_connected;

// main bluetooth thread
void base_ble_thread(void) {

    init_bluetooth();
    start_scan();
    
    while (1) {
    	
        k_msleep(100);
    }
}

// callback for when subscription notifies a new value
static uint8_t notify_func(struct bt_conn *conn, struct bt_gatt_subscribe_params *params,
	const void *msg, uint16_t length) {
	
    if (!msg) {
	LOG_INF("[UNSUBSCRIBED]\n");
	params->value_handle = 0U;
	return BT_GATT_ITER_STOP;
    }
    
    HCI_Msg hci_msg;
    memcpy(&hci_msg, msg, length);
    
    if (hci_msg.pre != 0xAA) {
        memset(&hci_msg, 0, sizeof(HCI_Msg));
        return BT_GATT_ITER_CONTINUE;
    }
    
    char out[100] = {0};
    
    char temp[6];
    for (int i = 0; i < 6; i++) {
    	temp[5 - i] = hci_msg.payload.addr[i];
    }
    
    char addr_str[BT_ADDR_LE_STR_LEN];
    bt_addr_t bt_addr;
    memcpy(&bt_addr.val, temp, sizeof(temp));
    
    bt_addr_le_t bt_addr_le;
    bt_addr_le.a = bt_addr;
    bt_addr_le_to_str(&bt_addr_le, addr_str, sizeof(addr_str));
    
    char addr[20] = {0};
    strncpy(addr, addr_str, ADDR_LEN);
    addr[ADDR_LEN - 1] = 0;
    strcpy(out, addr);
    
    char r[15] = {0};
    sprintf(r, ",%d\r\n", hci_msg.payload.rssi);
    strcat(out, r);
    
    while (k_msgq_put(&addr_msgq, out, K_NO_WAIT) != 0) {
    
        // message queue is full: purge old data & try again
        k_msgq_purge(&addr_msgq);
    }

    return BT_GATT_ITER_CONTINUE;
}


// callback to check if device matches uuid
bool data_cb(struct bt_data *data, void *user_data) {
	
    struct bt_uuid dev_uuid;
    char uuid_hm[16];

    switch (data->type) {
	case BT_DATA_UUID128_ALL:
	    // have to copy the data out to separate variable
	    memcpy(uuid_hm, data->data, 16);
	    // convert string to uuid
	    bool created = bt_uuid_create(&dev_uuid, uuid_hm, 16);
	    if (created) {
		if (bt_uuid_cmp(&dev_uuid, mobile_bt_uuid) == 0) {
		    // uuid matches ahu
		    memset(user_data, 1, sizeof(unsigned int));
		    return false;
		}
	    }
	    return true;
	default:
	    return true;
    }
}

// callback for when base writes to mobile
void write_cb(struct bt_conn *conn, uint8_t err, struct bt_gatt_write_params *params) {
    // LOG_INF("write cb\n\r");
}

// callback for then the base find the mobile
void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad) {
	
    char addr_str[BT_ADDR_LE_STR_LEN];
    int err;
    unsigned int found = 0;

    // run callback to check for uuid
    bt_data_parse(ad, data_cb, &found);

    bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
    LOG_INF("%s (RSSI %d)\n", addr_str, rssi);

    if (found) {

	// mobile uuid found
	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	// LOG_WRN("mobile node found: %s (RSSI %d)\n", addr_str, rssi);
	// print_uart("found mobile\r\n");

	// stop scanning to connect
	err = bt_le_scan_stop();
	if (err) {
	    LOG_INF("Problem stopping scan\n\r");
	}
	
	// attempt to connect
	err = bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN,
		BT_LE_CONN_PARAM_DEFAULT, &default_conn);
	if (err) {
	    LOG_INF("Create conn to %s failed (%u)\n", addr_str, err);
	}
    }
}

uint8_t discover_func(struct bt_conn *conn, struct bt_gatt_attr *attr, struct bt_gatt_discover_params *params) {
        
    if (!attr) {
        // LOG_INF("no attr\r\n");
	return BT_GATT_ITER_STOP;
    }

    uint16_t hdl = attr->handle;
    LOG_INF("[ATTRIBUTE] handle %u\n", hdl);

    if (!bt_uuid_cmp(discover_params.uuid, &mobile_req_uuid.uuid)) {
	// found ahu write characteristic
	uint16_t value_hdl = bt_gatt_attr_value_handle(attr);

	// write_hdl only for testing
	write_hdl = value_hdl;

	// move on to discovering subscribe sharacteristic
	discover_params.uuid = &mobile_val_uuid.uuid;
	int err = bt_gatt_discover(conn, &discover_params);
	if (err) {
	    LOG_INF("Discover failed(err %d)\n", err);
	}
    } else if (!bt_uuid_cmp(discover_params.uuid, &mobile_val_uuid.uuid)) {
	// found ahu subscribe characteristic
	uint16_t value_hdl = bt_gatt_attr_value_handle(attr);

	// setup subscribe params & subscribe to notifications
	subscribe_params.value_handle = value_hdl;
	subscribe_params.notify = notify_func;
	subscribe_params.ccc_handle = value_hdl+1;
	subscribe_params.value = BT_GATT_CCC_NOTIFY;

	int err = bt_gatt_subscribe(conn, &subscribe_params);
	if (err && err != -EALREADY) {
	    LOG_INF("Subscribe failed (err %d)\n", err);
	} else {
	    LOG_INF("[SUBSCRIBED]\n");
	    subscribed = 1;
	}
    }
    return BT_GATT_ITER_STOP;
}

// callback for when base connects to mobile
void connected(struct bt_conn *conn, uint8_t err) {

    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (err) {
	LOG_INF("Failed to connect to %s (%u)\n", addr, err);

	bt_conn_unref(default_conn);
	default_conn = NULL;
	
	start_scan();
	return;
    }

    if (conn != default_conn) {
	return;
    }
    LOG_INF("Connected: %s\n", addr);
    // shell_print(sh, "connected\r\n");
    
    is_mobile_connected = 1;

    memcpy(&uuid, &hci_bt_uuid, sizeof(uuid));
    discover_params.uuid = &uuid.uuid;
    discover_params.func = (bt_gatt_discover_func_t)discover_func;
    discover_params.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE;
    discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;
    discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;

    err = bt_gatt_discover(conn, &discover_params);
    if (err) {
	LOG_INF("Discover failed(err %d)\n", err);
	return;
    }
}

// callback for when the base disconnects from the mobile
void disconnected(struct bt_conn *conn, uint8_t reason) {
    char addr[BT_ADDR_LE_STR_LEN];

    if (conn != default_conn) {
	return;
    }

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    LOG_INF("Disconnected: %s (reason 0x%02x)\n", addr, reason);

    bt_conn_unref(default_conn);
    default_conn = NULL;
    
    is_mobile_connected = 0;
    start_scan();
}

// bsu starts scanning for the ble devices
void start_scan(void) {
    int err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, (void*)device_found);
    if (err) {
	LOG_INF("Scanning failed to start (err %d)\n", err);
	return;
    }
    LOG_INF("Scanning successfully started\n");
}

// bsu stops scanning for ble devices
void stop_scan(void) {
    int err = bt_le_scan_stop();
    if (err) {
	LOG_INF("Scanning failed to stop (err %d)\n", err);
	return;
    }
    LOG_INF("Scanning successfully stopped\n");
}

// initialise ble for base
void init_bluetooth(void) {
    int err;

    err = bt_enable(NULL);
    bt_conn_cb_register(&conn_callbacks);
    if (err) {
	LOG_INF("Bluetooth init failed (err %d)\n", err);
	return;
    }
    LOG_INF("Bluetooth initialized\n");
}







