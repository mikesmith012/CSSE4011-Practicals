#ifndef ALL_H
#define ALL_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

/* Function Prototypes..*/
int cmd_all(const struct shell* shell, size_t argc, char** argv);
void cmd_all_init(void);

#endif
