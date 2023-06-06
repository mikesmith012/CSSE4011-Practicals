#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

#include "hts221.h"

#include "ahu_hci.h"
#include "ahu_uart.h"

// LOG MODULE
LOG_MODULE_REGISTER(hts221);

float temperature = 30.2;

extern double measurement;

int cmd_hts221(const struct shell* shell, size_t argc, char** argv) {
    char msg[3] = {0};
    if (argv[1][0] == 'r') {
        hci_sensor(HTS221_TEMP, msg);
        uart_send_msg24(msg);
        
        k_msleep(DELAY);
    	LOG_INF("temperature: %f C\n", measurement);
    } else {
    	LOG_ERR("invalid command\n");
    }
    return 0;
}

void cmd_hts221_init(void) {
    SHELL_CMD_ARG_REGISTER(hts221, NULL, "hts221 <r>", cmd_hts221, 2, 0);
}
