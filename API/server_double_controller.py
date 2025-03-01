#!/usr/bin/env python3
from flask import Flask, render_template
from flask_socketio import SocketIO
import serial
import time
import threading

# Configure serial connection to Arduino Mega
arduino_port = '/dev/arduinoMega'  # Using the persistent symlink from your udev rule
baud_rate = 115200
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
    return render_template('controller.html')  # Your web interface

speed = 50

@socketio.on('command')
def handle_command(command):
    global speed
    # If the command is a speed update, e.g., "speed:60"
    if command.startswith("speed:"):
        try:
            new_speed = int(command.split(":")[1])
            speed = new_speed
            ser.write((command + "\n").encode())
            print(f"Speed updated to: {speed}")
        except ValueError:
            print("Invalid speed value received.")
        return

    print(f"Received command: {command} with speed: {speed}")
    ser.write((command + "\n").encode())
    print(f"Command '{command}' sent to Arduino.")

def serial_read_thread():
    """Continuously read from the Arduino and emit responses to connected clients."""
    while True:
        if ser.in_waiting:
            try:
                line = ser.readline().decode('utf-8').strip()
                if line:
                    print("Arduino:", line)
                    # Emit the line using the 'arduino_response' event
                    socketio.emit('arduino_response', {'data': line})
            except Exception as e:
                print("Error reading from serial:", e)
        time.sleep(0.01)

if __name__ == '__main__':
    thread = threading.Thread(target=serial_read_thread, daemon=True)
    thread.start()
    socketio.run(app, host='0.0.0.0', port=5001, debug=True, async_mode='eventlet')
