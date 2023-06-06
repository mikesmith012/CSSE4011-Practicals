#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#include <string.h>
#include "ahu_uart.h"

#define MSG_SIZE 16
#define DATA_SIZE 8

// queue to store up to 10 messages
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);

static const struct device *const uart_dev = DEVICE_DT_GET(DT_ALIAS(uart));

extern double measurement;
extern int device_id;
extern int all;

void serial_cb(const struct device *dev, void *user_data) {
	uint8_t c;
	
	static char rx_buf[MSG_SIZE];
	static int rx_buf_pos = 0;


	if (!uart_irq_update(uart_dev)) {
		return;
	}

	if (!uart_irq_rx_ready(uart_dev)) {
		return;
	}

	while (uart_fifo_read(uart_dev, &c, 1) == 1) {
	
		if (rx_buf_pos < (sizeof(rx_buf) - 1)) {
			rx_buf[rx_buf_pos++] = c;
		}
	}
	rx_buf[rx_buf_pos] = '\0';
	k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);
	rx_buf_pos = 0;
}


void print_uart(char *buf)
{
	int msg_len = strlen(buf);

	for (int i = 0; i < msg_len; i++) {
		uart_poll_out(uart_dev, buf[i]);
	}
}

void uart_send_byte(unsigned char buf) {
    uart_poll_out(uart_dev, buf);
}

void uart_send_msg24(char* msg) {
    uart_send_byte(msg[0]);
    uart_send_byte(msg[1]);
    uart_send_byte(msg[2]);
}

void uart_send_msg32(char* msg) {
    uart_send_byte(msg[0]);
    uart_send_byte(msg[1]);
    uart_send_byte(msg[2]);
    uart_send_byte(msg[3]);
}

void uart_send_msg40(char* msg) {
    uart_send_byte(msg[0]);
    uart_send_byte(msg[1]);
    uart_send_byte(msg[2]);	
    uart_send_byte(msg[3]);
    uart_send_byte(msg[4]);
}

void init_uart(void) {
    if (!device_is_ready(uart_dev)) {
	printk("UART device not found!");
	return;
    }

    int ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);
    
    if (ret < 0) {
	if (ret == -ENOTSUP) {
		printk("Interrupt-driven UART API support not enabled\n");
	} else if (ret == -ENOSYS) {
		printk("UART device does not support interrupt-driven API\n");
	} else {
		printk("Error setting UART callback: %d\n", ret);
	}
	return;
    }
    uart_irq_rx_enable(uart_dev);
}

void uart_thread(void) {
    init_uart();
    char tx_buf[MSG_SIZE];
    char data[DATA_SIZE];
    while (k_msgq_get(&uart_msgq, &tx_buf, K_FOREVER) == 0) {
    
        if (tx_buf[0] == 0xAA) {
        
            for (int i = 0; i < DATA_SIZE; i++) {
                data[i] = tx_buf[i + 3];
            
                // printk("%x, ", tx_buf[i]);
            }
            // printk("\n");
        
            measurement = *(double*)&data;
            device_id = tx_buf[2];
        
            if (all) {		
                printk("<%x>|<%f>\n", device_id, measurement);
            }
        }
        
	k_usleep(1);
    }
}
