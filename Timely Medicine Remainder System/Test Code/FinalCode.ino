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
