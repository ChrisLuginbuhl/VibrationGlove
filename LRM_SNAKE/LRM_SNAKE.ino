/* Drives LRA haptic motor.
 *  For use with rattlesnake 
    Apr 14 2018
*/

#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;

void setup() {
  Serial.begin(9600);
  Serial.println("DRV test");
  drv.begin();
  drv.useLRA();  
  // I2C trigger by sending 'go' command 
  drv.setMode(DRV2605_MODE_INTTRIG); // default, internal trigger when sending GO command

  drv.selectLibrary(1);
  drv.setWaveform(0, 10);  // ramp up medium 1, see datasheet part 11.2
//  drv.setWaveform(1, 1);  // strong click 100%, see datasheet part 11.2
  drv.setWaveform(1, 0);
//  drv.setWaveform(3, 0);  // end of waveforms
}

void loop() {
    drv.go();
    delay(1000);
}
