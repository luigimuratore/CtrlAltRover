#define SENSOR1_TRIG 35
#define SENSOR1_ECHO 37

#define SENSOR2_TRIG 31
#define SENSOR2_ECHO 33

#define SENSOR3_TRIG 27
#define SENSOR3_ECHO 29

#define SENSOR4_TRIG 23
#define SENSOR4_ECHO 25

// Global flag to control sensor readings
bool isRunning = true;

void setup() {
  Serial.begin(9600);

  pinMode(SENSOR1_TRIG, OUTPUT);
  pinMode(SENSOR1_ECHO, INPUT);

  pinMode(SENSOR2_TRIG, OUTPUT);
  pinMode(SENSOR2_ECHO, INPUT);

  pinMode(SENSOR3_TRIG, OUTPUT);
  pinMode(SENSOR3_ECHO, INPUT);

  pinMode(SENSOR4_TRIG, OUTPUT);
  pinMode(SENSOR4_ECHO, INPUT);

  Serial.println("HC-SR04 Ultrasonic Sensor Test Started");
  Serial.println("Type 'stop' to halt sensor readings, or 'run' to resume.");
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
  // Check if there is any Serial input
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any extra whitespace

    // Check for "stop" and "run" commands (case-insensitive)
    if (command.equalsIgnoreCase("stop")) {
      isRunning = false;
      Serial.println("Sensor readings stopped. Type 'run' to resume.");
    }
    else if (command.equalsIgnoreCase("run")) {
      isRunning = true;
      Serial.println("Sensor readings resumed.");
    }
  }

  // If running, perform sensor readings
  if (isRunning) {
    Serial.print("Sensor 1: ");
    Serial.print(getDistance(SENSOR1_TRIG, SENSOR1_ECHO));
    Serial.println(" cm");

    Serial.print("Sensor 2: ");
    Serial.print(getDistance(SENSOR2_TRIG, SENSOR2_ECHO));
    Serial.println(" cm");

    Serial.print("Sensor 3: ");
    Serial.print(getDistance(SENSOR3_TRIG, SENSOR3_ECHO));
    Serial.println(" cm");

    Serial.print("Sensor 4: ");
    Serial.print(getDistance(SENSOR4_TRIG, SENSOR4_ECHO));
    Serial.println(" cm");

    Serial.println("-------------------------");
    delay(500);
  }
  else {
    // Small delay to avoid spamming the loop when stopped
    delay(100);
  }
}
