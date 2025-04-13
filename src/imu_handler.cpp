/**
 * @file imu_handler.cpp
 * @brief handles IMU operations for horse controller project
 *
 * implements functionality for ICM20948 IMU sensor with Madgwick filter
 * for orientation estimation. provides methods for initialization,
 * calibration, motion detection, and calculating orientation angles.
 *
 * uses sensor data to provide dial control based on device
 * orientation, mapping roll angle to 0-127 range.
 *
 * @note filter rate set to 100Hz with matching magnetometer config
 *
 * @author oddhorse (John Trinh)
 */

#include "imu_handler.h"
#include <Arduino.h>
#include <Adafruit_LIS3DH.h>
#include "util.h"
#include <Adafruit_Sensor.h>
#include "midi_handler.h"

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

// timestamp for madgwick calculations
unsigned long microsPrevious;

// flips to true when accelerometer peaks. must be flipped off by other means!
boolean noteTriggered = false;
int IMUVelocityOut = 0;

int minDialAngle = 14;	// min roll value
int maxDialAngle = -90; // max roll value

// get deltas
sensors_event_t oldAccel;
bool isFirstReading = true;
unsigned long punchTime = millis();
int highest = 0;
bool peakDetected = false;
int accum = 0;

int getYAccelVal()
{
	sensors_event_t accel;
	lis.getEvent(&accel);

	int y = accel.acceleration.y;
	y = map(y, -20, 20, 0, 127);
	return y;
}

void setupIMU()
{
	if (!lis.begin(0x18))
	{ // change this to 0x19 for alternative i2c address
		Serial.println("Couldnt start");
		while (1)
			yield();
	}
	Serial.println("LIS3DH found!");

	// lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

	Serial.print("Range = ");
	Serial.print(2 << lis.getRange());
	Serial.println("G");
}

#define PUNCH_TIMEOUT 20
// monitors accelerometer peaking
void updatePeakDetection()
{
	//  /* Get a new normalized sensor event */

	sensors_event_t accel;
	lis.getEvent(&accel);

	if (isFirstReading)
	{
		oldAccel = accel;
		isFirstReading = false;
	}

	// if no buttons are pressed, need a way to flip noteTriggered back
	if (noteTriggered && millis() - punchTime > PUNCH_TIMEOUT)
	{
		noteTriggered = false;
	}

	// get deltas
	float dX, dY, dZ;
	dX = accel.acceleration.x - oldAccel.acceleration.x;
	dY = accel.acceleration.y - oldAccel.acceleration.y;
	dZ = accel.acceleration.z - oldAccel.acceleration.z;

	Util::teleplot("accel_x", accel.acceleration.x);
	Util::teleplot("accel_y", accel.acceleration.y);
	Util::teleplot("accel_z", accel.acceleration.z);
	Util::teleplot("delta_accel_x", dX);
	Util::teleplot("delta_accel_y", dY);
	Util::teleplot("delta_accel_z", dZ);

	if (dZ > 8)
	{
		if (!peakDetected && millis() - punchTime > 41)
		{
			peakDetected = true;
			highest = dZ;
			accum = 0;
		}
	}

	if (peakDetected)
	{
		accum += dZ; // accumulates change in accel while change is positive; final value is full acceleration change from beginning of peak
		if (dZ > highest)
		{
			highest = dZ;
		}
	}

	if (dZ <= 0 && peakDetected)
	{
		int timeBetween = millis() - punchTime;
		Serial.print("time since last note: ");
		Serial.println(timeBetween);
		// TODO: choose between highest change in acceleration or accumulated change for determining velocity!
		Serial.print("highest change in acceleration: ");
		Serial.println(highest);
		Util::teleplot("highest_change", highest);
		Serial.print("accumulated change in acceleration: ");
		Serial.println(accum);
		Util::teleplot("accum", accum);
		noteTriggered = true;
		peakDetected = false;
		IMUVelocityOut = round(map(accum, 8, 112, 1, 127));

		punchTime = millis();
	}

	oldAccel = accel;
}