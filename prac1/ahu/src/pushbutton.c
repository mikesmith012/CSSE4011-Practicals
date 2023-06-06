#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include "ahu_hci.h"
#include "ahu_uart.h"
#include "pushbutton.h"

// LOG MODULE
LOG_MODULE_REGISTER(pb);

extern double measurement;

int cmd_pb(const struct shell* shell, size_t argc, char** argv) {
    char msg[3] = {0};
    if (argv[1][0] == 'r') {
        hci_sensor(PB_STATE, msg);
        uart_send_msg24(msg);
    
        k_msleep(DELAY);
    	LOG_INF("%f\n", measurement);
    	
    } else {
    	LOG_ERR("invalid command\n");
    }
    return 0;
}

void cmd_pb_init(void) {
    SHELL_CMD_ARG_REGISTER(pb, NULL, "pb <r>", cmd_pb, 2, 0);
}
