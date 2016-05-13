#ifndef IMU_H
#define IMU_H

    #include "I2C.h"

    #define IMU_ADDRESS 0x69 //change to 0x68 if it doesn't work, see AD0 pin

    /*
     *  Description: this class will hold functions for IMU communication
     * in this case the MPU-9150
     *  exemples: initialization, read gyro, read acc ....
    */
    class imu {
        public:
            imu();

            void init();
            void getAccGyr(uint16_t *ax, uint16_t *ay, uint16_t *az, uint16_t *gx, uint16_t *gy, uint16_t *gz);
            void getAcc(uint16_t *ax, uint16_t *ay, uint16_t *az);
            void getGyr(uint16_t *gx, uint16_t *gy, uint16_t *gz);
            void getMag(uint16_t *mx, uint16_t *my, uint16_t *mz);
    };


#endif
