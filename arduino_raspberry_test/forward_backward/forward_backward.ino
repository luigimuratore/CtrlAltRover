// Motor pin definitions
#define MOTOR1_ENA 13
#define MOTOR1_IN1 51
#define MOTOR1_IN2 53

#define MOTOR2_ENA 11
#define MOTOR2_IN1 45
#define MOTOR2_IN2 43

#define MOTOR3_ENA 12
#define MOTOR3_IN1 49
#define MOTOR3_IN2 47

#define MOTOR4_ENA 10
#define MOTOR4_IN1 41
#define MOTOR4_IN2 39

// Set a default motor speed (PWM value: 0-255)
const int motorSpeed = 150; 

void setup() {
  Serial.begin(9600);
  
  // Set motor control pins as outputs
  pinMode(MOTOR1_ENA, OUTPUT);
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  
  pinMode(MOTOR2_ENA, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
  
  pinMode(MOTOR3_ENA, OUTPUT);
  pinMode(MOTOR3_IN1, OUTPUT);
  pinMode(MOTOR3_IN2, OUTPUT);
  
  pinMode(MOTOR4_ENA, OUTPUT);
  pinMode(MOTOR4_IN1, OUTPUT);
  pinMode(MOTOR4_IN2, OUTPUT);

  Serial.println("Arduino Motor Controller Ready");
  Serial.println("Commands: 'forward', 'backward', 'stop'");
}

void loop() {
  // Check if a command was sent over Serial
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();  // Remove any extra whitespace

    Serial.print("Received command: ");
    Serial.println(command);
    
    if (command.equalsIgnoreCase("forward") || command.equalsIgnoreCase("F")) {
      moveForward();
    }
    else if (command.equalsIgnoreCase("backward") || command.equalsIgnoreCase("B")) {
      moveBackward();
    }
    else if (command.equalsIgnoreCase("stop") || command.equalsIgnoreCase("S")) {
      stopMotors();
    }
    else {
      Serial.println("Unknown command. Use 'forward', 'backward', or 'stop'.");
    }
  }
}

// Function to run all motors forward
void moveForward() {
  // For each motor: IN1 = HIGH, IN2 = LOW, PWM set to motorSpeed
  digitalWrite(MOTOR1_IN1, HIGH);
  digitalWrite(MOTOR1_IN2, LOW);
  analogWrite(MOTOR1_ENA, motorSpeed);
  
  digitalWrite(MOTOR2_IN1, HIGH);
  digitalWrite(MOTOR2_IN2, LOW);
  analogWrite(MOTOR2_ENA, motorSpeed);
  
  digitalWrite(MOTOR3_IN1, HIGH);
  digitalWrite(MOTOR3_IN2, LOW);
  analogWrite(MOTOR3_ENA, motorSpeed);
  
  digitalWrite(MOTOR4_IN1, HIGH);
  digitalWrite(MOTOR4_IN2, LOW);
  analogWrite(MOTOR4_ENA, motorSpeed);

  Serial.println("Moving forward");
}

// Function to run all motors backward
void moveBackward() {
  // For each motor: IN1 = LOW, IN2 = HIGH, PWM set to motorSpeed
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, HIGH);
  analogWrite(MOTOR1_ENA, motorSpeed);
  
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, HIGH);
  analogWrite(MOTOR2_ENA, motorSpeed);
  
  digitalWrite(MOTOR3_IN1, LOW);
  digitalWrite(MOTOR3_IN2, HIGH);
  analogWrite(MOTOR3_ENA, motorSpeed);
  
  digitalWrite(MOTOR4_IN1, LOW);
  digitalWrite(MOTOR4_IN2, HIGH);
  analogWrite(MOTOR4_ENA, motorSpeed);

  Serial.println("Moving backward");
}

// Function to stop all motors
void stopMotors() {
  // For each motor: IN1 and IN2 LOW, PWM set to 0
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, LOW);
  analogWrite(MOTOR1_ENA, 0);
  
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, LOW);
  analogWrite(MOTOR2_ENA, 0);
  
  digitalWrite(MOTOR3_IN1, LOW);
  digitalWrite(MOTOR3_IN2, LOW);
  analogWrite(MOTOR3_ENA, 0);
  
  digitalWrite(MOTOR4_IN1, LOW);
  digitalWrite(MOTOR4_IN2, LOW);
  analogWrite(MOTOR4_ENA, 0);

  Serial.println("Motors stopped");
}
