#include <Arduino.h>
#include <Servo.h>

// ---------------- Motor Control Pins ----------------
const int PWM_ENA = 13;   // Motor 1 PWM
const int IN1_PIN   = 51; // Motor 1 direction
const int IN2_PIN   = 53;

const int PWM_ENB = 11;   // Motor 2 PWM
const int IN3_PIN   = 45;
const int IN4_PIN   = 43;

const int PWM_ENC = 12;   // Motor 3 PWM
const int IN5_PIN   = 49;
const int IN6_PIN   = 47;

const int PWM_END = 10;   // Motor 4 PWM
const int IN7_PIN   = 41;
const int IN8_PIN   = 39;

int currentSpeed = 50; // Default speed (percentage 0-100%)

// ---------------- HC-SR04 Sensor Pins ----------------
// "up" sensor (front)
const int TRIG_UP = 35;
const int ECHO_UP = 37;
// "down" sensor (rear)
const int TRIG_DOWN = 27;
const int ECHO_DOWN = 29;
// "left" sensor
const int TRIG_LEFT = 31;
const int ECHO_LEFT = 33;
// "right" sensor
const int TRIG_RIGHT = 23;
const int ECHO_RIGHT = 25;

// ---------------- Servo Pins ----------------
// Continuous rotation servos (positional control not possible)
// Servo 1: PWM pin 7
// Servo 2: PWM pin 6
// Servo 3: PWM pin 5
const int SERVO1_PIN = 7;
const int SERVO2_PIN = 6;
const int SERVO3_PIN = 5;

// Standard positional (micro) servos:
// Servo 4: PWM pin 4
// Servo 5: PWM pin 3
// Servo 6: PWM pin 2
const int SERVO4_PIN = 4;
const int SERVO5_PIN = 3;
const int SERVO6_PIN = 2;

// Create Servo objects for all six servos
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

// ---------------- Helper for Continuous Rotation Servos ----------------
// For continuous rotation servos, 90° is neutral (stop).
// Values within a small deadband around 90 will be treated as "stop."
const int deadband = 5;  // Adjust as needed

void setContinuousServo(Servo &servo, int angle) {
  // If within the deadband of 90, stop the servo.
  if (abs(angle - 90) < deadband) {
    servo.write(90);
  } else {
    servo.write(angle);
  }
}

// ---------------- Utility Functions ----------------

// Measure distance (cm) using an HC-SR04 sensor.
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000);  // timeout after 30ms
  long distance = duration * 0.0343 / 2;
  return distance;
}

// Stop all motors.
void stopMotors() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  digitalWrite(IN5_PIN, LOW);
  digitalWrite(IN6_PIN, LOW);
  digitalWrite(IN7_PIN, LOW);
  digitalWrite(IN8_PIN, LOW);

  analogWrite(PWM_ENA, 0);
  analogWrite(PWM_ENB, 0);
  analogWrite(PWM_ENC, 0);
  analogWrite(PWM_END, 0);
}

// Set speed for all motors (0-100% mapped to 0-255 PWM).
void setMotorSpeed(int speed) {
  int pwmValue = map(speed, 0, 100, 0, 255);
  analogWrite(PWM_ENA, pwmValue);
  analogWrite(PWM_ENB, pwmValue);
  analogWrite(PWM_ENC, pwmValue);
  analogWrite(PWM_END, pwmValue);
}

// Set motor directions based on desired mode.
void setMotorMode(String mode) {
  stopMotors(); // Ensure motors are stopped before new direction
  
  if (mode == "forward") {
    digitalWrite(IN1_PIN, HIGH); digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, HIGH); digitalWrite(IN4_PIN, LOW);
    digitalWrite(IN5_PIN, HIGH); digitalWrite(IN6_PIN, LOW);
    digitalWrite(IN7_PIN, HIGH); digitalWrite(IN8_PIN, LOW);
  } else if (mode == "backward") {
    digitalWrite(IN1_PIN, LOW); digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN3_PIN, LOW); digitalWrite(IN4_PIN, HIGH);
    digitalWrite(IN5_PIN, LOW); digitalWrite(IN6_PIN, HIGH);
    digitalWrite(IN7_PIN, LOW); digitalWrite(IN8_PIN, HIGH);
  } else if (mode == "translation_left") {
    digitalWrite(IN1_PIN, LOW); digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN3_PIN, HIGH); digitalWrite(IN4_PIN, LOW);
    digitalWrite(IN5_PIN, HIGH); digitalWrite(IN6_PIN, LOW);
    digitalWrite(IN7_PIN, LOW); digitalWrite(IN8_PIN, HIGH);
  } else if (mode == "translation_right") {
    digitalWrite(IN1_PIN, HIGH); digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, LOW); digitalWrite(IN4_PIN, HIGH);
    digitalWrite(IN5_PIN, LOW); digitalWrite(IN6_PIN, HIGH);
    digitalWrite(IN7_PIN, HIGH); digitalWrite(IN8_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize motor pins
  pinMode(PWM_ENA, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(PWM_ENB, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(PWM_ENC, OUTPUT);
  pinMode(IN5_PIN, OUTPUT);
  pinMode(IN6_PIN, OUTPUT);
  pinMode(PWM_END, OUTPUT);
  pinMode(IN7_PIN, OUTPUT);
  pinMode(IN8_PIN, OUTPUT);
  
  // Initialize sensor pins
  pinMode(TRIG_UP, OUTPUT);
  pinMode(ECHO_UP, INPUT);
  pinMode(TRIG_DOWN, OUTPUT);
  pinMode(ECHO_DOWN, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);
  
  // Initialize servo pins
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);
  servo5.attach(SERVO5_PIN);
  servo6.attach(SERVO6_PIN);
  
  // Set default positions:
  // For continuous rotation servos (servo1-3): set to neutral (90) to stop.
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  
  // For positional micro servos (servo4-6): set to desired initial angles.
  servo4.write(180);
  servo5.write(137);
  servo6.write(100);
  
  stopMotors();
  Serial.println("STATUS: Arduino Mega ready. Awaiting commands...");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    Serial.print("Received: ");
    Serial.println(command);
    
    // Process speed update command
    if (command.startsWith("speed:")) {
      int newSpeed = command.substring(6).toInt();
      currentSpeed = newSpeed;
      Serial.print("STATUS: Speed updated to: ");
      Serial.println(currentSpeed);
      return;
    }
    
    // Check if it's a servo command (format: "servoX:angle")
    if (command.startsWith("servo")) {
      int colonIndex = command.indexOf(":");
      if (colonIndex > 0) {
        String servoId = command.substring(0, colonIndex);
        int angle = command.substring(colonIndex + 1).toInt();
        if (servoId == "servo1") {
          setContinuousServo(servo1, angle);
          Serial.print("STATUS: Continuous Servo 1 set to ");
          Serial.print(angle);
          Serial.println("°");
        } else if (servoId == "servo2") {
          setContinuousServo(servo2, angle);
          Serial.print("STATUS: Continuous Servo 2 set to ");
          Serial.print(angle);
          Serial.println("°");
        } else if (servoId == "servo3") {
          setContinuousServo(servo3, angle);
          Serial.print("STATUS: Continuous Servo 3 set to ");
          Serial.print(angle);
          Serial.println("°");
        } else if (servoId == "servo4") {
          servo4.write(angle);
          Serial.print("STATUS: Servo 4 set to ");
          Serial.print(angle);
          Serial.println("°");
        } else if (servoId == "servo5") {
          servo5.write(angle);
          Serial.print("STATUS: Servo 5 set to ");
          Serial.print(angle);
          Serial.println("°");
        } else if (servoId == "servo6") {
          servo6.write(angle);
          Serial.print("STATUS: Servo 6 set to ");
          Serial.print(angle);
          Serial.println("°");
        }
      }
      return; // Servo command processed.
    }
    
    // Sensor measurement commands
    if (command == "up") {
      long dist = measureDistance(TRIG_UP, ECHO_UP);
      Serial.print("SENSOR: Up sensor distance: ");
      Serial.print(dist);
      Serial.println(" cm");
    }
    else if (command == "down") {
      long dist = measureDistance(TRIG_DOWN, ECHO_DOWN);
      Serial.print("SENSOR: Down sensor distance: ");
      Serial.print(dist);
      Serial.println(" cm");
    }
    else if (command == "left") {
      long dist = measureDistance(TRIG_LEFT, ECHO_LEFT);
      Serial.print("SENSOR: Left sensor distance: ");
      Serial.print(dist);
      Serial.println(" cm");
    }
    else if (command == "right") {
      long dist = measureDistance(TRIG_RIGHT, ECHO_RIGHT);
      Serial.print("SENSOR: Right sensor distance: ");
      Serial.print(dist);
      Serial.println(" cm");
    }
    // Motor control commands
    else if (command == "avanti") {
      setMotorMode("forward");
      setMotorSpeed(currentSpeed);
      Serial.println("STATUS: Moving forward");
    }
    else if (command == "indietro") {
      setMotorMode("backward");
      setMotorSpeed(currentSpeed);
      Serial.println("STATUS: Moving backward");
    }
    else if (command == "sinistra") {
      setMotorMode("translation_left");
      setMotorSpeed(currentSpeed);
      Serial.println("STATUS: Turning left");
    }
    else if (command == "destra") {
      setMotorMode("translation_right");
      setMotorSpeed(currentSpeed);
      Serial.println("STATUS: Turning right");
    }
    else if (command == "stop") {
      stopMotors();
      Serial.println("STATUS: Stop - Motors stopped");
    }
    else if (command == "emergency") {
      stopMotors();
      Serial.println("STATUS: EMERGENCY STOP - Motors stopped");
    }
  }
}
