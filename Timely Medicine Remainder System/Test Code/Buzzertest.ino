void setup() {
  pinMode(26, OUTPUT);
}

void loop() {
  tone(26, 1000);   // 1000 Hz tone
  delay(1000);      // 1 second
  noTone(26);       
  delay(1000);      // 1 second off
}
