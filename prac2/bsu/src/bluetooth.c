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

#include "bluetooth.h"
#include "bsu_hci.h"

LOG_MODULE_REGISTER(ble);

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

static struct bt_conn *default_conn;

static struct bt_uuid* ahu_bt_uuid = BT_UUID_DECLARE_128(AHU_BT_UUID);
// static struct bt_uuid* bsu_bt_uuid = BT_UUID_DECLARE_128(BSU_BT_UUID);

static struct bt_uuid_128 bsu_hci_bt_uuid = BT_UUID_INIT_128(BSU_HCI_BT_UUID);

static struct bt_uuid_128 ahu_req_uuid = BT_UUID_INIT_128(AHU_REQ_UUID);
static struct bt_uuid_128 ahu_val_uuid = BT_UUID_INIT_128(AHU_VAL_UUID);

static struct bt_gatt_discover_params discover_params;
static struct bt_gatt_subscribe_params subscribe_params;

static struct bt_uuid_128 uuid = BT_UUID_INIT_128(0);

static struct bt_gatt_write_params wps;

extern struct shell* sh;

// for testing
static uint16_t write_hdl;
static int subscribed;

// callback for when the ahu subscription notifies a new value
static uint8_t notify_func(struct bt_conn *conn, struct bt_gatt_subscribe_params *params,
	const void *msg, uint16_t length) {
	
    // LOG_INF("notify cb\n\r");
    if (!msg) {
	LOG_INF("[UNSUBSCRIBED]\n");
	params->value_handle = 0U;
	return BT_GATT_ITER_STOP;
    }

    // LOG_INF("[NOTIFICATION] msg %p length %u\n", msg, length);

    // HCI_Recv hci_recv;
    HCIPacket hci_recv;
    memcpy(&hci_recv, msg, length);
    
    if ( hci_recv.pre != 0xAA) {
        memset(&hci_recv, 0, sizeof(HCIPacket));
        return BT_GATT_ITER_CONTINUE;
    }
    
    // hci_recv_msg(&hci_recv, (char*)msg);

    // LOG_WRN("pre: %x, type: %x, len: %x, device id: %x, select: %x, data: %f\r\n", 
	    // hci_recv.pre, hci_recv.type, hci_recv.len, hci_recv.payload.device_id, hci_recv.payload.select, hci_recv.payload.data);
	
    int time_elapsed = k_uptime_get() / 1000;
    shell_print(sh, "{%d, %d, %d: [%f]}\r\n", time_elapsed, hci_recv.payload.device_id, hci_recv.payload.select, hci_recv.payload.data);

    return BT_GATT_ITER_CONTINUE;
}


// callback to check if device matches ahu uuid
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
		if (bt_uuid_cmp(&dev_uuid, ahu_bt_uuid) == 0) {
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

// callback for when bsu writes to the ahu
void write_cb(struct bt_conn *conn, uint8_t err, struct bt_gatt_write_params *params) {
    // LOG_INF("err: %d\r\n", err);
    // LOG_INF("write cb\n\r");
}

// callback for then the bsu find the ahu
void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad) {
	
    char addr_str[BT_ADDR_LE_STR_LEN];
    int err;
    unsigned int found = 0;

    // run callback to check for uuid
    bt_data_parse(ad, data_cb, &found);

    bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
    LOG_INF("%s (RSSI %d)\n", addr_str, rssi);

    if (found) {

	// ahu uuid found
	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	LOG_WRN("AHU found: %s (RSSI %d)\n", addr_str, rssi);

	// stop scanning so you can connect
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

    if (!bt_uuid_cmp(discover_params.uuid, &ahu_req_uuid.uuid)) {
	// found ahu write characteristic
	uint16_t value_hdl = bt_gatt_attr_value_handle(attr);

	// write_hdl only for testing
	write_hdl = value_hdl;

	// move on to discovering subscribe sharacteristic
	discover_params.uuid = &ahu_val_uuid.uuid;
	int err = bt_gatt_discover(conn, &discover_params);
	if (err) {
		LOG_INF("Discover failed(err %d)\n", err);
	}
    } else if (!bt_uuid_cmp(discover_params.uuid, &ahu_val_uuid.uuid)) {
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
		// subscribed only used for testing purposes
		subscribed = 1;
	}
    }
    return BT_GATT_ITER_STOP;
}

// callback for when bsu connects to the ahu
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

    memcpy(&uuid, &bsu_hci_bt_uuid, sizeof(uuid));
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

// callback for when the bsu disconnects from the ahu
void disconnected(struct bt_conn *conn, uint8_t reason) {
    char addr[BT_ADDR_LE_STR_LEN];

    if (conn != default_conn) {
	return;
    }

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    LOG_INF("Disconnected: %s (reason 0x%02x)\n", addr, reason);

    bt_conn_unref(default_conn);
    default_conn = NULL;

    // start_scan();
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

// initialise ble for bsu
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

// sends request command to the AHU depending on the Device ID
void handle_device(int device_id) {
    if (device_id > 0 && device_id <= 0xF) {
        char msg[SENSOR_MSG_SIZE] = {0};
        int ret;
        
        wps.func = write_cb;
        wps.handle = write_hdl;
        wps.offset = 0;
        
        HCI_Send hci_send;
        hci_send.pre = PREAMBLE;
        hci_send.type = REQUEST;
        hci_send.len = SENSOR_REQUEST_LEN;
        hci_send.device_id = device_id;
        hci_send.data = 0;
        
        if (device_id == ORIENT || device_id == ALTITUDE) {
        
            hci_send.select = COURSE;
            hci_send_msg(&hci_send, msg, SENSOR_MSG_SIZE);
            
            /* for (int i = 0; i < SENSOR_MSG_SIZE; i++) {
    	        LOG_WRN("%x", msg[i]);
            } */
            
            wps.data = (const void*)msg;
            wps.length = sizeof(msg);
            ret = bt_gatt_write(default_conn, &wps);
            // LOG_INF("ret: %d\r\n", ret);
            
            hci_send.select = FINE;
            hci_send_msg(&hci_send, msg, SENSOR_MSG_SIZE);
            
            /* for (int i = 0; i < SENSOR_MSG_SIZE; i++) {
    	        LOG_WRN("%x", msg[i]);
            } */
            
            wps.data = (const void*)msg;
            wps.length = sizeof(msg);
            ret = bt_gatt_write(default_conn, &wps);
            // LOG_INF("ret: %d\r\n", ret);
            
        } else {
            hci_send.select = NO_SELECT;
            hci_send_msg(&hci_send, msg, SENSOR_MSG_SIZE);
            
            /* for (int i = 0; i < SENSOR_MSG_SIZE; i++) {
    	        LOG_WRN("%x", msg[i]);
            } */
            
            wps.data = (const void*)msg;
            wps.length = sizeof(msg);
            ret = bt_gatt_write(default_conn, &wps);
            // LOG_INF("ret: %d\r\n", ret);  
        }
    } else {
        LOG_ERR("invalid device id\r\n");
    }
}

// sends "sample on" command to the AHU
void sampling_on(const struct shell* shell, int sample_time) {
    if (sample_time >= MIN_SAMPLE && sample_time <= MAX_SAMPLE) {
    	// LOG_INF("sample time is %d sec\r\n", sample_time);
    	
    	char sample_msg[SAMPLE_MSG_SIZE] = {0};
    	char rate_msg[RATE_MSG_SIZE] = {0};
    	int ret;
    	
    	wps.func = write_cb;
	wps.handle = write_hdl;
	wps.offset = 0;
        
        HCI_Send hci_send;
        hci_send.pre = PREAMBLE;
        hci_send.type = REQUEST;
        
        /*** sample on msg ***/
        hci_send.len = CONT_SAMPLE_LEN;
        hci_send.device_id = CONT_SAMPLE;
        hci_send.select = NO_SELECT;
        hci_send.data = ON << 8;
        
        hci_send_msg(&hci_send, sample_msg, SAMPLE_MSG_SIZE);
        
	/* for (int i = 0; i < SAMPLE_MSG_SIZE; i++) {
	    LOG_WRN("%x", sample_msg[i]);
	} */
	
	wps.data = (const void*)sample_msg;
	wps.length = sizeof(sample_msg);
	ret = bt_gatt_write(default_conn, &wps);
	// LOG_INF("ret: %d\r\n", ret);
	
	/*** sample rate msg ***/
        hci_send.len = SAMPLE_RATE_LEN;
        hci_send.device_id = SAMPLE_RATE;
        hci_send.select = NO_SELECT;
        hci_send.data = ((sample_time & 0x00FF) << 8) | ((sample_time & 0xFF00) >> 8);
        
        hci_send_msg(&hci_send, rate_msg, RATE_MSG_SIZE);
        
	/* for (int i = 0; i < RATE_MSG_SIZE; i++) {
	    LOG_WRN("%x", rate_msg[i]);
	} */
	
	wps.data = (const void*)rate_msg;
	wps.length = sizeof(rate_msg);
	ret = bt_gatt_write(default_conn, &wps);
	// LOG_INF("ret: %d\r\n", ret);
	
	/* */
	
    } else {
    	LOG_ERR("invalid sample time\r\n");
    } 
}

// sends "sample off" command to the AHU
void sampling_off(void) {
    // LOG_INF("sampling is off\r\n");
    
    char msg[SAMPLE_MSG_SIZE] = {0};
    int ret;
        
    HCI_Send hci_send;
    hci_send.pre = PREAMBLE;
    hci_send.type = REQUEST;
    hci_send.len = CONT_SAMPLE_LEN;
    hci_send.device_id = CONT_SAMPLE;
    hci_send.select = NO_SELECT;
    hci_send.data = OFF << 8;
        
    hci_send_msg(&hci_send, msg, SAMPLE_MSG_SIZE);
            
    /* for (int i = 0; i < SAMPLE_MSG_SIZE; i++) {
        LOG_WRN("%x", msg[i]);
    } */
    
    wps.func = write_cb;
    wps.handle = write_hdl;
    wps.offset = 0;
    wps.data = (const void*)msg;
    wps.length = sizeof(msg);
    ret = bt_gatt_write(default_conn, &wps);
    // LOG_INF("ret: %d\r\n", ret);    
}

// handles invalid commands from the shell
void invalid_cmd(void) {
    LOG_ERR("invalid command\r\n");
}





