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
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
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
- `Wire.h` (pre-installed)

---

## 🛠️ How to Use

1. Open each test sketch in Arduino IDE.
2. Select the correct board: **ESP32 Dev Module**.
3. Select the correct **COM port**.
4. Upload the code and open Serial Monitor (for RTC tests).
5. Adjust `hour()` and `minute()` in the third test to your desired alarm time.

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

## 👨‍💻 Author

Made with ❤️ by [Arisudan TH]

---
