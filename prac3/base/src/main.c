/* Base Node: NRF52840 USB Dongle
 * 
 * $ west build -p auto -b nrf52840dongle_nrf52840 base
 * $ nrfutil pkg generate --hw-version 52 --sd-req=0x00 --application build/zephyr/zephyr.hex --application-version 1 base.zip
 * $ nrfutil dfu usb-serial -pkg base.zip -p /dev/ttyACM0
 * $ screen /dev/ttyACM0 115200
 *
 */

#include "bluetooth.h"
#include "uart.h"
#include "usb.h"
#include "list.h"

struct shell* sh = NULL;
struct k_msgq addr_msgq;

int is_mobile_connected = 0;

sys_slist_t static_node_list;
sys_slist_t all_static_node_list;


K_THREAD_DEFINE(base_uart_thread_tid, BASE_UART_THREAD_STACK, base_uart_thread, NULL, NULL, NULL, BASE_UART_THREAD_PRIORITY, 0, 0);
K_THREAD_DEFINE(base_usb_thread_tid, BASE_USB_THREAD_STACK, base_usb_thread, NULL, NULL, NULL, BASE_USB_THREAD_PRIORITY, 0, 0);
K_THREAD_DEFINE(base_ble_thread_tid, BASE_BLE_THREAD_STACK, base_ble_thread, NULL, NULL, NULL, BASE_BLE_THREAD_PRIORITY, 0, 0);

void main(void) {

    while (1) {
        k_msleep(1000);
    }
    
}





