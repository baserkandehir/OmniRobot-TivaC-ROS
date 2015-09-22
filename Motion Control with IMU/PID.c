#include "PID.h"            

/** @brief Implementation of PID algorithm  
  * @input *ptr: Pointer to motor_t
  *        *mControl: Pointer to motorControl function
  *	       period: Time difference between quadrature encoder counts 
  * @output None
	* @description Function takes period (which is calculated from encoders) as an input
	* and calculates speed in rpm. Then applies PID algorithm and gives resulting pwm
	* to motorControl function as an input.
  */
void PID(motor_t *ptr, void(*mControl)(unsigned long, bool), unsigned long period)
{
	/* Rpm calculation from period */
	if(period == 0)    // period might not be calculated
		ptr->speed = 0;
	else
		ptr->speed = (float)(MIN_TO_SEC * SEC_TO_USEC) / (float)(period * QUAD_CPR);  
	
	ptr->pid.proportional = ptr->pid.set_point - ptr->speed;
	
	/* Integral windup prevention */
	if(fabs(ptr->pid.integral) < ptr->pid.integral_windup)
		ptr->pid.integral += ptr->pid.proportional;
	else
		ptr->pid.integral = 0;
	
	ptr->pid.derivative = ptr->pid.proportional - ptr->pid.last_proportional;
	ptr->pid.last_proportional = ptr->pid.proportional;
	ptr->pid.error = (ptr->pid.Kp * ptr->pid.proportional) + (ptr->pid.Ki * ptr->pid.integral) + (ptr->pid.Kd * ptr->pid.derivative);
	
	/* PWM underflow prevention */
	if((fabs(ptr->pid.error) > ptr->pwm) && (ptr->pid.error < 0))
		ptr->pwm = 0;
	else
		ptr->pwm = ptr->pwm + (signed long)(ptr->pid.error);
	
	/* PWM overflow prevention */
	if(ptr->pwm > MAX_PWM)
		ptr->pwm = MAX_PWM;
	
	mControl(ptr->pwm, ptr->dir);
}

/** @brief  Set PID constants
	* @input  *ptr: Pointer to motor_t
	*	        Kp, Ki, Kd: PID constants
	*	        integral_windup: Constant to prevent integral windup
  * @output None
  */
void PID_setConstants(motor_t *ptr, float Kp, float Ki, float Kd, unsigned long integral_windup)
{
	ptr->pid.Kp = Kp;
	ptr->pid.Ki = Ki;
	ptr->pid.Kd = Kd;
	ptr->pid.integral_windup = integral_windup;
}

/** @brief  Initialize PID s by setting all the PID constants
	* @input  Kp, Ki, Kd: PID constants
	*	        integral_windup: Constant to prevent integral windup
  * @output None
  */
void PID_Init(float Kp, float Ki, float Kd, unsigned long integral_windup)
{    
	unsigned long i;
	for(i = 0; i < 3 ; PID_setConstants(&motor[i++], Kp, Ki, Kd, integral_windup));
}
