#!/usr/bin/env python3
import serial
import time

# Set the serial port and baud rate (match the Arduino's Serial.begin value)
SERIAL_PORT = '/dev/ttyACM0'  # Change this if your Arduino is on a different port
BAUD_RATE = 9600

def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        time.sleep(2)  # Allow time for Arduino to reset and initialize
        print(f"Connected to Arduino on {SERIAL_PORT}")
    except Exception as e:
        print("Error connecting to Arduino:", e)
        return

    print("Motor Controller Interface")
    print("Available commands:")
    print("  forward   - Move motors forward")
    print("  backward  - Move motors backward")
    print("  stop      - Stop all motors")
    print("  quit      - Exit this program")

    try:
        while True:
            command = input("Enter command: ").strip()
            if command.lower() == "quit":
                print("Exiting...")
                break

            # Send the command to the Arduino (append newline so Arduino can use readStringUntil('\n'))
            ser.write((command + "\n").encode('utf-8'))

            # Give the Arduino some time to process the command
            time.sleep(0.1)

            # Read and print any response from the Arduino
            while ser.in_waiting:
                response = ser.readline().decode('utf-8').strip()
                if response:
                    print("Arduino:", response)
    finally:
        ser.close()

if __name__ == '__main__':
    main()
