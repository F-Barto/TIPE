/**
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include "imu.h"

//containers for accel and gyro data
int16_t ax, ay, az;
int16_t gx, gy, gz;

imu mpu9150;

bool blink = false;//

void setup()
{
    // Arduino initializations
    Wire.begin();
    Serial.begin(115200);

    mpu9150.init();

    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{

    mpu9150.getAccGyr(&ax, &ay, &az, &gx, &gy, &gz);

    // display tab-separated accel/gyro x/y/z values
    Serial.print("a |:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print("g |:\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.print(gz); Serial.println("\t");

    /*
    blink = !blink;
    digitalWrite(LED_BUILTIN, blink);// indicator light to make sure that data is communicated
    */

}
