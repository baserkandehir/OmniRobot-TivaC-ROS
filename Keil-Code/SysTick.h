#ifndef SYSTICK_H
#define SYSTICK_H

#include "MacroDefinitions.h"

/** @brief  Initialize Systick periodic interrupts
  * @input  Interrupt period
  * @output None
  */
void SysTick_Init(unsigned long period);

/** @brief  Executed every (Period / CLOCK_FREQ) sec
  * @input  None
  * @output None
  */
void SysTick_Handler(void);

/** @brief  Delay function in seconds
  * @input  Seconds
  * @output None
  */
void delay_sec(unsigned long time);

/** @brief  Delay function in miliseconds
  * @input  Miliseconds
  * @output None
  */
void delay_ms(unsigned long time);

/** @brief  Delay function in microseconds
  * @input  Microseconds
  * @output None
  */
void delay_us(unsigned long time);

/* Functions that are defined in startup.s file */
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

#endif /* SYSTICK_H */
