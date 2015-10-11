#include "OmniControl.h"
#include "Timers.h"         
#include "PID.h"            
#include "MotorControl.h"  
#include "UART.h" 
#include "SysTick.h" 
#include "Behaviors.h"
#include "Ultrasonic.h" 

motor_t motor[3];                            // initially, compiler clears all the variables to 0 
volatile unsigned long time[3] = {0, 0, 0};  // Variables to reduce period to zero when motors are not rotating
volatile float heading = 0;                  // heading angle of the robot
float wheel_disp[3] = {0,0,0};               // total displacement made by wheels
float positionX = 0, positionY = 0, positionTheta = 0;

float d_goal, fi, t_d;

float maxFunc(float x, float y)
{
	return (x>y) ? x : y;
}

void (*motorFunc_ptr[3])(unsigned long, bool) = {Motor1, Motor2, Motor3}; // array of function pointers to simplify code

/** @brief  Timer tasks to be called in timer handlers
  * @input  timerNum: Determines which timer function to be selected
  * @output None
  */
void Timer_Task(unsigned long timerNum)
{
	PID(&motor[timerNum], motorFunc_ptr[timerNum], motor[timerNum].enc.period);

	/* Reduce period to zero when motors are not rotating  */
	if(Counts - time[timerNum] > 25000)  // if motor is not rotating since 25 ms
	{
		motor[timerNum].enc.period = 0;    // period = 0 then speed = 0
	}
	
	/* Wheel displacements in meters */
	if(motor[timerNum].enc.dir == 1)
		wheel_disp[timerNum] += (motor[timerNum].speed/MIN_TO_SEC)*(2*PI*R)*MOTOR_PID_dt;
	else
		wheel_disp[timerNum] -= (motor[timerNum].speed/MIN_TO_SEC)*(2*PI*R)*MOTOR_PID_dt;
}

void Timer0_Task(void){Timer_Task(0);}
void Timer1_Task(void){Timer_Task(1);}
void Timer2_Task(void){Timer_Task(2);}

void Timer3_Task(void)	
{
	OmniControl_getData();
}

void Timer4_Task(void)	
{/*
	GoToGoal(d_goal, fi, t_d);	
*/
	
	Ultrasonic1_sendTrigger();
	Ultrasonic2_sendTrigger();
	
	/* Calculate pose of the robot with respect to its starting point where position is in meters and theta is in deg/s */
	convertFromMotorVelToRobotVel(&positionX, &positionY, &positionTheta, THETA, wheel_disp);
}

/** @brief  Timer initializations for PID loops
  * @input  None
  * @output None
  */
void OmniControl_Init(void)
{
	Timer0_Init(&Timer0_Task, 80000);      // timer period: 1ms
  Timer1_Init(&Timer1_Task, 80000);      // timer period: 1ms
	Timer2_Init(&Timer2_Task, 80000);      // timer period: 1ms
	Timer3_Init(&Timer3_Task, 80000);      // timer period: 1ms
	Timer4_Init(&Timer4_Task, 6*80000);    // timer period: 6ms
}

/** @brief Set desired speed and direction of a motor
  * @input *ptr: Pointer to motor_t
  *        set_point: Desired speed value
  *        dir: Direction of rotation
  * @output None
  */
void OmniControl_setSpeedDir(motor_t *ptr, volatile float set_point, bool dir)
{
  ptr->pid.set_point = set_point;
	ptr->dir = dir;
}

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
void OmniControl(float Vx, float Vy, float Vang, float theta)
{               
	float w[3] = {0, 0, 0};           // angular velocity of each wheel
 	static bool dir[3] = {0, 0, 0};   // direction of rotation of each wheel
	unsigned long i;
	float maxAng;
	
	/* Conversion of Vang from deg/s to rad/sec */
	Vang /= RAD_TO_DEG;
	
	convertFromRobotVelToMotorVel(Vx, Vy, Vang, THETA+theta, w); 
	
	/* Conversion from rad/sec to rev/min */
	for(i = 0; i < 3; i++)
		w[i] = w[i] / (2*PI) * MIN_TO_SEC;
	
	/* Ensure the required motion by scaling down the angular velocities of the motors */
	if((w[0]>MAX_SPEED) || (w[1]>MAX_SPEED) || (w[2]>MAX_SPEED))  // if any of the required angular vel. is greater than max angular vel.
	{
		maxAng = maxFunc(maxFunc(w[0],w[1]),w[2]);             // determine which angular vel. is max
		for(i = 0; i < 3; i++) w[i] /= (maxAng / MAX_SPEED);   // scale down all the angular velocities
	}
	
	/* Set speed and direction of motors */
	for(i = 0; i < 3; i++)
	{
		if(w[i] > 0)
			dir[i] = 1;
		else if(w[i] < 0)
		{
			dir[i] = 0;
			w[i] = w[i] * (-1);
		}
		OmniControl_setSpeedDir(&motor[i], w[i], dir[i]);
	}
}

/** @brief Convert translational and angular velocity of the robot to wheel angular velocities  
  * @input Vx: Velocity of the robot in x direction 
  *        Vy: Velocity of the robot in y direction 
  *        Vang: Angular velocity of the robot 	
	*        theta: Angle between robots frame and world frame
	*        *ptr: Pointer to wheel angular velocities
  * @output None
  */
void convertFromRobotVelToMotorVel(float Vx, float Vy, float Vang, float theta, float *ptr)
{
	theta = theta / RAD_TO_DEG;
	
	ptr[1] = (-sinf(theta)        * Vx + cosf(theta)         * Vy + L * Vang) / R;  
	ptr[2] = (-sinf(PI/3 - theta) * Vx - cosf(PI/3 - theta)  * Vy + L * Vang) / R;
	ptr[0] = (sinf(PI/3 + theta)  * Vx + -cosf(PI/3 + theta) * Vy + L * Vang) / R;
}

/** @brief Convert wheel angular velocities to translational and angular velocity of the robot
  * @input Vx: Velocity of the robot in x direction 
  *        Vy: Velocity of the robot in y direction 
  *        Vang: Angular velocity of the robot 	
	*        theta: Angle between robots frame and world frame
	*        *ptr: Pointer to wheel angular velocities
  * @output None
  */
void convertFromMotorVelToRobotVel(float *Vx, float *Vy, float *Vang, float theta, float *ptr)
{
	theta = theta / RAD_TO_DEG;
	
	*Vx = (-0.667f)*sinf(theta) * ptr[1] + (-0.667f)*sinf(PI/3 - theta) * ptr[2] + (0.667f)*sinf(PI/3 + theta) * ptr[0];
	*Vy = (0.667f)*cosf(theta)  * ptr[1] + (-0.667f)*cosf(PI/3 - theta) * ptr[2] + (-0.667f)*cosf(PI/3 + theta)* ptr[0];                  
	*Vang = (ptr[1] + ptr[2] + ptr[0]) / (3*L) * RAD_TO_DEG;
}

/** @brief  Receive data from ST NUCLEO microcontroller
  * @input  None
  * @output None
  * @description Function gets the data from ST NUCLEO microcontroller.
  * Format of the received data is;
  *   |bit 31:20|: empty
  *   |bit 19:8| : heading_data   (heading * 10)
	*		|bit 7 :0| : bluetooth_data (incoming_data)
  */
void OmniControl_getData(void)
{
	unsigned long received_data = 0;
	unsigned char incoming_data = 0;
	static float speed = 0;  // previous speed value will be used in OmniControl func.
	
	received_data = Receive_Long();
	incoming_data = (received_data % 256);
	heading = ((received_data - incoming_data) >> 8 ) / 10.0;
	heading = heading - 180;
	
	/* Adjust the speed	*/
	if((incoming_data >= '0') && (incoming_data <= '9'))
		speed = (incoming_data - '0') / 9.0 * MAX_SPEED * R;
	
	switch(incoming_data)
	{
		case 'F': OmniControl(speed, 0, 0, 0);       // go forward
			break;
		case 'B': OmniControl((-1)*speed, 0, 0, 0);  // go backward
			break;
		case 'L': OmniControl(0, (-1)*speed, 0, 0);  // go left
			break;
		case 'R': OmniControl(0, speed, 0, 0);       // go right
			break;
		
		case 'G': 
		case 'H': OmniControl(0, 0, (-1)*speed/L, 0);   // turn around in anti-clockwise direction
			break;
		case 'I': 
		case 'J': OmniControl(0, 0, speed/L, 0);        // turn around in clockwise direction	
			break;
		
		case 'S': OmniControl(0, 0, 0, 0); // stop
			break;
	}
}

/** @brief  Calculate average speed values for debugging and setting PID constants
  * @input  *ptr: Pointer to float (for storing average speed values) 
  * @output None
  */
void getSpeed_Average(float *ptr)
{
	unsigned long j, k, numberOfSamples = 50;
	static float rpm_array[3][50];
	static unsigned long t = 0;
	float total[3];
		
	for(k = 0; k < 3; k++)
	{
		rpm_array[k][t] = motor[k].speed;
	
		for(j = 0; j < numberOfSamples ; j++)
			total[k] += rpm_array[k][j];
		
		ptr[k] = total[k] / numberOfSamples;
		total[k] = 0;
  }
	t++; t%=numberOfSamples;
	delay_ms(1);
}
