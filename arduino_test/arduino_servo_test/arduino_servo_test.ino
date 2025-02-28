#include <Servo.h>

Servo servos[6];  // Array to hold all 6 servos
int servoPins[6] = {7, 6, 5, 4, 3, 2}; // Corresponding PWM pins
int servoAngles[6] = {90, 90, 90, 90, 90, 90}; // Default angles at startup

void setup() {
  Serial.begin(9600);
  
  // Attach each servo to its respective pin
  for (int i = 0; i < 6; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(servoAngles[i]); // Set initial position
  }

  Serial.println("Servo Control Ready!");
  Serial.println("Commands: 'set [servo] [angle]', 'move [servo] [angle] [speed]', 'all [angle]', 'sweep [servo]'");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.startsWith("set")) {
      int servoNum, angle;
      sscanf(command.c_str(), "set %d %d", &servoNum, &angle);
      setServo(servoNum, angle);
    } 
    
    else if (command.startsWith("move")) {
      int servoNum, angle, speed;
      sscanf(command.c_str(), "move %d %d %d", &servoNum, &angle, &speed);
      moveServoSmooth(servoNum, angle, speed);
    } 
    
    else if (command.startsWith("all")) {
      int angle;
      sscanf(command.c_str(), "all %d", &angle);
      moveAllServos(angle);
    } 
    
    else if (command.startsWith("sweep")) {
      int servoNum;
      sscanf(command.c_str(), "sweep %d", &servoNum);
      sweepServo(servoNum);
    }
  }
}

// Smoothly move a single servo to an angle
void moveServoSmooth(int servoNum, int targetAngle, int speed) {
  if (servoNum < 1 || servoNum > 6) return;
  
  int index = servoNum - 1;
  int startAngle = servoAngles[index];
  
  if (startAngle < targetAngle) {
    for (int pos = startAngle; pos <= targetAngle; pos++) {
      servos[index].write(pos);
      delay(speed);
    }
  } else {
    for (int pos = startAngle; pos >= targetAngle; pos--) {
      servos[index].write(pos);
      delay(speed);
    }
  }
  
  servoAngles[index] = targetAngle;
  Serial.print("Servo ");
  Serial.print(servoNum);
  Serial.print(" moved to ");
  Serial.println(targetAngle);
}

// Instantly set a servo to an angle
void setServo(int servoNum, int angle) {
  if (servoNum < 1 || servoNum > 6) return;
  
  int index = servoNum - 1;
  servos[index].write(angle);
  servoAngles[index] = angle;
  
  Serial.print("Servo ");
  Serial.print(servoNum);
  Serial.print(" set to ");
  Serial.println(angle);
}

// Move all servos to the same angle smoothly
void moveAllServos(int targetAngle) {
  for (int i = 0; i < 6; i++) {
    moveServoSmooth(i + 1, targetAngle, 5); // Move smoothly with speed 5
  }
}

// Sweep a single servo from 0 to 180 and back
void sweepServo(int servoNum) {
  if (servoNum < 1 || servoNum > 6) return;
  
  int index = servoNum - 1;
  for (int pos = 0; pos <= 180; pos++) {
    servos[index].write(pos);
    delay(10);
  }
  for (int pos = 180; pos >= 0; pos--) {
    servos[index].write(pos);
    delay(10);
  }
  
  Serial.print("Sweep test on Servo ");
  Serial.println(servoNum);
}
