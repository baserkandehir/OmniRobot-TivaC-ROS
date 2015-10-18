#ifndef BEHAVIORS_H
#define BEHAVIORS_H

#include "OmniStructure.h" 
#include "MacroDefinitions.h"
#include <stdbool.h>   
#include <math.h>  

typedef enum 
{ left, 
	right
}direction_t;

/** @brief  Go to goal behavior from initial point to final point
  * @input  d_goal: Distance to the goal in meters
  *         fi: Angle of direction of robot
  *         t_d : Desired time to reach from initialState to finalState in sec
  * @output None
  */
void GoToGoal(float d_goal,float fi, float t_d);

/** @brief  Wall following behavior
  * @input  dir: The direction of wall following
  * @output None
  */
void FollowWall(direction_t dir);

#endif /* BEHAVIORS_H */
