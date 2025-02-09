from flask import Flask, render_template
from flask_socketio import SocketIO
import time

raspberry = False

if raspberry:
    import RPi.GPIO as GPIO

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

    # Function definitions remain the same, except for the new mode
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


app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

@app.route('/')
def index():
    return render_template('controller.html')


speed = 50

@socketio.on('command')
def handle_command(command):
    global speed
    # Check if the command is a speed update
    if command.startswith("speed:"):
        try:
            # Extract and update the speed value
            new_speed = int(command.split(":")[1])
            speed = new_speed
            print(f"Speed updated to: {speed}")
        except ValueError:
            print("Invalid speed value received.")
        return  # Return early; no further processing needed

    # For non-speed commands, print the command with the current speed
    print(f"Ricevuto comando: {command}, with speed: {speed}")
    
    #codice per muovere il robot
    if command == "avanti":
        if raspberry:
            set_motor_mode("forward")
            set_motor_speed(speed)
        print("Robot va avanti - FORWARD")

    elif command == "indietro":
        if raspberry:
            set_motor_mode("backward")
            set_motor_speed(speed)
        print("Robot va indietro - BACKWARD")

    elif command == "sinistra":
        if raspberry:
            set_motor_mode("translation_left")
            set_motor_speed(speed)
        print("Robot va a sinistra - LEFT")

    elif command == "destra":
        if raspberry:
            set_motor_mode("translation_right")
            set_motor_speed(speed)
        print("Robot va destra - RIGHT")

    elif command == "stop":
        if raspberry:
            stop_motors()
        print("Robot FERMO")

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5001, debug=True)
