import serial,sys,glob
import numpy as np
import serial.tools.list_ports as COMs

arduino_ports = "COM4"
ser = serial.Serial(arduino_ports,baudrate=115200)
ser.flush()


while True:
    try:
        ser_bytes = ser.readline() 
        decoded_bytes = ser_bytes.decode('utf-8') 
        data = ((decoded_bytes.replace('\r','')).replace('\n','')).split(" ")
        dist = data[0]
        angulo = data[1]
        
        print("dist", dist)
        print("ang", angulo)

    except KeyboardInterrupt:
        print('Keyboard Interrupt')
        break
    