#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

#include "lps22.h"

#include "ahu_hci.h"
#include "ahu_uart.h"

// LOG MODULE
LOG_MODULE_REGISTER(lps22);

float air_pressure = 1.09;

extern double measurement;

int cmd_lps22(const struct shell* shell, size_t argc, char** argv) {
    char msg[3] = {0};
    if (argv[1][0] == 'r') {
        hci_sensor(LPS22_AIR_PRESSURE, msg);
        uart_send_msg24(msg);
        
        k_msleep(DELAY);
    	LOG_INF("air pressure: %f kPa\n", measurement);
    } else {
    	LOG_ERR("invalid command\n");
    }
    return 0;
}

void cmd_lps22_init(void) {
    SHELL_CMD_ARG_REGISTER(lps22, NULL, "lps22 <r>", cmd_lps22, 2, 0);
}
