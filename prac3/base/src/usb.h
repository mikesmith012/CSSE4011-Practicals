#ifndef USB_H
#define USB_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/console/console.h>
#include <zephyr/usb/usb_device.h>

#define BASE_USB_THREAD_PRIORITY 9
#define BASE_USB_THREAD_STACK 1000

void base_usb_thread(void);


#endif
