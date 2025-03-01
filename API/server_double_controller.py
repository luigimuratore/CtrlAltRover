from flask import Flask, render_template
from flask_socketio import SocketIO
import time
import serial

# Set this flag to False since all low-level control is now on Arduino
raspberry = False

# Establish a serial connection to the Arduino Mega
try:
    arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    time.sleep(2)  # Wait for Arduino to reset and initialize
    print("Arduino connected via serial on /dev/ttyUSB0")
except Exception as e:
    print("Error connecting to Arduino:", e)
    arduino = None

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

@app.route('/')
def index():
    return render_template('controller.html')

# Global variable for speed (if needed for drive commands)
speed = 50

@socketio.on('command')
def handle_command(command):
    global speed
    print(f"Received command: {command}")
    
    # Handle speed command updates
    if command.startswith("speed:"):
        try:
            new_speed = int(command.split(":")[1])
            speed = new_speed
            print(f"Speed updated to: {speed}")
        except ValueError:
            print("Invalid speed value received.")
        if arduino:
            arduino.write((command + "\n").encode())
        return

    # For all other commands, forward the command over serial to the Arduino
    if arduino:
        arduino.write((command + "\n").encode())
        print(f"Sent command to Arduino: {command}")
    else:
        print("Arduino not connected, command not sent.")

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5001, debug=True)
