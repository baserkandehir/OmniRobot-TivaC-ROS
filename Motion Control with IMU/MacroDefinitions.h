#ifndef MACRODEFINITIONS_H
#define MACRODEFINITIONS_H

#define MIN_TO_SEC 60         // 1 minute = 60 seconds
#define SEC_TO_MSEC 1000      // 1 second = 1000 miliseconds
#define SEC_TO_USEC 1000000   // 1 second = 10^6 microseconds
#define CM_TO_M 0.01f         // 1 cm = 0.01 meter 
#define RAD_TO_DEG 57.3f      // 2*pi radians = 360 degrees
#define PI 3.14159265359f     // PI
#define CLOCK_FREQ  80000000  // 80 MHz clock frequency      
#define QUAD_CPR 4480         // Quadrature counts per revolution: 4480
#define R 0.05f               // radius of the wheel: 5 cm
#define L 0.13f               // the distance from the center of mass to the omni-wheel: 13 cm
#define MAX_PWM 3998          // Max PWM = 3998
#define MAX_SPEED 120         // max speed: 150 rpm at 12V
#define MIN_SPEED 2           // min speed defined as 2 rpm 
#define MOTOR_PID_dt 0.001f   // motor PID functions will be executed every 1 ms  
#define TIMER4_dt 0.006f      // Timer4 period: 6ms
#define THETA 90              // The angle between robots frame and world frame

#endif /* MACRODEFINITIONS_H */
