/* Basic SPL meter to use a microphone to
    read mechanical vibrations using a line level microphone
    Chris Luginbuhl
    Apr 14 2018
*/

#include <Adafruit_DotStar.h>


#define sensorPin A0
#define ledPin 13
#define noise
#define NUM_SAMPLES 1

int baseline = 0;
Adafruit_DotStar strip = Adafruit_DotStar(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

void setup() {

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  strip.begin();

  int calibrationAccumValue = 0;
  // read the value from the sensor:
  for (int i = 0; i < 50; i++) {
    calibrationAccumValue += analogRead(sensorPin);
  }
  baseline = calibrationAccumValue / 50;
}

void loop() {
  int reading = 0;
  int sensorValue = 0;
  long accumValue = 0;

  // read the value from the sensor:

  for (int i = 0; i < NUM_SAMPLES; i++) {
    reading = analogRead(sensorPin);
    accumValue += abs(reading - baseline); //removing DC offset
    //      Serial.println(abs(reading - baseline));
  }
  sensorValue = accumValue / NUM_SAMPLES;
  Serial.println(sensorValue);

  switch (sensorValue) {
    case 0 ...5:
      strip.setPixelColor(0, 0, 64, 0);
      break;
    case 6 ...10:
      strip.setPixelColor(0, 48, 48, 0);
      break;
    case 11 ...15:
      strip.setPixelColor(0, 64, 0, 0);
      break;

      strip.setPixelColor(0, 48, 48, 0);
  }
  strip.show();

}
