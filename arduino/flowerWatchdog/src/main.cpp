#include <Arduino.h>

#define HYGROMETER_DATA_PIN 7
#define WARNING_LED_PIN 3
#define DRY_THRESHOLD 150

void setup()
{
  Serial.begin(9600);

  pinMode(WARNING_LED_PIN, OUTPUT);
  digitalWrite(WARNING_LED_PIN, LOW);
}

void loop()
{
  int hygrometerData = analogRead(HYGROMETER_DATA_PIN);
  if (hygrometerData >= DRY_THRESHOLD)
  {
    digitalWrite(WARNING_LED_PIN, HIGH);
  } else {
    digitalWrite(WARNING_LED_PIN, LOW);
  }
  delay(1000);
}