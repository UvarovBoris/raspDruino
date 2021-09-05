#include <Arduino.h>
#include "GyverTM1637.h"

#define HYGROMETER_DATA_PIN A7
#define HYGROMETER_OUT_CLK_PIN 11
#define HYGROMETER_OUT_DIO_PIN 12
#define HYGROMETER_READS_AMOUNT 512
#define HYGROMETER_ANALOG_MIN 220
#define HYGROMETER_ANALOG_MAX 490
#define HYGROMETER_RATIO_MIN 0
#define HYGROMETER_RATIO_MAX 99

GyverTM1637 hygrometerDisplay(HYGROMETER_OUT_CLK_PIN, HYGROMETER_OUT_DIO_PIN);

void setup()
{
  Serial.begin(9600);

  hygrometerDisplay.clear();
  hygrometerDisplay.brightness(7);
}

void loop()
{
  long hygrometerData = analogRead(HYGROMETER_DATA_PIN);
  for (int i = 0; i < HYGROMETER_READS_AMOUNT; i++)
  {
    hygrometerData += analogRead(HYGROMETER_DATA_PIN);
  }
  hygrometerData = hygrometerData / HYGROMETER_READS_AMOUNT;

  int hygrometerRatio = HYGROMETER_RATIO_MAX - map(hygrometerData, HYGROMETER_ANALOG_MIN, HYGROMETER_ANALOG_MAX, HYGROMETER_RATIO_MIN, HYGROMETER_RATIO_MAX);
  
  hygrometerDisplay.displayInt(hygrometerRatio);
  
  delay(1000);
}