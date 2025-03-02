#include <Arduino.h>
#include <Adafruit_ICM20948.h>
#include <MadgwickAHRS.h>

// sets icm and madgwick filter rate in hz
// must be one of: 10, 20, 50, 100 Hz!
// magnetometer can only be set to these
// this definition will not automatically set
// magnetometer filter rate.
#define FILTER_RATE 100
// thus...
#define MAG_RATE AK09916_MAG_DATARATE_100_HZ

Madgwick filter;

Adafruit_ICM20948 icm;

// timestamp for madgwick calculations
unsigned long microsPrevious;

// roll, pitch, heading from madgwick filter
float roll, pitch, heading;

// counts up when delta heading/pitch/roll are all under threshold
int stationaryCount = 0;

boolean deviceIsStationary = false;

float gyroBiasX = 0, gyroBiasY = 0, gyroBiasZ = 0;
// takes a number of gyro readings and averages them. i'm not sure if this really works. regardless i'm not using it for now cuz it seems to be working fine without
void calibrateGyro(int samples = 500) {
  float sumX = 0, sumY = 0, sumZ = 0;

  Serial.println("Calibrating gyro... Keep the sensor still!");
  for (int i = 0; i < samples; i++) {
    sensors_event_t accel, gyro, mag, temp;
    icm.getEvent(&accel, &gyro, &temp, &mag);
    sumX += gyro.gyro.x;
    sumY += gyro.gyro.y;
    sumZ += gyro.gyro.z;
    delay(5);
  }
  gyroBiasX = sumX / samples;
  gyroBiasY = sumY / samples;
  gyroBiasZ = sumZ / samples;
  
  Serial.println("Gyro calibration complete!");
}

int minDialAngle = 14; // min roll value
int maxDialAngle = -90; // max roll value

// returns imu roll value mapped to 0-127
int getDialValue() {
  int dialVal = map(roll, minDialAngle, maxDialAngle, 0, 127);
  if (dialVal < 0) dialVal = 0;
  else if (dialVal >127) dialVal = 127;
  return dialVal;
}

// initializes imu, sets sensor rates to match madgwick filter
void setupIMU() {
  // Try to initialize!
  if (!icm.begin_I2C()) {
    Serial.println("Failed to find ICM20948 chip");
    while (1) {
      delay(10);
      Serial.println("help!");
    }
  }
  Serial.println("ICM20948 Found!");
  microsPrevious = micros();
  //calibrateGyro();

  // SET ACCEL RATE BASED ON FILTER RATE
  uint16_t new_accel_divisor = (1125 / FILTER_RATE) - 1;
  icm.setAccelRateDivisor(new_accel_divisor);

  // SET GYRO RATE BASED ON FILTER RATE
  uint8_t new_gyro_divisor = (1100 / FILTER_RATE) - 1;
  icm.setGyroRateDivisor(new_gyro_divisor);

  // SET MAGNETOMETER RATE
  icm.setMagDataRate(MAG_RATE);

  // TODO: play with built in sensor filters?
  //icm.enableAccelDLPF(enable, 1.3);

  // init madgwick filter
  filter.begin(FILTER_RATE);
}

// prints ranges and rates for imu to serial
void printIMUSettings() {
  // RANGES

  // ACCEL RANGE
  // icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
  Serial.print("Accelerometer range set to: ");
  switch (icm.getAccelRange()) {
  case ICM20948_ACCEL_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case ICM20948_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case ICM20948_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case ICM20948_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }

  // GYRO RANGE
  icm.setGyroRange(ICM20948_GYRO_RANGE_2000_DPS);
  Serial.print("Gyro range set to: ");
  switch (icm.getGyroRange()) {
  case ICM20948_GYRO_RANGE_250_DPS:
    Serial.println("250 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  }

  // RATES

  // ACCEL RATE
  uint16_t accel_divisor = icm.getAccelRateDivisor();
  float accel_rate = 1125 / (1.0 + accel_divisor);

  Serial.print("Accelerometer data rate divisor set to: ");
  Serial.println(accel_divisor);
  Serial.print("Accelerometer data rate (Hz) is approximately: ");
  Serial.println(accel_rate);

  // GYRO RATE
  uint8_t gyro_divisor = icm.getGyroRateDivisor();
  float gyro_rate = 1100 / (1.0 + gyro_divisor);

  Serial.print("Gyro data rate divisor set to: ");
  Serial.println(gyro_divisor);
  Serial.print("Gyro data rate (Hz) is approximately: ");
  Serial.println(gyro_rate);

  // MAGNETOMETER RATE
  Serial.print("Magnetometer data rate set to: ");
  switch (icm.getMagDataRate()) {
  case AK09916_MAG_DATARATE_SHUTDOWN:
    Serial.println("Shutdown");
    break;
  case AK09916_MAG_DATARATE_SINGLE:
    Serial.println("Single/One shot");
    break;
  case AK09916_MAG_DATARATE_10_HZ:
    Serial.println("10 Hz");
    break;
  case AK09916_MAG_DATARATE_20_HZ:
    Serial.println("20 Hz");
    break;
  case AK09916_MAG_DATARATE_50_HZ:
    Serial.println("50 Hz");
    break;
  case AK09916_MAG_DATARATE_100_HZ:
    Serial.println("100 Hz");
    break;
  }
  Serial.println();
}

// this is ugly and isn't clocked to filter rate. don't use!
void printIMUReading() {
  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  icm.getEvent(&accel, &gyro, &temp, &mag);

  Serial.print("\t\tTemperature ");
  Serial.print(temp.temperature);
  Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tAccel X: ");
  Serial.print(accel.acceleration.x);
  Serial.print(" \tY: ");
  Serial.print(accel.acceleration.y);
  Serial.print(" \tZ: ");
  Serial.print(accel.acceleration.z);
  Serial.println(" m/s^2 ");

  Serial.print("\t\tMag X: ");
  Serial.print(mag.magnetic.x);
  Serial.print(" \tY: ");
  Serial.print(mag.magnetic.y);
  Serial.print(" \tZ: ");
  Serial.print(mag.magnetic.z);
  Serial.println(" uT");

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tGyro X: ");
  Serial.print(gyro.gyro.x);
  Serial.print(" \tY: ");
  Serial.print(gyro.gyro.y);
  Serial.print(" \tZ: ");
  Serial.print(gyro.gyro.z);
  Serial.println(" radians/s ");
  Serial.println();
}

//>varName:1234\n
void printMadgwick() {
  Serial.println(">heading:" + String(heading));
  Serial.println(">pitch:" + String(pitch));
  Serial.println(">roll:" + String(roll));
}

#define MICROS_PER_READING 1000000 / FILTER_RATE // number of microseconds to wait for clocking madgwick calcs to reading rate
#define SENSOR_DELTA_STATIONARY_THRESHOLD 0.03 // change in sensor values smaller than this number will trigger sleep mode
#define SENSOR_STATIONARY_TIMEOUT_SECS 10 // in seconds; time to wait before declaring movement timeout
#define SENSOR_STATIONARY_TIMEOUT_CYCLES SENSOR_STATIONARY_TIMEOUT_SECS * FILTER_RATE // in cycles; number to wait before declaring movement timeout
// updates pitch, yaw, roll variables
void updateFilter() {
  unsigned long microsNow = micros();

  if (microsNow - microsPrevious >= MICROS_PER_READING) {
    //  /* Get a new normalized sensor event */
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t mag;
    sensors_event_t temp;
    icm.getEvent(&accel, &gyro, &temp, &mag);

    // convert radians from gyro to degrees
    float gx, gy, gz;
    gx = (gyro.gyro.x - gyroBiasX) * 180 / PI;
    gy = (gyro.gyro.y - gyroBiasY) * 180 / PI;
    gz = (gyro.gyro.z - gyroBiasZ) * 180 / PI;

    // update madgwick
    filter.update(
        gx, gy, gz,
        accel.acceleration.x,
        accel.acceleration.y,
        accel.acceleration.z,
        mag.magnetic.x,
        mag.magnetic.y,
        mag.magnetic.z
      );

    //get new heading, pitch, roll
    float newHeading, newPitch, newRoll;
    newHeading = filter.getYaw();
    newPitch = filter.getPitch();
    newRoll = filter.getRoll();

    // get deltas
    float dH, dP, dR;
    dH = abs(newHeading - heading);
    dP = abs(newPitch - pitch);
    dR = abs(newRoll - roll);

    // TODO: remove print
    Serial.println(">deltaHeading:" + String(dH));
    Serial.println(">deltaPitch:" + String(dP));
    Serial.println(">deltaRoll:" + String(dR));

    // check if deltas are all under stationary threshold
    if (
        dH < SENSOR_DELTA_STATIONARY_THRESHOLD &&
        dP < SENSOR_DELTA_STATIONARY_THRESHOLD &&
        dR < SENSOR_DELTA_STATIONARY_THRESHOLD
      ) {
      // count how many cycles deltas remain under threshold
      if (stationaryCount < SENSOR_STATIONARY_TIMEOUT_CYCLES) stationaryCount++;
      // if timeout limit reached, flip flag and stop counting
      else if (stationaryCount == SENSOR_STATIONARY_TIMEOUT_CYCLES) {
        deviceIsStationary = true;
      }
    } 
    // once threshold exceeded once more
    else {
      stationaryCount = 0;
      deviceIsStationary = false;
    }

    // set heading, pitch and roll
    heading = newHeading;
    pitch = newPitch;
    roll = newRoll;

    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + MICROS_PER_READING;
  }
}

