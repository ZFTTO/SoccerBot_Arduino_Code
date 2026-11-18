/*
ESP32 Web SoccerBot Controller (motor + 360servo)
WiFi + WebServer web remote UI with pointer-event touch buttons
Libraries: ESP32 Core, ESP32Servo
*/

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// 1. define pin
const int leftMotor1 = 0;
const int leftMotor2 = 1;
const int rightMotor1 = 2;
const int rightMotor2 = 3;
const int kickerPin = 4;

// 2. variable
int currentSpeed = 180; 
const char *ssid = "LuluLab_button9";
const char *password = "12345678";

WebServer server(80);
Servo kickerServo;

unsigned long lastMoveTime = 0;
const unsigned long TIMEOUT = 600;
bool isMoving = false;

// 3. web UI
const char* htmlPage = R"rawliteral(
<!doctype html>
<html>
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
        <title>Soccer Bot C3</title>
        <style>
            body {
                font-family: sans-serif;
                background: #f0f2f5;
                margin: 0;
                overflow: hidden;
                touch-action: none;
                text-align: center;
            }
            .header {
                background: #1976d2;
                color: white;
                padding: 15px;
                font-weight: bold;
            }
            .container {
                display: flex;
                justify-content: space-around;
                align-items: center;
                height: 65vh;
            }
            .d-pad {
                display: grid;
                grid-template-columns: repeat(3, 75px);
                grid-gap: 10px;
            }
            .btn {
                width: 75px;
                height: 75px;
                border-radius: 15px;
                border: none;
                background: white;
                box-shadow: 0 4px #bbb;
                font-size: 18px;
                font-weight: bold;
            }
            .btn:active {
                box-shadow: 0 1px #bbb;
                transform: translateY(3px);
                background: #eee;
            }
            .btn-kick {
                width: 110px;
                height: 110px;
                border-radius: 50%;
                border: 4px solid white;
                background: #ff9800;
                color: white;
                font-size: 35px;
                box-shadow: 0 6px #e68a00;
            }
            .btn-kick:active {
                transform: translateY(5px);
                background: #ff9800;
                box-shadow: 0 1px #e68a00;
            }
            .speed-ctrl {
                padding: 20px;
                background: white;
                border-radius: 20px;
                margin: 10px 20px;
                font-weight: 800;
                font-size: 15px;
                color: #333;
            }
            input[type="range"] {
                width: 80%;
            }
        </style>
    </head>
    <body>
        <div class="header">⚽ ESP32-C3 SoccerBot</div>
        <div class="container">
            <div class="d-pad">
                <div></div>
                <button
                    class="btn"
                    onpointerdown="send('forward')"
                    onpointerup="send('stop')"
                    onpointercancel="send('stop')"
                    onpointerleave="send('stop')"
                >
                    ▲
                </button>
                <div></div>
                <button
                    class="btn"
                    onpointerdown="send('left')"
                    onpointerup="send('stop')"
                    onpointercancel="send('stop')"
                    onpointerleave="send('stop')"
                >
                    ◀
                </button>
                <button class="btn" style="background: #ffcdd2" onclick="send('stop')">■</button>
                <button
                    class="btn"
                    onpointerdown="send('right')"
                    onpointerup="send('stop')"
                    onpointercancel="send('stop')"
                    onpointerleave="send('stop')"
                >
                    ▶
                </button>
                <div></div>
                <button
                    class="btn"
                    onpointerdown="send('backward')"
                    onpointerup="send('stop')"
                    onpointercancel="send('stop')"
                    onpointerleave="send('stop')"
                >
                    ▼
                </button>
                <div></div>
            </div>
            <button
                class="btn-kick"
                onpointerdown="send('kick')"
                onpointerup="send('kicker_stop')"
                onpointerleave="send('kicker_stop')"
                onpointercancel="send('kicker_stop')"
            >
                ⚽
            </button>
        </div>
        <div class="speed-ctrl">
            Speed Control: <span id="speedVal">180</span><br />
            <input type="range" min="0" max="255" value="180" oninput="updateSpeed(this.value)" />
        </div>
        <script>
            function send(cmd) {
                fetch("/" + cmd);
            }
            function updateSpeed(val) {
                document.getElementById("speedVal").innerText = val;
                fetch("/setSpeed?val=" + val);
            }
        </script>
    </body>
</html>
)rawliteral";

// 4. funtion
void move(int l1, int l2, int r1, int r2) {
  analogWrite(leftMotor1, l1);
  analogWrite(leftMotor2, l2);
  analogWrite(rightMotor1, r1);
  analogWrite(rightMotor2, r2);
  
  lastMoveTime = millis();
  isMoving = (l1 != 0 || l2 != 0 || l3 != 0 || l4 != 0);
}

void moveForward(){
  move(currentSpeed, 0, currentSpeed, 0);
}

void moveBackward(){
  move(0, currentSpeed, 0, currentSpeed);
}

void turnLeft(){
  move(0, currentSpeed, currentSpeed, 0);
}

void turnRight(){
  move(currentSpeed, 0, 0, currentSpeed);
}

void stopMoving(){
  move(0, 0, 0, 0);
}

void kicking(){
  kickerServo.write(180);
}

void stopKick(){
  kickerServo.write(90);
}

// 5.setup
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, LOW);
  Serial.println("Motors Initialized.");
  

  ESP32PWM::allocateTimer(0);
  kickerServo.setPeriodHertz(50);
  kickerServo.attach(kickerPin, 1000, 2000);
  kickerServo.write(90); 
  Serial.println("Servo Initialized.");

  WiFi.softAP(ssid, password);
  Serial.println("WiFi OK.");
  Serial.print("WiFi's softAPIP: ");
  Serial.println(WiFi.softAPIP());

  // Set WiFi transmit power to a medium stable value (range 8-20, 11-13 is relatively stable)
  WiFi.setTxPower(WIFI_POWER_11dBm);
  // Disable WiFi auto-sleep (critical! Sleep will cause extremely high latency for the first packet)
  WiFi.setSleep(false);

  server.on("/", []() { server.send(200, "text/html", htmlPage); });
  server.on("/forward", []() { moveForward(); server.send(200); });
  server.on("/backward", []() { moveBackward(); server.send(200); });
  server.on("/left", []() { turnLeft(); server.send(200); });
  server.on("/right", []() { turnRight(); server.send(200); });
  server.on("/stop", []() { stopMoving(); server.send(200); });
  server.on("/kick", []() { kicking(); server.send(200); });
  server.on("/kicker_stop", []() { stopKick(); server.send(200); });
  server.on("/setSpeed", []() {
    if (server.hasArg("val")) currentSpeed = server.arg("val").toInt();
    server.send(200);
  });                                

  server.begin();
}

// 6. loop
void loop() {
  server.handleClient();

  if (isMoving && millis() - lastMoveTime > TIMEOUT) {
    stopMoving();
  }
}