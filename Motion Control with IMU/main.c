#include "main.h"

int main()
{		
	PLL_Init();                       // 80 MHz system clock
	SysTick_Init(80);                 // 1 us SysTick periodic interrupts 
	PWM_Init();                       // Initialize PF1,PF2 and PF3 for PWM operation
	MotorInput_Init();                // Initialize motor inputs for 3 motors
	UART_Init();                      // Initialize UART4	with 115200 baud rate
	EdgeInterrupts_Init();            // Initialize all available edge interrupts 
  PID_Init(5, 0, 100, 200000);  // Initialize PID s by setting all the PID constants
	OmniControl_Init();               // Timer initializations for PID loops
	Ultrasonic1_Init();               // Intitialize HC-SR04 ultrasonic sensor1
	Ultrasonic2_Init();               // Intitialize HC-SR04 ultrasonic sensor2

	while(1)
	{
		OmniControl(0, 0, 15, 0);
	}	
}
