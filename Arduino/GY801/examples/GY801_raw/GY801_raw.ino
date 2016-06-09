// I2C device class (I2Cdev) demonstration Arduino sketch for GY801 class
// 06/09/2016 by Jon Gallant <jon@jongallant.com>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2016-06-09 Initial Release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "GY801.h"

GY801 imu;

#define LED_PIN 0 // (Arduino is 13, Teensy is 6, Huzzah is 0)
bool blinkState = false;

void setup(){
  // configure LED for output
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  
  imu.initialize();
}

void loop(){
  if(imu.isConnected()){
    IMU data = imu.getData();
    Serial.print("Accelerometer:\t");
    Serial.print(data.Accelerometer.x); Serial.print("\t");
    Serial.print(data.Accelerometer.y); Serial.print("\t");
    Serial.println(data.Accelerometer.z);
    
    Serial.print("Gyroscope:\t");
    Serial.print(data.Gyroscope.x); Serial.print("\t");
    Serial.print(data.Gyroscope.y); Serial.print("\t");
    Serial.println(data.Gyroscope.z); 
    
    Serial.print("Barometer:\t");
    Serial.print(data.Barometer.temperature); Serial.print("\t");
    Serial.print(data.Barometer.pressure); Serial.print("\t");
    Serial.println(data.Barometer.altitude);

    Serial.print("Magnetometer:\t");
    Serial.print(data.Magnetometer.x); Serial.print("\t");
    Serial.print(data.Magnetometer.y); Serial.print("\t");
    Serial.print(data.Magnetometer.z); Serial.print("\t");
    Serial.println(data.Magnetometer.heading);

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
  }
}
