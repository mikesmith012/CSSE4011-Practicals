#ifndef BSU_HCI_H
#define BSU_HCI_H

// struct for receiving messages from AHU
typedef struct HCI_Recv {
    int pre;
    int type;
    int len;
    int device_id;
    int select;
    double data;
} HCI_Recv;

// struct for sending messages to AHU
typedef struct HCI_Send {
    int pre;
    int type;
    int len;
    int device_id;
    int select;
    int data;
} HCI_Send;


#pragma pack(1)
typedef struct {
    // preamble
    unsigned int pre:8;
    // HCI packet type
    unsigned int type:4;
    // data length
    unsigned int len:4;
    // data
    struct {
        // SCU device id
        unsigned int device_id:8;
        unsigned int select:8;
        double data;
    } payload;
} HCIPacket;


// function declarations
void hci_recv_msg(HCI_Recv* recv, char* msg);
void hci_send_msg(HCI_Send* send, char* msg, int len);

#define DATA_SIZE 8

#define SENSOR_MSG_SIZE 4
#define SAMPLE_MSG_SIZE 5
#define RATE_MSG_SIZE 6

#define SENSOR_MSG_LEN 4
#define SAMPLE_MSG_LEN 5

#define ERROR -1

// Byte 1: Preamble
#define PREAMBLE_POS 0
#define PREAMBLE 0xAA

// Byte 2: Type & Length
#define TYPE_POS 1
#define REQUEST 0x10
#define RESPONSE 0x20
#define SENSOR_REQUEST_LEN 0x02
#define CONT_SAMPLE_LEN 0x03
#define SAMPLE_RATE_LEN 0x04


// Byte 3: Device ID
#define DID_POS 2
#define TEMP 0x01
#define AIR_PRESS 0x02

#define ACC_X 0x03
#define ACC_Y 0x04
#define ACC_Z 0x05
#define GYR_X 0x06
#define GYR_Y 0x07
#define GYR_Z 0x08
#define MAG_X 0x09
#define MAG_Y 0x0A
#define MAG_Z 0x0B

#define ULTRA_SON 0x0C
#define ORIENT 0x0D
#define ALTITUDE 0x0E
#define PB 0x0F

#define LED1 0x10
#define LED2 0x11

#define CONT_SAMPLE 0xFE
#define SAMPLE_RATE 0xFF

// Byte 4: Select
#define SELECT_POS 3
#define NO_SELECT 0

#define COURSE 1
#define FINE 2

#define PITCH 1
#define ROLL 2

// Byte 5: Command or Data
#define DATA_POS_1 4
#define DATA_POS_2 5

#define OFF 0x00
#define ON 0x01
#define TOGGLE 0x02


#endif
