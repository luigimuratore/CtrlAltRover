#!/usr/bin/env python3
from flask import Flask, render_template
from flask_socketio import SocketIO
import serial
import time

# Configure serial connection to Arduino Mega
arduino_port = '/dev/ttyUSB0'  # adjust if needed
baud_rate = 9600
try:
    ser = serial.Serial(arduino_port, baud_rate, timeout=1)
except Exception as e:
    print(f"Error opening serial port: {e}")
    exit(1)
time.sleep(2)  # Allow Arduino to reset

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

@app.route('/')
def index():
    return render_template('controller.html')  # Your web interface for sending commands

speed = 50

@socketio.on('command')
def handle_command(command):
    global speed
    # If the command is a speed update, e.g., "speed:60"
    if command.startswith("speed:"):
        try:
            new_speed = int(command.split(":")[1])
            speed = new_speed
            # Send the speed update command to Arduino
            ser.write((command + "\n").encode())
            print(f"Speed updated to: {speed}")
        except ValueError:
            print("Invalid speed value received.")
        return

    print(f"Received command: {command} with speed: {speed}")
    # Send the command string to the Arduino over Serial
    ser.write((command + "\n").encode())
    print(f"Command '{command}' sent to Arduino.")

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5001, debug=True)
