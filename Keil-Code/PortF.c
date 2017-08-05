#include "tm4c123gh6pm.h"   
#include "PortF.h"         

/** @brief  Initialize PortF  (PF0 - PF4 as Input and PF1 - PF2 - PF3 as Output) 			
  * @input  None
  * @output None
  */
void PortF_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R|=0x00000020;      // activate clock for Port F
	delay=SYSCTL_RCGC2_R;            // allow time for clock to start
	GPIO_PORTF_LOCK_R=0x4C4F434B;    // unlock GPIO PORTF   (only PF0 needs to be unlocked other bits cant be locked)
	GPIO_PORTF_CR_R|=0x1F;           // allow changes to PF4-0
	GPIO_PORTF_AMSEL_R=0x00;         // disable analog
	GPIO_PORTF_PCTL_R=0;             // used when there is an alternative function
	GPIO_PORTF_DIR_R=0x0E;           // PF4-0 INPUT PF1 2 3 OUTPUT
	GPIO_PORTF_AFSEL_R=0x00;         // disable alternative functions
 	GPIO_PORTF_PUR_R=0x11;           // enable pull-up on PF4 and PF0
	GPIO_PORTF_DEN_R=0x1F;           // enable digital I/O on Port F
}
