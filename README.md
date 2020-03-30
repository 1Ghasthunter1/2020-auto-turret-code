The python script uses pygame to take the position of the joysticks plugged in via
usb to the computer, and pyserial sends those values over serial to an arduino controlling the turret

the arduino script takes the values from serial translates them into motor movement
