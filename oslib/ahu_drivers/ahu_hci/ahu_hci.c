#include <zephyr/kernel.h>
#include <zephyr/device.h>

#include "ahu_hci.h"

// baud rate: 115200


void hci_led(int led, int state, char* msg) {
    msg[0] = PREAMBLE;
    msg[1] = (REQUEST << 4) | (TWO_BYTES);
    
    if (state == 1) {
        if (led == 1) {
            msg[2] = (LED1_ON & 0xFF00) >> 8;
            msg[3] = (LED1_ON & 0x00FF) >> 0;
        } else if (led == 2) {
            msg[2] = (LED2_ON & 0xFF00) >> 8;
            msg[3] = (LED1_ON & 0x00FF) >> 0;
        }
    } else if (state == 2) {
        if (led == 1) {
            msg[2] = (LED1_TOGGLE & 0xFF00) >> 8;
            msg[3] = (LED1_TOGGLE & 0x00FF) >> 0;
        } else if (led == 2) {
            msg[2] = (LED2_TOGGLE & 0xFF00) >> 8;
            msg[3] = (LED1_TOGGLE & 0x00FF) >> 0;
        }
    } else {
        if (led == 1) {
            msg[2] = (LED1_OFF & 0xFF00) >> 8;
            msg[3] = (LED1_OFF & 0x00FF) >> 0;
        } else if (led == 2) {
            msg[2] = (LED2_OFF & 0xFF00) >> 8;
            msg[3] = (LED1_OFF & 0x00FF) >> 0;
        }
    }
}

void hci_all(int state, char* msg) {
    msg[0] = PREAMBLE;
    msg[1] = (REQUEST << 4) | (TWO_BYTES);
    
    if (state) {
        msg[2] = (ALL_ON & 0xFF00) >> 8;
        msg[3] = (ALL_ON & 0x00FF) >> 0;
    } else {
        msg[2] = (ALL_OFF & 0xFF00) >> 8;
        msg[3] = (ALL_OFF & 0x00FF) >> 0;
    }
}

void hci_sample(int rate, char* msg) {
    msg[0] = PREAMBLE;
    msg[1] = (REQUEST << 4) | (THREE_BYTES);
    msg[2] = SAMPLE;
    msg[3] = (rate & 0xFF00) >> 8;
    msg[4] = (rate & 0x00FF) >> 0;
}

void hci_sensor(int sensor, char* msg) {
    msg[0] = PREAMBLE;
    msg[1] = (REQUEST << 4) | (ONE_BYTE);
    msg[2] = sensor;
}
