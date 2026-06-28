#include <WiFi.h>
#include <WebServer.h>

// ========== 硬件引脚配置 ==========
#define TOUCH_SENSOR_PIN 4   // 触摸检测引脚
#define ALARM_LED_PIN 2      // 报警LED引脚
#define TOUCH_THRESHOLD 500  // 触摸阈值

// ========== WiFi配置（替换成自己WiFi） ==========
const char* ssid     = "Xiaomi 14 Pro";
const char* password = "aei482637";

// ========== Web服务器实例 ==========
WebServer server(80);

// ========== 全局安防状态变量 ==========
bool isArm = false;      // false=撤防  true=布防
bool isAlarm = false;    // false=正常  true=报警锁定
unsigned long lastTouchDebounce = 0;
const unsigned long DEBOUNCE_MS = 600;

// ========== 网页HTML页面（双按钮+JS fetch通信） ==========
const String htmlPage = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>物联网安防报警器</title>
    <style>
        body { text-align:center; margin-top:120px; font-size:26px; }
        button { width:220px; height:60px; font-size:24px; margin:20px 10px; cursor:pointer; }
        #stateText { color:#d02020; font-weight:bold; }
    </style>
</head>
<body>
    <h2>ESP32 安防报警主机控制面板</h2>
    <p>当前系统状态：<span id="stateText">未布防</span></p>
    <br>
    <button onclick="armSystem()">布防(Arm)</button>
    <button onclick="disarmSystem()">撤防(Disarm)</button>

    <script>
        // 布防按钮点击
        function armSystem(){
            fetch("/set?state=arm").then(()=>location.reload());
        }
        // 撤防按钮点击
        function disarmSystem(){
            fetch("/set?state=disarm").then(()=>location.reload());
        }
    </script>
</body>
</html>
)HTML";

// ========== 触摸中断回调函数 ==========
void touchDetectCallback(){
  unsigned long now = millis();
  // 防抖拦截
  if(now - lastTouchDebounce < DEBOUNCE_MS) return;

  // 三次采样滤波消除杂波
  int v1 = touchRead(TOUCH_SENSOR_PIN);
  delayMicroseconds(200);
  int v2 = touchRead(TOUCH_SENSOR_PIN);
  delayMicroseconds(200);
  int v3 = touchRead(TOUCH_SENSOR_PIN);
  if(v1 >= TOUCH_THRESHOLD || v2 >= TOUCH_THRESHOLD || v3 >= TOUCH_THRESHOLD) return;

  lastTouchDebounce = now;
  // 仅布防状态才触发锁定报警
  if(isArm){
    isAlarm = true;
    Serial.println("⚠️ 检测到入侵，报警锁定启动！");
  }
}

// ========== Web路由处理函数 ==========
// 返回控制面板网页
void handleRootPage(){
  server.send(200, "text/html", htmlPage);
}

// 接收布防/撤防指令，更新全局状态
void handleSetSystemState(){
  if(server.hasArg("state")){
    String cmd = server.arg("state");
    if(cmd == "arm"){
      isArm = true;
      Serial.println("✅ 系统已布防，触摸引脚将触发报警");
    }else if(cmd == "disarm"){
      isArm = false;
      isAlarm = false; // 撤防同步清除报警锁定
      digitalWrite(ALARM_LED_PIN, LOW);
      Serial.println("❌ 系统已撤防，报警复位关闭");
    }
  }
  // 跳转回控制面板页面
  server.sendHeader("Location", "/");
  server.send(302);
}

void setup() {
  Serial.begin(115200);
  // LED引脚初始化
  pinMode(ALARM_LED_PIN, OUTPUT);
  digitalWrite(ALARM_LED_PIN, LOW);

  // 绑定触摸中断
  touchAttachInterrupt(TOUCH_SENSOR_PIN, touchDetectCallback, TOUCH_THRESHOLD);

  // WiFi连接逻辑
  Serial.print("正在连接WiFi：");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功！");
  Serial.print("安防主机访问地址：");
  Serial.println(WiFi.localIP());

  // 绑定Web服务路由
  server.on("/", handleRootPage);
  server.on("/set", handleSetSystemState);
  server.begin();
  Serial.println("Web安防控制面板已启动");
}

void loop() {
  server.handleClient();

  // 报警锁定状态：LED高频狂闪
  if(isAlarm){
    digitalWrite(ALARM_LED_PIN, HIGH);
    delay(80);
    digitalWrite(ALARM_LED_PIN, LOW);
    delay(80);
  }else{
    digitalWrite(ALARM_LED_PIN, LOW);
  }
}
