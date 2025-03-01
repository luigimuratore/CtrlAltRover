// #include <Servo.h>

// -----------------------
// DRIVE MOTOR CONTROL SETUP
// -----------------------
// Define LN298 driver pins for 4 drive motors (adjust as needed)
#define MOTOR1_PWM 13
#define MOTOR1_IN1 51
#define MOTOR1_IN2 53

#define MOTOR2_PWM 11
#define MOTOR2_IN1 45
#define MOTOR2_IN2 43

#define MOTOR3_PWM 12
#define MOTOR3_IN1 49
#define MOTOR3_IN2 47

#define MOTOR4_PWM 10
#define MOTOR4_IN1 39
#define MOTOR4_IN2 41

// -----------------------
// MANIPULATOR ARM (SERVO) SETUP
// -----------------------
#define SERVO1_PIN 7
#define SERVO2_PIN 6
#define SERVO3_PIN 5
#define SERVO4_PIN 4
#define SERVO5_PIN 3
#define SERVO6_PIN 2

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

// Default servo positions (in degrees)
int pos1 = 90;
int pos2 = 90;
int pos3 = 90;
int pos4 = 90;
int pos5 = 90;
int pos6 = 90;

// -----------------------
// DRIVE MOTOR CONTROL FUNCTIONS
// -----------------------
void driveForward(int pwmVal) {
  analogWrite(MOTOR1_PWM, pwmVal);
  digitalWrite(MOTOR1_IN1, HIGH);
  digitalWrite(MOTOR1_IN2, LOW);
  
  analogWrite(MOTOR2_PWM, pwmVal);
  digitalWrite(MOTOR2_IN1, HIGH);
  digitalWrite(MOTOR2_IN2, LOW);
  
  analogWrite(MOTOR3_PWM, pwmVal);
  digitalWrite(MOTOR3_IN1, LOW);
  digitalWrite(MOTOR3_IN2, HIGH);
  
  analogWrite(MOTOR4_PWM, pwmVal);
  digitalWrite(MOTOR4_IN1, LOW);
  digitalWrite(MOTOR4_IN2, HIGH);
}

void driveBackward(int pwmVal) {
  analogWrite(MOTOR1_PWM, pwmVal);
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, HIGH);
  
  analogWrite(MOTOR2_PWM, pwmVal);
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, HIGH);
  
  analogWrite(MOTOR3_PWM, pwmVal);
  digitalWrite(MOTOR3_IN1, HIGH);
  digitalWrite(MOTOR3_IN2, LOW);
  
  analogWrite(MOTOR4_PWM, pwmVal);
  digitalWrite(MOTOR4_IN1, HIGH);
  digitalWrite(MOTOR4_IN2, LOW);
}

void driveLeft(int pwmVal) {
  // Example pattern for translation left
  analogWrite(MOTOR1_PWM, pwmVal);
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, HIGH);
  
  analogWrite(MOTOR2_PWM, pwmVal);
  digitalWrite(MOTOR2_IN1, HIGH);
  digitalWrite(MOTOR2_IN2, LOW);
  
  analogWrite(MOTOR3_PWM, pwmVal);
  digitalWrite(MOTOR3_IN1, HIGH);
  digitalWrite(MOTOR3_IN2, LOW);
  
  analogWrite(MOTOR4_PWM, pwmVal);
  digitalWrite(MOTOR4_IN1, LOW);
  digitalWrite(MOTOR4_IN2, HIGH);
}

void driveRight(int pwmVal) {
  // Example pattern for translation right
  analogWrite(MOTOR1_PWM, pwmVal);
  digitalWrite(MOTOR1_IN1, HIGH);
  digitalWrite(MOTOR1_IN2, LOW);
  
  analogWrite(MOTOR2_PWM, pwmVal);
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, HIGH);
  
  analogWrite(MOTOR3_PWM, pwmVal);
  digitalWrite(MOTOR3_IN1, LOW);
  digitalWrite(MOTOR3_IN2, HIGH);
  
  analogWrite(MOTOR4_PWM, pwmVal);
  digitalWrite(MOTOR4_IN1, HIGH);
  digitalWrite(MOTOR4_IN2, LOW);
}

void stopDrive() {
  analogWrite(MOTOR1_PWM, 0);
  analogWrite(MOTOR2_PWM, 0);
  analogWrite(MOTOR3_PWM, 0);
  analogWrite(MOTOR4_PWM, 0);
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, LOW);
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, LOW);
  digitalWrite(MOTOR3_IN1, LOW);
  digitalWrite(MOTOR3_IN2, LOW);
  digitalWrite(MOTOR4_IN1, LOW);
  digitalWrite(MOTOR4_IN2, LOW);
}

// -----------------------
// SETUP
// -----------------------
void setup() {
  Serial.begin(115200);

  // Initialize drive motor pins
  pinMode(MOTOR1_PWM, OUTPUT);
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  
  pinMode(MOTOR2_PWM, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
  
  pinMode(MOTOR3_PWM, OUTPUT);
  pinMode(MOTOR3_IN1, OUTPUT);
  pinMode(MOTOR3_IN2, OUTPUT);
  
  pinMode(MOTOR4_PWM, OUTPUT);
  pinMode(MOTOR4_IN1, OUTPUT);
  pinMode(MOTOR4_IN2, OUTPUT);
  
  // Attach servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);
  servo5.attach(SERVO5_PIN);
  
  // Set default positions for servos
  servo1.write(pos1);
  servo2.write(pos2);
  servo3.write(pos3);
  servo4.write(pos4);
  servo5.write(pos5);
  
  Serial.println("Arduino Ready");
}

// -----------------------
// MAIN LOOP: Read serial commands and execute them
// -----------------------
void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    Serial.print("Command Received: ");
    Serial.println(command);
    
    // Drive Commands
    if (command == "avanti") {
      driveForward(150);  // PWM value example; adjust as needed
    } else if (command == "indietro") {
      driveBackward(150);
    } else if (command == "sinistra") {
      driveLeft(150);
    } else if (command == "destra") {
      driveRight(150);
    } else if (command == "stop") {
      stopDrive();
    }
    // Manipulator Commands
    else if (command == "reset_arm") {
      pos1 = 90; pos2 = 90; pos3 = 90; pos4 = 90; pos5 = 90;
      servo1.write(pos1);
      servo2.write(pos2);
      servo3.write(pos3);
      servo4.write(pos4);
      servo5.write(pos5);
      Serial.println("Manipulator arm reset to 90° for all servos.");
    }
    // Servo Commands (e.g., "servo1:120")
    else if (command.startsWith("servo1:")) {
      int angle = command.substring(7).toInt();
      pos1 = constrain(angle, 0, 180);
      servo1.write(pos1);
      Serial.print("Servo1 set to ");
      Serial.println(pos1);
    }
    else if (command.startsWith("servo2:")) {
      int angle = command.substring(7).toInt();
      pos2 = constrain(angle, 0, 180);
      servo2.write(pos2);
      Serial.print("Servo2 set to ");
      Serial.println(pos2);
    }
    else if (command.startsWith("servo3:")) {
      int angle = command.substring(7).toInt();
      pos3 = constrain(angle, 0, 180);
      servo3.write(pos3);
      Serial.print("Servo3 set to ");
      Serial.println(pos3);
    }
    else if (command.startsWith("servo4:")) {
      int angle = command.substring(7).toInt();
      pos4 = constrain(angle, 0, 180);
      servo4.write(pos4);
      Serial.print("Servo4 set to ");
      Serial.println(pos4);
    }
    else if (command.startsWith("servo5:")) {
      int angle = command.substring(7).toInt();
      pos5 = constrain(angle, 0, 180);
      servo5.write(pos5);
      Serial.print("Servo5 set to ");
      Serial.println(pos5);
    }
    else {
      Serial.println("Unknown command.");
    }
  }
}
