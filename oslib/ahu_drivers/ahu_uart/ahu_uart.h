#ifndef AHU_UART_H
#define AHU_UART_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#define UART_STACKSIZE 512
#define UART_PRIORITY 3

void serial_cb(const struct device *dev, void *user_data);
void print_uart(char *buf);
void uart_send_byte(unsigned char);
void uart_send_msg24(char*);
void uart_send_msg32(char*);
void uart_send_msg40(char*);
void init_uart(void);
void uart_thread(void);

#endif
