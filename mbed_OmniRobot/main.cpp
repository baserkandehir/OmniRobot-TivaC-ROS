#include "mbed.h"
#include "HMC5883L.h"
#include "MPU6050.h"
#include "ledControl.h"

Serial bluetx(PA_11, PA_12);    // PA_11: TX, PA_12: RX 
Serial bluerx(PA_15, PB_7);     // PA_15: TX, PB_7: RX 
Serial pc(USBTX,USBRX); 
Ticker ticker_sendData;
Ticker ticker_getData;
Ticker ticker_getHeading;
Ticker ticker_mpu6050_getHeading;
HMC5883L hmc5883l;
MPU6050 mpu6050;

/* Variable definitions */
volatile char data = 0;
volatile unsigned long Data = 0;
volatile unsigned long heading = 0;
volatile double Heading = 0;

/* Function prototypes */
void toggle_led1();
void toggle_led2();
void getData(void);
void sendData(void);
void get_Heading(void);
void mpu6050_getHeading(void);

int main() 
{
    bluetx.baud(115200);
    bluerx.baud(9600);
    
    ticker_sendData.attach(&sendData, 0.001);       // send data to TI launchpad every 1ms 
    ticker_getData.attach(&getData,   0.001);       // get data from bluetooth every 5ms  
    ticker_getHeading.attach(&get_Heading, 0.006);  // get heading angle every 6ms
    
    hmc5883l.init();
    
    mpu6050.whoAmI();                           // Communication test: WHO_AM_I register reading 
    mpu6050.calibrate(accelBias,gyroBias);      // Calibrate MPU6050 and load biases into bias registers
    mpu6050.init();                             // Initialize the sensor
    ticker_mpu6050_getHeading.attach(&mpu6050_getHeading, 0.005); 

    while(1) 
    { 
        pc.printf(" _______________\r\n");
        pc.printf("| heading: %d   \r\n", heading);
        pc.printf("| data: %d   \r\n", data);
        pc.printf("| Data: %d   \r\n", Data);
        pc.printf("|_______________\r\n\r\n");
        wait(1);
    }
}

void getData(void)
{
    if(bluerx.readable())
        data = bluerx.getc();    
}

void sendData(void)
{
    Data = (unsigned long)(heading << 8)|(unsigned long)data; 
    bluetx.printf("%d\r\n", Data);
}

void get_Heading(void)
{
   // Heading = hmc5883l.getHeading(); 
    heading = (unsigned long)(Heading * 10.0);
}

void mpu6050_getHeading(void)
{
    /* Get actual gyro value */
    mpu6050.readGyroData(gyroData);
    mpu6050.getGres();     
    gz = gyroData[2]*gRes;  
    Heading += gz * 0.005f;
    if(Heading > 360) Heading -= 360;
    else if(Heading < 0) Heading += 360;
}

void toggle_led1() {ledToggle(1);}
void toggle_led2() {ledToggle(2);}
