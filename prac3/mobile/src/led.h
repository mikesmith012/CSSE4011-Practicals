#ifndef LED_H
#define LED_H

#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define SLEEP_TIME_MS   10
#define LED0_NODE DT_ALIAS(led0)

#define MOBILE_LED_THREAD_STACK 500
#define MOBILE_LED_THREAD_PRIORITY 9

void mobile_led_thread(void);


#endif

