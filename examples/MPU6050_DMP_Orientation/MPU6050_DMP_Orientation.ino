/************************************************************
MPU6050_DMP_Orientation
Orientation example for MPU-6050 DMP Arduino Library 
Tomoaki Tanaka<ganymean@gmail.com> 

This libraly is ported version from SparkFun MPU-9250 DMP Arduino Library.
https://github.com/sparkfun/SparkFun_MPU-9250-DMP_Arduino_Library

original creation date: January 5, 2018
https://github.com/ganymean/Dragino-LoraMiniDev-MPU6050_DMP_Arduino_Library

Uses the MPU-6050's digital motion processing engine to
determine orientation of the board.

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

const signed char orientationMatrix[9] = {
  1, 0, 0,
  0, 1, 0,
  0, 0, 1
};
unsigned char lastOrient = 0;

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
  
  imu.dmpBegin(DMP_FEATURE_ANDROID_ORIENT);
  imu.dmpSetOrientation(orientationMatrix);
}

void loop() 
{
  if ( imu.fifoAvailable() )
  {
    imu.dmpUpdateFifo();
    unsigned char orient = imu.dmpGetOrientation();
    if (orient != lastOrient)
    {
      switch (orient)
      {
      case ORIENT_PORTRAIT:
        SerialPort.println("Portrait");
        break;
      case ORIENT_LANDSCAPE:
        SerialPort.println("Landscape");
        break;
      case ORIENT_REVERSE_PORTRAIT:
        SerialPort.println("Portrait (Reverse)");
        break;
      case ORIENT_REVERSE_LANDSCAPE:
        SerialPort.println("Landscape (Reverse)");
        break;
      }
      lastOrient = orient;
    }
  }
}

