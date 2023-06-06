#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include "time.h"

// LOG MODULE
LOG_MODULE_REGISTER(time);

int cmd_time(const struct shell* shell, size_t argc, char** argv) {
    int time_elapsed = k_uptime_get() / 1000;
    if (argc > 1 && argv[1][0] == 'f') {
    	int hr = time_elapsed / 3600;
    	int min = (time_elapsed % 3600) / 60;
    	int sec = time_elapsed % 60;
    	LOG_INF("%d hr %d min %d sec\n", hr, min, sec);
    } else if (argc > 1 && argv[1][0] != 'f') {
    	LOG_ERR("invalid command\n");
    } else {
    	LOG_INF("%d sec\n", time_elapsed);
    }
    return 0;
}

void cmd_time_init(void) {
    SHELL_CMD_ARG_REGISTER(time, NULL, "time <f>", cmd_time, 1, 1);
} 

