import serial, time
import pygame
from pygame.locals import *
import math

turret_axis = 0
pitch_axis = 1
serialData = ''
solenoidState = False

arduino = serial.Serial(port='COM4',
                        baudrate=250000,
                        parity = serial.PARITY_NONE,
                        stopbits = serial.STOPBITS_ONE,
                        bytesize=serial.EIGHTBITS,
                        timeout=0)
time.sleep(2);

pygame.init()

joystick_count = pygame.joystick.get_count()

for i in range(joystick_count):
    joystick = pygame.joystick.Joystick(i)
    joystick.init()

    print("Joystick{}".format(i))
    print(joystick.get_name())
    print(joystick.get_numaxes())


def getXVal():
    global turretVal
    if joystick.get_button(2) == 1:
        turretVal = "-1"
    else:
        turretVal = joystick.get_axis(turret_axis)
        turretVal = round(turretVal, 2)
        turretVal = turretVal*100
        turretVal = turretVal + 200
        turretVal = str(turretVal)
        turretVal = turretVal[:3]

def getYVal():
    global pitchVal
    pitchVal = joystick.get_axis(pitch_axis)
    pitchVal = round(pitchVal, 2)
    pitchVal = pitchVal*100
    pitchVal = pitchVal + 200
    pitchVal = str(pitchVal)
    pitchVal = pitchVal[:3]

def getSolenoidState():
    global serialSolenoidState
    solenoidState = joystick.get_button(1)
    if solenoidState == True:
        serialSolenoidState = "1"
        return serialSolenoidState
    elif solenoidState == False:
        serialSolenoidState = "0"

        

    

    
def writeToSerial():
    time.sleep(.01)
    getXVal()
    getYVal()
    getSolenoidState()
    serialData = ('<'+turretVal+", "+pitchVal+", "+serialSolenoidState+">")
    print(serialData)
    arduino.write(bytes(serialData, 'utf-8'))
        

while True:
    pygame.event.pump()
    writeToSerial()
    
    




