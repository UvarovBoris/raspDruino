#include "GyverTM1637.h"
#include "microDS3231.h"

#define HT_IN_DATA_PIN 12
#define HT_DATA_SIZE 5
//DHT11 low time is 24
#define LOW_MIN_TIME 9
#define LOW_MAX_TIME 39
//DHT11 high time is 70
#define HIGH_MIN_TIME 55
#define HIGH_MAX_TIME 85

#define HT_OUT_CLK_PIN 5
#define HT_OUT_DIO_PIN 6

#define CLOCK_OUT_CLK_PIN 3
#define CLOCK_OUT_DIO_PIN 4

unsigned char htData[HT_DATA_SIZE];
GyverTM1637 htDisplay(HT_OUT_CLK_PIN, HT_OUT_DIO_PIN);
long lastHtCheckMS = 0;

MicroDS3231 rtc;
GyverTM1637 clockDisplay(CLOCK_OUT_CLK_PIN, CLOCK_OUT_DIO_PIN);
long lastClockCheckMS = 0;
bool clockPoint = true;

void setup() {
  Serial.begin(9600);

  htDisplay.clear();
  htDisplay.brightness(7);

  //if (rtc.lostPower()) {
  //rtc.setTime(COMPILE_TIME);
  rtc.setTime(0, 4, 10, 26, 06, 2021);
  //}

  clockDisplay.clear();
  clockDisplay.brightness(7);
}

void loop() {
  if (lastHtCheckMS == 0 || millis() - lastHtCheckMS > 5000) {
    pinMode(HT_IN_DATA_PIN, OUTPUT);
    digitalWrite(HT_IN_DATA_PIN, HIGH);
    digitalWrite(HT_IN_DATA_PIN, LOW);

    delay(20);

    pinMode(HT_IN_DATA_PIN, INPUT);

    while (digitalRead(HT_IN_DATA_PIN));
    while (!digitalRead(HT_IN_DATA_PIN));
    while (digitalRead(HT_IN_DATA_PIN));

    int status = readHTData(HT_IN_DATA_PIN, htData, HT_DATA_SIZE);
    if (status == 0) {
      //Check data is correct by checksum
      if (htData[0] + htData[1] + htData[2] + htData[3] == htData[4]) {
        showHTData(htData[2], htData[0]);
      } else {
        Serial.println("Error");
      }
    } else {
      Serial.println("Error");
    }
    lastHtCheckMS = millis();
  }

  if (lastClockCheckMS == 0 || millis() - lastClockCheckMS > 500) {
    clockDisplay.displayClock(rtc.getHours(), rtc.getMinutes());
    clockDisplay.point(clockPoint);
    clockPoint = !clockPoint;
    lastClockCheckMS = millis();
  }
}

int readHTData(int dataPin, unsigned char* data, int dataSize) {
  for (int byteIndex = 0; byteIndex < dataSize; byteIndex++) {
    for (int bitIndex = 7; bitIndex >= 0; bitIndex--) {
      while (!digitalRead(dataPin));

      int highTime = 0;
      int highStart = micros();
      while (digitalRead(dataPin)) {
        highTime = micros() - highStart;
        if (highTime > HIGH_MAX_TIME) return 1;
      }

      if (highTime >= LOW_MIN_TIME && highTime <= LOW_MAX_TIME) {
        htData[byteIndex] &= ~(1 << bitIndex);
      } else if (highTime >= HIGH_MIN_TIME && highTime <= HIGH_MAX_TIME) {
        htData[byteIndex] |= 1 << bitIndex;
      }
    }
  }
  return 0;
}

void showHTData(int t, int h) {
  for (int i = 1; i >= 0; i--) {
    htDisplay.display(i, t % 10);
    t /= 10;
  }
  for (int i = 3; i >= 2; i--) {
    htDisplay.display(i, h % 10);
    h /= 10;
  }
  htDisplay.point(true);
}
