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
