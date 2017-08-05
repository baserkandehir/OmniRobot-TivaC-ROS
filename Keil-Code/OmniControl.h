#ifndef OMNICONTROL_H
#define OMNICONTROL_H

#include <stdbool.h> 
#include "OmniStructure.h" 
#include "MacroDefinitions.h"

extern volatile unsigned long long Counts;  // defined in SysTick.c (increments every 1 us)

/** @brief  Timer tasks to be called in timer handlers
  * @input  timerNum: Determines which timer function to be selected
  * @output None
  */
void Timer_Task(unsigned long timerNum);

/* Timer tasks to be called in timer handlers */
void Timer0_Task(void);
void Timer1_Task(void);
void Timer2_Task(void);
void Timer3_Task(void);	
void Timer4_Task(void);

/** @brief  Timer initializations for PID loops
  * @input  None
  * @output None
  */
void OmniControl_Init(void);

/** @brief Set desired speed and direction of a motor
  * @input *ptr: Pointer to motor_t
  *        set_point: Desired speed value
  *        dir: Direction of rotation
  * @output None
  */
void OmniControl_setSpeedDir(motor_t *ptr, volatile float set_point, bool dir);

/** @brief  Control function of a three-wheeled omni-directional robot
  * @input  Vx: Velocity of the robot in x direction (in m/s)
  *         Vy: Velocity of the robot in y direction (in m/s)
  *         Vang: Angular velocity of the robot (in deg/s)
	*				  theta: Angle between robots frame and world frame
  * @output None
  * @description This function takes translational and angular velocity as input and
  * calculates the corresponding angular velocities of the wheels and gives these values
  * as set_point to the motor PID functions.
  * Vx_max = 0.63 m/s
	*	Vy_max = 0.73 m/s
	*	Vang_max = 276.5 deg/s 
  */
void OmniControl(float Vx, float Vy, float Vang, float theta);

/** @brief Convert translational and angular velocity of the robot to wheel angular velocities  
  * @input Vx: Velocity of the robot in x direction 
  *        Vy: Velocity of the robot in y direction 
  *        Vang: Angular velocity of the robot 	
	*        theta: Angle between robots frame and world frame
	*        *ptr: Pointer to wheel angular velocities
  * @output None
  */
void convertFromRobotVelToMotorVel(float Vx, float Vy, float Vang, float theta, float *ptr);

/** @brief Convert wheel angular velocities to translational and angular velocity of the robot
  * @input Vx: Velocity of the robot in x direction 
  *        Vy: Velocity of the robot in y direction 
  *        Vang: Angular velocity of the robot 	
	*        theta: Angle between robots frame and world frame
	*        *ptr: Pointer to wheel angular velocities
  * @output None
  */
void convertFromMotorVelToRobotVel(float *Vx, float *Vy, float *Vang, float theta, float *ptr);

/** @brief  Receive data from ST NUCLEO microcontroller
  * @input  None
  * @output None
  * @description Function gets the data from ST NUCLEO microcontroller.
  * Format of the received data is;
  *   |bit 31:20|: empty
  *   |bit 19:8| : heading_data
	*		|bit 7 :0| : bluetooth_data
  */
void OmniControl_getData(void);

/** @brief  Calculate average speed values for debugging and setting PID constants
  * @input  *ptr: Pointer to float (for storing average speed values) 
  * @output None
  */
void getSpeed_Average(float *ptr);

#endif /* OMNICONTROL_H */
