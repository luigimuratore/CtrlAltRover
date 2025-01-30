import RPi.GPIO as GPIO
import time

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

# Motor GPIO pin setup
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

def measure_distance(trig, echo):
    """
    Measure distance using HC-SR04.
    """
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
    elif mode == "traslation_left":
        GPIO.output(IN1, GPIO.LOW)
        GPIO.output(IN2, GPIO.HIGH)
        GPIO.output(IN3, GPIO.HIGH)
        GPIO.output(IN4, GPIO.LOW)
        GPIO.output(IN5, GPIO.HIGH)
        GPIO.output(IN6, GPIO.LOW)
        GPIO.output(IN7, GPIO.LOW)
        GPIO.output(IN8, GPIO.HIGH)

def parking():
    """
    Parking logic using the left sensor:
    - Moves forward until the left sensor detects a value greater than 40 cm.
    - Waits for 2 seconds.
    - Moves 5 cm ahead after detecting a value > 40 cm.
    - Translates left until the left sensor detects a value below 20 cm.
    - Stops the robot.
    """
    try:
        print("Parking mode activated. Using left sensor.")
        while True:
            # Measure distance from the left sensor
            distance_left = measure_distance(TRIG_LEFT, ECHO_LEFT)
            print(f"Distance (Left Sensor): {distance_left:.2f} cm")

            # Go forward if the left sensor detects a value < 20 cm
            if distance_left < 20:
                print("Moving forward...")
                set_motor_mode("forward")
                set_motor_speed(70)  # Set speed to 50% for forward motion
            # Stop and prepare for parking if the left sensor detects a value > 40 cm
            elif distance_left > 40:
                print("Stopping for 2 seconds...")
                stop_motors()
                time.sleep(0.5)

                # Move forward 5 cm (extra distance)
                print("Moving forward an additional 5 cm...")
                set_motor_mode("forward")
                set_motor_speed(70)  # Adjust speed as needed
                time.sleep(0.5)  # Adjust time for approximately 5 cm
                stop_motors()

                # Translate left while the left sensor detects a value > 20 cm
                while distance_left > 20:
                    print("Translating left...")
                    set_motor_mode("traslation_left")
                    set_motor_speed(70)
                    distance_left = measure_distance(TRIG_LEFT, ECHO_LEFT)
                    print(f"Distance (Left Sensor during Translation): {distance_left:.2f} cm")

                # Stop after translation
                print("Parking complete. Stopping motors.")
                stop_motors()
                break
            else:
                # Otherwise, just stop
                stop_motors()

            # Short delay to stabilize sensor readings
            time.sleep(0.1)

    except KeyboardInterrupt:
        print("Parking mode interrupted. Stopping motors.")
        stop_motors()


def main():
    """
    Main function to control the robot.
    """
    try:
        while True:
            print("Choose a mode: [1] Forward, [2] Translate Right, [3] Diagonal Right Forward, [4] Parking, [5] Stop")
            user_input = input("Enter your choice (1-5): ")

            if user_input == "1":
                set_motor_mode("forward")
                set_motor_speed(50)  # Adjust speed as needed
            elif user_input == "2":
                set_motor_mode("traslation_right")
                set_motor_speed(50)  # Adjust speed as needed
            elif user_input == "3":
                set_motor_mode("diagonal_right_forward")
                set_motor_speed(50)  # Adjust speed as needed
            elif user_input == "4":
                print("Starting parking routine...")
                parking()
            elif user_input == "5":
                print("Stopping motors.")
                stop_motors()
            else:
                print("Invalid input. Please enter a number between 1 and 5.")

    except KeyboardInterrupt:
        # Graceful exit on user interruption
        print("\nExiting program.")
        stop_motors()
        GPIO.cleanup()

    except Exception as e:
        # Catch any other exceptions
        print(f"An error occurred: {e}")
        stop_motors()
        GPIO.cleanup()

if __name__ == "__main__":
    main()
