#ifndef LSM6DSL_H
#define LSM6DSL_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

/* Function Prototypes..*/
int cmd_lsm6dsl(const struct shell* shell, size_t argc, char** argv);
void cmd_lsm6dsl_init(void);

#endif
