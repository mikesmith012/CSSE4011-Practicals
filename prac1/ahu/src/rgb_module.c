#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>

#include "rgb_module.h"
#include "pin_ctrl.h"
#include "ahu_hci.h"

#define RED 'r'
#define GREEN 'g'
#define BLUE 'b'

#define RGB 8

extern int is_red_on;
extern int is_green_on;
extern int is_blue_on;

extern int red_toggle;
extern int green_toggle;
extern int blue_toggle;

// LOG MODULE
LOG_MODULE_REGISTER(rgb_module);

void rgb_error(void) {
    LOG_ERR("invalid command\n");
}


void rgb_one_pulse(void) {
    pin_on(RGB);
    pin_on(RGB);
    pin_on(RGB);
    pin_off(RGB);
    pin_off(RGB);
}

void rgb_zero_pulse(void) {
    pin_on(RGB);
    pin_off(RGB);
    pin_off(RGB);
    pin_off(RGB);
}

void rgb_set(void) {
    if (is_green_on == 1) {
        for (int i = 0; i < 8; i++) {
    	    rgb_one_pulse();   
        }
    } else {
        for (int i = 0; i < 8; i++) {
    	    rgb_zero_pulse();   
        }
    }
    if (is_red_on == 1) {
        for (int i = 0; i < 8; i++) {
    	    rgb_one_pulse();   
        }
    } else {
        for (int i = 0; i < 8; i++) {
    	    rgb_zero_pulse();   
        }
    }
    if (is_blue_on == 1) {
        for (int i = 0; i < 8; i++) {
    	    rgb_one_pulse();   
        }
    } else {
        for (int i = 0; i < 8; i++) {
    	    rgb_zero_pulse();   
        }
    }
}

void rgb_set_toggle(char rgb, int toggle) {
    if (toggle) {
	    if (green_toggle) {
		for (int i = 0; i < 8; i++) {
	    	    rgb_one_pulse();   
		}
	    } else {
		for (int i = 0; i < 8; i++) {
	    	    rgb_zero_pulse();   
		}
	    }
	    if (red_toggle) {
		for (int i = 0; i < 8; i++) {
	    	    rgb_one_pulse();   
		}
	    } else {
		for (int i = 0; i < 8; i++) {
	    	    rgb_zero_pulse();   
		}
	    }
	    if (blue_toggle) {
		for (int i = 0; i < 8; i++) {
	    	    rgb_one_pulse();   
		}
	    } else {
		for (int i = 0; i < 8; i++) {
	    	    rgb_zero_pulse();   
		}
	    }
    } else {
        rgb_set();
    }
}

// SHELL CMD CALLBACKS
int cmd_rgb_on(int rgb) {
    switch(rgb) {
	case RED:
	    red_toggle = 1;
	    if (is_red_on == 1) {
	    	LOG_WRN("red is already on\n");
	    } else {
		LOG_INF("red is on\n");
	    }
	    is_red_on = 1;
	    break;
	case GREEN:
	    green_toggle = 1;
	    if (is_green_on == 1) {
	    	LOG_WRN("green is already on\n");
	    } else {
		LOG_INF("green is on\n");
	    }
	    is_green_on = 1;
	    break;
	case BLUE:
	    blue_toggle = 1;
	    if (is_blue_on == 1) {
	    	LOG_WRN("blue is already on\n");
	    } else {
		LOG_INF("blue is on\n");
	    }
	    is_blue_on = 1;
	    break;
	default:
	    rgb_error();
	    break;
    }
    rgb_set();
    k_usleep(50);
    rgb_set();
    return 0;
}

int cmd_rgb_off(int rgb) {
    switch(rgb) {
	case RED:
	    red_toggle = 0;
	    if (!is_red_on) {
	    	LOG_WRN("red is already off\n");
	    } else {
		LOG_INF("red is off\n");
	    }
	    is_red_on = 0;
	    break;
	case GREEN:
	    green_toggle = 0;
	    if (!is_green_on) {
	    	LOG_WRN("green is already off\n");
	    } else {
		LOG_INF("green is off\n");
	    }
	    is_green_on = 0;
	    break;
	case BLUE:
	    blue_toggle = 0;
	    if (!is_blue_on) {
	    	LOG_WRN("blue is already off\n");
	    } else {
		LOG_INF("blue is off\n");
	    }
	    is_blue_on = 0;
	    break;
	default:
	    rgb_error();
	    break;
    }
    rgb_set();
    k_usleep(50);
    rgb_set();
    return 0;
}

void cmd_rgb_toggle(int rgb) {
    switch(rgb) {
	case RED:
	    red_toggle = 1;
	    is_red_on = 2;
	    LOG_INF("red toggling\n");
	    break;
	case GREEN:
	    green_toggle = 1;
	    is_green_on = 2;
	    LOG_INF("green toggling\n");
	    break;
	case BLUE:
	    blue_toggle = 1;
	    is_blue_on = 2;
	    LOG_INF("blue toggling\n");
	    break;
	default:
	    rgb_error();
	    break;
    }
}

int cmd_rgb(const struct shell* shell, size_t argc, char** argv) {
    if (argv[1][0] != 'w') {
    	rgb_error();
    } else {
    	if (argv[2][0] == 'o') {
    	    cmd_rgb_on(argv[3][0]);
    	} else if (argv[2][0] == 'f') {
    	    cmd_rgb_off(argv[3][0]);
    	} else if (argv[2][0] == 't') {
    	    cmd_rgb_toggle(argv[3][0]);
    	} else {
    	    rgb_error();
    	}
    }
    return 0;
}

void cmd_rgb_init(void) {
    pin_init(7);
    pin_init(8);
    pin_init(9);
    
    LOG_DBG("rgb init OK\n");
    
    SHELL_CMD_ARG_REGISTER(rgb, NULL, "rgb w <o, f, t> <r, g, b>", cmd_rgb, 4, 0);
}

void rgb_toggle_thread(void) {
    int toggle = 0;
    while (1) {
    	rgb_set_toggle(' ', toggle);
    	k_usleep(50);
    	rgb_set_toggle(' ', toggle);
    	
    	toggle = 1 - toggle;
        k_msleep(DELAY);
    }
}


