#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>
#include "ahu_hci.h"
#include "ahu_uart.h"
#include "pin_ctrl.h"
#include "led_module.h"

#define LED1 1
#define LED2 2

static int is_led1_on = 0;
static int is_led2_on = 0;

extern int led1_toggle;
extern int led2_toggle;

// LOG MODULE
LOG_MODULE_REGISTER(led_module);

void led_error(void) {
    LOG_ERR("invalid command\n");
}

void led1_ctrl(int on) {
    if (on) {
        pin_on(7);
    } else {
        pin_off(7);
    }
}

void led2_ctrl(int on) {
    if (on) {
        pin_on(9);
    } else {
        pin_off(9);
    }
}

// SHELL CMD CALLBACKS
int cmd_led_on(int led) {
    char msg[4] = {0};
    switch(led) {
	case LED1:
	    led1_toggle = 0;
	    if (is_led1_on) {
	    	LOG_WRN("led 1 is already on\n");
	    } else {
		led1_ctrl(1);
		
		hci_led(1, 1, msg);
		uart_send_msg32(msg);
		
		LOG_INF("led 1 is on\n");
	 	
	    }
	    is_led1_on = 1;
	    break;
	case LED2:
	    led2_toggle = 0;
	    if (is_led2_on) {
	    	LOG_WRN("led 2 is already on\n");
	    } else {
	        led2_ctrl(1);
	       
	        hci_led(2, 1, msg);
	        uart_send_msg32(msg);
	        
		LOG_INF("led 2 is on\n");
	    }
	    is_led2_on = 1;
	    break;
	default:
	    LOG_ERR("invalid led\n");
	    break;
    }
    return 0;
}

int cmd_led_off(int led) {
    char msg[4] = {0};
    switch(led) {
	case LED1:
	    led1_toggle = 0;
	    if (!is_led1_on) {
	    	LOG_WRN("led 1 is already off\n");
	    } else {
		led1_ctrl(0);
		
		hci_led(1, 0, msg);
		uart_send_msg32(msg);
		
		LOG_INF("led 1 is off\n");
	    }
	    is_led1_on = 0;
	    break;
	case LED2:
	    led2_toggle = 0;
	    if (!is_led2_on) {
	    	LOG_WRN("led 2 is already off\n");
	    } else {
		led2_ctrl(0);
		
		hci_led(2, 0, msg);
		uart_send_msg32(msg);
		
		LOG_INF("led 2 is off\n");
	    }
	    is_led2_on = 0;
	    break;
	default:
	    LOG_ERR("invalid led\n");
	    break;
    }
    return 0;
}

int cmd_led_toggle(int led) {
    char msg[4] = {0};
    switch(led) {
	case LED1:
	    led1_toggle = 1;
	    is_led1_on = 1 - is_led1_on;
	    
	    hci_led(1, 2, msg);
	    uart_send_msg32(msg);
	    
	    LOG_INF("led 1 toggled\n");
	    break;
	case LED2:
	    led2_toggle = 1;
	    is_led2_on = 1 - is_led2_on;
	    
	    hci_led(2, 2, msg);
	    uart_send_msg32(msg);
	    
	    LOG_INF("led 2 toggled\n");
	    break;
	default:
	    LOG_ERR("invalid led\n");
	    break;
    }
    return 0;
}

int cmd_led(const struct shell* shell, size_t argc, char** argv) {
    if (argv[1][0] != 'w') {
    	led_error();
    } else {
    	if (argv[2][0] == 'o') {
    	    cmd_led_on(atoi(argv[3]));
    	} else if (argv[2][0] == 'f') {
    	    cmd_led_off(atoi(argv[3]));
    	} else if (argv[2][0] == 't') {
    	    cmd_led_toggle(atoi(argv[3]));
    	} else {
    	    led_error();
    	}
    }
    return 0;
}

void cmd_led_init(void) {
    pin_init(LED1);
    pin_init(LED2);
    
    LOG_DBG("led init OK\n");
    
    SHELL_CMD_ARG_REGISTER(led, NULL, "led w <o, f, t> <1, 2>", cmd_led, 4, 0);
    
    /*SHELL_STATIC_SUBCMD_SET_CREATE(
            led_cmds,
            SHELL_CMD_ARG(o, NULL, "led on\n", cmd_led_on, 1, 0),
            SHELL_CMD_ARG(f, NULL, "led off\n", cmd_led_off, 1, 0),
            SHELL_SUBCMD_SET_END
    );
    SHELL_CMD_REGISTER(led, &led_cmds, "led", NULL);*/
}
