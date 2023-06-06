# Localisation with Machine Learning

## Mike Smith 46408789

### Folder Structure
```
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

### Base Node Serial Commands
- `view` send all node data
- `add` add a static node
- `update` update static node data
- `delete` removes a static node
- `get` gets the rssi values and position for each added node
- `status` get the connection status of the base
- `connect` reconnect base to mobile

### HCI Protocol (Mobile -> Base)
- Preamble (8-bits): 0x AA
- Length (8-bits): 0x ??
- Data
    - Address (48-bits): 0x ?? ?? ?? ?? ?? ??
    - RSSI (32-bits) 0x ?? ?? ?? ??

## References
zephyr/samples/bluetooth
zephyr/samples/subsys/usb


