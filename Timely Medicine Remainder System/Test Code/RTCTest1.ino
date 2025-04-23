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
