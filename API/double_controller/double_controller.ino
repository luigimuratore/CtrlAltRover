#include <Arduino.h>

// Define motor control pins (example assignments; adjust to match your wiring)
const int PWM_ENA = 13;   // Motor 1 PWM
const int IN1_PIN = 51; // Motor 1 direction
const int IN2_PIN = 53;

const int PWM_ENB = 11;   // Motor 2 PWM
const int IN3_PIN = 45;
const int IN4_PIN = 43;

const int PWM_ENC = 12;   // Motor 3 PWM
const int IN5_PIN = 49;
const int IN6_PIN = 47;

const int PWM_END = 10;   // Motor 4 PWM
const int IN7_PIN = 39;
const int IN8_PIN = 41;

int currentSpeed = 50; // default speed percentage (0 to 100)

// Stop all motors and set PWM outputs to 0
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

// Set speed for all motors (speed is percentage 0-100, converted to PWM 0-255)
void setMotorSpeed(int speed) {
  int pwmValue = map(speed, 0, 100, 0, 255);
  analogWrite(PWM_ENA, pwmValue);
  analogWrite(PWM_ENB, pwmValue);
  analogWrite(PWM_ENC, pwmValue);
  analogWrite(PWM_END, pwmValue);
}

// Set motor directions based on desired mode
void setMotorMode(String mode) {
  stopMotors(); // Ensure motors are stopped before changing directions

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
  Serial.begin(9600);
  
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
  
  stopMotors();
  Serial.println("Arduino Mega ready. Awaiting commands...");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    Serial.print("Received: ");
    Serial.println(command);
    
    // If command is a speed update, e.g. "speed:60"
    if (command.startsWith("speed:")) {
      int newSpeed = command.substring(6).toInt();
      currentSpeed = newSpeed;
      Serial.print("Speed updated to: ");
      Serial.println(currentSpeed);
      return; // exit early after updating speed
    }
    
    // Process movement commands
    if (command == "avanti") {
      setMotorMode("forward");
      setMotorSpeed(currentSpeed);
      Serial.println("Moving forward");
    } else if (command == "indietro") {
      setMotorMode("backward");
      setMotorSpeed(currentSpeed);
      Serial.println("Moving backward");
    } else if (command == "sinistra") {
      setMotorMode("translation_left");
      setMotorSpeed(currentSpeed);
      Serial.println("Turning left");
    } else if (command == "destra") {
      setMotorMode("translation_right");
      setMotorSpeed(currentSpeed);
      Serial.println("Turning right");
    } else if (command == "stop") {
      stopMotors();
      Serial.println("Stopping");
    }
  }
}
