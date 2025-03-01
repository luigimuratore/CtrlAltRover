#!/usr/bin/env python3
import serial
import time

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)  # Give Arduino time to initialize

print("Raspberry Pi - Arduino Sensor Reader")
print("Enter sensor number (1-4) or 'q' to quit.")

while True:
    sensor_input = input("Sensor number: ").strip()
    if sensor_input.lower() == 'q':
        break
    if sensor_input not in ['1', '2', '3', '4']:
        print("Invalid sensor number. Please enter 1, 2, 3, or 4.")
        continue

    # Send the sensor number command to the Arduino
    ser.write((sensor_input + "\n").encode())
    time.sleep(0.5)  # Wait a bit for the Arduino to respond

    # Read and display the response from Arduino
    while ser.in_waiting:
        response = ser.readline().decode('utf-8').rstrip()
        print(response)

ser.close()
