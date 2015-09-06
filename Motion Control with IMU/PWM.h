#ifndef PWM_H
#define PWM_H

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
void PWM_Init(void);

/** @brief  Change duty cycle on PF1
  * @input  PWM values from 0 to 3998
  * @output None
  */
void set_duty1(unsigned long pwm_value);

/** @brief  Change duty cycle on PF2
  * @input  PWM values from 0 to 3998
  * @output None
  */
void set_duty2(unsigned long pwm_value);

/** @brief  Change duty cycle on PF3
  * @input  PWM values from 0 to 3998
  * @output None
  */
void set_duty3(unsigned long pwm_value);

#endif /* PWM_H */

