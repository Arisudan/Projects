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
