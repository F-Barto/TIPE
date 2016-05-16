/* Adapted from jrowberg's code to fit my need */


#ifndef I2C_H
#define I2C_H

    #include "Arduino.h"
    #include <Wire.h>

    //#define DEBUG

    /*
     *  Description: this class will hold functions for I2C communication
     *  exemples: read_bytes, write_bytes, etc....
    */
    class I2C {
        public:
            I2C();//constructor

            static int8_t readBytes(uint8_t addr, uint8_t reg, uint8_t N, uint8_t *container);
            static int8_t readByte(uint8_t addr, uint8_t reg, uint8_t *container);
            static bool writeBits(uint8_t addr, uint8_t reg, uint8_t N, uint8_t index, uint8_t value);
            static bool writeBytes(uint8_t addr, uint8_t reg, uint8_t N, uint8_t *value);
            static bool writeByte(uint8_t addr, uint8_t reg, uint8_t value);


    };

#endif
