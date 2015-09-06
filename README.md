# Omnidirectional-Robot-Project

**_What does this code do?_**

To test the motion control algorithm, robot draws square motion path autonomously and corrects its orientation with respect to the world frame by the data taken from digital compass or gyroscope. 

## General Info
I started building a semi-autonomous omnidirectional robot as an experimental robotic platform in supervision of Assoc. Prof. Afsar Saranli. This project is one of the Star Projects that are offered this year at METU EEE. For further information check out the [website](http://star.eee.metu.edu.tr/).

## Current Features
* Can move in a desired direction
* Can be controlled over bluetooth
* Go-to-goal behavior is designed so that it can move to any point while following a time-dependent trajectory 

## Technical Details
* Microcontrollers: ARM based 32-bit Cortex M4 TI Launchpad and mbed NUCLEO-F411RE
* Currently NUCLEO and TI launchpad is communicating over UART with a simple protocol
* Actuation system: 12V DC Motors with high resolution magnetic encoders
* PID control algorithm is applied to all three motors to control their speed and robots position
* Body: Made out of aliminium
* 10-DOF IMU is used for heading information

To see video of the robot in action visit [here](https://www.youtube.com/channel/UCSQE0Ju1SnRpRMVGIcckjnA/videos).

For suggestions, questions or further information about the project, please contact me from baser.kandehir[at]ieee.metu.edu.tr

