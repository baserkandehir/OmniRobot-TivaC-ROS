#include "tm4c123gh6pm.h"  
#include "MotorControl.h" 
#include "PWM.h"

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
void MotorInput_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R|=0x0000001E;      // activate clock for Port B,C,D,E
  delay=SYSCTL_RCGC2_R;            // allow time for clock to start

	GPIO_PORTB_CR_R|=0x08;           // allow changes to PB3
	GPIO_PORTB_AMSEL_R&=~0x08;       // disable analog
	GPIO_PORTB_PCTL_R&=~0x08;        // used when there is an alternative function
	GPIO_PORTB_DIR_R|=0x08;          // PB3 Output
	GPIO_PORTB_AFSEL_R&=~0x08;       // disable alternative functions
	GPIO_PORTB_DEN_R|=0x08;          // enable digital I/O 
	
	GPIO_PORTC_CR_R|=0xC0;           // allow changes to PC6-PC7
	GPIO_PORTC_AMSEL_R&=~0xC0;       // disable analog
	GPIO_PORTC_PCTL_R&=~0xC0;        // used when there is an alternative function
	GPIO_PORTC_DIR_R|=0xC0;          // PC6-PC7 Output
	GPIO_PORTC_AFSEL_R&=~0xC0;       // disable alternative functions
	GPIO_PORTC_DEN_R|=0xC0;          // enable digital I/O 
	
	GPIO_PORTD_CR_R|=0x04;           // allow changes to PD2
	GPIO_PORTD_AMSEL_R&=~0x04;       // disable analog
	GPIO_PORTD_PCTL_R&=~0x04;        // used when there is an alternative function
	GPIO_PORTD_DIR_R|=0x04;          // PD2 Output
	GPIO_PORTD_AFSEL_R&=~0x04;       // disable alternative functions
	GPIO_PORTD_DEN_R|=0x04;          // enable digital I/O 
	
	GPIO_PORTE_CR_R|=0x0C;           // allow changes to PE2-PE3
	GPIO_PORTE_AMSEL_R&=~0x0C;       // disable analog
	GPIO_PORTE_PCTL_R&=~0x0C;        // used when there is an alternative function
	GPIO_PORTE_DIR_R|=0x0C;          // PE2-PE3 Output
	GPIO_PORTE_AFSEL_R&=~0x0C;       // disable alternative functions
	GPIO_PORTE_DEN_R|=0x0C;          // enable digital I/O 
	
	/* Motors are initially at rest */	
	set_duty1(0);
	set_duty2(0);
	set_duty3(0);
}

/** @brief  Motor1 Control Function				
  * @input  Speed[0,3998] and direction(1 or 0)
  * @output None
  */
void Motor1(unsigned long speed, bool direction)
{
	if(direction)
	{
		GPIO_PORTB_DATA_R&=~0x08;   // PB3 0   
		GPIO_PORTD_DATA_R|=0x04;    // PD2 1
	}
	else
	{
		GPIO_PORTB_DATA_R|=0x08;    // PB3 1
		GPIO_PORTD_DATA_R&=~0x04;   // PD2 0	
	}	
	set_duty3(speed);
}

/** @brief  Motor2 Control Function				
  * @input  Speed[0,3998] and direction(1 or 0)
  * @output None
  */
void Motor2(unsigned long speed, bool direction)
{
  if(direction)
	{
		GPIO_PORTE_DATA_R|=0x04;    // PE2 1
		GPIO_PORTE_DATA_R&=~0x08;   // PE3 0		
	}
	else
	{
		GPIO_PORTE_DATA_R&=~0x04;   // PE2 0 
		GPIO_PORTE_DATA_R|=0x08;    // PE3 1
	}	
	set_duty1(speed);
}

/** @brief  Motor3 Control Function				
  * @input  Speed[0,3998] and direction(1 or 0)
  * @output None
  */
void Motor3(unsigned long speed, bool direction)
{
	if(direction)
	{
		GPIO_PORTC_DATA_R|=0x80;   // PC7 1
		GPIO_PORTC_DATA_R&=~0x40;  // PC6 0
	}
	else
	{
		GPIO_PORTC_DATA_R&=~0x80;  // PC7 0 
		GPIO_PORTC_DATA_R|=0x40;   // PC6 1
	}	
	set_duty2(speed);
}
