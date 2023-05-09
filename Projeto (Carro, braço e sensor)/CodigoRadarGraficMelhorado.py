import matplotlib.pyplot as plt
import math
import serial
from IPython import get_ipython
import numpy as np

get_ipython().run_line_magic('matplotlib', 'qt')

arduino_ports = "COM3"
ser = serial.Serial(arduino_ports,baudrate=115200)
ser.flush()

DIST_MAX = 350
ax = plt.subplot(111, polar=True)

pilha_hor = []
pilha_ant = []
aux = True


while True:
    try:
        ser_bytes = ser.readline()
        decoded_bytes = ser_bytes.decode('utf-8') 
        data = ((decoded_bytes.replace('\r','')).replace('\n','')).replace('et','').replace('s','').split(" ")
        print(data)
        try:
            dist = int(data[0])
        except:
            continue            
        angulo = int(data[1])
     
        ax.clear()
        ax.set_thetamax(180)
        ax.plot(math.pi, DIST_MAX, linewidth=10, linestyle='solid')
        
        line1, = ax.plot([],color='r', linewidth=4.0)
        
        
        if angulo == 0:
            aux = True
        if angulo == 180:
            aux = False
            
        if aux == True:
            pilha_hor.append((angulo*math.pi/180, dist))
            if len(pilha_ant) != 0:
                pilha_ant.pop()
        else:
            pilha_ant.append((angulo*math.pi/180, dist))
            if len(pilha_hor) != 0:
                pilha_hor.pop()
        
        for i in range(len(pilha_hor)):
            if pilha_hor[i][1] < DIST_MAX:
                if i % 5 == 0:
                    ax.plot(*pilha_hor[i], marker="o", color="black", markersize=25)
        for i in range(len(pilha_ant)):
            if i % 5 == 0:
                if pilha_ant[i][1] < DIST_MAX:
                    ax.plot(*pilha_ant[i], marker="o", color="black", markersize=25)
                    #line1.set_data(np.repeat((angulo*(np.pi/180.0)),2), np.linspace(0.0,DIST_MAX,2)), ax.draw_artist(line1)
            
        plt.pause(0.0001)
        plt.ion()
        plt.show(block=False)
    
    
    
    except KeyboardInterrupt:
        print('Keyboard Interrupt')
        break
