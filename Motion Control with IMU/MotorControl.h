#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <stdbool.h>

// -----------------------------------
//       |  Motor1 | Motor2 | Motor3 |
//       -----------------------------
// PWM   |  PF3    | PF1    | PF2    | 
// IN1   |  PB3    | PE2    | PC6    |
// IN2   |  PD2    | PE3    | PC7    |
// CHA   |  PB7    | PE5    | PD7    |
// CHB   |  PB6    | PB4    | PD6    |
// -----------------------------------

/** @brief  Initialize motor inputs for 3 motors
  *         Outputs: PB3 PC6 PC7 PD2 PE2 PE3 				
  * @input  None
  * @output None
  */
void MotorInput_Init(void);

/** @brief  Motor1 Control Function				
  * @input  Speed[0,3998] and direction(1 or 0)
  * @output None
  */
void Motor1(unsigned long speed, bool direction);

/** @brief  Motor2 Control Function				
  * @input  Speed[0,3998] and direction(1 or 0)
  * @output None
  */
void Motor2(unsigned long speed, bool direction);

/** @brief  Motor3 Control Function				
  * @input  Speed[0,3998] and direction(1 or 0)
  * @output None
  */
void Motor3(unsigned long speed, bool direction);

#endif /* MOTORCONTROL_H */
