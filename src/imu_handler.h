/**
 * @file imu_handler.h
 * @brief handler for ICM20948 imu (inertial measurement unit)
 *
 * provides functionality for initializing, configuring,
 * and reading data from ICM20948 imu sensor. includes functions
 * for calibration, value reading, and processing sensor data through
 * madgwick filter for orientation estimation.
 *
 * @note uses Adafruit_ICM20948 library
 *
 * @author oddhorse (John Trinh)
 */
#ifndef IMU_HANDLER_H
#define IMU_HANDLER_H

#include <Arduino.h>
#include <Adafruit_LIS3DH.h>

extern Adafruit_LIS3DH lis;

extern boolean noteTriggered;
extern int IMUVelocityOut;

void setupIMU();
void updatePeakDetection();

#endif // IMU_HANDLER_H