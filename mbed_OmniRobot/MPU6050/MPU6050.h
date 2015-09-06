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

// Most of the code is adapted from Kris Winer's MPU6050 library

#ifndef MPU6050_H
#define MPU6050_H

#include "mbed.h"
#include "math.h"
#include "MPU6050RegDef.h"

#define PI 3.14159265359    // This value will be used when calculating angles
#define dt 0.005            // 200 Hz sampling period

extern float aRes, gRes; 

/* whoAmI func uses this func, variables etc */
extern Ticker toggler1;  
extern Serial pc;   
extern DigitalOut led2;
extern void toggle_led1();

/* Sensor datas to be used in program */
extern float ax,ay,az;
extern float gx,gy,gz;
extern int16_t accelData[3],gyroData[3],tempData;
extern float accelBias[3], gyroBias[3];
 
/* Function Prototypes */
class MPU6050 
{
    protected:
    public: 
    void getAres();
    void getGres();
    void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
    char readByte(uint8_t address, uint8_t subAddress); 
    void readBytes(uint8_t address, uint8_t subAddress, uint8_t byteNum, uint8_t* dest);
    void whoAmI();
    void init();
    void reset();
    void readAccelData(int16_t* dest);
    void readGyroData(int16_t* dest);
    int16_t readTempData();
    void calibrate(float* dest1, float* dest2);
    void complementaryFilter(float* pitch, float* roll);
};

#endif