# Seismic Activity Monitoring Network - Base Station Unit (BSU)

## Mike Smith 46408789

### Functionality / Design Tasks
- BLE scanning (Auto-connect to AHU)
- Sensor commands (single and continuous)
- Data Viewer for Continuous Mode

### Folder Structure
```
├───bsu
│   │   app.overlay
│   │   CMakeLists.txt
│   │   prj.conf
│   │   README.md
│   │   sample.yaml
│   │
│   └───src
│           bluetooth.c
│           bluetooth.h
│           main.c
│
├───diagrams
│       CSSE4011_Prac2_Diagrams.pdf
│
└───dv
        dv.py
```

### BSU Shell Commands
#### BLE Scanning
- scan <s, p>
#### Sensor Values
- ble <g> <device_id>
#### Continuous mode
- ble <c> <s, p> [sample_time]

### HCI Protocol (BSU <-> AHU)
- Preamble (8-bits): 0xAA
- Type (4-bits): 0x01
- Length (4-bits): 0x??
- Device ID (8-bits): 0x??
- Select (8-bits): 0x00 or 0x01 or 0x02
- Data
    - Request (0-bits)
    - Response (64-bits) 0x???? ???? ???? ????

## References
- zephyr/samples/bluetooth
- zephyr/samples/subsys/usb
