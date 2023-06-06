/* Moblie Node: Thingy52 NRF52832
 * 
 * $ west build -p auto -b thingy52_nrf52832 mobile
 * $ west flash -r jlink
 * $ pyrtt-viewer
 *
 */

#include "bluetooth.h"
#include "led.h"

K_THREAD_DEFINE(mobile_led_thread_tid, MOBILE_LED_THREAD_STACK, mobile_led_thread, NULL, NULL, NULL, MOBILE_LED_THREAD_PRIORITY, 0, 0);
K_THREAD_DEFINE(mobile_ble_thread_tid, MOBILE_BLE_THREAD_STACK, mobile_ble_thread, NULL, NULL, NULL, MOBILE_BLE_THREAD_PRIORITY, 0, 0);

void main(void) {

    while (1) {
        k_msleep(1000);
    }
}

