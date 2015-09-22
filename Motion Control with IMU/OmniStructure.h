#ifndef OMNISTRUCTURE_H
#define OMNISTRUCTURE_H

#include <stdbool.h>        

typedef volatile struct   /* add volatile keyword to all variables */   
{
	signed long count;    // quadrature encoder count              
	float rev;            // count of revolution of the wheel  
	unsigned long period; // time difference between quadrature encoder counts 
	volatile bool dir;    // direction of rotation determined from encoders
}encoder_t;

typedef struct 
{
	float proportional;
	float last_proportional;
	float integral;
	float derivative;
	float error;
	volatile float set_point;
	unsigned long integral_windup;
	float Kp;
	float Ki;
	float Kd;
}pid_t;

typedef struct 
{
	volatile float speed;    // real speed calculated from encoders in rpm
	volatile bool dir;       // direction of rotation
	unsigned long pwm;       // required pwm to reach set_point
	pid_t pid;
	encoder_t enc;
}motor_t;

typedef volatile struct
{
	float x, y, theta;
	float x_dot, y_dot, theta_dot;
}state_t;      // state of the omnidirectional robot

#endif /* OMNISTRUCTURE_H */
