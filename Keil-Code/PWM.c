#include "tm4c123gh6pm.h"   
#include "PWM.h"

/** @brief  Initialize PF1,PF2 and PF3 for PWM operation
  * @input  None
  * @output None
  * @description
	* PWM module 1, Generator block 2 , PWMB PWM5 (PF1) with %100 duty cycle
	* PWM module 1, Generator block 3 , PWMA PWM6 (PF2) with %50 duty cycle
	* PWM module 1, Generator block 3 , PWMB PWM7 (PF3) with %75 duty cylce
	* 10 kHz pwm freq = 100 us period
	* 80 MHz/2 = 40 MHz clock = 25 ns
	* That corresponds to 4000 ticks per period
	* This value will be used in PWM3LOAD(also PWM2LOAD) register
	*/
void PWM_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGCPWM_R|=0x00000002;    // enable the PWM module 1 clock
	delay=SYSCTL_RCGCPWM_R; 
	SYSCTL_RCGC2_R|=0x20;            // enable clock for Port F
	delay=SYSCTL_RCGC2_R;
	
	GPIO_PORTF_LOCK_R=0x4C4F434B;    // unlock GPIO PORTF   (only PF0 needs to be unlocked other bits cant be locked)
	GPIO_PORTF_CR_R|=0x1F;           // allow changes to PF4-0
	GPIO_PORTF_DIR_R=0x0E;           // PF4-0 INPUT PF1 2 3 OUTPUT
	GPIO_PORTF_AFSEL_R|=0x0E;        // enable alt func for PF1,2,3   //IMPORTANT
	GPIO_PORTF_PCTL_R|=0x5550;       // pin mux=5 
	GPIO_PORTF_AMSEL_R&=~0x1F;       // disable analog
	GPIO_PORTF_PUR_R|=0x11;          // enable pull-up on PF4 and PF0
  GPIO_PORTF_DEN_R|=0x1F;          // enable digital I/O on Port F
	
	SYSCTL_RCC_R|=0x00100000;        // use pwm divide
	SYSCTL_RCC_R&=~0x000E0000;
  SYSCTL_RCC_R+=0x00000000;        // set the divider (PWMDIV) to divide by 2 (000).  80MHz/2= 40 MHz
	
	// Initializes PF2 and PF3 for PWM operation
	PWM1_3_CTL_R=0;                 // stop all the timers
	PWM1_3_GENA_R|=0x000000C8;      // Action for Comparator A Down: Drive pwmA High. Action for Counter=LOAD: Drive pwmA Low
	PWM1_3_GENB_R=0x00000C08;       // Action for Comparator B Down: Drive pwmB High. Action for Counter=LOAD: Drive pwmA Low
	PWM1_3_LOAD_R|=4000-1;          // 4000 ticks per period (calculated above)
	PWM1_3_CMPA_R|=2000-1;          // %50 duty cycle on PF2
  PWM1_3_CMPB_R=3000-1;           // %75 duty cycle on PF3
  PWM1_3_CTL_R|=0x01;             // start timers  Important note:(PWM1_CTL_R is different from PWM1_3_CTL_R)
	PWM1_ENABLE_R|=0xC0;            // enable PWM6 and PWM7 
	
	// Initializes PF1 for PWM operation
	PWM1_2_CTL_R=0;                 // stop all the timers
	PWM1_2_GENB_R=0x00000C08;       // Action for Comparator B Down: Drive pwmB High. Action for Counter=LOAD: Drive pwmA Low
	PWM1_2_LOAD_R|=4000-1;          // 4000 ticks per period (calculated above)
  PWM1_2_CMPB_R=3999-1;           // %100 duty cycle on PF1
  PWM1_2_CTL_R|=0x01;             // start timers  Important note:(PWM1_CTL_R is different from PWM1_3_CTL_R)
	PWM1_ENABLE_R|=0x20;            // enable PWM5 
}

/** @brief  Change duty cycle on PF1
  * @input  PWM values from 0 to 3998
  * @output None
  */
void set_duty1(unsigned long pwm_value)
{
	PWM1_2_CMPB_R = pwm_value;
}

/** @brief  Change duty cycle on PF2
  * @input  PWM values from 0 to 3998
  * @output None
  */
void set_duty2(unsigned long pwm_value)
{
	PWM1_3_CMPA_R = pwm_value;
}

/** @brief  Change duty cycle on PF3
  * @input  PWM values from 0 to 3998
  * @output None
  */
void set_duty3(unsigned long pwm_value)
{
	PWM1_3_CMPB_R = pwm_value;
}
