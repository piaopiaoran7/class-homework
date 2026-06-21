const int ledPin = 4;
unsigned long lastTime = 0;
const unsigned long interval = 500;
bool ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long now = millis();
  if (now - lastTime >= interval) {
    lastTime = now;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}
