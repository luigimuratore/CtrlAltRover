/*
  Interactive Motor Rotation Test with Encoder Measurements

  Motor Connections:
    Motor 1: ENA (PWM) -> Pin 13, IN1 -> Pin 51, IN2 -> Pin 53
    Motor 2: ENA (PWM) -> Pin 11, IN1 -> Pin 45, IN2 -> Pin 43
    Motor 3: ENA (PWM) -> Pin 12, IN1 -> Pin 49, IN2 -> Pin 47
    Motor 4: ENA (PWM) -> Pin 10, IN1 -> Pin 39, IN2 -> Pin 41

  Encoder Connections:
    Encoder 1 (Motor 1): Phase A -> Pin 38, Phase B -> Pin 36
    Encoder 2 (Motor 2): Phase A -> Pin 30, Phase B -> Pin 28
    Encoder 3 (Motor 3): Phase A -> Pin 34, Phase B -> Pin 32
    Encoder 4 (Motor 4): Phase A -> Pin 26, Phase B -> Pin 24

  Command Format (via Serial Monitor):
    <motor number> <target count>
  Example:
    1 1000   --> Run Motor 1 until its encoder reads 1000 counts.
    
  Type "stop" during a test to abort it.
*/

const int numMotors = 4;

// Motor pin arrays (indexed 0 to 3 for motors 1–4)
int motorENA[numMotors] = {13, 11, 12, 10};
int motorIN1[numMotors] = {51, 45, 49, 39};
int motorIN2[numMotors] = {53, 43, 47, 41};

// Encoder pin arrays
int encA[numMotors] = {38, 30, 34, 26};
int encB[numMotors] = {36, 28, 32, 24};

// Encoder counts and previous state for channel A (for quadrature decoding)
long encoderCount[numMotors] = {0, 0, 0, 0};
int prevEncA[numMotors] = {0, 0, 0, 0};

bool testInProgress = false;  // Indicates if a test is currently running

void setup() {
  Serial.begin(9600);
  Serial.println("Interactive Motor Rotation Test with Encoders");
  Serial.println("Enter command: <motor number> <target count>");
  Serial.println("Example: 1 1000");
  Serial.println("Type 'stop' to abort a test.");
  
  // Initialize motor pins
  for (int i = 0; i < numMotors; i++) {
    pinMode(motorENA[i], OUTPUT);
    pinMode(motorIN1[i], OUTPUT);
    pinMode(motorIN2[i], OUTPUT);
  }
  
  // Initialize encoder pins and record initial state
  for (int i = 0; i < numMotors; i++) {
    pinMode(encA[i], INPUT);
    pinMode(encB[i], INPUT);
    prevEncA[i] = digitalRead(encA[i]);
  }
}

void loop() {
  // When no test is running, check for a new command from Serial
  if (!testInProgress && Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input.equalsIgnoreCase("stop")) {
      Serial.println("No test is running.");
      return;
    }
    
    // Expecting input format: <motor number> <target count>
    int spaceIndex = input.indexOf(' ');
    if (spaceIndex > 0) {
      String motorStr = input.substring(0, spaceIndex);
      String targetStr = input.substring(spaceIndex + 1);
      int motorNum = motorStr.toInt();
      long targetCount = targetStr.toInt();
      
      if (motorNum < 1 || motorNum > numMotors) {
        Serial.println("Invalid motor number. Enter a value 1 to 4.");
      } else if (targetCount <= 0) {
        Serial.println("Target count must be a positive number.");
      } else {
        runMotorTest(motorNum - 1, targetCount);
      }
    }
    else {
      Serial.println("Invalid input. Format: <motor number> <target count>");
    }
  }
  
  // Continually update all encoder counts in the background
  updateAllEncoders();
  delay(1);
}

//---------------------------
// Encoder Update Functions
//---------------------------

// Update encoder count for a given motor (using simple quadrature decoding)
void updateEncoder(int index) {
  int currentA = digitalRead(encA[index]);
  if (currentA != prevEncA[index]) { // Change detected on channel A
    int currentB = digitalRead(encB[index]);
    // Determine rotation direction
    if (currentA == HIGH) {
      if (currentB == LOW)
        encoderCount[index]++;
      else
        encoderCount[index]--;
    } else { // currentA is LOW
      if (currentB == HIGH)
        encoderCount[index]++;
      else
        encoderCount[index]--;
    }
    prevEncA[index] = currentA;
  }
}

// Update all encoders
void updateAllEncoders() {
  for (int i = 0; i < numMotors; i++) {
    updateEncoder(i);
  }
}

//---------------------------
// Motor Test Function
//---------------------------

// Run a test for the specified motor until its encoder count reaches the target.
// The motor is driven forward until abs(encoderCount) >= targetCount.
void runMotorTest(int motorIndex, long targetCount) {
  testInProgress = true;
  
  // Reset the encoder count for this motor
  encoderCount[motorIndex] = 0;
  prevEncA[motorIndex] = digitalRead(encA[motorIndex]);
  
  Serial.print("Starting test for Motor ");
  Serial.print(motorIndex + 1);
  Serial.print(" until encoder count reaches ");
  Serial.println(targetCount);
  
  // Start motor in forward direction (adjust as needed for your setup)
  digitalWrite(motorIN1[motorIndex], HIGH);
  digitalWrite(motorIN2[motorIndex], LOW);
  analogWrite(motorENA[motorIndex], 200);  // set speed (0-255)
  
  unsigned long lastPrintTime = millis();
  
  // Run until the (absolute) encoder count reaches the target
  while (abs(encoderCount[motorIndex]) < targetCount) {
    updateEncoder(motorIndex);
    
    // Print current encoder count every 100ms
    if (millis() - lastPrintTime >= 100) {
      Serial.print("Motor ");
      Serial.print(motorIndex + 1);
      Serial.print(" encoder count: ");
      Serial.println(encoderCount[motorIndex]);
      lastPrintTime = millis();
    }
    
    // Allow user to type "stop" to abort the test
    if (Serial.available() > 0) {
      String cmd = Serial.readStringUntil('\n');
      cmd.trim();
      if (cmd.equalsIgnoreCase("stop")) {
        Serial.println("Test aborted by user.");
        break;
      }
    }
    delay(1);
  }
  
  // Stop the motor
  digitalWrite(motorIN1[motorIndex], LOW);
  digitalWrite(motorIN2[motorIndex], LOW);
  analogWrite(motorENA[motorIndex], 0);
  
  Serial.print("Test completed for Motor ");
  Serial.print(motorIndex + 1);
  Serial.print(". Final encoder count: ");
  Serial.println(encoderCount[motorIndex]);
  Serial.println("Enter a new command: <motor number> <target count>");
  
  testInProgress = false;
}
