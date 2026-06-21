// ex03_sos.ino millis实现SOS摩斯信号 LED接GPIO4
const int ledPin = 4;
// 时间参数定义
const unsigned long shortLight = 200;    // 短亮时长
const unsigned long longLight = 600;     // 长亮时长
const unsigned long dotGap = 200;        // 单次闪烁间隔
const unsigned long groupPause = 500;    // 分组（3短/3长）中间停顿
const unsigned long sosEndPause = 2000;  // 整套SOS结束长停顿

unsigned long lastTime = 0;
unsigned long delayWait = 0;
bool ledLight = false;
int workState = 0; // 0空闲 1三短 2三长 3三短
int flashCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("ex03 SOS信号实验启动，LED接GPIO4");
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
            Serial.println("一轮SOS信号发送完成");
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
