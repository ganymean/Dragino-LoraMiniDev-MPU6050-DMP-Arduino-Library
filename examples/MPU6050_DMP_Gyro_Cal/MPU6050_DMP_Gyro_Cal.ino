/************************************************************
MPU6050_DMP_Gyro_Cal
Gyro calibration example for MPU-6050 DMP Arduino Library 
Tomoaki Tanaka<ganymean@gmail.com> 

This libraly is ported version from SparkFun MPU-9250 DMP Arduino Library.
https://github.com/sparkfun/SparkFun_MPU-9250-DMP_Arduino_Library

original creation date: January 5, 2018
https://github.com/ganymean/Dragino-LoraMiniDev-MPU6050_DMP_Arduino_Library

This example sketch demonstrates how to use the MPU-6050's
digital motion processor (DMP) to calibrate the gyroscope.
After eight seconds of no motion, the DMP will compute
gyro biases and subtract them.

Development environment specifics:
Arduino IDE 1.6.12
GY-521 Breakout board

Supported Platforms:
- Dragino LoRa mini DEV
*************************************************************/
#include <MPU6050-DMP.h>

#define SerialPort SerialUSB

MPU6050_DMP imu;

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

  imu.setSensors(INV_XYZ_GYRO); // Enable gyroscope only
  imu.setGyroFSR(2000); // Set gyro to 2000 dps

  imu.dmpBegin(DMP_FEATURE_GYRO_CAL |   // Enable gyro cal
              DMP_FEATURE_SEND_CAL_GYRO,// Send cal'd gyro values
              10);                   // Set DMP rate to 10 Hz
}

void loop() 
{
  // Check for new data in the FIFO
  if ( imu.fifoAvailable() )
  {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if ( imu.dmpUpdateFifo() == INV_SUCCESS)
    {
      printIMUData();
    }
  }
}

void printIMUData(void)
{  
  // After calling dmpUpdateFifo() the ax, gx, mx, etc. values
  // are all updated.
  float gyroX = imu.calcGyro(imu.gx);
  float gyroY = imu.calcGyro(imu.gy);
  float gyroZ = imu.calcGyro(imu.gz);
  
  SerialPort.println("Gyro: " + String(gyroX) + ", " +
              String(gyroY) + ", " + String(gyroZ) + " dps");
  SerialPort.println("Time: " + String(imu.time) + " ms");
  SerialPort.println();
}

