// I2Cdev library collection - GY801 I2C device class
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

GY801::GY801(){
  
}

void GY801::initialize(){
  Wire.begin();
  accel.initialize();
  gyro.initialize();
  baro.initialize();
  mag.initialize();
  testConnection();
  gyro.setFullScale(2000);
}
  
bool GY801::testConnection(){
  
  bool accelTest = accel.testConnection();
  Serial.println(accelTest ? "Accelerometer connected." : "ERROR: Accelerometer not connected.");
  
  bool gyroTest = gyro.testConnection();
  Serial.println(gyroTest ? "Gyroscope connected." : "ERROR: Gyroscope not connected.");

  bool baroTest = baro.testConnection();
  Serial.println(baroTest ? "Barometer connected." : "ERROR: Barometer not connected.");
  
  bool magnetoTest = mag.testConnection();
  Serial.println(magnetoTest ? "Magnetometer connected." : "ERROR: Magnetometer not connected.");

  connected = accelTest && gyroTest && baroTest && magnetoTest;
  return connected;
}

bool GY801::isConnected(){
  return connected;
}

IMU GY801::getData(){
  COORDS a = getAccelerometerData();
  COORDS g = getGyroscopeData();
  TPA b = getBarometerData();
  COORDS m = getMagnetometerData();
  return IMU {a, g, b, m};
}

COORDS GY801::getAccelerometerData(){
  int16_t ax, ay, az;
  accel.getAcceleration(&ax, &ay, &az);
  return COORDS {ax, ay, az};
}

COORDS GY801::getGyroscopeData(){
  int16_t avx, avy, avz;
  gyro.getAngularVelocity(&avx, &avy, &avz);
  return COORDS {avx, avy, avz};
}

TPA GY801::getBarometerData(){
  float temperature;
  float pressure;
  float altitude;
  int32_t lastMicros;

  // request temperature
  baro.setControl(BMP085_MODE_TEMPERATURE);
  
  // wait appropriate time for conversion (4.5ms delay)
  lastMicros = micros();
  while (micros() - lastMicros < baro.getMeasureDelayMicroseconds());

  // read calibrated temperature value in degrees Celsius
  temperature = baro.getTemperatureC();

  // request pressure (3x oversampling mode, high detail, 23.5ms delay)
  baro.setControl(BMP085_MODE_PRESSURE_3);
  while (micros() - lastMicros < baro.getMeasureDelayMicroseconds());

  // read calibrated pressure value in Pascals (Pa)
  pressure = baro.getPressure();

  // calculate absolute altitude in meters based on known pressure
  // (may pass a second "sea level pressure" parameter here,
  // otherwise uses the standard value of 101325 Pa)
  altitude = baro.getAltitude(pressure);
  
  return TPA {temperature, pressure, altitude};
}

COORDS GY801::getMagnetometerData(){
  int16_t mx, my, mz;
  float heading;
  
  // read raw heading measurements from device
  mag.getHeading(&mx, &my, &mz);

  // To calculate heading in degrees. 0 degree indicates North
  heading = atan2(my, mx);
  if(heading < 0)
    heading += 2 * M_PI;
  heading = heading * 180/M_PI;
  
  return COORDS {mx, my, mz, heading};
}