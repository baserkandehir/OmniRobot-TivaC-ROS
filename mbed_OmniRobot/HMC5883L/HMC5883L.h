/*     
*   Copyright (c) 2015, Baser Kandehir, baser.kandehir@ieee.metu.edu.tr
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in
*   all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*   THE SOFTWARE.
*
*/

// Some part of the code is adapted from Adafruit HMC5883 library

#ifndef HMC5883L_H
#define HMC5883L_H

#include "mbed.h"
#include "math.h"

#define PI 3.14159265359 
#define GAUSS_TO_MICROTESLA 100
#define HMC5883L_ADDRESS 0x3C

/* Register Definitions */
#define CONFIG_A     0x00
#define CONFIG_B     0x01
#define MODE         0x02
#define OUT_X_MSB    0x03
#define OUT_X_LSB    0x04
#define OUT_Z_MSB    0x05
#define OUT_Z_LSB    0x06
#define OUT_Y_MSB    0x07
#define OUT_Y_LSB    0x08
#define STATUS       0x09
#define ID_A         0x0A
#define ID_B         0x0B
#define ID_C         0x0C

/* Magnetometer Gain Settings */
enum MagGain
{
    MagGain_088 =  0x00,      // +/- 0.88 Ga
    MagGain_13  =  0x20,      // +/- 1.3  Ga
    MagGain_19  =  0x40,      // +/- 1.9  Ga
    MagGain_25  =  0x60,      // +/- 2.5  Ga
    MagGain_40  =  0x80,      // +/- 4.0  Ga
    MagGain_47  =  0xA0,      // +/- 4.7  Ga
    MagGain_56  =  0xC0,      // +/- 5.6  Ga
    MagGain_81  =  0xE0       // +/- 8.1  Ga
};

class HMC5883L
{
    public:
        void init();
        double getHeading();
        void readMagData(float* dest);  
    private:
        void setMagGain(MagGain gain);
        void writeByte(uint8_t address, uint8_t regAddress, uint8_t data);
        char readByte(uint8_t address, uint8_t regAddress);
        void readBytes(uint8_t address, uint8_t regAddress, uint8_t byteNum, uint8_t* dest);
};

#endif