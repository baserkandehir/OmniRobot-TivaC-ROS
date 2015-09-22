#include "SysTick.h"
#include "tm4c123gh6pm.h"

volatile unsigned long long Counts = 0;
float Period; 

/** @brief  Initialize Systick periodic interrupts
  * @input  Interrupt period 
  * @output None
  */
void SysTick_Init(unsigned long period)
{
	Period = period;   // to be used in delay functions
	
	NVIC_ST_CTRL_R = 0;                // disable SysTick during setup
	NVIC_ST_RELOAD_R = period - 1 ;    // reload value (max: 2^24 - 1)
 	NVIC_ST_CURRENT_R = 0;             // any write to current clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;  // priority 2
  NVIC_ST_CTRL_R = 0x07;             // enable SysTick with core clock and interrupts
	
  EnableInterrupts();
}

/** @brief  Executed every (Period / CLOCK_FREQ) sec
  * @input  None
  * @output None
  */
void SysTick_Handler(void)
{
	Counts++;
}

/** @brief  Delay function in seconds
  * @input  Seconds
  * @output None
  */
void delay_sec(unsigned long time)
{
	float curTime;
	curTime = Counts * Period / CLOCK_FREQ;     // read the current time in sec
	while((curTime + time) > (Counts * Period / CLOCK_FREQ));  // wait required amount of time
}

/** @brief  Delay function in miliseconds
  * @input  Miliseconds
  * @output None
  */
void delay_ms(unsigned long time)
{
	float curTime;
	curTime = Counts * Period / CLOCK_FREQ * SEC_TO_MSEC;     // read the current time in msec
	while((curTime + time) > (Counts * Period / CLOCK_FREQ * SEC_TO_MSEC));  // wait required amount of time
}

/** @brief  Delay function in microseconds
  * @input  Microseconds
  * @output None
  */
void delay_us(unsigned long time)
{
	float curTime;
	curTime = Counts * Period / CLOCK_FREQ * SEC_TO_USEC;     // read the current time in usec
	while((curTime + time) > (Counts * Period / CLOCK_FREQ * SEC_TO_USEC));  // wait required amount of time
}
