# SpeedTracker

## Overview
The goal of this Speed Tracking device is to continuously monitor if a vehicle speeds more accurately.  Using the accelerometer on the FRDM-K64F microcontroller to check if a moving system is “speeding” (i.e moving faster than set ranges), the speed tracker will time stamp every instance of speeding using a real time clock, and send that data to the main system via bluetooth. It will also caution the user by blinking the LED on the board with a different color to indicate different modes of speeding. The motivation for this project is to make current speeding systems more efficient and effective at monitoring if vehicles are actually speeding intentionally as well as to caution the driver that they are speeding. 

For more details on system design and code, see the Project_Report file in the repository.
