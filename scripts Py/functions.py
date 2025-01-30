import RPi.GPIO as GPIO
import time

# GPIO pin configuration for 4 motors
# Motor 1
ENA = 13
IN1 = 6
IN2 = 5
# Motor 2
ENB = 12
IN3 = 16
IN4 = 26
# Motor 3
ENC = 20
IN5 = 19
IN6 = 21
# Motor 4
END = 18
IN7 = 23
IN8 = 24

# List of all motor pins
motor_pins = [ENA, IN1, IN2, ENB, IN3, IN4, ENC, IN5, IN6, END, IN7, IN8]

# GPIO setup
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

for pin in motor_pins:
    GPIO.setup(pin, GPIO.OUT)

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

def set_motor_speed(speed_percentage):
    """
    Set speed for all motors in percentage (0-100%).
    """
    if 0 <= speed_percentage <= 100:
        pwm_ena.ChangeDutyCycle(speed_percentage)
        pwm_enb.ChangeDutyCycle(speed_percentage)
        pwm_enc.ChangeDutyCycle(speed_percentage)
        pwm_end.ChangeDutyCycle(speed_percentage)
    else:
        print("Invalid speed percentage. Enter a value between 0 and 100.")

def stop_motors():
    """
    Stop all motors.
    """
    pwm_ena.ChangeDutyCycle(0)
    pwm_enb.ChangeDutyCycle(0)
    pwm_enc.ChangeDutyCycle(0)
    pwm_end.ChangeDutyCycle(0)
    GPIO.output(IN1, GPIO.LOW)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.LOW)
    GPIO.output(IN4, GPIO.LOW)
    GPIO.output(IN5, GPIO.LOW)
    GPIO.output(IN6, GPIO.LOW)
    GPIO.output(IN7, GPIO.LOW)
    GPIO.output(IN8, GPIO.LOW)

def set_motor_mode(mode):
    """
    Set motor direction and pattern based on the selected mode.
    """
    # Stop all motors first
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
    elif mode == "traslation_right":
        GPIO.output(IN1, GPIO.LOW)
        GPIO.output(IN2, GPIO.HIGH)
        GPIO.output(IN3, GPIO.HIGH)
        GPIO.output(IN4, GPIO.LOW)
        GPIO.output(IN5, GPIO.HIGH)
        GPIO.output(IN6, GPIO.LOW)
        GPIO.output(IN7, GPIO.LOW)
        GPIO.output(IN8, GPIO.HIGH)
    elif mode == "traslation_left":
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
    else:
        print("Invalid mode.")


def main():
    try:
        while True:
            # Get user input for mode and speed
            mode = input("Enter mode (forward/backward/traslation_right/traslation_left/diagonal_right_forward/diagonal_right_backward/diagonal_left_forward/diagonal_left_backward/turn_left/turn_right): ").strip().lower()
            speed = float(input("Enter speed percentage (0-100): "))

            # Set motor mode and speed
            set_motor_mode(mode)
            set_motor_speed(speed)

            # Run motors for 5 seconds
            print(f"Running motors in {mode} mode at {speed}% speed...")
            time.sleep(5)

            # Stop motors
            print("Stopping motors...")
            stop_motors()
            time.sleep(1)

    except KeyboardInterrupt:
        print("\nExiting program.")
        stop_motors()
        GPIO.cleanup()

if __name__ == "__main__":
    main()
