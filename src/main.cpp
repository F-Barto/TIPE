/**
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"

#include "imu.h"


imu mpu9150;


//containers for accel and gyro data
int16_t ax, ay, az;
int16_t gx, gy, gz;

float ax_offset, ay_offset, az_offset;
float gx_offset, gy_offset, gz_offset;

float deltat;           // integration interval for complementary filter
uint16_t t_last;        // used to calculate integration interval
uint16_t t_now;         // used to calculate integration interval

uint16_t cal_cycles = 1000;    //number of measures to do in the calib routine

float pitch, roll; // containers for pitch and roll angles

//calibration routine
void calibrate() {

    for (uint16_t i = 0; i < 100; i++) {    // Discard the 100 first readings
        mpu9150.getAccGyr(&ax, &ay, &az, &gx, &gy, &gz);
    }
    delay(100);

    for (uint16_t i = 0; i < cal_cycles; i++) {
        mpu9150.getAccGyr(&ax, &ay, &az, &gx, &gy, &gz);
        ax_offset += ax;
        ay_offset += ay;
        az_offset += az;
        gx_offset += gx;
        gy_offset += gy;
        gz_offset += gz;
        if (i % 100 == 0) Serial.print(".");
    }

    ax_offset /= cal_cycles;
    ay_offset /= cal_cycles;
    az_offset /= cal_cycles;
    gx_offset /= cal_cycles;
    gy_offset /= cal_cycles;
    gz_offset /= cal_cycles;

    Serial.println("");
}

void setDelta() {
    t_now = micros();
    deltat = ((t_now - t_last)/1000000.0f); // time elapsed since last filter update
    t_last = t_now;
}

void setup()
{

    deltat = 0.0f;  // integration interval for complementary filter
    t_last = 0.0f;  // used to calculate integration interval
    t_now = 0.0f;   // used to calculate integration interval

    ax_offset = 0.0f;
    ay_offset = 0.0f;
    az_offset = 0.0f;
    gx_offset = 0.0f;
    gy_offset = 0.0f;
    gz_offset = 0.0f;


    // Arduino initializations
    Wire.begin();
    Serial.begin(38400);
    delay(3000);

    Serial.println("Initialization .... ");
    mpu9150.init();
    delay(1000);

    Serial.print("\tcalibrating ...");
    calibrate();
    delay(1000);

    Serial.println("Initialized ... ");
/*
    Serial.print("gx_off: ");Serial.println(gx_offset);
    Serial.print("g: ");Serial.println(gx);
*/
}


/*===========================================================*\
*                   Complementary filter                     *
\*===========================================================*/


void ComplementaryFilter(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz, float *pitch, float *roll) {

    float pitchAcc, rollAcc;

    setDelta();

    // Integrate the gyroscope data
    *pitch += ((float)gx / GYRO_SENSITIVITY) * deltat;  // Angle around the X-axis
    *roll -= ((float)gy / GYRO_SENSITIVITY) * deltat;   // Angle around the Y-axis

	// Turning around the X axis results in a vector on the Y-axis
    pitchAcc = atan2f((float)ay, (float)az) * 180.0f / M_PI;
    *pitch = *pitch * 0.98f + pitchAcc * 0.02f;

	// Turning around the Y axis results in a vector on the X-axis
    rollAcc = atan2f((float)ax, (float)az) * 180.0f / M_PI;
    *roll = *roll * 0.98f + rollAcc * 0.02f;
}


void loop()
{

    mpu9150.getAccGyr(&ax, &ay, &az, &gx, &gy, &gz);

    ComplementaryFilter(ax, ay, az, gx, gy, gz, &pitch, &roll);

    Serial.print("pitch |:\t");
    Serial.print(pitch);
    Serial.print(" roll |:\t");
    Serial.println(roll);

    /*
    Serial.print("a |:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print("g |:\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.print(gz); Serial.println("\t");
    delay(3000);
    */
}
