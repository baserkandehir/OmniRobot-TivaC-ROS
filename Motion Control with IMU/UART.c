#include "tm4c123gh6pm.h"  
#include "UART.h"         

/** @brief  Initialize UART4		
  * @input  None
  * @output None
	* @description Pin C4(receiver) and C5(transmitter) Baud Rate: 115200
	* 8 bit word length (no parity bits, one stop bit, FIFOs)
  */
void UART_Init(void)
{
	SYSCTL_RCGCUART_R|=0x00000010 ;  // activate UART4 for Pin C4(receiver) and C5(transmitter)
	SYSCTL_RCGC2_R|=0x00000004;      // activate clock for Port C
	UART4_CTL_R&=~0x00000001;        // disable UART
	UART4_IBRD_R=520;                 // IBRD=int(80000000/(16*9600))=int(520,83)
	UART4_FBRD_R=54;                 // FBRD=int(64*0.83+0.5)=53.62
	UART4_LCRH_R|=0x70;              // 8 bit word length (no parity bits, one stop bit, FIFOs)
	UART4_CTL_R|=0x00000001;         // enable UART
	
	GPIO_PORTC_AFSEL_R|=0x30;        // enable alternative function on PC4-5
  GPIO_PORTC_DEN_R|=0x30;          // enable digital I/O on PC4-5
	
  GPIO_PORTC_PCTL_R|=(GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00110000;	
	GPIO_PORTC_AMSEL_R&=0x30;        // disable analog functionality on PC4-5
}

/** @brief  Wait for new serial port input 				
  * @input  None
  * @output ASCII code for key typed
  */
unsigned char Receive_Char(void)         
{
	while((UART4_FR_R&0x10)!=0); // Wait for RXFE to be zero. If it is zero, that means FIFO is not empty so we can read data from FIFO.
	return((unsigned char)(UART4_DR_R&0xFF));
}

/** @brief  Send 8-bit to serial port				
  * @input  8-bit ASCII character to be transferred
  * @output None
  */
void Transmit_Char(unsigned char data)    
{
	 while((UART4_FR_R&0x20)!=0); // Wait for TXFF to be zero. If it is zero, that means FIFO is empty therefore we can write data to FIFO.
	 UART4_DR_R = data;
}


/** @brief  Send unsigned long data			
  * @input  32-bit data to be transferred
  * @output None
  */
void Transmit_Long(unsigned long data)    
{
	unsigned long count=0;
	unsigned long buffer[100]={0};
	
	while(data>0)
	{
		buffer[count]=data%10;
		data=data/10;
		count++;
	}
	for(;count;count--)
	{
		Transmit_Char(buffer[count-1]+'0');
	}
}

/** @brief  Receive unsigned long data			
  * @input  None
  * @output 32-bit unsigned long data
  */
unsigned long Receive_Long(void)
{
	unsigned long number = 0;
	char incomingData;
	
	incomingData = Receive_Char();
	while(incomingData != CR)   // accepts until /r is typed
	{
		if((incomingData >= '0') && (incomingData <= '9')) // if incomingData is not a digit, ignore it
		{
			number = 10*number + (incomingData - '0'); // construct the number
		}
		incomingData = Receive_Char();
	}
	return number;
}
