#include <WiFi.h>
#include <WebServer.h>

// WiFi配置，替换为自己的WiFi名称与密码
const char* ssid     = "Xiaomi 14 Pro";
const char* password = "aei482637";

// PWM LED引脚定义
#define LED_PIN 2
const int PWM_FREQ = 5000;
const int PWM_RES = 8;
int ledBright = 0; // 存储当前LED亮度 0~255

// Web服务器绑定80端口
WebServer server(80);

// 网页HTML页面（含滑动条+JS fetch通信）
const String htmlPage = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>Web无极调光器</title>
    <style>
        body { text-align:center; margin-top:100px; font-size:24px; }
        #slider { width:80%; height:30px; margin:40px 0; }
        #valShow { color:#2266cc; font-weight:bold; }
    </style>
</head>
<body>
    <h2>ESP32 Web 无极调光</h2>
    <p>亮度数值：<span id="valShow">0</span></p>
    <!-- 0~255滑动条 -->
    <input type="range" id="slider" min="0" max="255" value="0">

    <script>
        const slider = document.getElementById("slider");
        const valShow = document.getElementById("valShow");

        // 监听滑块拖动事件
        slider.addEventListener("input", function(){
            let val = this.value;
            valShow.innerText = val;
            // fetch发送GET请求传递亮度值
            fetch("/set?bright="+val);
        });
    </script>
</body>
</html>
)HTML";

// 根路径返回网页
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// 接收滑块亮度参数，更新PWM
void handleSetBright() {
  if(server.hasArg("bright")){
    ledBright = server.arg("bright").toInt();
    ledcWrite(LED_PIN, ledBright);
  }
  // 刷新页面保持滑块状态
  server.sendHeader("Location", "/");
  server.send(302);
}

void setup() {
  Serial.begin(115200);
  // 初始化PWM通道
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RES);
  ledcWrite(LED_PIN, ledBright);

  // 连接WiFi
  Serial.print("连接WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("网页访问地址：");
  Serial.println(WiFi.localIP());

  // 绑定Web路由
  server.on("/", handleRoot);
  server.on("/set", handleSetBright);
  server.begin();
  Serial.println("Web服务器启动完成");
}

void loop() {
  server.handleClient();
}
