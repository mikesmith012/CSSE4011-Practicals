#ifndef LED_MODULE_H
#define LED_MODULE_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

/* Function Prototypes..*/
int cmd_led(const struct shell* shell, size_t argc, char** argv);
void cmd_led_init(void);

void led1_ctrl(int);
void led2_ctrl(int);


#endif
