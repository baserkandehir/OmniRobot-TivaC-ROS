#ifndef PID_H
#define PID_H

#include <stdbool.h>        
#include <math.h>
#include "OmniStructure.h"  
#include "MacroDefinitions.h"

extern motor_t motor[3];                    // defined in OmniControl.c 

/** @brief Implementation of PID algorithm  
  * @input *ptr: Pointer to motor_t
  *        *mControl: Pointer to motorControl function
  *	       period: Time difference between quadrature encoder counts 
  * @output None
	* @description Function takes period (which is calculated from encoders) as an input
	* and calculates speed in rpm. Then applies PID algorithm and gives resulting pwm
	* to motorControl function as an input.
  */
void PID(motor_t *ptr, void(*mControl)(unsigned long, bool), unsigned long period);

/** @brief  Set PID constants
	* @input  *ptr: Pointer to motor_t
	*	        Kp, Ki, Kd: PID constants
	*	        integral_windup: Constant to prevent integral windup
  * @output None
  */
void PID_setConstants(motor_t *ptr, float Kp, float Ki, float Kd, unsigned long integral_windup);

/** @brief  Initialize PID s by setting all the PID constants
	* @input  Kp, Ki, Kd: PID constants
	*	        integral_windup: Constant to prevent integral windup
  * @output None
  */
void PID_Init(float Kp, float Ki, float Kd, unsigned long integral_windup);

#endif /* PID_H */
