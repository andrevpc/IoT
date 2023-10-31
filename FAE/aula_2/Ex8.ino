void setup() {
  for (int i = 2; i <= 6; i++)
    pinMode(i, OUTPUT);
}

void loop() {
  for (int i = 2; i <= 5; i++) {
    digitalWrite(i, HIGH);
    delay(1000);
    digitalWrite(i + 1, HIGH);
    digitalWrite(i - 1, LOW);
  }
  for (int i = 5; i >= 2; i--) {
    digitalWrite(i, HIGH);
    delay(1000);
    digitalWrite(i - 1, HIGH);
    digitalWrite(i + 1, LOW);
  }
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
  digitalWrite(3, LOW);
  delay(1000);
}