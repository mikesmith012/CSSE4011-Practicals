#ifndef COMMON_HCI_H
#define COMMON_HCI_H


#pragma pack(1)
typedef struct {
    unsigned int pre: 8; // preamble
    unsigned int len: 8; // data length
    struct {
        char addr[6];	// mac address
        int rssi;	// rssi signal strength
    } payload;
} HCI_Msg;


#endif
