#include "bsu_hci.h"

// formats msg to be sent to AHU
void hci_send_msg(HCI_Send* send, char* msg, int len) {
    for (int i = 0; i < len; i++) {
        switch (i) {
            case PREAMBLE_POS:
                msg[i] = send->pre;
                break;
            case TYPE_POS:
                msg[i] = send->type | send->len;
                break;
            case DID_POS:
                msg[i] = send->device_id;
                break;
            case SELECT_POS:
                msg[i] = send->select;
                break;
            case DATA_POS_1:
                msg[i] = (send->data & 0xFF00) >> 8;
                break;
            case DATA_POS_2:
                msg[i] = (send->data & 0x00FF) >> 0;
                break;
        }
    }
}

// unpacks formatted msg from AHU
void hci_recv_msg(HCI_Recv* recv, char* msg) {
    recv->pre = msg[0];
    recv->type = (msg[1] & 0xF0) >> 4;
    recv->len = msg[1] & 0xF;
    recv->device_id = msg[2];
    recv->select = msg[3];
    
    char data[DATA_SIZE] = {0};
    for (int i = 0; i < recv->len && i < DATA_SIZE; i++) {
        data[i] = msg[4 + i];
    }
    recv->data = *(double*)&data;
}


