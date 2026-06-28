// 两路LED引脚（ESP32 PWM引脚4、5）
const int ledPin4 = 4;
const int ledPin5 = 5;

// PWM基础参数：频率5kHz，8位分辨率（0~255占空比）
const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);
  // 为两个LED分配独立PWM通道
  ledcAttach(ledPin4, freq, resolution);
  ledcAttach(ledPin5, freq, resolution);
  Serial.println("双通道PWM反相渐变警车双闪程序启动");
}

void loop() {
  // 灯4渐亮、灯5同步渐暗（反相关系）
  for(int duty = 0; duty <= 255; duty++){
    ledcWrite(ledPin4, duty);
    ledcWrite(ledPin5, 255 - duty);
    delay(10);
  }

  // 灯4渐暗、灯5同步渐亮
  for(int duty = 255; duty >= 0; duty--){
    ledcWrite(ledPin4, duty);
    ledcWrite(ledPin5, 255 - duty);
  }

  Serial.println("一次完整交替渐变循环结束");
}
