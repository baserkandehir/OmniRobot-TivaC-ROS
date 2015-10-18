#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdbool.h> 

extern volatile unsigned long long Counts;

typedef volatile struct
{
	float dist;        // distance measured from ultrasonic sensor
	float change;      // total echo high time in ms
	unsigned long long first_time, second_time, high_time;
	unsigned long flag;
	bool distMeasure;  
	bool done;         // true after distance measurement completed
}ultrasonic_t;

/** @brief  This routine intitializes HC-SR04 ultrasonic sensor1
  * @input  None
  * @output None
  */
void Ultrasonic1_Init(void);

/** @brief  This routine intitializes HC-SR04 ultrasonic sensor2
  * @input  None
  * @output None
  */
void Ultrasonic2_Init(void);

/** @brief  Send trigger to activate first ultrasonic sensor for distance measuring
  * @input  None
  * @output None
  * @description HC-SR04 provides 2cm - 400cm distance measurement. Measuring 400 cm takes nearly 23.5 ms. 
	* When there is no obstacle in front of the sensor within the 400 cm range, echo stays high approx. 200 ms.
  * Triggering sensor while echo is high, does not cause any interrupt on the sensor. Therefore one can
  * call this triggering function every ms or faster. 
  */
void Ultrasonic1_sendTrigger(void);

/** @brief  Send trigger to activate second ultrasonic sensor for distance measuring
  * @input  None
  * @output None
  * @description HC-SR04 provides 2cm - 400cm distance measurement. Measuring 400 cm takes nearly 23.5 ms. 
	* When there is no obstacle in front of the sensor within the 400 cm range, echo stays high approx. 200 ms.
  * Triggering sensor while echo is high, does not cause any interrupt on the sensor. Therefore one can
  * call this triggering function every ms or faster. 
  */
void Ultrasonic2_sendTrigger(void);

/* Global functions to be called in handlers */

/** @brief  Measure time difference between echo high and low, then calculate distance
  * @input  None
  * @output None
  * @description The speed of sound is 340m/s or 29.41us/cm.
	* Sound wave travels out and back, so in order to find the distance we should divide by 2.
	* Therefore distance(cm)=time(us)/58.82;
  */
void GPIOPortB_UltrasonicTask(void);
	
/** @brief  Measure time difference between echo high and low, then calculate distance
  * @input  None
  * @output None
  * @description The speed of sound is 340m/s or 29.41us/cm.
	* Sound wave travels out and back, so in order to find the distance we should divide by 2.
	* Therefore distance(cm)=time(us)/58.82;
  */
void GPIOPortE_UltrasonicTask(void);

/** @brief  This routine controls ultrasonic sensor1's power
  * @input  on: Determines if ultrasonic sensor is on or off
  * @output None
  * @description This routine is neccessary because HC-SR04 is lack of timeout and echo stucks at high after some time.
  */
void Ultrasonic1_power(bool on);

/** @brief  This routine controls ultrasonic sensor2's power
  * @input  on: Determines if ultrasonic sensor is on or off
  * @output None
  * @description This routine is neccessary because HC-SR04 is lack of timeout and echo stucks at high after some time.
  */
void Ultrasonic2_power(bool on);

#endif /* ULTRASONIC_H */ 
