/*
  Motor Connections:
  Motor 1: ENA (PWM)  -> Pin 13, IN1 -> Pin 51, IN2 -> Pin 53
  Motor 2: ENA (PWM)  -> Pin 11, IN1 -> Pin 45, IN2 -> Pin 43
  Motor 3: ENA (PWM)  -> Pin 12, IN1 -> Pin 49, IN2 -> Pin 47
  Motor 4: ENA (PWM)  -> Pin 10, IN1 -> Pin 41, IN2 -> Pin 39
*/

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

// Global flag to control motor testing
bool isRunning = true;

// Function to test a single motor: runs forward, reverse, then stops.
void testMotor(int pwmPin, int in1Pin, int in2Pin) {
  // Run forward
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(pwmPin, 200); // Set speed (0-255)
  delay(2000);

  // Run in reverse
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(pwmPin, 200);
  delay(2000);

  // Stop the motor
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(pwmPin, 0);
  delay(1000);
}

void setup() {
  // Initialize motor control pins as outputs
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
  
  // Start Serial for command input and debugging
  Serial.begin(9600);
  Serial.println("DC Motor Test Started");
  Serial.println("Type 'stop' to halt motor testing, or 'run' to resume.");
}

void loop() {
  // Check Serial input for commands
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any extra whitespace

    if (command.equalsIgnoreCase("stop")) {
      isRunning = false;
      Serial.println("Motor testing stopped. Type 'run' to resume.");
    }
    else if (command.equalsIgnoreCase("run")) {
      isRunning = true;
      Serial.println("Motor testing resumed.");
    }
  }

  // If in "run" mode, perform motor tests
  if (isRunning) {
    Serial.println("Testing Motor 1");
    testMotor(MOTOR1_ENA, MOTOR1_IN1, MOTOR1_IN2);
    
    Serial.println("Testing Motor 2");
    testMotor(MOTOR2_ENA, MOTOR2_IN1, MOTOR2_IN2);
    
    Serial.println("Testing Motor 3");
    testMotor(MOTOR3_ENA, MOTOR3_IN1, MOTOR3_IN2);
    
    Serial.println("Testing Motor 4");
    testMotor(MOTOR4_ENA, MOTOR4_IN1, MOTOR4_IN2);
    
    // Wait before repeating the test sequence
    delay(3000);
  }
  else {
    // When stopped, add a short delay while still checking for Serial commands
    delay(100);
  }
}
