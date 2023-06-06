#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

/* Function Prototypes..*/
int cmd_pb(const struct shell* shell, size_t argc, char** argv);
void cmd_pb_init(void);

#endif
