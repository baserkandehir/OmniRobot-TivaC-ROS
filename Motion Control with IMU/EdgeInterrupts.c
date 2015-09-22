#include "tm4c123gh6pm.h"   
#include "EdgeInterrupts.h" 

// -----------------------------------
//       |  Motor1 | Motor2 | Motor3 |
//       -----------------------------
// CHA   |  PB7    | PE5    | PD7    |
// CHB   |  PB6    | PB4    | PD6    |
// -----------------------------------

/** @brief  Initialize all available edge interrupts 
  * @input  None
  * @output None
  */
void EdgeInterrupts_Init(void)
{	
	EdgeInterrupts_PB6_Init();
	EdgeInterrupts_PD6_Init();
	EdgeInterrupts_PE5_Init();
	EdgeInterrupts_PB4_Init();
	EdgeInterrupts_PB7_Init();
	EdgeInterrupts_PD7_Init();

	EnableInterrupts();   // Clears the I bit 
}

/** @brief  PB6 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PB6_Init(void)
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R|=0x00000002;            // activate clock for port B
  delay=SYSCTL_RCGC2_R;                  // dummy delay
	GPIO_PORTB_DIR_R&=~0x40;               // make PB6 input
	GPIO_PORTB_AFSEL_R&=~0x40;             // disable alt funct on PB6
	GPIO_PORTB_DEN_R|=0x40;                // enable digital I/O on PB6   
	GPIO_PORTB_PCTL_R&=~0x0F000000;        // configure PB6 as GPIO
	GPIO_PORTB_AMSEL_R= 0;                 // disable analog functionality on PB
	GPIO_PORTB_PUR_R|=0x40;                // enable weak pull-up on PB6
	GPIO_PORTB_IS_R&=~0x40;                // PB6 is edge-sensitive
	GPIO_PORTB_IBE_R|=0x40;                // PB6 is both edges
	GPIO_PORTB_ICR_R|=0x40;                // clear flag6
	GPIO_PORTB_IM_R|=0x40;                 // arm interrupt on PB6
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF)|0x00006000; // priority 3
	NVIC_EN0_R|= 0x00000002;               // enable interrupt 1 in NVIC (GPIO PORTB: Interrupt number: 1)
}

/** @brief  PD6 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PD6_Init(void)
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R|=0x00000008;            // activate clock for port D
  delay=SYSCTL_RCGC2_R;                  // dummy delay
  GPIO_PORTD_DIR_R&=~0x40;               // make PD6 input
  GPIO_PORTD_AFSEL_R&=~0x40;             // disable alt funct on PD6
  GPIO_PORTD_DEN_R|=0x40;                // enable digital I/O on PD6   
  GPIO_PORTD_PCTL_R&=~0x0F000000;        // configure PD6 as GPIO
  GPIO_PORTD_AMSEL_R= 0;                 // disable analog functionality on PD
  GPIO_PORTD_PUR_R|=0x40;                // enable weak pull-up on PD6
  GPIO_PORTD_IS_R&=~0x40;                // PD6 is edge-sensitive
  GPIO_PORTD_IBE_R|=0x40;                // PD6 is both edges
  GPIO_PORTD_ICR_R|=0x40;                // clear flag6
  GPIO_PORTD_IM_R|=0x40;                 // arm interrupt on PD6
  NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF)|0x60000000; // priority 3
	NVIC_EN0_R|= 0x00000008;               // enable interrupt 3 in NVIC (GPIO PORTD: Interrupt number: 3)
}

/** @brief  PE5 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PE5_Init(void)
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R|=0x00000010;            // activate clock for port E
  delay=SYSCTL_RCGC2_R;                  // dummy delay
	GPIO_PORTE_DIR_R&=~0x20;               // make PE5 input
	GPIO_PORTE_AFSEL_R&=~0x20;             // disable alt funct on PE5
	GPIO_PORTE_DEN_R|=0x20;                // enable digital I/O on PE5   
	GPIO_PORTE_PCTL_R&=~0x00F00000;        // configure PE5 as GPIO
	GPIO_PORTE_AMSEL_R= 0;                 // disable analog functionality on PE
	GPIO_PORTE_PUR_R|=0x20;                // enable weak pull-up on PE5
	GPIO_PORTE_IS_R&=~0x20;                // PE5 is edge-sensitive
	GPIO_PORTE_IBE_R|=0x20;                // PE5 is both edges
	GPIO_PORTE_ICR_R|=0x20;                // clear flag5
	GPIO_PORTE_IM_R|=0x20;                 // arm interrupt on PE5
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00)|0x00000060; // priority 3
  NVIC_EN0_R|= 0x00000010;               // enable interrupt 4 in NVIC (GPIO PORTE: Interrupt number: 4)
}

/** @brief  PB4 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PB4_Init(void)
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R|=0x00000002;            // activate clock for port B
  delay=SYSCTL_RCGC2_R;                  // dummy delay
	GPIO_PORTB_DIR_R&=~0x10;               // make PB4 input
	GPIO_PORTB_AFSEL_R&=~0x10;             // disable alt func on PB4
	GPIO_PORTB_DEN_R|=0x10;                // enable digital I/0 on PB4
	GPIO_PORTB_PCTL_R&=~0x000F0000;        // configure PB4 as GPI0
	GPIO_PORTB_AMSEL_R= 0;                 // disable analog functionality on PB
	GPIO_PORTB_PUR_R|=0x10;                // enable weak pull-up on PB4
	GPIO_PORTB_IS_R&=~0x10;                // PB4 is edge-sensitive
	GPIO_PORTB_IBE_R|=0x10;                // PB4 is both edges
	GPIO_PORTB_ICR_R|=0x10;                // clear flag4
	GPIO_PORTB_IM_R|=0x10;                 // arm interrupt on PB4
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF)|0x00006000; // priority 3
	NVIC_EN0_R|= 0x00000002;               // enable interrupt 1 in NVIC (GPIO PORTB: Interrupt number: 1)
}

/** @brief  PB7 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PB7_Init(void)
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R|=0x00000002;            // activate clock for port B
	delay=SYSCTL_RCGC2_R;                  // dummy delay
	GPIO_PORTB_DIR_R&=~0x80;               // make PB7 input
	GPIO_PORTB_AFSEL_R&=~0x80;             // disable alt func on PB7
	GPIO_PORTB_DEN_R|=0x80;                // enable digital I/O on PB7
	GPIO_PORTB_PCTL_R&=~0xF0000000;        // configure PB7 as GPIO
	GPIO_PORTB_AMSEL_R= 0;                 // disable analog functionality on PB
	GPIO_PORTB_PUR_R|=0x80;                // enable weak pull-up on PB7
	GPIO_PORTB_IS_R&=~0x80;                // PB7 is edge-sensitive
	GPIO_PORTB_IBE_R|=0x80;                // PB7 is both edges
	GPIO_PORTB_ICR_R|=0x80;                // clear flag7
	GPIO_PORTB_IM_R|=0x80;                 // arm interrupt on PB7
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF)|0x00006000; // priority 3
	NVIC_EN0_R|= 0x00000002;               // enable interrupt 1 in NVIC (GPIO PORTB: Interrupt number: 1)
}

/** @brief  PD7 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PD7_Init(void)
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R|=0x00000008;            // activate clock for port D
	delay=SYSCTL_RCGC2_R;                  // dummy delay
	GPIO_PORTD_LOCK_R=0x4C4F434B;          // unlock GPIO PORTD (only PD7 needs to be unlocked) 
	GPIO_PORTD_CR_R|=0x80;                 // allow changes on PD7
	GPIO_PORTD_DIR_R&=~0x80;               // make PD7 input
	GPIO_PORTD_AFSEL_R&=~0x80;             // disable alt func on PD7
	GPIO_PORTD_DEN_R|=0x80;                // enable digital I/O on PD7
	GPIO_PORTD_PCTL_R&=~0xF0000000;        // configure PD7 as GPIO
	GPIO_PORTD_AMSEL_R= 0;                 // disable analog functionality on PD
	GPIO_PORTD_PUR_R|=0x80;                // enable weak pull-up on PD7
	GPIO_PORTD_IS_R&=~0x80;                // PD7 is edge-sensitive
	GPIO_PORTD_IBE_R|=0x80;                // PD7 is both edges
	GPIO_PORTD_ICR_R|=0x80;                // clear flag7
  GPIO_PORTD_IM_R|=0x80;                 // arm interrupt on PD7
	NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF)|0x60000000; // priority 3
	NVIC_EN0_R|= 0x00000008;               // enable interrupt 3 in NVIC (GPIO PORTD: Interrupt number: 3)
}

void GPIOPortB_Handler(void)     
{
	GPIOPortB_Task();
}

void GPIOPortD_Handler(void)     
{
	GPIOPortD_Task();
}

void GPIOPortE_Handler(void)     
{
	GPIOPortE_Task();
}
