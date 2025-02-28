const int encA = 34;  // Encoder 2 Phase A
const int encB = 32;  // Encoder 2 Phase B

void setup() {
  Serial.begin(9600);
  // Try with and without pull-ups:
  pinMode(encA, INPUT_PULLUP);
  pinMode(encB, INPUT_PULLUP);
}

void loop() {
  Serial.print("Phase A: ");
  Serial.print(digitalRead(encA));
  Serial.print("  Phase B: ");
  Serial.println(digitalRead(encB));
  delay(200);
}
