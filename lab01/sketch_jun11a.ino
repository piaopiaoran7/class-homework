#define LED_PIN 4
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("初始化完成，开始闪烁LED");
}
void loop() {
  Serial.println("LED 点亮");
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  Serial.println("LED 熄灭");
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}

