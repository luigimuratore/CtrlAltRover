#!/usr/bin/env python3
import serial
import time
import numpy as np
import matplotlib.pyplot as plt

# Open serial port
ser = serial.Serial('/dev/ttyUSB1', 230400, timeout=1)
time.sleep(2)  # Wait for sensor to initialize

# Initialize interactive plot (using polar coordinates)
plt.ion()
fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
ax.set_ylim(0, 12)  # Adjust according to maximum distance (e.g., 12 m)

def parse_lidar_data(data):
    """
    Dummy parser: Replace this with actual parsing logic that
    extracts (angle, distance) pairs from each 47-byte packet.
    """
    # For demonstration, create dummy data: 360 points around a circle.
    angles = np.deg2rad(np.linspace(0, 359, 360))
    distances = np.random.uniform(0.5, 12, size=360)
    return angles, distances

try:
    while True:
        if ser.in_waiting:
            raw_data = ser.read(ser.in_waiting)
            # Here, you should segment raw_data into packets and parse them.
            angles, distances = parse_lidar_data(raw_data)
            
            ax.clear()
            ax.set_ylim(0, 12)
            ax.scatter(angles, distances, c='r', s=5)
            plt.draw()
            plt.pause(0.05)
        time.sleep(0.1)
except KeyboardInterrupt:
    print("Exiting...")
finally:
    ser.close()
