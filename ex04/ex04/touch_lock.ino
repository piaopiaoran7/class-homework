#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 500        // 根据串口读数修改，触摸值低于此值触发

bool ledState = false;
unsigned long lastTouch = 0;
const unsigned long DEBOUNCE_MS = 600; // 加长防抖到600ms，抑制连续抖动

void gotTouch() {
  unsigned long now = millis();
  // 时间防抖拦截
  if (now - lastTouch < DEBOUNCE_MS) return;
  
  // 二次读取触摸值，连续3次校验彻底过滤杂波
  int val1 = touchRead(TOUCH_PIN);
  delayMicroseconds(200);
  int val2 = touchRead(TOUCH_PIN);
  delayMicroseconds(200);
  int val3 = touchRead(TOUCH_PIN);

  // 三次读数全部低于阈值才判定为有效触摸
  if(val1 >= THRESHOLD || val2 >= THRESHOLD || val3 >= THRESHOLD) return;

  lastTouch = now;
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);
  // 绑定触摸中断
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
  Serial.println("触摸中断增强消抖程序已启动");
  Serial.println("操作：触摸GPIO4杜邦线切换LED亮灭");
}

void loop() {
  // 实时打印触摸数值，用于现场校准阈值
  int touchVal = touchRead(TOUCH_PIN);
  Serial.print("当前Touch数值：");
  Serial.println(touchVal);
  delay(100);
}
