#include <Arduino.h>
#include "GyverPower.h"

#define HYGROMETER_POWER_PIN 2
#define HYGROMETER_DATA_PIN 7
#define HYGROMETER_POWER_DURATION 200

#define WARNING_LED_PIN 3
#define WARNING_LED_DURATION 300

#define DRY_THRESHOLD 800

#define WET_SLEEP 10000
#define DRY_SLEEP 3000

void setup()
{
  Serial.begin(9600);

  pinMode(HYGROMETER_POWER_PIN, OUTPUT);
  digitalWrite(WARNING_LED_PIN, LOW);

  pinMode(WARNING_LED_PIN, OUTPUT);
  digitalWrite(WARNING_LED_PIN, LOW);
}

void loop()
{
  digitalWrite(HYGROMETER_POWER_PIN, HIGH);
  delay(HYGROMETER_POWER_DURATION);
  int hygrometerData = analogRead(HYGROMETER_DATA_PIN);
  digitalWrite(HYGROMETER_POWER_PIN, LOW);

  if (hygrometerData >= DRY_THRESHOLD)
  {
    digitalWrite(WARNING_LED_PIN, HIGH);
    delay(WARNING_LED_DURATION);
    digitalWrite(WARNING_LED_PIN, LOW);
    power.sleepDelay(DRY_SLEEP);
  }
  else
  {
    power.sleepDelay(WET_SLEEP);
  }
}