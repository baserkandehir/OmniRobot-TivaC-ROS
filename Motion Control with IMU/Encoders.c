#include "tm4c123gh6pm.h"   
#include "EdgeInterrupts.h" 
#include "Encoders.h"

// -----------------------------------
//       |  Motor1 | Motor2 | Motor3 |
//       -----------------------------
// CHA   |  PB7    | PE5    | PD7    |
// CHB   |  PB6    | PB4    | PD6    |
// -----------------------------------

#define GPIO_PORTB_PB4_M 0x00000010   // PB4 mask
#define GPIO_PORTB_PB6_M 0x00000040   // PB6 mask
#define GPIO_PORTB_PB7_M 0x00000080   // PB7 mask
#define GPIO_PORTD_PD6_M 0x00000040   // PD6 mask
#define GPIO_PORTD_PD7_M 0x00000080   // PD7 mask
#define GPIO_PORTE_PE5_M 0x00000020   // PE5 mask
									 
#define ENC1_CHA GPIO_PORTB_PB7_M 
#define ENC1_CHB GPIO_PORTB_PB6_M 
#define ENC2_CHA GPIO_PORTE_PE5_M 
#define ENC2_CHB GPIO_PORTB_PB4_M 
#define ENC3_CHA GPIO_PORTD_PD7_M 
#define ENC3_CHB GPIO_PORTD_PD6_M 

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
void getEncoderData(unsigned long Port1, unsigned long CH1, unsigned long Port2, unsigned long CH2, motor_t *ptr, bool inv)
{
	/* Variables to calculate period */
	static unsigned long time[3] = {0, 0, 0};
	static unsigned long prev_time[3] = {0, 0, 0};
	unsigned long i = 0;
	
	/* Get encoder direction and count */
	if(Port1 & CH1)
	{
		if(Port2 & CH2)
		{
			(!inv) ? (ptr->enc.dir = 0) : (ptr->enc.dir = 1);
			(!inv) ? (ptr->enc.count--) : (ptr->enc.count++);
		}
		else
		{
			(!inv) ? (ptr->enc.dir = 1) : (ptr->enc.dir = 0);
			(!inv) ? (ptr->enc.count++) : (ptr->enc.count--);
		}
	}
	else
	{
		if(Port2 & CH2)
		{
			(!inv) ? (ptr->enc.dir = 1) : (ptr->enc.dir = 0);
			(!inv) ? (ptr->enc.count++) : (ptr->enc.count--);
		}
		else
		{
			(!inv) ? (ptr->enc.dir = 0) : (ptr->enc.dir = 1);
			(!inv) ? (ptr->enc.count--) : (ptr->enc.count++);
		}		
	}
	
	/* Calculate count of revolution */
	ptr->enc.rev = ptr->enc.count / (float)QUAD_CPR;
	
	/* Calculate period */
	for(i = 0; i < 3; i++)
	{
		if(ptr == &motor[i])
		{
			time[i] = Counts;
			ptr->enc.period = (unsigned long)fabs(time[i] - prev_time[i]);
			prev_time[i] = time[i];
			break;
		}
	}
}

void GPIOPortB_Task(void)     
{
	if(GPIO_PORTB_RIS_R & ENC2_CHB)   // enc2 CHB interrupt  
	{
		GPIO_PORTB_ICR_R |= ENC2_CHB;   // acknowledge flag
	  getEncoderData(GPIO_PORTB_DATA_R, ENC2_CHB, GPIO_PORTE_DATA_R, ENC2_CHA, &motor[1], 0);
		time[1] = Counts;
	}
	else if(GPIO_PORTB_RIS_R & ENC1_CHB) // enc1 CHB interrupt 
	{
		GPIO_PORTB_ICR_R |= ENC1_CHB;   // acknowledge flag
		getEncoderData(GPIO_PORTB_DATA_R, ENC1_CHB, GPIO_PORTB_DATA_R, ENC1_CHA, &motor[0], 0);
		time[0] = Counts;
	}
	else if(GPIO_PORTB_RIS_R & ENC1_CHA) // enc1 CHA interrupt 
	{
		GPIO_PORTB_ICR_R |= ENC1_CHA;   // acknowledge flag	
	  getEncoderData(GPIO_PORTB_DATA_R, ENC1_CHB, GPIO_PORTB_DATA_R, ENC1_CHA, &motor[0], 1);  
		time[0] = Counts;		
	}	
}

void GPIOPortD_Task(void)     
{
	if(GPIO_PORTD_RIS_R & ENC3_CHB) // enc3 CHB interrupt
	{
		GPIO_PORTD_ICR_R |= ENC3_CHB;   // acknowledge flag	
    getEncoderData(GPIO_PORTD_DATA_R, ENC3_CHB, GPIO_PORTD_DATA_R, ENC3_CHA, &motor[2], 0); 
		time[2] = Counts;
	}
	else if(GPIO_PORTD_RIS_R & ENC3_CHA) // enc3 CHA interrupt
	{
		GPIO_PORTD_ICR_R |= ENC3_CHA;   // acknowledge flag	
	  getEncoderData(GPIO_PORTD_DATA_R, ENC3_CHB, GPIO_PORTD_DATA_R, ENC3_CHA, &motor[2], 1); 
		time[2] = Counts;
	}
}

void GPIOPortE_Task(void) // enc2 CHA interrupt     
{
	GPIO_PORTE_ICR_R |= ENC2_CHA;   // acknowledge flag	
	getEncoderData(GPIO_PORTB_DATA_R, ENC2_CHB, GPIO_PORTE_DATA_R, ENC2_CHA, &motor[1], 1);
	time[1] = Counts;
}      
