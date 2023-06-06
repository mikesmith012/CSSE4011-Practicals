#include "led.h"

void init_led(void);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

void mobile_led_thread(void) {
    init_led();
    int count = 0;
    while (1) {
	int ret = gpio_pin_toggle_dt(&led);
	if (ret < 0) {
	    printk("led toggle error\r\n");
	}

	if (count == 1000) {
	    count = 0;
	} else {
	    count++;
	}
	k_msleep(SLEEP_TIME_MS);
    }
}

void init_led(void) {
    if (!gpio_is_ready_dt(&led)) {
	printk("led is not ready\r\n");
    }
    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
	printk("led config error\r\n");
    }
}


