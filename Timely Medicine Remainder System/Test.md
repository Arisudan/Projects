# ⏰ Timely Medicine Reminder – RTC and Buzzer Testing (ESP32)

This repository contains test codes for the core modules of the **Timely Medicine Reminder System** using an **ESP32**, including the **DS3231 RTC** and **buzzer**. These tests help validate that your components are working before integrating them into the full system.

---

## 📦 Components Used

- ESP32 Dev Board  
- DS3231 RTC Module  
- Active Buzzer  
- Breadboard + Jumper Wires  

---

## 🧪 Test 1 – DS3231 RTC Module

### 📌 Description:
This sketch initializes and checks if the RTC is functioning correctly. It prints the current time to the Serial Monitor.

### 💻 Code:
```cpp
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

void setup () {
  Serial.begin(115200);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set to compile time
  }
}

void loop () {
  DateTime now = rtc.now();

  Serial.print("Current Time: ");
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.println(now.second());

  delay(1000);
}
```

---

## 🔊 Test 2 – Buzzer Test

### 📌 Description:
This sketch activates a buzzer every second using the `tone()` function to verify that the buzzer is connected and working.

### 💻 Code:
```cpp
void setup() {
  pinMode(26, OUTPUT);
}

void loop() {
  tone(26, 1000);   // 1000 Hz tone
  delay(1000);      // 1 second
  noTone(26);       
  delay(1000);      // 1 second off
}
```

---

## ⏰🔔 Test 3 – Combined RTC & Buzzer Alarm

### 📌 Description:
This sketch uses the DS3231 to read time and activates the buzzer when the time matches a predefined value (e.g., `15:15`).

### 💻 Code:
```cpp
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

void setup () {
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  pinMode(26, OUTPUT);
}

void loop () {
  DateTime now = rtc.now();

  if (now.hour() == 15 && now.minute() == 15) {
    tone(26, 1000); // Ring buzzer
    delay(10000);   // for 10 seconds
    noTone(26);
    delay(60000);   // Prevent retriggering in same minute
  }
}
```

---

## ⏰ Test 4 – Full System (Medicine Reminder with Buzzer & Display)

### 📌 Description:
This sketch integrates the **RTC** module, **buzzer**, and **OLED display**. It shows the current time and reminder time on the OLED, and triggers the buzzer at the set reminder time.

### 💻 Code:
```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1  // Not used, but required
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS3231 rtc;

int reminderHour = 15;     // Set your reminder hour here
int reminderMinute = 37;   // Set your reminder minute here
bool reminderTriggered = false;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set to compile time
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (1);
  }

  pinMode(26, OUTPUT); // Buzzer pin
  display.clearDisplay();
}

void loop() {
  DateTime now = rtc.now();

  // Display current time
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.printf("%02d:%02d:%02d", now.hour(), now.minute(), now.second());

  // Display reminder time
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.print("Reminder: ");
  display.printf("%02d:%02d", reminderHour, reminderMinute);
  display.display();

  // Check for alarm time
  if (now.hour() == reminderHour && now.minute() == reminderMinute && !reminderTriggered) {
    tone(26, 1000);  // Buzzer on
    delay(10000);     // Ring for 10 seconds
    noTone(26);      // Buzzer off
    reminderTriggered = true;
  }

  // Reset reminder trigger flag after a minute
  if (now.minute() != reminderMinute) {
    reminderTriggered = false;
  }

  delay(1000); // Update every second
}
```

---

## 🧷 Wiring Diagrams

### DS3231 RTC to ESP32
| RTC Pin | ESP32 Pin |
|---------|-----------|
| VCC     | 3.3V      |
| GND     | GND       |
| SDA     | GPIO 21   |
| SCL     | GPIO 22   |

### Buzzer to ESP32
| Buzzer Pin | ESP32 Pin |
|------------|-----------|
| +          | GPIO 26   |
| -          | GND       |

---

## 📚 Libraries Required

Install these libraries via Arduino Library Manager:
- `RTClib` by Adafruit
- `Adafruit_SSD1306` by Adafruit
- `Wire.h` (pre-installed)

---

## 🛠️ How to Use

1. Open each test sketch in Arduino IDE.
2. Select the correct board: **ESP32 Dev Module**.
3. Select the correct **COM port**.
4. Upload the code and open Serial Monitor (for RTC tests).
5. Adjust `hour()` and `minute()` in the third and fourth tests to your desired alarm time.

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

## 👨‍💻 Author

Made with ❤️ by Arisudan TH
