
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
    
    else {
      Serial.println("Unknown command.");
    }
  }
}
