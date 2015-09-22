/*   HMC5883L Digital Compass Library
*
*    @author: Baser Kandehir 
*    @date: August 5, 2015
*    @license: MIT license
*     
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

#include "HMC5883L.h"

/* NUCLEO F411RE board */
static I2C i2c(D14, D15);         // setup i2c (SDA,SCL)  

static float Gauss_LSB_XY = 1100.0F; // Varies with gain
static float Gauss_LSB_Z =  980.0F;  // Varies with gain
 
void HMC5883L::setMagGain(MagGain gain)
{
    writeByte(HMC5883L_ADDRESS, CONFIG_B, (int8_t)gain);    
    
    switch(gain)
    {
        case MagGain_13:
            Gauss_LSB_XY = 1100;
            Gauss_LSB_Z  = 980;
            break;
        case MagGain_19:
            Gauss_LSB_XY = 855;
            Gauss_LSB_Z  = 760;
            break;
        case MagGain_25:
            Gauss_LSB_XY = 670;
            Gauss_LSB_Z  = 600;
            break;
        case MagGain_40:
            Gauss_LSB_XY = 450;
            Gauss_LSB_Z  = 400;
            break;
        case MagGain_47:
            Gauss_LSB_XY = 400;
            Gauss_LSB_Z  = 255;
            break;
        case MagGain_56:
            Gauss_LSB_XY = 330;
            Gauss_LSB_Z  = 295;
            break;
        case MagGain_81:
            Gauss_LSB_XY = 230;
            Gauss_LSB_Z  = 205;
            break;
    }  
}

void HMC5883L::writeByte(uint8_t address, uint8_t regAddress, uint8_t data)
{
    char data_write[2];
    data_write[0]=regAddress;           // I2C sends MSB first. Namely  >>|regAddress|>>|data|
    data_write[1]=data;
    i2c.write(address,data_write,2,0);  // i2c.write(int address, char* data, int length, bool repeated=false);  
}

char HMC5883L::readByte(uint8_t address, uint8_t regAddress)
{
    char data_read[1];                   // will store the register data    
    char data_write[1];
    data_write[0]=regAddress;
    i2c.write(address,data_write,1,1);   // repeated = true
    i2c.read(address,data_read,1,0);     // read the data and stop
    return data_read[0];
} 

void HMC5883L::readBytes(uint8_t address, uint8_t regAddress, uint8_t byteNum, uint8_t* dest)
{
    char data[10],data_write[1];  
    data_write[0]=regAddress;      
    i2c.write(address,data_write,1,1);
    i2c.read(address,data,byteNum,0);
    for(int i=0;i<byteNum;i++)          // equate the addresses
        dest[i]=data[i];
}

void HMC5883L::init()
{
    writeByte(HMC5883L_ADDRESS, CONFIG_A, 0x78);  // Number of samples averaged: 8, Data output rate: 75 Hz
    writeByte(HMC5883L_ADDRESS, MODE,     0x01);  // Single-Measurement Mode, 160 Hz max output rate
    setMagGain(MagGain_13);
    wait_ms(10);
}

void HMC5883L::readMagData(float* dest)
{
    uint8_t rawData[6]; // x,y,z mag data
    
    writeByte(HMC5883L_ADDRESS, MODE,     0x01);  // Single-Measurement Mode, 160 Hz max output rate 
    
    /* Read six raw data registers sequentially and write them into data array */
    readBytes(HMC5883L_ADDRESS, OUT_X_MSB, 6, &rawData[0]); 
    
    /* Turn the MSB LSB into signed 16-bit value */
    dest[0] = (int16_t)(((int16_t)rawData[0]<<8) | rawData[1]);  // MAG_XOUT
    dest[2] = (int16_t)(((int16_t)rawData[2]<<8) | rawData[3]);  // MAG_ZOUT
    dest[1] = (int16_t)(((int16_t)rawData[4]<<8) | rawData[5]);  // MAG_YOUT 
    
    /* Convert raw data to magnetic field values in microtesla */
     dest[0] = dest[0] / Gauss_LSB_XY * GAUSS_TO_MICROTESLA;
     dest[1] = dest[1] / Gauss_LSB_XY * GAUSS_TO_MICROTESLA;
     dest[2] = dest[2] / Gauss_LSB_Z  * GAUSS_TO_MICROTESLA;
}

double HMC5883L::getHeading()
{
    float magData[3];
    readMagData(magData);

    magData[0] -= 6.8f; // calibration
    
    /* Calculate the heading while Z axis of the module is pointing up */
    double heading = atan2(magData[1], magData[0]);
    
    // After calculating heading declination angle should be added to heading which is the error of the magnetic field in specific location.
    // declinationAngle can be found here http://www.magnetic-declination.com/
    // For Ankara (my location) declinationAngle is ~5.5 degrees (0.096 radians)
    float declinationAngle = 0.096;
    heading += declinationAngle;
    
    // Correct for when signs are reversed.
    if(heading < 0)
        heading += 2*PI;
    
    // Check for wrap due to addition of declination.
    if(heading > 2*PI)
        heading -= 2*PI;
    
    /* Convert radian to degrees */
    heading = heading * 180 / PI;  
    
    return heading;    
}
