#include <Board_Accelerometer.h> 
#include <Board_Magnetometer.h>
#include <fsl_debug_console.h> 
#include <board.h> 
#include <Driver_I2C.h>
#include "utils.h"

#define SPEEDLIMIT 10
#define MARGIN 5
#define num_speed 3

ACCELEROMETER_STATE *pState;
MAGNETOMETER_STATE mstate;

int data[50];
int proc_data[180]; 
int time_stamp[180]; 
//const sys_clock = g_defaultClockConfigRun;

	unsigned int i = 0; 
	unsigned int sum = 0; 
		

void init_timer(){
	
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK;
	PIT->MCR &= ~(1 << 1); 
	PIT->CHANNEL[0].LDVAL = 0x2710; //120 *10^6 / (60*10^3); 240000 -- 0x3A980
	PIT->CHANNEL[0].TCTRL = 0x003;
	PIT -> CHANNEL[0].TFLG = 0x001; 
	NVIC_EnableIRQ(PIT0_IRQn);
	
}

/*void init_RTC(){
	
}*/

void PIT0_IRQHandler(void) {

	PIT->CHANNEL[0].TCTRL = 0x002; //resets loads
	
	// ptrI2C -> Control	(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST); 
			ACCELEROMETER_STATE pstate;
			Accelerometer_GetState(pState);
			sum += (int)pState->x;
			 //sum+=2*i; 
		  i++;
			PIT -> CHANNEL[0].TCTRL = 0x003; 
			PIT -> CHANNEL[0].TFLG = 1;
	

}

int main(){

	hardware_init();
	Accelerometer_Initialize();
	init_timer();
	LED_Initialize(); 
	LEDGreen_On();
	//LEDBlue_On(); 
	 
	unsigned int speed = 0;  
	unsigned int j = 0; 
	
	unsigned int x = 0; 
	//unsigned int z = 0; 
	
	while(1){	
	
	//debug_printf("%5d %5d %5d\r\n", pState->x, pState->y, pState->z); 
	
	while(i<500); 
	i = 0; 
		
	proc_data[j] = sum/500; 
	
	//time_stamp[j] = get_timestamp(); 
	
	if(proc_data[j] >= SPEEDLIMIT+MARGIN) speed++; 
	else speed = 0; 
	
	//check speeding
	if(speed > num_speed) {
	if(proc_data[j] >= 2*(SPEEDLIMIT+MARGIN)) { 
			LEDGreen_Off();
		  LEDRed_Toggle(); 
			speed++; }
	
	else if(proc_data[j] >= 1.5*(SPEEDLIMIT+MARGIN)) { 
			LEDGreen_Off();
			LEDBlue_Toggle(); 
			speed++; }
	
	else if(proc_data[j] >= (SPEEDLIMIT+MARGIN)) {
			for(int i = 0; i<3; i++) LEDGreen_Toggle(); 
			speed++; }
	
	//debug_printf("%5d %5d\r\n", speed, proc_data[j]);
	
	//handle array size for processed data and timestamps
	if(j<180) j++; 
	else j = 0; 		

		}
	}
	
}
