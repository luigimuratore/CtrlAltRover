// Arduino Mega Motor Control Sketch

// Define motor pins (adjust these pin numbers as needed)
const int ENA = 26;
const int IN1 = 21;
const int IN2 = 16;

const int ENB = 25;
const int IN3 = 0;
const int IN4 = 5;

const int ENC = 20;
const int IN5 = 13;
const int IN6 = 19;

const int END = 12;
const int IN7 = 6;
const int IN8 = 1;

// Stop all motors by turning off outputs and setting PWM to 0
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, LOW);
  
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  analogWrite(ENC, 0);
  analogWrite(END, 0);
}

// Set motor speed based on a percentage (0-100)
void setMotorSpeed(int speedPercentage) {
  // Convert percentage (0-100) to PWM value (0-255)
  int pwmValue = map(speedPercentage, 0, 100, 0, 255);
  analogWrite(ENA, pwmValue);
  analogWrite(ENB, pwmValue);
  analogWrite(ENC, pwmValue);
  analogWrite(END, pwmValue);
}

// Set motor mode/direction
void setMotorMode(String mode) {
  stopMotors();  // ensure motors are stopped before changing direction
  
  if (mode == "forward") {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
    digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
    
  } else if (mode == "backward") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
    digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
    
  } else if (mode == "traslation_left") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
    digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
    
  } else if (mode == "traslation_right") {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
    digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
    
  } else if (mode == "diagonal_right_forward") {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    digitalWrite(IN5, LOW); digitalWrite(IN6, LOW);
    digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
    
  } else if (mode == "diagonal_right_backward") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    digitalWrite(IN5, LOW); digitalWrite(IN6, LOW);
    digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
    
  } else if (mode == "diagonal_left_forward") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
    digitalWrite(IN7, LOW); digitalWrite(IN8, LOW);
    
  } else if (mode == "diagonal_left_backward") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
    digitalWrite(IN7, LOW); digitalWrite(IN8, LOW);
    
  } else if (mode == "turn_left") {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
    digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
    
  } else if (mode == "turn_right") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
    digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
    
  } else if (mode == "stop") {
    // do nothing; motors are already stopped
  }
}

void setup() {
  Serial.begin(9600);
  
  // Set motor control pins as OUTPUT
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENC, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(END, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  
  stopMotors();
}

void loop() {
  if (Serial.available()) {
    // Expect command in format: "mode,speed" (e.g., "forward,60")
    String command = Serial.readStringUntil('\n');
    command.trim();
    int commaIndex = command.indexOf(',');
    if (commaIndex > 0) {
      String mode = command.substring(0, commaIndex);
      int speed = command.substring(commaIndex + 1).toInt();
      
      setMotorMode(mode);
      setMotorSpeed(speed);
    }
  }
}
