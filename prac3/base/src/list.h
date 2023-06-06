#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/slist.h>


#define BASE_LIST_THREAD_PRIORITY 10
#define BASE_LIST_THREAD_STACK 1000

#define NODE_NAME_LEN 7
#define ADDR_LEN 18
#define NUM_OF_NODES 12

// struct for each static node
typedef struct {
    char name[NODE_NAME_LEN];
    char addr[ADDR_LEN];
    int major;
    int minor;
    int x;
    int y;
    char left[NODE_NAME_LEN];
    char right[NODE_NAME_LEN];
    int rssi;
    sys_snode_t node;
} StaticNode;

// reference struct for all static nodes
typedef struct {
    char addr[ADDR_LEN];
    StaticNode* static_node;
    sys_snode_t node;
} StaticNodeID;

// function declarations
void init_list(void);


// defualt static node definitions

#define A_NAME "4011-A"
#define A_ADDR "F5:75:FE:85:34:67"
#define A_MAJOR 2753
#define A_MINOR 32998 
#define A_X 0
#define A_Y 0

#define B_NAME "4011-B"
#define B_ADDR "E5:73:87:06:1E:86"
#define B_MAJOR 32975
#define B_MINOR 20959 
#define B_X 200
#define B_Y 0

#define C_NAME "4011-C"
#define C_ADDR "CA:99:9E:FD:98:B1"
#define C_MAJOR 26679
#define C_MINOR 40363
#define C_X 400
#define C_Y 0

#define D_NAME "4011-D"
#define D_ADDR "CB:1B:89:82:FF:FE"
#define D_MAJOR 41747
#define D_MINOR 38800
#define D_X 400
#define D_Y 200

#define E_NAME "4011-E"
#define E_ADDR "D4:D2:A0:A4:5C:AC"
#define E_MAJOR 30679
#define E_MINOR 51963
#define E_X 400
#define E_Y 400

#define F_NAME "4011-F"
#define F_ADDR "C1:13:27:E9:B7:7C"
#define F_MAJOR 6195
#define F_MINOR 18394 
#define F_X 200
#define F_Y 400

#define G_NAME "4011-G"
#define G_ADDR "F1:04:48:06:39:A0"
#define G_MAJOR 30525
#define G_MINOR 30544 
#define G_X 0
#define G_Y 400

#define H_NAME "4011-H"
#define H_ADDR "CA:0C:E0:DB:CE:60"
#define H_MAJOR 57395
#define H_MINOR 28931
#define H_X 0
#define H_Y 200


// not deployed (I - L)

#define I_NAME "4011-I"
#define I_ADDR "D4:7F:D4:7C:20:13" 
#define I_MAJOR 60345
#define I_MINOR 49995

#define J_NAME "4011-J"
#define J_ADDR "F7:0B:21:F1:C8:E1" 
#define J_MAJOR 12249
#define J_MINOR 30916

#define K_NAME "4011-K"
#define K_ADDR "FD:E0:8D:FA:3E:4A" 
#define K_MAJOR 36748
#define K_MINOR 11457

#define L_NAME "4011-L"
#define L_ADDR "EE:32:F7:28:FA:AC"
#define L_MAJOR 27564
#define L_MINOR 27589



#define NA "NA"


#endif
