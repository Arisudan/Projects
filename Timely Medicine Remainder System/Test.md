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

## ⏰ Test 4 – Medicine Reminder with Buzzer & Display)

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

## ⏰ Test 5 – Interactive Medicine Reminder with Push Buttons

### 📌 Description:
This sketch allows users to set and update a medicine reminder time using four push buttons. It uses a DS3231 RTC for real-time tracking, a buzzer for alerts, and an OLED display to show the current time and reminder time.
Users can press the Set button at any time to re-enter the time-setting mode. The reminder time triggers the buzzer for 10 seconds when matched.

### 💻 Code:
```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS3231 rtc;

// Pins
const int buzzerPin = 26;
const int btnHourPin = 14;
const int btnMinutePin = 27;
const int btnSetPin = 33;
const int btnResetPin = 32;

int reminderHour = 15;
int reminderMinute = 30;
bool reminderTriggered = false;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(btnHourPin, INPUT_PULLUP);
  pinMode(btnMinutePin, INPUT_PULLUP);
  pinMode(btnSetPin, INPUT_PULLUP);
  pinMode(btnResetPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Medicine Reminder");
  display.display();
  delay(2000);

  enterTimeSetMode(); // initial set
}

void loop() {
  // Listen for either Set or Reset button to enter time set mode
  if (digitalRead(btnSetPin) == LOW || digitalRead(btnResetPin) == LOW) {
    enterTimeSetMode();
  }

  DateTime now = rtc.now();

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.printf("%02d:%02d:%02d", now.hour(), now.minute(), now.second());

  display.setTextSize(1);
  display.setCursor(0, 50);
  display.print("Reminder: ");
  display.printf("%02d:%02d", reminderHour, reminderMinute);
  display.display();

  if (now.hour() == reminderHour && now.minute() == reminderMinute && !reminderTriggered) {
    tone(buzzerPin, 1000);
    delay(10000);
    noTone(buzzerPin);
    reminderTriggered = true;
  }

  if (now.minute() != reminderMinute) {
    reminderTriggered = false;
  }

  delay(1000);
}

void enterTimeSetMode() {
  bool setting = true;
  while (setting) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.print("Set: ");
    display.printf("%02d:%02d", reminderHour, reminderMinute);
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("Btn1:Hour Btn2:Min Btn3:Set");
    display.display();

    if (digitalRead(btnHourPin) == LOW) {
      reminderHour = (reminderHour + 1) % 24;
      delay(300);
    }

    if (digitalRead(btnMinutePin) == LOW) {
      reminderMinute = (reminderMinute + 1) % 60;
      delay(300);
    }

    if (digitalRead(btnSetPin) == LOW) {
      delay(500); // debounce
      setting = false;
    }
  }
}
```

---
## ⏰ Test 6 – Interactive Medicine Reminder with Push Buttons (Final)

### 📌 Description:

This sketch allows users to set and update a medicine reminder time using four push buttons. It utilizes a DS3231 RTC for real-time tracking, a buzzer for alerts, and an OLED display to show the current time and reminder time.

### 💻 Code:
```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS3231 rtc;

#define BTN_HOUR 14    // Button for setting the hour
#define BTN_MINUTE 27  // Button for setting the minute
#define BTN_SET 33     // Button for confirming the time
#define BTN_RESET 32   // Button to reset reminders
#define BUZZER 26      // Buzzer for reminder sound

struct Reminder {
  int hour;
  int minute;
};

const int MAX_REMINDERS = 5;
Reminder reminders[MAX_REMINDERS];
int reminderCount = 0;
bool reminderTriggered[MAX_REMINDERS];

int tempHour = 0, tempMinute = 0;
bool settingTime = true;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(BTN_HOUR, INPUT_PULLUP);
  pinMode(BTN_MINUTE, INPUT_PULLUP);
  pinMode(BTN_SET, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
}

void loop() {
  DateTime now = rtc.now();

  // Reset reminders if BTN_RESET is pressed
  if (digitalRead(BTN_RESET) == LOW) {
    reminderCount = 0;
    settingTime = true;
    displayMessage("Reminders Cleared");
    delay(500);
  }

  // Time setting mode
  if (settingTime) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Set Reminder Time:");
    display.setTextSize(2);
    display.setCursor(10, 25);
    display.printf("%02d:%02d", tempHour, tempMinute);
    display.display();

    // Increment hour when BTN_HOUR is pressed
    if (digitalRead(BTN_HOUR) == LOW) {
      tempHour = (tempHour + 1) % 24;
      delay(250);  // Debouncing
    }

    // Increment minute when BTN_MINUTE is pressed
    if (digitalRead(BTN_MINUTE) == LOW) {
      tempMinute = (tempMinute + 1) % 60;
      delay(250);  // Debouncing
    }

    // Confirm the time with BTN_SET
    if (digitalRead(BTN_SET) == LOW) {
      if (reminderCount < MAX_REMINDERS) {
        reminders[reminderCount].hour = tempHour;
        reminders[reminderCount].minute = tempMinute;
        reminderTriggered[reminderCount] = false;
        reminderCount++;
      }
      settingTime = false;
      delay(500);  // Debouncing
    }
  } else {
    // Display current time and reminders
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Time: ");
    display.printf("%02d:%02d:%02d", now.hour(), now.minute(), now.second());

    display.setCursor(0, 16);
    display.print("Reminders:");
    for (int i = 0; i < reminderCount; i++) {
      display.setCursor(0, 26 + (i * 8));
      display.printf("%d. %02d:%02d", i + 1, reminders[i].hour, reminders[i].minute);

      // Trigger the reminder if the time matches
      if (now.hour() == reminders[i].hour && now.minute() == reminders[i].minute && !reminderTriggered[i]) {
        tone(BUZZER, 1000);
        delay(3000);
        noTone(BUZZER);
        reminderTriggered[i] = true;
      }
    }

    display.display();

    // Press BTN_SET again to add more reminders
    if (digitalRead(BTN_SET) == LOW) {
      settingTime = true;
      tempHour = 0;  // Reset temp time for next setting
      tempMinute = 0;
      delay(300);  // Debouncing
    }
  }
  delay(200);  // Delay between loop iterations
}

void displayMessage(String msg) {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.println(msg);
  display.display();
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

### OLED Display to ESP32
| OLED Display Pin | Connect to ESP32 Pin          |
| ---------------- | ----------------------------- |
| **VCC**          | **3.3V**                      |
| **GND**          | **GND**                       |
| **SCL**          | **GPIO 22** (Default I2C SCL) |
| **SDA**          | **GPIO 21** (Default I2C SDA) |

### Push Button Connections

| **Button Function** | **Pin Number** | 
| ------------------- | -------------- | 
| Button 1 (Hour+)    | D14            | 
| Button 2 (Min+)     | D27            | 
| Button 3 (Set)      | D33            | 
| Button 4 (Reset)    | D32            | 

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
