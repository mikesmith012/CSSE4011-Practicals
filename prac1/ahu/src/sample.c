#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

#include <stdlib.h>
#include "ahu_uart.h"
#include "ahu_hci.h"
#include "sample.h"

extern int sample_rate;

// LOG MODULE
LOG_MODULE_REGISTER(sample);

void set_sample_rate(char* rate) {
    char msg[5] = {0};
    if (atoi(rate) == 0) {
        LOG_ERR("invalid sample rate\n");
    } else {
        sample_rate = atoi(rate);
        
        hci_sample(sample_rate, msg);
        uart_send_msg40(msg);
        
        LOG_INF("%d sec\n", sample_rate);
    }
}

int cmd_sample_rate(const struct shell* shell, size_t argc, char** argv) {
    if (argv[1][0] == 'w') {
    	set_sample_rate(argv[2]);
    } else {
    	LOG_ERR("invalid command\n");
    }
    return 0;
}

void cmd_sample_rate_init(void) {
    SHELL_CMD_ARG_REGISTER(sample, NULL, "sample <w> <sample_rate>", cmd_sample_rate, 3, 0);
}
