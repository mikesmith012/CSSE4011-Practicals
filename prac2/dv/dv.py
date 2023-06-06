import sys
import time
import serial
import threading
import matplotlib.pyplot as plt
import re
from matplotlib.animation import FuncAnimation

ALT_DID = 14
ORI_DID = 13

COURSE = 2
FINE = 1

PITCH = 1
ROLL = 2

alt_course_times = []
alt_course_values = []

alt_fine_times = []
alt_fine_values = []

ori_pitch_times = []
ori_pitch_values = []

ori_roll_times = []
ori_roll_values = []

fig, axs = plt.subplots(2, 2)

def test_thread():
    while True:
        print("test")
        time.sleep(1)
        
def serial_thread():
    try:
        ser = serial.Serial("/dev/ttyACM1")
    except:
        ser = serial.Serial("/dev/ttyACM0")
        
    ser.reset_input_buffer()
    print("serial port: " + ser.name + "\n")
    
    # send scan command (looks for the AHU)
    time.sleep(3)
    if ser.isOpen:
        cmd = "scan s\r\n"
        ser.write(cmd.encode("ASCII"))
    
    # sends continuous mode command
    time.sleep(3)
    if ser.isOpen:
        cmd = "ble c s 2\r\n"
        ser.write(cmd.encode("ASCII"))
    
    while True:
        if ser.isOpen():
        
            msg = (ser.readline()).decode("ASCII")

            if "{" in msg and "}" in msg:
                # print(f'msg: {msg}')
                
                [(x1, x2, x3, x4)] = re.findall(r"\{(.*?), (.*?), (.*?): \[(.*?)\]\}", msg)
             
                    
                time_elapsed = int(x1)
                device_id = int(x2)
                select = int(x3)
                value = float(x4)
                    
                if device_id == ALT_DID:
                    if select == COURSE:
                        alt_course_times.append(time_elapsed)
                        alt_course_values.append(value)
                    elif select == FINE:
                        alt_fine_times.append(time_elapsed)
                        alt_fine_values.append(value)
                    else:
                        print("invalid select value for altitude")
                elif device_id == ORI_DID:
                    if select == PITCH:
                        ori_pitch_times.append(time_elapsed)
                        ori_pitch_values.append(value)
                    elif select == ROLL:
                        ori_roll_times.append(time_elapsed)
                        ori_roll_values.append(value)
                    else:
                        print("invalid select value for orientation")
                else:
                    print("invalid device id")
                    
                
                print(f'time: {time_elapsed}, device id: {device_id}, select: {select}, value: {value}')
                    
            
def plot(index):
    #plt.cla()
    #plt.plot(alt_course_times, alt_course_values)
    
    axs[0, 0].cla()
    axs[0, 0].plot(alt_course_times, alt_course_values)
    axs[0, 0].set_title("Altitude (Course)")
    axs[0, 0].set_ylim([0, 100])
    axs[0, 0].set(xlabel = "time (s)", ylabel = "course alt (m)")
    
    axs[0, 1].cla()
    axs[0, 1].plot(alt_fine_times, alt_fine_values)
    axs[0, 1].set_title("Altitude (Fine)")
    axs[0, 1].set_ylim([0, 1])
    axs[0, 1].set(xlabel = "time (s)", ylabel = "fine alt (m)")
    
    axs[1, 0].cla()
    axs[1, 0].plot(ori_pitch_times, ori_pitch_values)
    axs[1, 0].set_title("Orientation (Pitch)")
    axs[1, 0].set_ylim([-90, 90])
    axs[1, 0].set(xlabel = "time (s)", ylabel = "pitch (deg)")
    
    axs[1, 1].cla()
    axs[1, 1].plot(ori_roll_times, ori_roll_values)
    axs[1, 1].set_title("Orientation (Roll)")
    axs[1, 1].set_ylim([-90, 90])
    axs[1, 1].set(xlabel = "time (s)", ylabel = "roll (deg)")
    
    fig.tight_layout()
    

def main():
    print("okay")
    
    th = threading.Thread(target = serial_thread)
    th.start()
    
    ani = FuncAnimation(fig, plot, interval = 1000, cache_frame_data=False)
    
    plt.tight_layout()
    plt.show()
    

if __name__ == "__main__":
    main()
    
    
