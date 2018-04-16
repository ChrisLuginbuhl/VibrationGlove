/* Basic SPL meter to use a microphone to
    read vibrations from a haptic motor
    Chris Luginbuhl
    Apr 14 2018
*/


#include <Wire.h>
#include "Adafruit_DRV2605.h"
#define sensorPin A1
#define ledPin 13
#define noise
#define NUM_SAMPLES 20

Adafruit_DRV2605 drv;
int baseline = 0;

void setup() {

  Serial.begin(9600);
  Serial.println("DRV test");
  drv.begin();
  drv.useLRA();
  drv.selectLibrary(1);
  // I2C trigger by sending 'go' command
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG);
  pinMode(ledPin, OUTPUT);

  //calibrate microphone by getting average reading
  int calibrationAccumValue = 0;
  // read the value from the sensor:
  for (int i = 0; i < 50; i++) {
    calibrationAccumValue += analogRead(sensorPin);
  }
  baseline = calibrationAccumValue / 50;
}

uint8_t effect = 1;
long lastMillis = 0;

void loop() {
  int reading = 0;
  int sensorValue = 0;
  long accumValue = 0;

  //  Serial.print("Effect #"); Serial.println(effect);
  // set the effect to play
 
  drv.setWaveform(0, 84);  // ramp up medium 1, see datasheet part 11.2
  drv.setWaveform(1, 1);  // strong click 100%, see datasheet part 11.2
  drv.setWaveform(2, 10); //double click
  drv.setWaveform(3, 27); //short double click strong
  
  drv.setWaveform(4, 0);  // end of waveforms

//  drv.setWaveform(0, effect);  // play effect
//  drv.setWaveform(1, 0);       // end waveform
  // play the effect!
  drv.go();
  effect++;
  if (effect > 117) effect = 1;

  // read the value from the sensor:
  while ((millis() - lastMillis) < 500) {
    for (int i = 0; i < NUM_SAMPLES; i++) {
      reading = analogRead(sensorPin);
      accumValue += abs(reading - baseline); //removing DC offset 
//      Serial.println(abs(reading - baseline));
    }
    sensorValue = accumValue / NUM_SAMPLES;
    Serial.println(sensorValue);
    accumValue = 0;
  }
  lastMillis = millis();
}
