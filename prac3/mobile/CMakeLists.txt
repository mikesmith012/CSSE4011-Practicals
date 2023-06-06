# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(mobile)

include_directories(
        src
        ../../oslib/common_drivers/common_hci
)

target_sources(app PRIVATE 
	src/main.c
	src/bluetooth.c
	src/led.c
	../../oslib/common_drivers/common_hci/common_hci.c
)

zephyr_library_include_directories(${ZEPHYR_BASE}/samples/bluetooth)
