#!/usr/bin/env python3
import serial
import time

# Open the serial port with the LiDAR's UART settings
try:
    ser = serial.Serial('/dev/ttyUSB1', 230400, timeout=1)
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit(1)

time.sleep(2)  # Allow time for the LiDAR to initialize

print("Reading LiDAR data from /dev/ttyUSB1. Press Ctrl+C to exit.")

try:
    while True:
        # Check if data is available
        if ser.in_waiting:
            # Read all available data
            data = ser.read(ser.in_waiting)
            # Convert the binary data to a hex string for easier viewing
            hex_data = data.hex(' ')
            print(hex_data)
        time.sleep(0.1)
except KeyboardInterrupt:
    print("Exiting...")
finally:
    ser.close()
