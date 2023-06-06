cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(bsu)

include_directories(
        src
        ../../oslib/bsu_drivers/bsu_hci
)

target_sources(app PRIVATE 
	src/main.c
	src/bluetooth.c
	../../oslib/bsu_drivers/bsu_hci/bsu_hci.c
)

zephyr_library_include_directories(${ZEPHYR_BASE}/samples/bluetooth)
