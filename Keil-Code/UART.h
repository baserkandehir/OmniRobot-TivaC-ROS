#ifndef UART_H
#define UART_H

#include "tm4c123gh6pm.h"  
#include "UART.h"     

/* Standard ASCII symbols */
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

/** @brief  Initialize UART4		
  * @input  None
  * @output None
	* @description Pin C4(receiver) and C5(transmitter) Baud Rate: 115200
	* 8 bit word length (no parity bits, one stop bit, FIFOs)
  */
void UART_Init(void);

/** @brief  Wait for new serial port input 				
  * @input  None
  * @output ASCII code for key typed
  */
unsigned char Receive_Char(void);         

/** @brief  Send 8-bit to serial port				
  * @input  8-bit ASCII character to be transferred
  * @output None
  */
void Transmit_Char(unsigned char data);    

/** @brief  Send unsigned long data			
  * @input  32-bit data to be transferred
  * @output None
  */
void Transmit_Long(unsigned long data);   

/** @brief  Receive unsigned long data			
  * @input  None
  * @output 32-bit unsigned long data
  */
unsigned long Receive_Long(void);

// Sends float numbers over UART
void Transmit_Float(float number);

// Receives float numbers over UART
float Receive_Float(void);

#endif /* UART_H */
