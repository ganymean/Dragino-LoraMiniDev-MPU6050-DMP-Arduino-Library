/************************************************************
MPU6050_DMP_Pedometer
Pedometer example for MPU-6050 DMP Arduino Library 
Tomoaki Tanaka<ganymean@gmail.com> 

This libraly is ported version from SparkFun MPU-9250 DMP Arduino Library.
https://github.com/sparkfun/SparkFun_MPU-9250-DMP_Arduino_Library

original creation date: January 5, 2018
https://github.com/ganymean/Dragino-LoraMiniDev-MPU6050_DMP_Arduino_Library

The MPU-6050's digital motion processor (DMP) can estimate
steps taken -- effecting a pedometer.

After uploading the code, try shaking the 9DoF up and
down at a "stepping speed."

Development environment specifics:
Arduino IDE 1.6.12
GY-521 Breakout board

Supported Platforms:
- Dragino LoRa mini DEV
*************************************************************/
#include <MPU6050-DMP.h>

#define SerialPort SerialUSB

MPU6050_DMP imu;

unsigned long stepCount = 0;
unsigned long stepTime = 0;
unsigned long lastStepCount = 0;

void setup() 
{
  SerialPort.begin(115200);

  // Call imu.begin() to verify communication and initialize
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      SerialPort.println("Unable to communicate with MPU-6050");
      SerialPort.println("Check connections, and try again.");
      SerialPort.println();
      delay(5000);
    }
  }
  
  imu.dmpBegin(DMP_FEATURE_PEDOMETER);
  imu.dmpSetPedometerSteps(stepCount);
  imu.dmpSetPedometerTime(stepTime);
}

void loop() 
{
  stepCount = imu.dmpGetPedometerSteps();
  stepTime = imu.dmpGetPedometerTime();
  
  if (stepCount != lastStepCount)
  {
    lastStepCount = stepCount;
    SerialPort.print("Walked " + String(stepCount) + 
                     " steps");
    SerialPort.println(" (" + 
              String((float)stepTime / 1000.0) + " s)");
  }
}

