#ifndef AHU_HCI_H
#define AHU_HCI_H

#define PREAMBLE 0xAA

#define REQUEST 0x1
#define RESPONSE 0x2

#define ONE_BYTE 0x1
#define TWO_BYTES 0x2
#define THREE_BYTES 0x3

#define HTS221_TEMP 0x01
#define LPS22_AIR_PRESSURE 0x02

#define LSM6DSL_ACC_X 0x03
#define LSM6DSL_ACC_Y 0x04
#define LSM6DSL_ACC_Z 0x05

#define LSM6DSL_GYR_X 0x06
#define LSM6DSL_GYR_Y 0x07
#define LSM6DSL_GYR_Z 0x08

#define LSM6DSL_MAG_X 0x09
#define LSM6DSL_MAG_Y 0x0A
#define LSM6DSL_MAG_Z 0x0B

#define PB_STATE 0x0C

#define ALL_OFF 0x1000
#define ALL_ON 0x1001

#define SAMPLE 0x11

#define LED1_OFF 0x0D00
#define LED1_ON 0x0D01
#define LED1_TOGGLE 0x0D02

#define LED2_OFF 0x0E00
#define LED2_ON 0x0E01
#define LED2_TOGGLE 0x0E02

#define DELAY 1000

void hci_led(int, int, char*);
void hci_sensor(int, char*);
void hci_sample(int rate, char* msg);
void hci_all(int state, char* msg);


#endif
