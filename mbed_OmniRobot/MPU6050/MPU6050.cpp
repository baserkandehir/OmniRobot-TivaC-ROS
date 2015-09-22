/*   MPU6050 Library   
*    
*    @author: Baser Kandehir 
*    @date: July 16, 2015
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

// Most of the code is adapted from Kris Winer's MPU6050 library

#include "MPU6050.h"

Ticker toggler1;

/* For LPC1768 board */
//I2C i2c(p9,p10);         // setup i2c (SDA,SCL)  

/* For NUCLEO-F411RE board */
static I2C i2c(D14,D15);         // setup i2c (SDA,SCL)

/* Set initial input parameters */

// Acc Full Scale Range  +-2G 4G 8G 16G  
enum Ascale
{
    AFS_2G=0,  
    AFS_4G,
    AFS_8G,
    AFS_16G
};

// Gyro Full Scale Range +-250 500 1000 2000 Degrees per second
enum Gscale
{
    GFS_250DPS=0,   
    GFS_500DPS,
    GFS_1000DPS,
    GFS_2000DPS
};

// Sensor datas
float ax,ay,az;
float gx,gy,gz;
int16_t accelData[3],gyroData[3],tempData;
float accelBias[3] = {0, 0, 0};  // Bias corrections for acc
float gyroBias[3] = {0, 0, 0};   // Bias corrections for gyro 

// Specify sensor full scale range
int Ascale = AFS_2G;
int Gscale = GFS_250DPS;

// Scale resolutions per LSB for the sensors
float aRes, gRes; 

// Calculates Acc resolution
void MPU6050::getAres()
{
    switch(Ascale)
    {
        case AFS_2G:
            aRes = 2.0/32768.0;
            break;
        case AFS_4G:
            aRes = 4.0/32768.0;
            break;
        case AFS_8G:
            aRes = 8.0/32768.0;
            break;
        case AFS_16G:
            aRes = 16.0/32768.0;
            break;         
    }
}

// Calculates Gyro resolution
void MPU6050::getGres()
{
    switch(Gscale)
    {
        case GFS_250DPS:
            gRes = 250.0/32768.0;
            break;
        case GFS_500DPS:
            gRes = 500.0/32768.0;
            break;
        case GFS_1000DPS:
            gRes = 1000.0/32768.0;
            break;
        case GFS_2000DPS:
            gRes = 2000.0/32768.0;
            break;
    }
}
    
void MPU6050::writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
    char data_write[2];
    data_write[0]=subAddress;           // I2C sends MSB first. Namely  >>|subAddress|>>|data|
    data_write[1]=data;
    i2c.write(address,data_write,2,0);  // i2c.write(int address, char* data, int length, bool repeated=false);  
}

char MPU6050::readByte(uint8_t address, uint8_t subAddress)
{
    char data_read[1];  // will store the register data    
    char data_write[1];
    data_write[0]=subAddress;
    i2c.write(address,data_write,1,1);  // have not stopped yet
    i2c.read(address,data_read,1,0);    // read the data and stop
    return data_read[0];
} 

void MPU6050::readBytes(uint8_t address, uint8_t subAddress, uint8_t byteNum, uint8_t* dest)
{
    char data[14],data_write[1];  
    data_write[0]=subAddress;      
    i2c.write(address,data_write,1,1);
    i2c.read(address,data,byteNum,0);
    for(int i=0;i<byteNum;i++)         // equate the addresses
        dest[i]=data[i];
}

// Communication test: WHO_AM_I register reading 
void MPU6050::whoAmI()
{
    uint8_t whoAmI = readByte(MPU6050_ADDRESS, WHO_AM_I_MPU6050);   // Should return 0x68
    pc.printf("I AM 0x%x \r\n",whoAmI);
    
    if(whoAmI==0x68)
    {
        pc.printf("MPU6050 is online... \r\n");  
        led2=1;
    }
    else
    {
        pc.printf("Could not connect to MPU6050 \r\nCheck the connections... \r\n");  
        toggler1.attach(&toggle_led1,0.1);     // toggles led1 every 100 ms
    }  
}

// Initializes MPU6050 with the following config:
// PLL with X axis gyroscope reference 
// Sample rate: 200Hz for gyro and acc
// Interrupts are disabled
void MPU6050::init()
{      
    i2c.frequency(400000);                      // fast i2c: 400 kHz
  
    /* Wake up the device */
    writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);  // wake up the device by clearing the sleep bit (bit6) 
    wait_ms(100); // wait 100 ms to stabilize  
    
    /* Get stable time source */
    // PLL with X axis gyroscope reference is used to improve stability
    writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);
    
    /* Configure Gyroscope and Accelerometer */
    // Disable FSYNC, acc bandwidth: 44 Hz, gyro bandwidth: 42 Hz
    // Sample rates: 1kHz, maximum delay: 4.9ms (which is pretty good for a 200 Hz maximum rate)
    writeByte(MPU6050_ADDRESS, CONFIG, 0x03);
    
    /* Set sample rate = gyroscope output rate/(1+SMPLRT_DIV) */
    // SMPLRT_DIV=4 and sample rate=200 Hz (compatible with config above)
    writeByte(MPU6050_ADDRESS, SMPLRT_DIV, 0x04);
    
    /* Accelerometer configuration */
    uint8_t temp = readByte(MPU6050_ADDRESS, ACCEL_CONFIG);
    writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, temp & ~0xE0);      // Clear self-test bits [7:5]
    writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, temp & ~0x18);      // Clear AFS bits [4:3]
    writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, temp | Ascale<<3);  // Set full scale range 
    
    /* Gyroscope configuration */       
    temp = readByte(MPU6050_ADDRESS, GYRO_CONFIG);
    writeByte(MPU6050_ADDRESS, GYRO_CONFIG, temp & ~0xE0);      // Clear self-test bits [7:5]
    writeByte(MPU6050_ADDRESS, GYRO_CONFIG, temp & ~0x18);      // Clear FS bits [4:3]
    writeByte(MPU6050_ADDRESS, GYRO_CONFIG, temp | Gscale<<3);  // Set full scale range 
}

// Resets the device
void MPU6050::reset()
{
    writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x80);  // set bit7 to reset the device
    wait_ms(100);    // wait 100 ms to stabilize        
}

void MPU6050::readAccelData(int16_t* dest)
{
    uint8_t rawData[6];  // x,y,z acc data            
    readBytes(MPU6050_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]);   // read six raw data registers sequentially and write them into data array
    
    /* Turn the MSB LSB into signed 16-bit value */
    dest[0] = (int16_t)(((int16_t)rawData[0]<<8) | rawData[1]);  // ACCEL_XOUT
    dest[1] = (int16_t)(((int16_t)rawData[2]<<8) | rawData[3]);  // ACCEL_YOUT
    dest[2] = (int16_t)(((int16_t)rawData[4]<<8) | rawData[5]);  // ACCEL_ZOUT
}

void MPU6050::readGyroData(int16_t* dest)
{
    uint8_t rawData[6];  // x,y,z gyro data            
    readBytes(MPU6050_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]);   // read the six raw data registers sequentially and write them into data array
    
    /* Turn the MSB LSB into signed 16-bit value */
    dest[0] = (int16_t)(((int16_t)rawData[0]<<8) | rawData[1]);  // GYRO_XOUT
    dest[1] = (int16_t)(((int16_t)rawData[2]<<8) | rawData[3]);  // GYRO_YOUT
    dest[2] = (int16_t)(((int16_t)rawData[4]<<8) | rawData[5]);  // GYRO_ZOUT    
}
    
int16_t MPU6050::readTempData()
{
    uint8_t rawData[2];  // temperature data
    readBytes(MPU6050_ADDRESS, TEMP_OUT_H, 2, &rawData[0]);   // read the two raw data registers sequentially and write them into data array 
    return (int16_t)(((int16_t)rawData[0]<<8) | rawData[1]);   // turn the MSB LSB into signed 16-bit value
}

/* Function which accumulates gyro and accelerometer data after device initialization. 
   It calculates the average of the at-rest readings and 
   then loads the resulting offsets into accelerometer and gyro bias registers. */
/* 
    IMPORTANT NOTE: In this function;
         Resulting accel offsets are NOT pushed to the accel bias registers. accelBias[i] offsets are used in the main program.
         Resulting gyro offsets are pushed to the gyro bias registers. gyroBias[i] offsets are NOT used in the main program.
         Resulting data seems satisfactory.
*/
// dest1: accelBias dest2: gyroBias
void MPU6050::calibrate(float* dest1, float* dest2)
{
    uint8_t data[12];       // data array to hold acc and gyro x,y,z data
    uint16_t fifo_count, packet_count, count;   
    int32_t accel_bias[3] = {0,0,0}; 
    int32_t gyro_bias[3] = {0,0,0};
    float aRes = 2.0/32768.0;   
    float gRes = 250.0/32768.0;
    uint16_t accelsensitivity = 16384; // = 1/aRes = 16384 LSB/g
    //uint16_t gyrosensitivity = 131;    // = 1/gRes = 131 LSB/dps
    
    reset();     // Reset device
    
    /* Get stable time source */
    writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);    // PLL with X axis gyroscope reference is used to improve stability
    writeByte(MPU6050_ADDRESS, PWR_MGMT_2, 0x00);    // Disable accel only low power mode 
    wait(0.2);
      
    /* Configure device for bias calculation */
    writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x00);   // Disable all interrupts
    writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);      // Disable FIFO
    writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);   // Turn on internal clock source
    writeByte(MPU6050_ADDRESS, I2C_MST_CTRL, 0x00); // Disable I2C master
    writeByte(MPU6050_ADDRESS, USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
    writeByte(MPU6050_ADDRESS, USER_CTRL, 0x04);    // Reset FIFO
    wait(0.015);   
    
    /* Configure accel and gyro for bias calculation */
    writeByte(MPU6050_ADDRESS, CONFIG, 0x01);       // Set low-pass filter to 188 Hz
    writeByte(MPU6050_ADDRESS, SMPLRT_DIV, 0x00);   // Set sample rate to 1 kHz
    writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity
    writeByte(MPU6050_ADDRESS, GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
     
    /* Configure FIFO to capture accelerometer and gyro data for bias calculation */
    writeByte(MPU6050_ADDRESS, USER_CTRL, 0x40);   // Enable FIFO  
    writeByte(MPU6050_ADDRESS, FIFO_EN, 0x78);     // Enable accelerometer and gyro for FIFO  (max size 1024 bytes in MPU-6050)
    wait(0.08);                                    // Sample rate is 1 kHz, accumulates 80 samples in 80 milliseconds. 
    // accX: 2 byte, accY: 2 byte, accZ: 2 byte. gyroX: 2 byte, gyroY: 2 byte, gyroZ: 2 byte.   12*80=960 byte < 1024 byte                                               
                                                                                               
    /* At end of sample accumulation, turn off FIFO sensor read */
    writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);             // Disable FIFO
    readBytes(MPU6050_ADDRESS, FIFO_COUNTH, 2, &data[0]);  // Read FIFO sample count
    fifo_count = ((uint16_t)data[0] << 8) | data[1];
    packet_count = fifo_count/12;                          // The number of sets of full acc and gyro data for averaging. packet_count = 80 in this case
    
    for(count=0; count<packet_count; count++)
    {
        int16_t accel_temp[3]={0,0,0}; 
        int16_t gyro_temp[3]={0,0,0};
        readBytes(MPU6050_ADDRESS, FIFO_R_W, 12, &data[0]); // read data for averaging
        
        /* Form signed 16-bit integer for each sample in FIFO */
        accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ; 
        accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
        accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;    
        gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
        gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
        gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;
        
        /* Sum individual signed 16-bit biases to get accumulated signed 32-bit biases */
        accel_bias[0] += (int32_t) accel_temp[0]; 
        accel_bias[1] += (int32_t) accel_temp[1];
        accel_bias[2] += (int32_t) accel_temp[2];  
        gyro_bias[0]  += (int32_t) gyro_temp[0];
        gyro_bias[1]  += (int32_t) gyro_temp[1];
        gyro_bias[2]  += (int32_t) gyro_temp[2];
    }
    
    /* Normalize sums to get average count biases */
    accel_bias[0] /= (int32_t) packet_count; 
    accel_bias[1] /= (int32_t) packet_count;
    accel_bias[2] /= (int32_t) packet_count;
    gyro_bias[0]  /= (int32_t) packet_count;
    gyro_bias[1]  /= (int32_t) packet_count;
    gyro_bias[2]  /= (int32_t) packet_count;
      
    /* Remove gravity from the z-axis accelerometer bias calculation */  
    if(accel_bias[2] > 0) {accel_bias[2] -= (int32_t) accelsensitivity;}  
    else {accel_bias[2] += (int32_t) accelsensitivity;}
    
    /* Output scaled accelerometer biases for manual subtraction in the main program */
    dest1[0] = accel_bias[0]*aRes;
    dest1[1] = accel_bias[1]*aRes;
    dest1[2] = accel_bias[2]*aRes;
    
    /* Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup */
    data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
    data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
    data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
    data[3] = (-gyro_bias[1]/4)       & 0xFF;
    data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
    data[5] = (-gyro_bias[2]/4)       & 0xFF;

    /* Push gyro biases to hardware registers */
    writeByte(MPU6050_ADDRESS, XG_OFFS_USRH, data[0]); 
    writeByte(MPU6050_ADDRESS, XG_OFFS_USRL, data[1]);
    writeByte(MPU6050_ADDRESS, YG_OFFS_USRH, data[2]);
    writeByte(MPU6050_ADDRESS, YG_OFFS_USRL, data[3]);
    writeByte(MPU6050_ADDRESS, ZG_OFFS_USRH, data[4]);
    writeByte(MPU6050_ADDRESS, ZG_OFFS_USRL, data[5]);

    /* Construct gyro bias in deg/s for later manual subtraction */
    dest2[0] = gyro_bias[0]*gRes;   
    dest2[1] = gyro_bias[1]*gRes;
    dest2[2] = gyro_bias[2]*gRes;
}

void MPU6050::complementaryFilter(float* pitch, float* roll)
{
    /* Get actual acc value */
    readAccelData(accelData);
    getAres();
    ax = accelData[0]*aRes - accelBias[0];
    ay = accelData[1]*aRes - accelBias[1];
    az = accelData[2]*aRes - accelBias[2]; 

    /* Get actual gyro value */
    readGyroData(gyroData);
    getGres();     
    gx = gyroData[0]*gRes;  // - gyroBias[0];      // Results are better without extracting gyroBias[i]
    gy = gyroData[1]*gRes;  // - gyroBias[1]; 
    gz = gyroData[2]*gRes;  // - gyroBias[2]; 

    float pitchAcc, rollAcc;

    /* Integrate the gyro data(deg/s) over time to get angle */
    *pitch += gx * dt;  // Angle around the X-axis
    *roll -=  gy * dt;  // Angle around the Y-axis
    
    /* Turning around the X-axis results in a vector on the Y-axis
    whereas turning around the Y-axis results in a vector on the X-axis. */
    pitchAcc = atan2f(accelData[1], accelData[2])*180/PI;
    rollAcc  = atan2f(accelData[0], accelData[2])*180/PI;
  
    /* Apply Complementary Filter */
    *pitch = *pitch * 0.98 + pitchAcc * 0.02;
    *roll  = *roll  * 0.98 + rollAcc  * 0.02;  
}
