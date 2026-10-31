#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin for the IR LED (D2)
IRsend irsend(kIrLed);

const char* ssid = "Epson_Remote";
const char* password = "12345678"; 

ESP8266WebServer server(80);

// ==========================================
// SEND FUNCTION FOR EPSON CODES
// ==========================================
void sendEpsonCode(uint64_t code) {
  irsend.sendEpson(code, 32);
  delay(100); 
}

// ==========================================
// WEB INTERFACE
// ==========================================
const char* htmlHome = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Epson Projector Remote</title>
  <style>
    body { font-family: Arial; text-align: center; background-color: #1a1a2e; color: white; padding: 20px; }
    h1 { color: #e94560; }
    .grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px; max-width: 400px; margin: 0 auto; }
    button { padding: 15px; font-size: 14px; font-weight: bold; border: none; border-radius: 10px; color: white; cursor: pointer; transition: 0.2s; }
    button:active { transform: scale(0.95); }
    .power { background-color: #e94560; grid-column: span 3; }
    .action { background-color: #0f3460; }
    .num { background-color: #16213e; }
    .custom { background-color: #2b6cb0; grid-column: span 3; margin-top: 10px;}
    .status { margin-top: 15px; color: #4ade80; font-size: 16px; height: 20px; }
    input { width: 80%; padding: 10px; margin-top: 10px; border-radius: 5px; border: none; text-align: center; }
  </style>
</head>
<body>
  <h1>Epson Remote</h1>
  <div class="grid">
    <button class="power" onclick="fire('power')">POWER (Toggle & Confirm)</button>
    
    <button class="action" onclick="fire('menu')">MENU</button>
    <button class="action" onclick="fire('source')">SOURCE</button>
    <button class="action" onclick="fire('auto')">AUTO</button>
    
    <button class="action" onclick="fire('freeze')">FREEZE</button>
    <button class="action" onclick="fire('blank')">BLANK</button>
    <button class="action" onclick="fire('mute')">MUTE</button>
    
    <button class="action" onclick="fire('volup')">VOL +</button>
    <button class="action" onclick="fire('voldown')">VOL -</button>
    <button class="action" onclick="fire('resize')">RESIZE</button>
    
    <button class="action" onclick="fire('custom1')">CUSTOM</button>
    <button class="num" onclick="fire('num1')">1</button>
    <button class="num" onclick="fire('num2')">2</button>
    <button class="num" onclick="fire('num3')">3</button>
    <button class="num" onclick="fire('num4')">4</button>
    <button class="num" onclick="fire('num5')">5</button>
  </div>
  
  <div style="margin-top: 20px;">
    <p>Send Custom Hex Code:</p>
    <input type="text" id="customHex" placeholder="e.g., C1AA09F6" value="C1AA09F6">
    <button class="custom" onclick="sendCustom()">SEND CUSTOM</button>
  </div>

  <div class="status" id="status"></div>

  <script>
    function fire(btn) {
      document.getElementById('status').innerHTML = "Sending...";
      fetch('/fire?btn=' + btn)
        .then(response => response.text())
        .then(data => {
          document.getElementById('status').innerHTML = "✅ Sent!";
          setTimeout(() => { document.getElementById('status').innerHTML = ""; }, 3000);
        });
    }
    
    function sendCustom() {
      var hex = document.getElementById('customHex').value;
      document.getElementById('status').innerHTML = "Sending Custom...";
      fetch('/custom?code=' + hex)
        .then(response => response.text())
        .then(data => {
          document.getElementById('status').innerHTML = "✅ Custom Sent!";
          setTimeout(() => { document.getElementById('status').innerHTML = ""; }, 2000);
        });
    }
  </script>
</body>
</html>
)=====";

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  irsend.begin();

  WiFi.softAP(ssid, password);
  Serial.print("Connect to 'Epson_Remote' and go to: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    server.send(200, "text/html", htmlHome);
  });

  // Handle Button Presses
  server.on("/fire", []() {
    String btn = server.arg("btn");
    
    if (btn == "power") {
      // ⭐ DOUBLE TAP FOR POWER OFF CONFIRMATION ⭐
      sendEpsonCode(0xC1AA09F6); // First press (brings up menu or turns on)
      delay(1000);               // Wait 1 second
      sendEpsonCode(0xC1AA09F6); // Second press (confirms power off)
    }
    else if (btn == "menu") sendEpsonCode(0xC1AA59A6);
    else if (btn == "source") sendEpsonCode(0xC1AAB946);
    else if (btn == "auto") sendEpsonCode(0xC1AA7986);
    else if (btn == "freeze") sendEpsonCode(0xC1AA49B6);
    else if (btn == "blank") sendEpsonCode(0xC1AAF10E);
    else if (btn == "mute") sendEpsonCode(0xC1AAC936);
    else if (btn == "volup") sendEpsonCode(0xC1AA19E6);
    else if (btn == "voldown") sendEpsonCode(0xC1AA9966);
    else if (btn == "resize") sendEpsonCode(0xC1AAF906);
    else if (btn == "custom1") sendEpsonCode(0xC1AAD926);
    else if (btn == "num1") sendEpsonCode(0xC1AA01FE);
    else if (btn == "num2") sendEpsonCode(0xC1AA817E);
    else if (btn == "num3") sendEpsonCode(0xC1AA41BE);
    else if (btn == "num4") sendEpsonCode(0xC1AAC13E);
    else if (btn == "num5") sendEpsonCode(0xC1AA21DE);
    
    server.send(200, "text/plain", "OK");
  });

  // Handle Custom Hex Code
  server.on("/custom", []() {
    String hexStr = server.arg("code");
    uint64_t code = strtoull(hexStr.c_str(), NULL, 16);
    sendEpsonCode(code);
    server.send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}