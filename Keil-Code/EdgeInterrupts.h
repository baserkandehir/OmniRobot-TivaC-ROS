#ifndef EDGEINTERRUPTS_H
#define EDGEINTERRUPTS_H

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
void EdgeInterrupts_Init(void);

/** @brief  PB6 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PB6_Init(void);

/** @brief  PD6 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PD6_Init(void);

/** @brief  PE5 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PE5_Init(void);

/** @brief  PB4 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PB4_Init(void);

/** @brief  PB7 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PB7_Init(void);

/** @brief  PD7 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PD7_Init(void);

/** @brief  PB5 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PB5_Init(void);

/** @brief  PE0 interrupt initialization on both edges
  * @input  None
  * @output None
  */
void EdgeInterrupts_PE0_Init(void);

/* Interrupt handlers */
void GPIOPortB_Handler(void);     
void GPIOPortD_Handler(void);    
void GPIOPortE_Handler(void);   

/* Global functions to be called in handlers */
void GPIOPortB_Task(void);
void GPIOPortD_Task(void);
void GPIOPortE_Task(void);
void GPIOPortB_UltrasonicTask(void);
void GPIOPortE_UltrasonicTask(void);

/* Functions that are defined in startup.s file */
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

#endif /* EDGEINTERRUPTS_H */
