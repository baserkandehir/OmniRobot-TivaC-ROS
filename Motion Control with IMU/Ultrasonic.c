#include "Ultrasonic.h"
#include "tm4c123gh6pm.h"
#include "Systick.h"

// ----------------------------------
//  ID        | TRIG_PIN | ECHO_PIN |
// ----------------------------------
// HC-SR04_1  |  PB2     | PE0      |
// HC-SR04_2  |  PE4     | PB5      |
// ----------------------------------

#define GPIO_PORTB_PB5_M 0x00000020   // PB5 mask
#define GPIO_PORTE_PE0_M 0x00000001   // PE0 mask

ultrasonic_t ult1,ult2;

/** @brief  This routine intitializes HC-SR04 ultrasonic sensor1
  * @input  None
  * @output None
  */
void Ultrasonic1_Init(void)
{
   volatile unsigned long delay;
   SYSCTL_RCGC2_R|=0x00000012;            // activate clock for Port B and Port E 
   delay=SYSCTL_RCGC2_R;                  // dummy delay
   
   /* Make PB2 OUTPUT */
   GPIO_PORTB_CR_R|=0x04;                 // allow changes to PB2   
   GPIO_PORTB_AMSEL_R&=~0x04;             // disable analog func for PB2       
   GPIO_PORTB_PCTL_R&=~0x00000100;        // no alternative func    
   GPIO_PORTB_DIR_R|=0x04;                // make PB2 output
   GPIO_PORTB_AFSEL_R&=~0x04;             // not alternative func   
   GPIO_PORTB_DEN_R|=0x04;                // enable digital I/0 for PB2 
   
   /* PE0 has already been configured as edge interrupt in EdgeInterrupts_PE0_Init() function */
}

/** @brief  This routine intitializes HC-SR04 ultrasonic sensor2
  * @input  None
  * @output None
  */
void Ultrasonic2_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R|=0x00000012;            // activate clock for Port B and Port E 
	delay=SYSCTL_RCGC2_R;                  // dummy delay
	 
	/* Make PE4 OUTPUT */
	GPIO_PORTE_CR_R|=0x10;                 // allow changes to PE4
	GPIO_PORTE_AMSEL_R=0;                  // disable analog functionality on PE
	GPIO_PORTE_PCTL_R&=~0x00010000;        // configure PE4 as GPIO 
	GPIO_PORTE_DIR_R|=0x10;                // make PE4 output
	GPIO_PORTE_AFSEL_R&=~0x10;             // disable alt func on PE4                 
	GPIO_PORTE_DEN_R|=0x10;                // enable digital I/O on PE4
	 
	/* PB5 has already been configured as edge interrupt in EdgeInterrupts_PB5_Init() function */
}

/** @brief  Send trigger to activate first ultrasonic sensor for distance measuring
  * @input  None
  * @output None
  * @description HC-SR04 provides 2cm - 400cm distance measurement. Measuring 400 cm takes nearly 23.5 ms. 
	* When there is no obstacle in front of the sensor within the 400 cm range, echo stays high approx. 200 ms.
  * Triggering sensor while echo is high, does not cause any interrupt on the sensor. Therefore one can
  * call this triggering function every ms or faster. 
  */
void Ultrasonic1_sendTrigger(void)
{
	GPIO_PORTB_DATA_R&=~0x04;  // Trigpin: LOW
	delay_us(2);               // wait 2 us
	GPIO_PORTB_DATA_R|=0x04;   // Trigpin: HIGH
	delay_us(10);              // wait 10 us for triggering
	GPIO_PORTB_DATA_R&=~0x04;  // Trigpin: LOW
}

/** @brief  Send trigger to activate second ultrasonic sensor for distance measuring
  * @input  None
  * @output None
  * @description HC-SR04 provides 2cm - 400cm distance measurement. Measuring 400 cm takes nearly 23.5 ms. 
	* When there is no obstacle in front of the sensor within the 400 cm range, echo stays high approx. 200 ms.
  * Triggering sensor while echo is high, does not cause any interrupt on the sensor. Therefore one can
  * call this triggering function every ms or faster. 
  */
void Ultrasonic2_sendTrigger(void)
{
	GPIO_PORTE_DATA_R&=~0x10;  // Trigpin: LOW
	delay_us(2);               // wait 2 us
	GPIO_PORTE_DATA_R|=0x10;   // Trigpin: HIGH
	delay_us(10);              // wait 10 us for triggering
	GPIO_PORTE_DATA_R&=~0x10;  // Trigpin: LOW 
}

/** @brief  Measure time difference between echo high and low, then calculate distance
  * @input  None
  * @output None
  * @description The speed of sound is 340m/s or 29.41us/cm.
	* Sound wave travels out and back, so in order to find the distance we should divide by 2.
	* Therefore distance(cm)=time(us)/58.82;
  */
void GPIOPortB_UltrasonicTask(void)
{
	if(GPIO_PORTB_RIS_R & GPIO_PORTB_PB5_M)    // PB5 interrupt occurred
	{
		GPIO_PORTB_ICR_R |= GPIO_PORTB_PB5_M ;   // ack flag5
		ult2.flag++;
		
		if(GPIO_PORTB_DATA_R & GPIO_PORTB_PB5_M) // if PB5 is high
			ult2.first_time = Counts;              // measure first time
		else                                     // if PB5 is low
		{
			ult2.second_time = Counts;             // mesaure second time
			ult2.distMeasure = 1;                  
		}
		
		if(ult2.distMeasure == 1)  // calculate distance only after echo pin goes low              
		{
			ult2.change = (ult2.second_time - ult2.first_time) / 1000.0;  // time change in ms
			ult2.dist = (ult2.second_time - ult2.first_time) / 58.82;     // calculate distance in cm
			ult2.distMeasure = 0;
		}
	}
}
	
/** @brief  Measure time difference between echo high and low, then calculate distance
  * @input  None
  * @output None
  * @description The speed of sound is 340m/s or 29.41us/cm.
	* Sound wave travels out and back, so in order to find the distance we should divide by 2.
	* Therefore distance(cm)=time(us)/58.82;
  */
void GPIOPortE_UltrasonicTask(void)
{ 
	if(GPIO_PORTE_RIS_R & GPIO_PORTE_PE0_M)    // PE0 interrupt occurred
	{
		GPIO_PORTE_ICR_R |= GPIO_PORTE_PE0_M ;   // ack flag0
		ult1.flag++;
		
		if(GPIO_PORTE_DATA_R & GPIO_PORTE_PE0_M) // if PE0 is high
			ult1.first_time = Counts;              // measure first time
		else                                     // if PE0 is low
		{
			ult1.second_time = Counts;             // measure second time
			ult1.distMeasure = 1;
		}
		
		if(ult1.distMeasure == 1)  // calculate distance only after echo pin goes low  
		{
			ult1.change = (ult1.second_time - ult1.first_time) / 1000.0; // time change in ms
			ult1.dist = (ult1.second_time - ult1.first_time) / 58.82;    // calculate distance in cm
			ult1.distMeasure = 0;
		}
	} 
}
