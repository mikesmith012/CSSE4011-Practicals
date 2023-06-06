#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

/* Local Library Include */
#include "pin_ctrl.h"

/* Device Tree Macros */
#define LED0_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static const struct device* gpio_ct_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));

void pin_on(int id) {
    gpio_pin_set_raw(gpio_ct_dev, id, 1);
}

void pin_off(int id) {
    gpio_pin_set_raw(gpio_ct_dev, id, 0);
}


int pin_toggle(int sleep) {
    int ret = gpio_pin_toggle_dt(&led);
    if (ret < 0) {
	return -1;
    }
    k_msleep(sleep);
    return 0;
}

int pin_init(int id) {
    if (!gpio_is_ready_dt(&led)) {
	return -1;
    }
    
    int ret;
    
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
	return -1;
    }
    
    if (!device_is_ready(gpio_ct_dev)) return -1;
    
    ret = gpio_pin_configure(gpio_ct_dev, id, GPIO_OUTPUT_ACTIVE);
    if (ret) return -1;
    
    pin_off(id);
    return 0;
}

