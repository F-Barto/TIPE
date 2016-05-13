#include "imu.h"

//constructor
imu::imu() {}

//register map:https://store.invensense.com/Datasheets/invensense/RM-MPU-9150A-00-v3.0.pdf


/*
 *  Description: initialiaze the imu according to the register map
 *  indications
 *  --------------------------------------------------------------
 *  Returns: true if success else false
 */
void imu::init() {
    //Note: The device will come up in sleep mode upon power up. (page 9 of 52)
    I2C::writeBits(IMU_ADDRESS, 0x6B, 1, 6, 0x00); // "waking up" the device (page 38-39 of 52)

    I2C::writeBits(IMU_ADDRESS, 0x6B, 2, 3, 0x01); // setting clock source according to page 38 of 52

    /* A good rule to remember: as the sensitivity (resolution) increases, the range decreases. *\
    \* Consequently the range is set at the minimum for the gyro and the accelerometer          */
    I2C::writeBits(IMU_ADDRESS, 0x1B, 2, 4, 0x00); // setting the rate at 250°/s (page 12 of 52)
    I2C::writeBits(IMU_ADDRESS, 0x1C, 2, 4, 0x00); // setting the rate at ± 2g (page of 52)

    /*
        implémentation future pour le magnétomètre
        voir (7.5 Block Diagram) du data sheet
    */
}


/*
*  Description: getting accelerometer and gyro data in one time
*  --------------------------------------------------------------
*  ax, ay, az: latest measurements of the accel X, Y, Z axis
*  gx, gy, gz: latest measurements of the gyro X, Y, Z axis
*  --------------------------------------------------------------
*  notes: see page 28 to 30 of 52; buffer[6] & buffer[7] are ignored
*         as they stand for the temperature measurements
*/
void imu::getAccGyr(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz) {

  uint8_t buffer[14];
  I2C::readBytes(IMU_ADDRESS, 0x3B, 14, buffer);

  // Creating 16 bits values from 8 bits data (MSB & LSB)

  // Accelerometer
  *ax = -(buffer[0]<<8 | buffer[1]);// ACCEL_XOUT[15:8] | ACCEL_XOUT[7:0]
  *ay = -(buffer[2]<<8 | buffer[3]);
  *az = buffer[4]<<8 | buffer[5];

  // Gyroscope
  *gx = -(buffer[8]<<8 | buffer[9]);
  *gy = -(buffer[10]<<8 | buffer[11]);
  *gz = buffer[12]<<8 | buffer[13];
}


/*
*  Description: getting accelerometer data
*  --------------------------------------------------------------
*  ax, ay, az: latest measurements of the accel's X, Y, Z axis
*  --------------------------------------------------------------
*  notes: see page 28 of 52
*/
void imu::getAcc(int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t buffer[6];
    I2C::readBytes(IMU_ADDRESS, 0x3B, 6, buffer);

    // Accelerometer
    *ax = -(buffer[0]<<8 | buffer[1]);// ACCEL_XOUT[15:8] | ACCEL_XOUT[7:0]
    *ay = -(buffer[2]<<8 | buffer[3]);
    *az = buffer[4]<<8 | buffer[5];
}

/*
*  Description: getting gyroscope data
*  --------------------------------------------------------------
*  gx, gy, gz: latest measurements of the gyro's X, Y, Z axis
*  --------------------------------------------------------------
*  notes: see page 28 of 52
*/
void imu::getGyr(int16_t *gx, int16_t *gy, int16_t *gz) {
    uint8_t buffer[6];
    I2C::readBytes(IMU_ADDRESS, 0x43, 6, buffer);

    // Accelerometer
    *gx = -(buffer[0]<<8 | buffer[1]);// GYRO_XOUT[15:8] | GYRO_XOUT[7:0]
    *gy = -(buffer[2]<<8 | buffer[3]);
    *gz = buffer[4]<<8 | buffer[5];
}
