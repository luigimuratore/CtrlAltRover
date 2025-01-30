import numpy as np
import time
import RPi.GPIO as GPIO
from stable_baselines3 import PPO
import os


# GPIO pin configuration for HC-SR04 sensors
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

# Motor GPIO pin setup (same as before)
# Motor 1
ENA = 26
IN1 = 21
IN2 = 16
# Motor 2
ENB = 25
IN3 = 0
IN4 = 5
# Motor 3
ENC = 20
IN5 = 13
IN6 = 19
# Motor 4
END = 12
IN7 = 6
IN8 = 1

# List of all motor pins
motor_pins = [ENA, IN1, IN2, ENB, IN3, IN4, ENC, IN5, IN6, END, IN7, IN8]
sensor_pins = [(TRIG_FRONT, ECHO_FRONT), (TRIG_REAR, ECHO_REAR), (TRIG_LEFT, ECHO_LEFT), (TRIG_RIGHT, ECHO_RIGHT)]

# GPIO setup
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

for pin in motor_pins:
    GPIO.setup(pin, GPIO.OUT)
for trig, echo in sensor_pins:
    GPIO.setup(trig, GPIO.OUT)
    GPIO.setup(echo, GPIO.IN)

# PWM setup for speed control
pwm_ena = GPIO.PWM(ENA, 100)  # Motor 1
pwm_enb = GPIO.PWM(ENB, 100)  # Motor 2
pwm_enc = GPIO.PWM(ENC, 100)  # Motor 3
pwm_end = GPIO.PWM(END, 100)  # Motor 4

# Start PWM with 0% duty cycle
pwm_ena.start(0)
pwm_enb.start(0)
pwm_enc.start(0)
pwm_end.start(0)


def get_distance(trigger, echo):
    """ Get distance from an HC-SR04 ultrasonic sensor """
    GPIO.output(trigger, True)
    time.sleep(0.00001)
    GPIO.output(trigger, False)
    
    start_time = time.time()
    stop_time = time.time()

    while GPIO.input(echo) == 0:
        start_time = time.time()
    
    while GPIO.input(echo) == 1:
        stop_time = time.time()
    
    elapsed_time = stop_time - start_time
    distance = (elapsed_time * 34300) / 2  # Convert to cm
    return np.clip(distance, 2, 400)  # Limit min/max range

def get_real_observation():
    """ Get real sensor readings as observation array """
    front_distance = get_distance(TRIG_FRONT, ECHO_FRONT)
    left_distance = get_distance(TRIG_LEFT, ECHO_LEFT)
    right_distance = get_distance(TRIG_RIGHT, ECHO_RIGHT)
    
    return np.array([front_distance, left_distance, right_distance], dtype=np.float32)

# --- Motor Control Functions ---
def stop_robot():
    GPIO.output(IN1, GPIO.LOW)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.LOW)
    GPIO.output(IN4, GPIO.LOW)
    GPIO.output(IN5, GPIO.LOW)
    GPIO.output(IN6, GPIO.LOW)
    GPIO.output(IN7, GPIO.LOW)
    GPIO.output(IN8, GPIO.LOW)
    pwm_left.ChangeDutyCycle(0)
    pwm_right.ChangeDutyCycle(0)

def move_forward(speed=50):
    GPIO.output(IN1, GPIO.HIGH)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.HIGH)
    GPIO.output(IN4, GPIO.LOW)
    GPIO.output(IN5, GPIO.HIGH)
    GPIO.output(IN6, GPIO.LOW)
    GPIO.output(IN7, GPIO.HIGH)
    GPIO.output(IN8, GPIO.LOW)
    pwm_left.ChangeDutyCycle(speed)
    pwm_right.ChangeDutyCycle(speed)

def translate_left(speed=50):
    GPIO.output(IN1, GPIO.LOW)
    GPIO.output(IN2, GPIO.HIGH)
    GPIO.output(IN3, GPIO.HIGH)
    GPIO.output(IN4, GPIO.LOW)
    GPIO.output(IN5, GPIO.HIGH)
    GPIO.output(IN6, GPIO.LOW)
    GPIO.output(IN7, GPIO.LOW)
    GPIO.output(IN8, GPIO.HIGH)
    pwm_left.ChangeDutyCycle(speed)
    pwm_right.ChangeDutyCycle(speed)

def translate_right(speed=50):
    GPIO.output(IN1, GPIO.HIGH)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.LOW)
    GPIO.output(IN4, GPIO.HIGH)
    GPIO.output(IN5, GPIO.LOW)
    GPIO.output(IN6, GPIO.HIGH)
    GPIO.output(IN7, GPIO.HIGH)
    GPIO.output(IN8, GPIO.LOW)
    pwm_left.ChangeDutyCycle(speed)
    pwm_right.ChangeDutyCycle(speed)

# --- Load Trained PPO Model ---
if not os.path.exists("parking.zip"):
    print("Error: parking.zip not found! Please transfer the trained model to the Raspberry Pi.")
    exit()

model = PPO.load("parking.zip")

# --- Main Control Loop ---
try:
    print("Starting RL Inference on Real Robot...")
    while True:
        obs = get_real_observation()
        action, _states = model.predict(obs, deterministic=True)

        # Map actions to motor commands
        if action == 0:
            move_forward(60)  # Adjust speed if needed
        elif action == 1:
            translate_left(60)
        elif action == 2:
            translate_right(60)
        else:
            stop_robot()

        time.sleep(0.1)  # Adjust loop timing

except KeyboardInterrupt:
    print("\nStopping Robot...")
    stop_robot()
    GPIO.cleanup()
