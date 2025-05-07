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
