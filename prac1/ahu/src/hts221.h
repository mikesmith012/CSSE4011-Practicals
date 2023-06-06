#ifndef HTS221_H
#define HTS221_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

/* Function Prototypes..*/
int cmd_hts221(const struct shell* shell, size_t argc, char** argv);
void cmd_hts221_init(void);

#endif
