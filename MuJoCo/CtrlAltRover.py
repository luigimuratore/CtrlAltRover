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


def get_real_observation():
    """ Get real sensor readings as observation array """
    front_distance = get_distance(TRIG_FRONT, ECHO_FRONT)
    rear_distance = get_distance(TRIG_REAR, ECHO_REAR)
    left_distance = get_distance(TRIG_LEFT, ECHO_LEFT)
    right_distance = get_distance(TRIG_RIGHT, ECHO_RIGHT)

    return np.array([front_distance, rear_distance, left_distance, right_distance], dtype=np.float32)


def get_distance(trigger, echo):
    """
    Measure distance using HC-SR04.
    """
    GPIO.output(trigger, GPIO.LOW)
    time.sleep(0.01)  # Stabilize sensor
    GPIO.output(trigger, GPIO.HIGH)
    time.sleep(0.00001)
    GPIO.output(trigger, GPIO.LOW)
    
    start_time = time.time()
    stop_time = time.time()

    while GPIO.input(echo) == 0:
        start_time = time.time()
    while GPIO.input(echo) == 1:
        stop_time = time.time()
    
    elapsed_time = stop_time - start_time
    distance = (elapsed_time * 34300) / 2  # Convert to cm
    return distance*30


def stop_motors():
    """
    Stops all motors by setting all motor control pins to LOW.
    """
    GPIO.output(IN1, GPIO.LOW)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.LOW)
    GPIO.output(IN4, GPIO.LOW)
    GPIO.output(IN5, GPIO.LOW)
    GPIO.output(IN6, GPIO.LOW)
    GPIO.output(IN7, GPIO.LOW)
    GPIO.output(IN8, GPIO.LOW)

    # Optionally set PWM duty cycles to 0
    pwm_ena.ChangeDutyCycle(0)
    pwm_enb.ChangeDutyCycle(0)
    pwm_enc.ChangeDutyCycle(0)
    pwm_end.ChangeDutyCycle(0)

def set_motor_speed(speed):
    """
    Set the speed of all motors using PWM.
    Speed is given as a percentage (0 to 100).
    """
    if 0 <= speed <= 100:
        pwm_ena.ChangeDutyCycle(speed)
        pwm_enb.ChangeDutyCycle(speed)
        pwm_enc.ChangeDutyCycle(speed)
        pwm_end.ChangeDutyCycle(speed)
    else:
        print("Invalid speed value. Please enter a percentage between 0 and 100.")


# --- Motor Control Functions ---
def set_motor_mode(mode):
    """
    Set motor direction and pattern based on the selected mode.
    """

    stop_motors()

    if mode == "forward":
        GPIO.output(IN1, GPIO.HIGH)
        GPIO.output(IN2, GPIO.LOW)
        GPIO.output(IN3, GPIO.HIGH)
        GPIO.output(IN4, GPIO.LOW)
        GPIO.output(IN5, GPIO.HIGH)
        GPIO.output(IN6, GPIO.LOW)
        GPIO.output(IN7, GPIO.HIGH)
        GPIO.output(IN8, GPIO.LOW)
    elif mode == "backward":
        GPIO.output(IN1, GPIO.LOW)
        GPIO.output(IN2, GPIO.HIGH)
        GPIO.output(IN3, GPIO.LOW)
        GPIO.output(IN4, GPIO.HIGH)
        GPIO.output(IN5, GPIO.LOW)
        GPIO.output(IN6, GPIO.HIGH)
        GPIO.output(IN7, GPIO.LOW)
        GPIO.output(IN8, GPIO.HIGH)
    elif mode == "translation_left":
        GPIO.output(IN1, GPIO.LOW)
        GPIO.output(IN2, GPIO.HIGH)
        GPIO.output(IN3, GPIO.HIGH)
        GPIO.output(IN4, GPIO.LOW)
        GPIO.output(IN5, GPIO.HIGH)
        GPIO.output(IN6, GPIO.LOW)
        GPIO.output(IN7, GPIO.LOW)
        GPIO.output(IN8, GPIO.HIGH)
    elif mode == "translation_right":
        GPIO.output(IN1, GPIO.HIGH)
        GPIO.output(IN2, GPIO.LOW)
        GPIO.output(IN3, GPIO.LOW)
        GPIO.output(IN4, GPIO.HIGH)
        GPIO.output(IN5, GPIO.LOW)
        GPIO.output(IN6, GPIO.HIGH)
        GPIO.output(IN7, GPIO.HIGH)
        GPIO.output(IN8, GPIO.LOW)
    elif mode == "diagonal_right_forward":
        GPIO.output(IN1, GPIO.HIGH)
        GPIO.output(IN2, GPIO.LOW)
        GPIO.output(IN3, GPIO.LOW)
        GPIO.output(IN4, GPIO.LOW)
        GPIO.output(IN5, GPIO.LOW)
        GPIO.output(IN6, GPIO.LOW)
        GPIO.output(IN7, GPIO.HIGH)
        GPIO.output(IN8, GPIO.LOW)
    elif mode == "diagonal_right_backward":
        GPIO.output(IN1, GPIO.LOW)
        GPIO.output(IN2, GPIO.HIGH)
        GPIO.output(IN3, GPIO.LOW)
        GPIO.output(IN4, GPIO.LOW)
        GPIO.output(IN5, GPIO.LOW)
        GPIO.output(IN6, GPIO.LOW)
        GPIO.output(IN7, GPIO.LOW)
        GPIO.output(IN8, GPIO.HIGH)
    elif mode == "diagonal_left_forward":
        GPIO.output(IN1, GPIO.LOW)
        GPIO.output(IN2, GPIO.LOW)
        GPIO.output(IN3, GPIO.HIGH)
        GPIO.output(IN4, GPIO.LOW)
        GPIO.output(IN5, GPIO.HIGH)
        GPIO.output(IN6, GPIO.LOW)
        GPIO.output(IN7, GPIO.LOW)
        GPIO.output(IN8, GPIO.LOW)
    elif mode == "diagonal_left_backward":
        GPIO.output(IN1, GPIO.LOW)
        GPIO.output(IN2, GPIO.LOW)
        GPIO.output(IN3, GPIO.LOW)
        GPIO.output(IN4, GPIO.HIGH)
        GPIO.output(IN5, GPIO.LOW)
        GPIO.output(IN6, GPIO.HIGH)
        GPIO.output(IN7, GPIO.LOW)
        GPIO.output(IN8, GPIO.LOW)
    elif mode == "turn_left":
        GPIO.output(IN1, GPIO.HIGH)
        GPIO.output(IN2, GPIO.LOW)
        GPIO.output(IN3, GPIO.LOW)
        GPIO.output(IN4, GPIO.HIGH)
        GPIO.output(IN5, GPIO.HIGH)
        GPIO.output(IN6, GPIO.LOW)
        GPIO.output(IN7, GPIO.LOW)
        GPIO.output(IN8, GPIO.HIGH)
    elif mode == "turn_right":
        GPIO.output(IN1, GPIO.LOW)
        GPIO.output(IN2, GPIO.HIGH)
        GPIO.output(IN3, GPIO.HIGH)
        GPIO.output(IN4, GPIO.LOW)
        GPIO.output(IN5, GPIO.LOW)
        GPIO.output(IN6, GPIO.HIGH)
        GPIO.output(IN7, GPIO.HIGH)
        GPIO.output(IN8, GPIO.LOW)
    elif mode == "stop":
        GPIO.output(IN1, GPIO.LOW)
        GPIO.output(IN2, GPIO.LOW)
        GPIO.output(IN3, GPIO.LOW)
        GPIO.output(IN4, GPIO.LOW)
        GPIO.output(IN5, GPIO.LOW)
        GPIO.output(IN6, GPIO.LOW)
        GPIO.output(IN7, GPIO.LOW)
        GPIO.output(IN8, GPIO.LOW)
        

# --- Load Trained PPO Model ---
if not os.path.exists("parking.zip"):
    print("Error: parking.zip not found! Please transfer the trained model to the Raspberry Pi.")
    exit()

try:
    # Load the trained model
    model = PPO.load("parking_old")
    print("Model loaded successfully.")
except Exception as e:
    print(f"Error loading model: {e}")
    exit()


# --- Main Control Loop ---
try:    
    speed = float(input("Choose a percentage speed value (0-100): "))
    
    print("Starting RL Inference on Real Robot...")

    while True:
        obs = get_real_observation()
        print("--------------------------------------------------")
        print("OBS: ", obs)

        action, _states = model.predict(obs, deterministic=False)
        print("Action: ", action)


        # Map actions to motor commands
        if action == 0:
            set_motor_mode("forward")
            set_motor_speed(speed)
            print("Forward - speed:", speed)
        elif action == 1:
            set_motor_mode("translation_right")
            set_motor_speed(speed)
            print("Right - speed:", speed)
        elif action == 2:
            set_motor_mode("stop")
            print("Stop")
        else:
            stop_motors()

        time.sleep(0.1)  # Adjust loop timing

except KeyboardInterrupt:
    print("\nStopping Robot...")
    stop_motors()
    GPIO.cleanup()
