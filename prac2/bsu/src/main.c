#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/console/console.h>
#include <zephyr/usb/usb_device.h>
#include <stdlib.h>

#include "bluetooth.h"
#include "bsu_hci.h"

#define BSU_SHELL_THREAD_PRIORITY 10
#define BSU_SHELL_THREAD_STACK 10000

struct shell* sh = NULL;

BUILD_ASSERT(DT_NODE_HAS_COMPAT(DT_CHOSEN(zephyr_shell_uart), zephyr_cdc_acm_uart),
	"Console device is not ACM CDC UART device");

#if defined(CONFIG_USB_DEVICE_STACK_NEXT)
USBD_CONFIGURATION_DEFINE(config_1, USB_SCD_SELF_POWERED, 200);

USBD_DESC_LANG_DEFINE(sample_lang);
USBD_DESC_STRING_DEFINE(sample_mfr, "ZEPHYR", 1);
USBD_DESC_STRING_DEFINE(sample_product, "Zephyr USBD ACM console", 2);
USBD_DESC_STRING_DEFINE(sample_sn, "0123456789ABCDEF", 3);

USBD_DEVICE_DEFINE(sample_usbd, DEVICE_DT_GET(DT_NODELABEL(zephyr_udc0)), 0x2fe3, 0x0001);
		   
static int enable_usb_device_next(void) {
    int err;
    err = usbd_add_descriptor(&sample_usbd, &sample_lang);
    if (err) {
	return err;
    }
    err = usbd_add_descriptor(&sample_usbd, &sample_mfr);
    if (err) {
	return err;
    }
    err = usbd_add_descriptor(&sample_usbd, &sample_product);
    if (err) {
	return err;
    }
    err = usbd_add_descriptor(&sample_usbd, &sample_sn);
    if (err) {
	return err;
    }
    err = usbd_add_configuration(&sample_usbd, &config_1);
    if (err) {
	return err;
    }
    err = usbd_register_class(&sample_usbd, "cdc_acm_0", 1);
    if (err) {
	return err;
    }
    err = usbd_init(&sample_usbd);
    if (err) {
	return err;
    }
    err = usbd_enable(&sample_usbd);
    if (err) {
	return err;
    }
    return 0;
}
#endif /* IS_ENABLED(CONFIG_USB_DEVICE_STACK_NEXT) */

// callback to handle parameters for the "scan" command
int cmd_scan(const struct shell* shell, size_t argc, char** argv) {
    if (argv[1][0] == 's') {
        start_scan();
    } else if (argv[1][0] == 'p') {
        stop_scan();
    }
    return 0;
}

// callback to handle parameters for the "ble" command
int cmd_ble(const struct shell* shell, size_t argc, char** argv) {

    sh = (struct shell*)shell;

    if (argv[1][0] == 'g') {
        handle_device(atoi(argv[2]));
    } else if (argv[1][0] == 'c') {
    	if (argv[2][0] == 's') {
    	    sampling_on(shell, atoi(argv[3]));
    	} else if (argv[2][0] == 'p') {
    	    sampling_off();
    	} else {
    	    invalid_cmd();
    	}
    }
    return 0;
}

// init the shell
void init_shell(void) {
    SHELL_CMD_ARG_REGISTER(scan, NULL, "scan <s, p>", cmd_scan, 2, 0);
    SHELL_CMD_ARG_REGISTER(ble, NULL, "ble <g, c> <device_id, s, p> [sample_time]", cmd_ble, 3, 1);
}

// thread for the shell
void bsu_shell_thread(void) {
    const struct device *dev;
    uint32_t dtr = 0;

    dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
    if (!device_is_ready(dev)) {
        return;
    }

    while (!dtr) {
        uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
        k_sleep(K_MSEC(100));
    }

    init_shell();

    while (1) {
        k_usleep(10);
    }
}

K_THREAD_DEFINE(bsu_shell_thread_tid, BSU_SHELL_THREAD_STACK, bsu_shell_thread, NULL, NULL, NULL, BSU_SHELL_THREAD_PRIORITY, 0, 0);

// MAIN
void main(void) {

    const struct device *const dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
    uint32_t dtr = 0;

#if defined(CONFIG_USB_DEVICE_STACK_NEXT)
    if (enable_usb_device_next()) {
	return;
    }
#else
    if (usb_enable(NULL)) {
	return;
    }
#endif

    /* Poll if the DTR flag was set */
    while (!dtr) {
	uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
	/* Give CPU resources to low priority threads. */
	k_sleep(K_MSEC(100));
    }

    init_bluetooth();
    while (1) {
        k_msleep(1000);
    }
    
}





