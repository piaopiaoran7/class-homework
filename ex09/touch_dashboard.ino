#include <WiFi.h>
#include <WebServer.h>

// ========== 硬件与WiFi配置 ==========
#define TOUCH_PIN 4
const char* ssid     = "Xiaomi 14 Pro";
const char* password = "aei482637";

WebServer server(80);
int touchRawVal = 0; // 存储实时触摸原始数值

// ========== 仪表盘HTML页面（AJAX自动拉取数据） ==========
const String dashHtml = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>实时触摸传感器仪表盘</title>
    <style>
        body { text-align:center; margin-top:150px; background:#111; color:#fff; }
        h2 { font-size:32px; }
        #dataShow { font-size:120px; color:#0cf; font-weight:bold; margin:40px 0; }
        .tip { font-size:20px; color:#aaa; }
    </style>
</head>
<body>
    <h2>ESP32 触摸传感器实时仪表盘</h2>
    <div id="dataShow">0</div>
    <p class="tip">手指靠近引脚 → 数值变小 | 松开手指 → 数值恢复变大</p>

    <script>
        // AJAX定时轮询，每100ms拉取一次传感器数据
        function fetchSensorData(){
            fetch("/data")
                .then(res => res.text())
                .then(val => {
                    document.getElementById("dataShow").innerText = val;
                });
        }
        // 每100毫秒自动刷新
        setInterval(fetchSensorData, 100);
    </script>
</body>
</html>
)HTML";

// 根路由：返回仪表盘网页
void handleIndex(){
  server.send(200, "text/html", dashHtml);
}

// 数据接口：仅返回当前触摸原始数值（AJAX拉取专用）
void handleGetData(){
  touchRawVal = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(touchRawVal));
}

void setup() {
  Serial.begin(115200);

  // WiFi连接
  Serial.print("连接WiFi:");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功！访问地址：");
  Serial.println(WiFi.localIP());

  // 绑定Web路由
  server.on("/", handleIndex);
  server.on("/data", handleGetData);
  server.begin();
  Serial.println("Web仪表盘服务已启动");
}

void loop() {
  server.handleClient();
  // 持续更新触摸采样值
  touchRawVal = touchRead(TOUCH_PIN);
}
