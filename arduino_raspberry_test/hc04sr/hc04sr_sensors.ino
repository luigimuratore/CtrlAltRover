// Arduino Mega Sketch

#define SENSOR1_TRIG 35
#define SENSOR1_ECHO 37

#define SENSOR2_TRIG 31
#define SENSOR2_ECHO 33

#define SENSOR3_TRIG 27
#define SENSOR3_ECHO 29

#define SENSOR4_TRIG 23
#define SENSOR4_ECHO 25

void setup() {
  Serial.begin(9600);
  
  // Initialize sensor pins
  pinMode(SENSOR1_TRIG, OUTPUT);
  pinMode(SENSOR1_ECHO, INPUT);
  
  pinMode(SENSOR2_TRIG, OUTPUT);
  pinMode(SENSOR2_ECHO, INPUT);
  
  pinMode(SENSOR3_TRIG, OUTPUT);
  pinMode(SENSOR3_ECHO, INPUT);
  
  pinMode(SENSOR4_TRIG, OUTPUT);
  pinMode(SENSOR4_ECHO, INPUT);

  Serial.println("Arduino ready. Send sensor number (1-4) to get distance measurement.");
}

float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.0343 / 2;  // Convert duration to cm

  if (distance >= 400 || distance <= 2)
    return -1;  // Out of range
  return distance;
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any extra whitespace

    if (command.equals("1")) {
      float distance = getDistance(SENSOR1_TRIG, SENSOR1_ECHO);
      Serial.print("Sensor 1: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
    else if (command.equals("2")) {
      float distance = getDistance(SENSOR2_TRIG, SENSOR2_ECHO);
      Serial.print("Sensor 2: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
    else if (command.equals("3")) {
      float distance = getDistance(SENSOR3_TRIG, SENSOR3_ECHO);
      Serial.print("Sensor 3: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
    else if (command.equals("4")) {
      float distance = getDistance(SENSOR4_TRIG, SENSOR4_ECHO);
      Serial.print("Sensor 4: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
    else {
      Serial.println("Invalid command. Please send 1, 2, 3, or 4.");
    }
  }
}
