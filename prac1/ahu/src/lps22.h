#ifndef LPS22_H
#define LPS22_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

/* Function Prototypes..*/
int cmd_lps22(const struct shell* shell, size_t argc, char** argv);
void cmd_lps22_init(void);

#endif
