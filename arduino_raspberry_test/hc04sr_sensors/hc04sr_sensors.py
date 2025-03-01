#!/usr/bin/env python3
import serial
import time
import sys
import select

# Adjust the serial port name as needed (e.g., '/dev/ttyACM0' or '/dev/ttyUSB0')
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
time.sleep(2)  # Allow Arduino time to reset

print("Raspberry Pi - Arduino Sensor Reader")
print("Enter a sensor number (1-4) for continuous real-time measurements.")
print("Type 'e' (and press Enter) during continuous reading to exit that mode.")
print("Type 'q' to quit the program.")

while True:
    sensor_input = input("\nSensor number: ").strip()
    if sensor_input.lower() == 'q':
        break
    if sensor_input not in ['1', '2', '3', '4']:
        print("Invalid sensor number. Please enter 1, 2, 3, or 4.")
        continue

    print(f"Starting continuous measurement for sensor {sensor_input}.")
    print("Press 'e' (and then Enter) at any time to exit continuous mode.")
    
    # Enter continuous measurement loop
    while True:
        # Send sensor command to Arduino
        ser.write((sensor_input + "\n").encode())
        time.sleep(0.1)  # Short delay for Arduino to process
        
        # Read and print any available response lines from Arduino
        while ser.in_waiting:
            response = ser.readline().decode('utf-8').rstrip()
            print(response)
        
        # Check non-blockingly if the user has entered 'e' to exit continuous mode
        # Using select.select for non-blocking stdin check
        i, _, _ = select.select([sys.stdin], [], [], 0.1)
        if i:
            line = sys.stdin.readline().strip()
            if line.lower() == 'e':
                print("Exiting continuous measurement mode.")
                break

ser.close()
