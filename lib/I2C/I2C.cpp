#include "I2C.h"

//constructor
I2C::I2C() {}

/*-------------------------------------------------------------
 C++ Program to comprehend the use of bitwise operators
#include<stdio.h>
#include<stdint.h>
int main()
{
    uint8_t a = 5, b = 9; // a = 4(00000101), b = 8(00001001)
    printf("a = %d, b = %d\n", a, b);
    printf("a&b = %d\n", a&b); // The result is 00000001
    printf("a|b = %d\n", a|b);  // The result is 00001101
    printf("a^b = %d\n", a^b); // The result is 00001100
    printf("~a = %d\n", a = ~a);   // The result is 11111010
    printf("b<<1 = %d\n", b<<1);  // The result is 00010010
    printf("b>>1 = %d\n", b>>1);  // The result is 00000100
    return 0;
}
-------------------------------------------------------------*/



/*===========================================================*\
*                       Reading functions                     *
\*===========================================================*/

/*
 *  Description: read 1 byte from the register & put it in the container
 *  --------------------------------------------------------------
 *  addr: device's address
 *  reg: register to read
 *  container: container for register's data
 *  --------------------------------------------------------------
 *  Returns: 1 if success else 0
 */
int8_t I2C::readByte(uint8_t addr, uint8_t reg, uint8_t *container) {
    return readBytes( addr, reg, 1, container);
}


/*
 *  Description: read N bytes from the register & put them in the container
 *  --------------------------------------------------------------
 *  addr: device's address
 *  reg: register to read
 *  N: number of bytes to read ()
 *  container: container for register's data
 *  --------------------------------------------------------------
 *  Returns: number of bytes read
 */
int8_t I2C::readBytes(uint8_t addr, uint8_t reg, uint8_t N, uint8_t *container) {
    #ifdef DEBUG
        Serial.print("reading ");
        Serial.print(N, DEC);
        Serial.print(" bytes from (0x");
        Serial.print(reg, HEX);
        Serial.print(") of (0x");
        Serial.print(addr, HEX);
        Serial.println(") ...");
    #endif

    int8_t count = 0;

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.beginTransmission(addr);
    Wire.requestFrom(addr, N);

    while( Wire.available() ) {
        container[count] = Wire.read();
        count++;
        #ifdef DEBUG
            Serial.print(container[count], HEX);
            if (count + 1 < N) Serial.print("; ");
            else Serial.println();
            delay(1000);
        #endif

    }

    Wire.endTransmission();

    #ifdef DEBUG
        Serial.print(count, DEC);
        Serial.println(" reading made.");
    #endif

    return count;
}


/*===========================================================*\
*                       Writing functions                     *
\*===========================================================*/

/*
*  Description: write N bits to the register at index pos
*  --------------------------------------------------------------
*  addr: device's address
*  reg: register to write to
*  N: number of bits to write [|1;8|]
*  index: index of the first bit to write, [|0;7|] from R to L
*  value: value to write in the register
*  --------------------------------------------------------------
*  Returns: true if success else false
*  --------------------------------------------------------------
*  notes: https://fr.wikipedia.org/wiki/Manipulation_de_bit
*         https://fr.wikipedia.org/wiki/Masquage
*/
bool I2C::writeBits(uint8_t addr, uint8_t reg, uint8_t N, uint8_t index, uint8_t value) {
    /*
    * Exemple: index = 6, N = 4, value = 1101
    * mask and shift
    *[7xxxx210]
    * 00000001 << 4
    * 00001000  - 1
    * 00001111 << 3
    * 01111000 ~ (write mask)
    * 10000111 (write mask)
    */
    uint8_t regValue;
    if (readByte(addr, reg, &regValue) != 0) { // regValue = 10111010
        uint8_t mask = ~(((1 << N) - 1) << (index - N + 1)); // mask = 10000111
        value <<= (index - N + 1); // value = 01101000
        regValue = (regValue & mask) | value; //regValue = (10000010) | 01101000 = 11101010
        return writeByte(addr, reg, regValue);
    } else {
        return false;
    }
}

/*
 *  Description: write 1 byte to the register
 *  --------------------------------------------------------------
 *  addr: device's address
 *  reg: register to write to
 *  N: number of bytes to write
 *  value: value to write in the register
 *  --------------------------------------------------------------
 *  Returns: true if success else false
 */
bool I2C::writeByte(uint8_t addr, uint8_t reg, uint8_t value) {
    return writeBytes(addr, reg, 1, &value);
}

/*
 *  Description: write N bytes to the register
 *  --------------------------------------------------------------
 *  addr: device's address
 *  reg: register to write to
 *  N: number of bytes to write
 *  value: value to write in the register
 *  --------------------------------------------------------------
 *  Returns: true if success else false
 */
bool I2C::writeBytes(uint8_t addr, uint8_t reg, uint8_t N, uint8_t* value) {
    #ifdef DEBUG
        Serial.print("writing ");
        Serial.print(N, DEC);
        Serial.print(" bytes to (0x");
        Serial.print(reg, HEX);
        Serial.print(") of (0x");
        Serial.print(addr, HEX);
        Serial.println(") ...");
    #endif

    Wire.beginTransmission(addr);
    Wire.write(reg);

    for (uint8_t i = 0; i < N; i++) {
        Wire.write(value[i]);

        #ifdef DEBUG
            Serial.print(value[i], HEX);
            if (i + 1 < N) Serial.print(" ");
            else Serial.println();
        #endif
    }

    uint8_t status = Wire.endTransmission(); //https://www.arduino.cc/en/Reference/WireEndTransmission

    return status == 0; //true si succès
}
