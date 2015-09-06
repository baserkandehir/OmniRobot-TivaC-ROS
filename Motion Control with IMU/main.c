#include "main.h"

extern float d_goal, fi, t_d;
unsigned long i;
extern bool firstTime;

int main()
{		
	PLL_Init();                       // 80 MHz system clock
	SysTick_Init(80);                 // 1 us SysTick periodic interrupts 
	PWM_Init();                       // Initialize PF1,PF2 and PF3 for PWM operation
	MotorInput_Init();                // Initialize motor inputs for 3 motors
	UART_Init();                      // Initialize UART4	with 115200 baud rate
	EdgeInterrupts_Init();            // Initialize all available edge interrupts 
  PID_Init(5, 0.001, 100, 200000);  // Initialize PID s by setting all the PID constants
	OmniControl_Init();               // Timer initializations for PID loops

	while(1)
	{
		for(i = 0; i < 4; i++)
		{
			d_goal = 1;
			fi = i*PI/2;
			t_d = 4;
			delay_sec(t_d+0.5f);
			firstTime = 1;
		}	
	}	
}
