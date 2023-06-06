#ifndef UART_H
#define UART_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#define BASE_UART_THREAD_STACK 8000
#define BASE_UART_THREAD_PRIORITY 9

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_console)
#define MSG_SIZE 1000

#define ADDR_MSG_BUF_SIZE 30
#define ADDR_MSG_SIZE 30

void base_uart_thread(void);
void print_uart(char *buf);

#endif
