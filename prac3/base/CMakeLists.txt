cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(bsu)

include_directories(
        src
        ../../oslib/bsu_drivers/bsu_hci
        ../../oslib/common_drivers/common_hci
)

target_sources(app PRIVATE 
	src/main.c
	src/bluetooth.c
	src/uart.c
	src/usb.c
	src/list.c
	../../oslib/bsu_drivers/bsu_hci/bsu_hci.c
	../../oslib/common_drivers/common_hci/common_hci.c
)

zephyr_library_include_directories(${ZEPHYR_BASE}/samples/bluetooth)
