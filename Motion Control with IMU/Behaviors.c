#include "Behaviors.h"
#include "OmniControl.h"

state_t initial = {0, 0, 0}; // initial state of the robot
state_t final;               // goal state of the robot
state_t desired;             // desired state of the robot calculated from interpolation
state_t integral;
float Kp = 10;
float Ki = 0; 
float t_cur;
float theta_error;   
bool firstTime = 1 ;
float angle_change = 0;

extern volatile float heading;
extern float positionX, positionY, positionTheta; 
extern float wheel_disp[3]; 

/** @brief  Go to goal behavior from initial point to final point
  * @input  d_goal: Distance to the goal in meters
  *         fi: Angle of direction of robot
  *         t_d : Desired time to reach from initialState to finalState in sec
  * @output None
  */
void GoToGoal(float d_goal,float fi, float t_d)
{
	if(firstTime)
	{
		initial.theta = heading;  
		final.theta = initial.theta; 
		final.x = d_goal*cos(fi);
		final.y = d_goal*sin(fi);
		t_cur = 0;
		wheel_disp[0] = 0;
		wheel_disp[1] = 0;
		wheel_disp[2] = 0;
		
		firstTime = 0;
	}

	desired.x = initial.x*(1 - t_cur/t_d) + final.x*(t_cur/t_d);
	desired.y = initial.y*(1 - t_cur/t_d) + final.y*(t_cur/t_d);
	desired.theta = initial.theta*(1 - t_cur/t_d) + final.theta*(t_cur/t_d);

	integral.x += (desired.x - positionX);
	integral.y += (desired.y - positionY);
	integral.theta += (theta_error);
	
	desired.x_dot = Kp*(desired.x - positionX) + Ki*integral.x;
	desired.y_dot = Kp*(desired.y - positionY) + Ki*integral.y;
	
	theta_error = desired.theta - heading;
	theta_error *= PI / 180;                                  // convert from degrees to radians
	theta_error = atan2(sin(theta_error), cos(theta_error));  // ensure that theta_error is between pi and -pi
	theta_error *= 180 / PI;                                  // convert from radians to degrees
	
	desired.theta_dot = Kp*(theta_error) + Ki*integral.theta;  

	OmniControl(desired.x_dot, desired.y_dot, desired.theta_dot);

	if(t_cur < t_d)
		t_cur += TIMER4_dt;
}
