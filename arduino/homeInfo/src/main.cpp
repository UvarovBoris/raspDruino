#include <Arduino.h>
#include "DHT.h"
#include "GyverTM1637.h"
#include "microDS3231.h"

#define DHT_IN_DATA_PIN 2
#define DHT_TYPE DHT11

#define HT_OUT_CLK_PIN 11
#define HT_OUT_DIO_PIN 12

#define CLOCK_OUT_CLK_PIN 3
#define CLOCK_OUT_DIO_PIN 4

DHT dht(DHT_IN_DATA_PIN, DHT_TYPE);
GyverTM1637 htDisplay(HT_OUT_CLK_PIN, HT_OUT_DIO_PIN);
long lastHtCheckMS = 0;

MicroDS3231 rtc;
GyverTM1637 clockDisplay(CLOCK_OUT_CLK_PIN, CLOCK_OUT_DIO_PIN);
long lastClockCheckMS = 0;
bool clockPoint = true;

void showHTData(int t, int h)
{
  for (int i = 1; i >= 0; i--)
  {
    htDisplay.display(i, t % 10);
    t /= 10;
  }
  for (int i = 3; i >= 2; i--)
  {
    htDisplay.display(i, h % 10);
    h /= 10;
  }
  htDisplay.point(true);
}

void setup()
{
  Serial.begin(9600);

  dht.begin();

  htDisplay.clear();
  htDisplay.brightness(7);

  if (rtc.lostPower())
  {
    //rtc.setTime(COMPILE_TIME);
    rtc.setTime(0, 7, 15, 28, 06, 2021);
  }

  clockDisplay.clear();
  clockDisplay.brightness(7);
}

void loop()
{
  if (lastHtCheckMS == 0 || millis() - lastHtCheckMS > 5000)
  {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t) && !isnan(h))
    {
      showHTData(t, h);
    }
    lastHtCheckMS = millis();
  }

  if (lastClockCheckMS == 0 || millis() - lastClockCheckMS > 500)
  {
    clockDisplay.displayClock(rtc.getHours(), rtc.getMinutes());
    clockDisplay.point(clockPoint);
    clockPoint = !clockPoint;
    lastClockCheckMS = millis();
  }
}
