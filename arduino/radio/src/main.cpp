#include <Arduino.h>
#include "GyverTM1637.h"
#include <Wire.h>
#include <radio.h>
#include <TEA5767.h>

#define MIN_POTENTIOMETER_VALUE 0
#define MAX_POTENTIOMETER_VALUE 1023

#define FREQUENCY_IN_PIN A7

#define FREQUENCY_OUT_CLK_PIN 2
#define FREQUENCY_OUT_DIO_PIN 3

TEA5767 radio;
GyverTM1637 frequencyDisplay(FREQUENCY_OUT_CLK_PIN, FREQUENCY_OUT_DIO_PIN);

int minFrequency;
int maxFrequency;
int frequencyStep;
int currentFrequency;

void setup()
{
  Serial.begin(9600);
  delay(200);

  radio.init();
  //radio.debugEnable();
  radio.setBand(RADIO_BAND_FM);
  radio.setVolume(2);
  //radio.setMono(false);

  minFrequency = radio.getMinFrequency();
  maxFrequency = radio.getMaxFrequency();
  frequencyStep = radio.getFrequencyStep();

  frequencyDisplay.clear();
  frequencyDisplay.brightness(7);
}

void loop()
{
  long frequencyPotentiometerValue = 0;
  for (int i = 0; i < 64; i++)
  {
    frequencyPotentiometerValue += analogRead(FREQUENCY_IN_PIN);
  }
  frequencyPotentiometerValue = frequencyPotentiometerValue / 64;

  int newFrequency = map(frequencyPotentiometerValue, MIN_POTENTIOMETER_VALUE, MAX_POTENTIOMETER_VALUE, minFrequency, maxFrequency);

  if (abs(newFrequency - currentFrequency) >= frequencyStep)
  {
    radio.setFrequency(newFrequency);
    currentFrequency = newFrequency;
    frequencyDisplay.displayInt(currentFrequency);
    Serial.println(newFrequency);
  }

  /*
   char s[12];
   radio.formatFrequency(s, sizeof(s));
   Serial.print("Station:");
   Serial.println(s);

   Serial.print("Radio:");
   radio.debugRadioInfo();

   Serial.print("Audio:");
   radio.debugAudioInfo();

*/

  //delay(3000);
}