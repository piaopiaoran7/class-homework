// 硬件引脚定义
#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 500

// PWM基础参数
const int PWM_FREQ = 5000;
const int PWM_RES = 8;  // 0~255亮度区间

// 档位与触摸状态变量
int speedGear = 1;          // 当前速度档位 1/2/3
unsigned long lastTouchTime = 0;
const unsigned long DEBOUNCE = 600;

// 档位对应单步延时：数值越大，呼吸越慢
int gearDelay[4] = {0, 8, 4, 1}; 

void touchTrigger() {
  unsigned long now = millis();
  // 时间防抖过滤
  if (now - lastTouchTime < DEBOUNCE) return;

  // 三次采样校验，消除触摸杂波
  int v1 = touchRead(TOUCH_PIN);
  delayMicroseconds(200);
  int v2 = touchRead(TOUCH_PIN);
  delayMicroseconds(200);
  int v3 = touchRead(TOUCH_PIN);
  if(v1 >= THRESHOLD || v2 >= THRESHOLD || v3 >= THRESHOLD) return;

  lastTouchTime = now;
  // 档位循环切换 1→2→3→1
  speedGear += 1;
  if(speedGear > 3) speedGear = 1;
  Serial.print("切换档位：");
  Serial.println(speedGear);
  if(speedGear == 1) Serial.println("【1档 慢速平缓呼吸】");
  if(speedGear == 2) Serial.println("【2档 中等速度呼吸】");
  if(speedGear == 3) Serial.println("【3档 快速急促呼吸】");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  // 初始化PWM通道
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RES);
  // 绑定触摸中断
  touchAttachInterrupt(TOUCH_PIN, touchTrigger, THRESHOLD);
  Serial.println("=====多档位触摸调速呼吸灯（提速优化版）=====");
  Serial.println("触摸GPIO4切换呼吸速度，共3档循环");
  Serial.println("1档慢 / 2档中 / 3档快");
}

void loop() {
  int singleDelay = gearDelay[speedGear];
  // 渐亮
  for(int bright = 0; bright <= 255; bright++){
    ledcWrite(LED_PIN, bright);
    delay(singleDelay);
  }
  // 渐暗
  for(int bright = 255; bright >= 0; bright--){
    ledcWrite(LED_PIN, bright);
    delay(singleDelay);
  }
}
