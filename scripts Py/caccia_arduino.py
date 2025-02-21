#!/usr/bin/env python3
import time
import serial
import RPi.GPIO as GPIO

# Sensor pin configuration for HC-SR04
# Front sensor
TRIG_FRONT = 22
ECHO_FRONT = 4
# Rear sensor
TRIG_REAR = 3
ECHO_REAR = 18
# Left sensor
TRIG_LEFT = 24
ECHO_LEFT = 27
# Right sensor
TRIG_RIGHT = 17
ECHO_RIGHT = 23

sensor_pins = [
    (TRIG_FRONT, ECHO_FRONT),
    (TRIG_REAR, ECHO_REAR),
    (TRIG_LEFT, ECHO_LEFT),
    (TRIG_RIGHT, ECHO_RIGHT)
]

# GPIO setup for sensors
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
for trig, echo in sensor_pins:
    GPIO.setup(trig, GPIO.OUT)
    GPIO.setup(echo, GPIO.IN)

# Open serial connection to Arduino (adjust port if needed)
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
time.sleep(2)  # Wait for Arduino to reset

def measure_distance(trig, echo):
    GPIO.output(trig, GPIO.LOW)
    time.sleep(0.01)  # Stabilize sensor
    GPIO.output(trig, GPIO.HIGH)
    time.sleep(0.00001)
    GPIO.output(trig, GPIO.LOW)
    
    start_time = time.time()
    stop_time = time.time()
    
    while GPIO.input(echo) == 0:
        start_time = time.time()
    while GPIO.input(echo) == 1:
        stop_time = time.time()
        
    elapsed_time = stop_time - start_time
    distance = (elapsed_time * 34300) / 2  # Speed of sound: 343 m/s
    return distance

def send_motor_command(mode, speed):
    """
    Sends a command to the Arduino in the format "mode,speed\n"
    """
    command = "{},{}\n".format(mode, speed)
    ser.write(command.encode())

def main():
    try:
        while True:
            mode = input("Enter mode (forward/backward/traslation_right/traslation_left/diagonal/turn/caccia): ").strip().lower()
            if mode != "caccia":
                speed = float(input("Enter speed percentage (0-100): "))
                send_motor_command(mode, speed)
                print("Command sent. Running mode {} with speed {}%.".format(mode, speed))
                time.sleep(5)
                send_motor_command("stop", 0)
            else:
                print("Entering 'caccia' mode...")
                while True:
                    # Measure distances from all sensors
                    distances = {
                        "front": measure_distance(TRIG_FRONT, ECHO_FRONT),
                        "rear": measure_distance(TRIG_REAR, ECHO_REAR),
                        "left": measure_distance(TRIG_LEFT, ECHO_LEFT),
                        "right": measure_distance(TRIG_RIGHT, ECHO_RIGHT)
                    }
                    worst_condition = min(distances, key=distances.get)
                    worst_distance = distances[worst_condition]
                    print("Distances: {} | Worst: {} ({} cm)".format(distances, worst_condition, worst_distance))
                    
                    # Decide command based on sensor readings
                    if worst_distance < 20:  # Threshold for proximity
                        if worst_condition == "front":
                            print("Object too close in front! Moving backward.")
                            send_motor_command("backward", 60)
                        elif worst_condition == "rear":
                            print("Object too close in rear! Moving forward.")
                            send_motor_command("forward", 60)
                        elif worst_condition == "left":
                            print("Object too close on left! Moving right.")
                            send_motor_command("traslation_right", 60)
                        elif worst_condition == "right":
                            print("Object too close on right! Moving left.")
                            send_motor_command("traslation_left", 60)
                    else:
                        print("No immediate threats detected. Stopping motors.")
                        send_motor_command("stop", 0)
                    
                    time.sleep(0.1)
    except KeyboardInterrupt:
        print("\nExiting program.")
        send_motor_command("stop", 0)
        GPIO.cleanup()
        ser.close()

if __name__ == "__main__":
    main()
