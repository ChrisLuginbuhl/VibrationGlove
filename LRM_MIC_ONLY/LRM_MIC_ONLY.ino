/* Basic SPL meter to use a microphone to
    read mechanical vibrations using a line level microphone
    Chris Luginbuhl
    Apr 14 2018

    We are taking the rectified (absolute value) signal after removing the DC offset. Since there is drift and windup on the DC signal, we
    recalibrate baseline every NUM_LOOPS.
*/

#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>


#define sensorPin A0
#define NEOPIXEL_PIN 3
#define NUMPIXELS 1
#define ledPin 13
#define noise 3
#define NUM_SAMPLES 1
#define NUM_LOOPS 800

int baseline = 0;
long baselineAccum = 0;
int loopIndex = 0;
Adafruit_DotStar strip = Adafruit_DotStar(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  strip.begin();  //initialize the dotstar
  pixels.begin(); // This initializes the NeoPixel library.
  

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
  long smoothingSum = 0;

  // read the value from the sensor:

  for (int i = 0; i < NUM_SAMPLES; i++) {
    reading = analogRead(sensorPin);
    baselineAccum += reading;
    smoothingSum += abs(reading - baseline); //removing DC offset. Note docs say don't call another
    //                                             function from within abs() because of how it's implemented
  }
  sensorValue = smoothingSum / NUM_SAMPLES - noise;
  sensorValue = constrain(sensorValue, 0, 1024);

  Serial.println(sensorValue);

//light up neopixel and dotstar LEDs with colours/brightness according to sound.
  switch (sensorValue) {
    case 0 ...4:
       pixels.setPixelColor(0, pixels.Color(0,0,0));
      strip.setPixelColor(0, 0, 64, 0);
      break;
    case 5 ...10:
      strip.setPixelColor(0, 48, 48, 0);
      pixels.setPixelColor(0, pixels.Color(16,16,32));
      break;
    case 11 ...15:
      strip.setPixelColor(0, 64, 0, 0);
      pixels.setPixelColor(0, pixels.Color(32,32,64));
      break;
//else
      pixels.setPixelColor(0, pixels.Color(48,48,96));
      strip.setPixelColor(0, 128, 0, 0);
  }
  strip.show();  // update dotstar LED
  pixels.show(); // This sends the updated neopixel color to the hardware.

  //increment loopIndex, recalculat baseline to accommodate drift & ambient noise

  if (++loopIndex > NUM_LOOPS) {
    loopIndex = 0;
    baseline = baselineAccum / (NUM_LOOPS * NUM_SAMPLES);
    baselineAccum = 0;
  }

}
