#ifndef SAMPLE_H
#define SAMPLE_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

/* Function Prototypes..*/
int cmd_sample_rate(const struct shell* shell, size_t argc, char** argv);
void cmd_sample_rate_init(void);

#endif
