#include <Arduino.h>
#include <Wire.h>
#include <radio.h>
#include <TEA5767.h>

#define MIN_POTENTIOMETER_VALUE 0
#define MAX_POTENTIOMETER_VALUE 1023
#define POTENTIOMETER_READS_AMOUNT 512

#define FREQUENCY_IN_PIN A7

#define FREQUENCY_OUT_CLK_PIN 2
#define FREQUENCY_OUT_DIO_PIN 3

TEA5767 radio;

int minFrequency;
int maxFrequency;
int frequencyStep;
int currentFrequency;

void setup()
{
  Serial.begin(9600);
  delay(200);

  radio.init();
  radio.setBand(RADIO_BAND_FM);

  minFrequency = radio.getMinFrequency();
  maxFrequency = radio.getMaxFrequency();
  frequencyStep = 2; //radio.getFrequencyStep();
}

void loop()
{
  long frequencyPotentiometerValue = 0;
  for (int i = 0; i < POTENTIOMETER_READS_AMOUNT; i++)
  {
    frequencyPotentiometerValue += analogRead(FREQUENCY_IN_PIN);
  }
  frequencyPotentiometerValue = frequencyPotentiometerValue / POTENTIOMETER_READS_AMOUNT;

  int newFrequency = map(frequencyPotentiometerValue, MIN_POTENTIOMETER_VALUE, MAX_POTENTIOMETER_VALUE, minFrequency, maxFrequency);

  if (abs(newFrequency - currentFrequency) >= frequencyStep)
  {
    radio.setFrequency(newFrequency);
    currentFrequency = newFrequency;
    Serial.println(newFrequency);
  }
}