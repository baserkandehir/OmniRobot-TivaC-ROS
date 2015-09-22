#ifndef ENCODERS_H
#define ENCODERS_H

#include <stdbool.h>        
#include <math.h>         
#include "OmniStructure.h"  
#include "MacroDefinitions.h"

extern volatile unsigned long long Counts;  // defined in SysTick.c (increments every 1 us)
extern motor_t motor[3];                    // defined in OmniControl.c 
extern volatile unsigned long time[3];      // defined in OmniControl.c 

// -----------------------------------
//       |  Motor1 | Motor2 | Motor3 |
//       -----------------------------
// CHA   |  PB7    | PE5    | PD7    |
// CHB   |  PB6    | PB4    | PD6    |
// -----------------------------------

/** @brief  Detect direction of rotation, count up the quadrature encoder, calculate count of revolution and period
  * @input  Port1: Port of CH1
	*         CH1:   One of the encoder channels to compare and count up/down
	*         Port2: Port of CH2
	*         CH2:   One of the encoder channels to compare and count up/down
	*         *ptr:  Pointer to motor_t
	*         inv:   Determines count and dir data to be inversed or not
  * @output None
	* @description Function compares encoder channel status to get direction of rotation and increments or decrements 
	* the counts accordingly. It checks encoder channel status just after interrupt has occured. If channels are both high 
	* or both low, it decrements the count and sets direction to 0, if one channel is high when the other is low, it 
	* increments the count and sets direction to 1. For the other channel it reverses direction and count increment or 
	* decrement by setting inv variable. Function also calculates count of revolution and period.
  */
void getEncoderData(unsigned long Port1, unsigned long CH1, unsigned long Port2, unsigned long CH2, motor_t *ptr, bool inv);

/* Global functions to be called in handlers */
void GPIOPortB_Task(void);
void GPIOPortD_Task(void);
void GPIOPortE_Task(void);

#endif /* ENCODERS_H */
