#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include "ahu_hci.h"
#include "ahu_uart.h"
#include "pushbutton.h"

// LOG MODULE
LOG_MODULE_REGISTER(lsm6dsl);

float acceleration = 9.8;
float magnetometer = 0.2;
float gyroscope = 4.9;

extern double measurement;

void get_accelerometer_reading(char axis) {
    char msg[3] = {0};
    if (axis == 'x') {
        hci_sensor(LSM6DSL_ACC_X, msg);
        uart_send_msg24(msg);
        
        k_msleep(DELAY);
        LOG_INF("accelerometer x: %f m/s^2\n", measurement);
        
    } else if (axis == 'y') {
        hci_sensor(LSM6DSL_ACC_Y, msg);
        uart_send_msg24(msg);
        
        k_msleep(DELAY);
        LOG_INF("accelerometer y: %f m/s^2\n", measurement);
        
    } else if (axis == 'z') {
    	hci_sensor(LSM6DSL_ACC_Z, msg);
        uart_send_msg24(msg);
        
        k_msleep(DELAY);
        LOG_INF("accelerometer z: %f m/s^2\n", measurement);
        
    } else {
        LOG_ERR("invalid axis\n");
    }
}

void get_gyroscope_reading(char axis) {
    char msg[3] = {0};
    if (axis == 'x') {
        hci_sensor(LSM6DSL_GYR_X, msg);
        uart_send_msg24(msg);
        
        k_msleep(DELAY);
        LOG_INF("gyroscope x: %f dps\n", measurement);
        
    } else if (axis == 'y') {
        hci_sensor(LSM6DSL_GYR_Y, msg);
        uart_send_msg24(msg);
        
        k_msleep(DELAY);
        LOG_INF("gyroscope y: %f dps\n", measurement);
        
    } else if (axis == 'z') {
        hci_sensor(LSM6DSL_GYR_Z, msg);
        uart_send_msg24(msg);
        
        k_msleep(DELAY);
        LOG_INF("gyroscope z: %f dps\n", measurement);
        
    } else {
        LOG_ERR("invalid axis\n");
    }
}

void get_magnetometer_reading(char axis) {
    char msg[3] = {0};
    if (axis == 'x') {
        hci_sensor(LSM6DSL_MAG_X, msg);
        uart_send_msg24(msg);
        
        k_msleep(DELAY);
        LOG_INF("magnetometer x: %f gauss\n", measurement);
        
    } else if (axis == 'y') {
        hci_sensor(LSM6DSL_MAG_Y, msg);
        uart_send_msg24(msg);
        
        k_msleep(DELAY);
        LOG_INF("magnetometer y: %f gauss\n", measurement);
        
    } else if (axis == 'z') {
        hci_sensor(LSM6DSL_MAG_Z, msg);
        uart_send_msg24(msg);
        
        k_msleep(DELAY);
        LOG_INF("magnetometer z: %f gauss\n", measurement);
    } else {
        LOG_ERR("invalid axis\n");
    }
}

int cmd_lsm6dsl(const struct shell* shell, size_t argc, char** argv) {
    if (argv[1][0] == 'r') {
    	if (argv[2][0] == 'a') {
    	    get_accelerometer_reading(argv[3][0]);
    	} else if (argv[2][0] == 'g') {
    	    get_gyroscope_reading(argv[3][0]);
    	} else if (argv[2][0] == 'm') {
    	    get_magnetometer_reading(argv[3][0]);
    	} else {
    	    LOG_ERR("invalid sensor\n");
    	}
    } else {
    	LOG_ERR("invalid command\n");
    }
    return 0;
}

void cmd_lsm6dsl_init(void) {
    SHELL_CMD_ARG_REGISTER(lsm6dsl, NULL, "lsm6dsl <r> <a, g, m> <x, y, z>", cmd_lsm6dsl, 4, 0);
}


