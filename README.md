# CSSE4011 - Advanced Embedded Systems

Taken: Sem 1, 2023

## Repo Structure
```
│   .gitignore
│   README.md
│   
├───oslib
│   ├───ahu_drivers
│   │   ├───ahu_hci
│   │   │       ahu_hci.c
│   │   │       ahu_hci.h
│   │   │       
│   │   ├───ahu_pin_ctrl
│   │   │       pin_ctrl.c
│   │   │       pin_ctrl.h
│   │   │       
│   │   └───ahu_uart
│   │           ahu_uart.c
│   │           ahu_uart.h
│   │
│   ├───bsu_drivers
│   │   └───bsu_hci
│   │           bsu_hci.c
│   │           bsu_hci.h
│   │
│   └───common_drivers
│       └───common_hci
│               common_hci.c
│               common_hci.h
│
├───prac1
│   ├───ahu
│   │   │   app.overlay
│   │   │   CMakeLists.txt
│   │   │   prj.conf
│   │   │   README.md
│   │   │   sample.yaml
│   │   │
│   │   └───src
│   │           all.c
│   │           all.h
│   │           hts221.c
│   │           hts221.h
│   │           led_module.c
│   │           led_module.h
│   │           lps22.c
│   │           lps22.h
│   │           lsm6dsl.c
│   │           lsm6dsl.h
│   │           main.c
│   │           pushbutton.c
│   │           pushbutton.h
│   │           rgb_module.c
│   │           rgb_module.h
│   │           sample.c
│   │           sample.h
│   │           time.c
│   │           time.h
│   │
│   └───diagrams
│           flowcharts.pdf
│           msg_protocol.pdf
│
├───prac2
│   ├───bsu
│   │   │   app.overlay
│   │   │   CMakeLists.txt
│   │   │   prj.conf
│   │   │   README.md
│   │   │   sample.yaml
│   │   │
│   │   └───src
│   │           bluetooth.c
│   │           bluetooth.h
│   │           main.c
│   │
│   ├───diagrams
│   │       CSSE4011_Prac2_Diagrams.pdf
│   │
│   └───dv
│           dv.py
│
└───prac3
    │   README.md
    │
    ├───base
    │   │   app.overlay
    │   │   CMakeLists.txt
    │   │   prj.conf
    │   │   README.md
    │   │   sample.yaml
    │   │
    │   └───src
    │           bluetooth.c
    │           bluetooth.h
    │           list.c
    │           list.h
    │           main.c
    │           uart.c
    │           uart.h
    │           usb.c
    │           usb.h
    │
    └───mobile
        │   app.overlay
        │   CMakeLists.txt
        │   prj.conf
        │   README.md
        │   sample.yaml
        │
        └───src
                bluetooth.c
                bluetooth.h
                led.c
                led.h
                main.c
```