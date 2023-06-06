#ifndef RGB_MODULE_H
#define RGB_MODULE_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

#define RGB_STACKSIZE 128
#define RGB_PRIORITY 6

/* Function Prototypes..*/
void rgb_set(void);
void rgb_set_toggle(char rgb, int toggle);

int cmd_rgb(const struct shell* shell, size_t argc, char** argv);
void cmd_rgb_init(void);

void rgb_toggle_thread(void);

#endif
