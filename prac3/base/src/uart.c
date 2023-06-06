#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "list.h"
#include "bluetooth.h"

// queue to store up to 10 messages (aligned to 4-byte boundary)
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);

static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

// receive buffer used in UART ISR callback
static char rx_buf[MSG_SIZE];
static int rx_buf_pos;

extern sys_slist_t static_node_list;
extern sys_slist_t all_static_node_list;

extern int is_mobile_connected;

extern struct k_msgq addr_msgq;
static char __aligned(4) addr_msgq_buffer[ADDR_MSG_BUF_SIZE * ADDR_MSG_SIZE];

// Read characters from UART until line end is detected. Afterwards push the data to the message queue.
void serial_cb(const struct device *dev, void *user_data) {
    uint8_t c;

    if (!uart_irq_update(uart_dev)) {
	return;
    }

    if (!uart_irq_rx_ready(uart_dev)) {
	return;
    }

    // read until fifo empty
    while (uart_fifo_read(uart_dev, &c, 1) == 1) {
	if ((c == '\n' || c == '\r') && rx_buf_pos > 0) {
	    // terminate string
	    rx_buf[rx_buf_pos] = '\0';

	    // if queue is full, message is silently dropped
	    k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);

	    // reset the buffer (it was copied to the msgq)
	    rx_buf_pos = 0;
	} else if (rx_buf_pos < (sizeof(rx_buf) - 1)) {
	    rx_buf[rx_buf_pos++] = c;
	}
    }
}

// print a null-terminated string character by character to the UART interface
void print_uart(char *buf) {
    int msg_len = strlen(buf);

    for (int i = 0; i < msg_len; i++) {
	uart_poll_out(uart_dev, buf[i]);
    }
}

// function to update static node information
void update_node(char tx_buf[MSG_SIZE], StaticNode* static_node) {
    const char comma[] = ",";
    const char space[] = " ";
    char *msg, *token, *save;
    
    // strip leading command and seperate data by commas
    msg = strtok_r(tx_buf, space, &save);
    msg = strtok_r(NULL, space, &save);
    
    token = strtok_r(msg, comma, &save);
    for (int i = 0; token != NULL; i++) {
        switch (i) {
            case 0:
            	token[NODE_NAME_LEN - 1] = 0;
                sprintf(static_node->name, "%s", token);
                break;
            case 1:
                break;
            case 2:
                static_node->major = atoi(token);
                break;
            case 3:
                static_node->minor = atoi(token);
                break;
            case 4:
                static_node->x = atoi(token);
                break;
            case 5:
                static_node->y = atoi(token);
                break;
            case 6: 
            	token[NODE_NAME_LEN - 1] = 0;
                sprintf(static_node->left, "%s", token);
                break;
            case 7:
            	token[NODE_NAME_LEN - 1] = 0;
                sprintf(static_node->right, "%s", token);
                break;
        }
    	token = strtok_r(NULL, comma, &save);
    }
}

// main uart thread
void base_uart_thread(void) {

    char tx_buf[MSG_SIZE] = {0};

    if (!device_is_ready(uart_dev)) {
	return;
    }

    // configure interrupt and callback to receive data
    int ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);

    if (ret < 0) {
	return;
    }
    uart_irq_rx_enable(uart_dev);
    
    init_list();
    k_msgq_init(&addr_msgq, addr_msgq_buffer, ADDR_MSG_SIZE, ADDR_MSG_BUF_SIZE);
    
    while (1) {
        
        // get uart input
        if (k_msgq_get(&uart_msgq, &tx_buf, K_MSEC(10)) == 0) {
    
	    // print_uart(tx_buf);
	    // print_uart("\r\n");
	
	    StaticNode* static_node;
	    StaticNodeID* static_node_id;
	
	    // view all static nodes
	    if (strstr(tx_buf, "view") != NULL) {
    	        char out[MSG_SIZE] = {0};
    	    
    	        sprintf(out, "{\"nodes\": [");
	        int count = 0;
    	        SYS_SLIST_FOR_EACH_CONTAINER(&static_node_list, static_node, node) {
    	            char n[50] = {0};
    	            if (count) {
    	                strcat(out, ", ");
    	            }
	            sprintf(n, "[\"%s\", \"%s\", %d, %d, %d, %d, \"%s\", \"%s\"]", 
		            static_node->name, static_node->addr, static_node->major, static_node->minor, 
		            static_node->x, static_node->y, static_node->left, static_node->right);
	            strcat(out, n);
	            count++;
	        }
	        strcat(out, "]}\r\n");
	        print_uart(out);
	    
	    // add static node
	    } else if (strstr(tx_buf, "add") != NULL) {
	        SYS_SLIST_FOR_EACH_CONTAINER(&all_static_node_list, static_node_id, node) {
	    	    if (strstr(tx_buf, static_node_id->addr) != NULL) {
	    	        update_node(tx_buf, static_node_id->static_node);
	    	        sys_slist_append(&static_node_list, &(static_node_id->static_node->node));
	    	    }
	        }
	    
	    // update static node
	    } else if (strstr(tx_buf, "update") != NULL) {
	        SYS_SLIST_FOR_EACH_CONTAINER(&static_node_list, static_node, node) {
	    	    if (strstr(tx_buf, static_node->addr) != NULL) {
	    	        update_node(tx_buf, static_node);
	    	    }
	        }
	    
	    // delete static node
	    } else if (strstr(tx_buf, "delete") != NULL) {
	        sys_snode_t* prev_node = NULL;
	        SYS_SLIST_FOR_EACH_CONTAINER(&static_node_list, static_node, node) {
	    	    if (strstr(tx_buf, static_node->addr) != NULL) {
		        sys_slist_remove(&static_node_list, prev_node, &(static_node->node));
	    	    }
	    	    prev_node = &(static_node->node);
	        }
	    
	    // get data values
	    } else if (strstr(tx_buf, "get") != NULL) {
	        char out[MSG_SIZE] = {0};
    	        sprintf(out, "{\"data\": [");
	        int count = 0;
    	        SYS_SLIST_FOR_EACH_CONTAINER(&static_node_list, static_node, node) {
    	            char n[50] = {0};
    	            
    	            if (count) {
    	                strcat(out, ", ");
    	            }
	            sprintf(n, "[%d, %d, %d]", static_node->x, static_node->y, static_node->rssi);
	            strcat(out, n);
	            count++;
	        }
	        strcat(out, "]}\r\n");
	        print_uart(out);
	    
	    // get connection status
	    } else if (strstr(tx_buf, "status") != NULL) {
	        if (is_mobile_connected) {
	            print_uart("{\"status\": \"connected\"}\r\n");
	        } else {
	            print_uart("{\"status\": \"not connected\"}\r\n");
	        }
	        
	    // manual reconnect command
	    } else if (strstr(tx_buf, "connect") != NULL) {
	    	if (is_mobile_connected) {
	    	    print_uart("{\"error\": \"mobile is still connected\"}\r\n");
	    	} else {
	    	    start_scan();
	    	}
	    
	    // invalid command
	    } else {
	        print_uart("{\"error\": \"invalid command\"}\r\n");
	    }
        }
      
        // get addr and rssi values from bluetooth thread
        char addr[ADDR_MSG_SIZE] = {0};
        if (k_msgq_get(&addr_msgq, &addr, K_MSEC(10)) == 0) {
      
      	    StaticNode* static_node;

	    // update each addr with corresponding rssi value
    	    SYS_SLIST_FOR_EACH_CONTAINER(&static_node_list, static_node, node) {
    	        if (strstr(addr, static_node->addr) != NULL) {
    	            const char comma[] = ",";
    	            char* save;
    	            char* a = strtok_r(addr, comma, &save);
    	            if (a) {
    	                static_node->rssi = atoi(strtok_r(NULL, comma, &save));
    	            }
	        }
	    }
        }
    }
}

