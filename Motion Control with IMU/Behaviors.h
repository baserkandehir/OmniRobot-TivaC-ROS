#ifndef BEHAVIORS_H
#define BEHAVIORS_H

#include "OmniStructure.h" 
#include "MacroDefinitions.h"
#include <stdbool.h>   
#include <math.h>  

/** @brief  Go to goal behavior from initial point to final point
  * @input  d_goal: Distance to the goal in meters
  *         fi: Angle of direction of robot
  *         t_d : Desired time to reach from initialState to finalState in sec
  * @output None
  */
void GoToGoal(float d_goal,float fi, float t_d);

#endif /* BEHAVIORS_H */
