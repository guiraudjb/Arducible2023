#!/bin/bash 


SERIAL_PORT="/dev/ttyUSB0"
BAUDRATE=9600  
stty raw speed $BAUDRATE
