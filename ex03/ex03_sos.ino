const int ledPin = 5;
const unsigned long shortLight = 200;
const unsigned long longLight = 600;
const unsigned long dotGap = 200;
const unsigned long groupPause = 500;
const unsigned long sosEndPause = 2000;

unsigned long lastTime = 0;
unsigned long delayWait = 0;
bool ledLight = false;
int workState = 0;
int flashCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long now = millis();
  if (now - lastTime >= delayWait) {
    lastTime = now;
    switch (workState) {
      case 0:
        workState = 1;
        flashCount = 0;
        ledLight = true;
        digitalWrite(ledPin, HIGH);
        delayWait = shortLight;
        break;
      case 1:
        if (ledLight) {
          digitalWrite(ledPin, LOW);
          ledLight = false;
          delayWait = dotGap;
          flashCount++;
        } else {
          if (flashCount >= 3) {
            workState = 2;
            flashCount = 0;
            delayWait = groupPause;
          } else {
            digitalWrite(ledPin, HIGH);
            ledLight = true;
            delayWait = shortLight;
          }
        }
        break;
      case 2:
        if (ledLight) {
          digitalWrite(ledPin, LOW);
          ledLight = false;
          delayWait = dotGap;
          flashCount++;
        } else {
          if (flashCount >= 3) {
            workState = 3;
            flashCount = 0;
            delayWait = groupPause;
          } else {
            digitalWrite(ledPin, HIGH);
            ledLight = true;
            delayWait = longLight;
          }
        }
        break;
      case 3:
        if (ledLight) {
          digitalWrite(ledPin, LOW);
          ledLight = false;
          delayWait = dotGap;
          flashCount++;
        } else {
          if (flashCount >= 3) {
            workState = 0;
            digitalWrite(ledPin, LOW);
            delayWait = sosEndPause;
          } else {
            digitalWrite(ledPin, HIGH);
            ledLight = true;
            delayWait = shortLight;
          }
        }
        break;
    }
  }
}
