#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/console/console.h>
#include <stdlib.h>

#include "time.h"
#include "pushbutton.h"
#include "lsm6dsl.h"
#include "hts221.h"
#include "lps22.h"
#include "led_module.h"
#include "rgb_module.h"
#include "ahu_uart.h"
#include "sample.h"
#include "all.h"
#include "pin_ctrl.h"
#include "ahu_hci.h"

// SHELL THREAD PARAMS
#define SHELL_STACKSIZE 128
#define SHELL_PRIORITY 6

int is_red_on = 0;
int is_green_on = 0;
int is_blue_on = 0;

int red_toggle = 0;
int green_toggle = 0;
int blue_toggle = 0;

int led1_toggle = 0;
int led2_toggle = 0;

int sample_rate = 1;
double measurement = 0;
int device_id = 0;
int all = 0;

void ahu_shell_thread(void) {
    cmd_all_init();
    cmd_led_init();
    cmd_time_init();
    cmd_rgb_init();
    cmd_pb_init();
    cmd_lsm6dsl_init();
    cmd_lps22_init();
    cmd_hts221_init();
    cmd_sample_rate_init();
    
    while (1) {
        k_usleep(1);
    }
}

K_THREAD_DEFINE(rgb_toggle_id, RGB_STACKSIZE, rgb_toggle_thread, NULL, NULL, NULL, RGB_PRIORITY, 0, 0);

K_THREAD_DEFINE(ahu_shell_thread_id, SHELL_STACKSIZE, ahu_shell_thread, NULL, NULL, NULL, SHELL_PRIORITY, 0, 0);

K_THREAD_DEFINE(uart_thread_id, UART_STACKSIZE, uart_thread, NULL, NULL, NULL, UART_PRIORITY, 0, 0);

// MAIN
void main(void) {
    
    while (1) {
        k_msleep(DELAY);
    }
    
}



