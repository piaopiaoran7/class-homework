// 两路LED引脚
const int ledPin4 = 4;
const int ledPin5 = 5;

// PWM参数
const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);
  // 两个引脚绑定PWM
  ledcAttach(ledPin4, freq, resolution);
  ledcAttach(ledPin5, freq, resolution);
}

void loop() {
  // 4渐亮，5同步渐暗
  for(int duty = 0; duty <= 255; duty++){
    ledcWrite(ledPin4, duty);
    ledcWrite(ledPin5, 255 - duty);
    delay(10);
  }

  // 4渐暗，5同步渐亮
  for(int duty = 255; duty >= 0; duty--){
    ledcWrite(ledPin4, duty);
    ledcWrite(ledPin5, 255 - duty);
    delay(10);
  }

  Serial.println("一轮交替呼吸完成");
}
