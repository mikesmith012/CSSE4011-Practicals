#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

#include "ahu_hci.h"
#include "ahu_uart.h"
#include "all.h"

// LOG MODULE
LOG_MODULE_REGISTER(all);

extern double measurement;
extern int device_id;
extern int all;

int cmd_all(const struct shell* shell, size_t argc, char** argv) {
    char msg[4] = {0};
    if (argv[1][0] == 'o') {
    
        hci_all(1, msg);
        uart_send_msg32(msg);
        
    	LOG_INF("on\n");
    	
    	k_msleep(1000);
    	all = 1;

    } else if (argv[1][0] == 'f') {
    
        hci_all(0, msg);
        uart_send_msg32(msg);
    
        all = 0;
        LOG_INF("off\n");
    } else {
    	LOG_ERR("invalid command\n");
    }
    return 0;
}

void cmd_all_init(void) {
    SHELL_CMD_ARG_REGISTER(all, NULL, "all <o, f>", cmd_all, 2, 0);
}


