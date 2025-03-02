#ifndef IMU_HANDLER_H
#define IMU_HANDLER_H

#include <Arduino.h>
#include <Adafruit_ICM20948.h>

extern Adafruit_ICM20948 icm;

extern boolean deviceIsStationary;

void setupIMU();
void printIMUSettings();
void printIMUReading();
void printMadgwick();
void updateFilter();
void calibrateGyro(int samples = 500);
int getDialValue();

#endif // IMU_HANDLER_H