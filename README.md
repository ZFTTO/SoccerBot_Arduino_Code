# SoccerBot Arduino Code

This repository contains two Arduino sketches for LuluLab SoccerBot WiFi control.

- **soccerBot_wifi_test/**: Minimal test version, simple code for basic WiFi connection testing.
- **SoccerBot_wifi_btn/**: Full complete version, includes button control logic for official use.

## How to use
1. Open the `.ino` file in Arduino IDE
2. Fill in your WiFi SSID and password inside the code
3. Upload firmware to ESP32
4. Open the included `index.html` in browser for web control

## Required packages:
1. ESP32 board core (Espressif Systems)
2. ESP32Servo library (madhephaestus)

WiFi & WebServer are built-in inside ESP32 core, no extra install needed.


---

# SoccerBot Arduino 程式庫

本倉庫收錄兩組用於 LuluLab SoccerBot WiFi 遙控的 Arduino 程式。

- **soccerBot_wifi_test/**：簡易測試版，程式精簡，只用於測試 WiFi 連線是否正常。
- **SoccerBot_wifi_btn/**：完整功能版，內置按鈕控制邏輯，為正式使用版本。

## 使用方法
1. 在 Arduino IDE 打開對應的 `.ino` 檔案
2. 在程式內填入 WiFi 名稱（SSID）與密碼
3. 將程式上傳至 ESP32
4. 用瀏覽器打開資料夾內的 `index.html` 網頁，進行遙控

## 所需套件：
1. ESP32 開發板核心（Espressif Systems）
2. ESP32Servo 程式庫（madhephaestus）

WiFi 同 WebServer 屬於 ESP32 Core 內建，毋須另外安裝。
