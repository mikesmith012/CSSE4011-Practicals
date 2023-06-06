cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ahu)

include_directories(
        src
        ../../oslib/ahu_drivers/ahu_pin_ctrl
	../../oslib/ahu_drivers/ahu_hci
	../../oslib/ahu_drivers/ahu_uart
)

target_sources(app PRIVATE 
	src/main.c 
	src/all.c
	src/time.c
	src/led_module.c
	src/rgb_module.c
	src/pushbutton.c
	src/hts221.c
	src/lps22.c
	src/lsm6dsl.c
	src/sample.c
        ../../oslib/ahu_drivers/ahu_pin_ctrl/pin_ctrl.c
        ../../oslib/ahu_drivers/ahu_hci/ahu_hci.c
        ../../oslib/ahu_drivers/ahu_uart/ahu_uart.c
)
