# Seismic Activity Monitoring Station - Application Host Unit (AHU)

## Mike Smith 46408789

### Functionality / Design Tasks
- System Timer (format)
- AHU RGB LED (on, off, toggle)
- LOG Message Display (inf, wrn, err)
- SCU Sensor Commands (lsm6dsl, lps22, hts221, pb)
- Adjustable sample rate / all (JSON format)
- UART Communication with SCU
- HCI Protocol Implemented

### Folder Structure
```
├───ahu
│   │   app.overlay
│   │   CMakeLists.txt
│   │   prj.conf
│   │   README.md
│   │   sample.yaml
│   │
│   └───src
│           all.c
│           all.h
│           hts221.c
│           hts221.h
│           led_module.c
│           led_module.h
│           lps22.c
│           lps22.h
│           lsm6dsl.c
│           lsm6dsl.h
│           main.c
│           pushbutton.c
│           pushbutton.h
│           rgb_module.c
│           rgb_module.h
│           sample.c
│           sample.h
│           time.c
│           time.h
│
└───diagrams
        flowcharts.pdf
        msg_protocol.pdf
```

## AHU Shell Commands

### sys timer
time <f>

### temp
hts221 r

### acc, gyr, mag
lsm6dsl r <a, g, m> <x, y, z>

### air pressure
lps22 r

### scu leds
led w <o, f, t> <1, 2>

### ahu rgb led
rgb w <o, f, t> <r, g, b>

### push-button
pb r

### sample rate
sample w <sample_rate>

## References
- zephyr/samples/basic/blinky
- zephyr/samples/subsys/shell/shell_module
- zephyr/samples/drivers/uart/echobot



