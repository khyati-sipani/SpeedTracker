/*#include <Board_Accelerometer.h> 
#include <Board_Magnetometer.h>
#include <fsl_debug_console.h> 
#include <board.h> 

ACCELEROMETER_STATE state;
MAGNETOMETER_STATE mstate;

int data[50];
int proc_data[180]; 
char time_stamp[180]; 
		


void init_timer(){
	
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK;
	PIT->MCR &= ~(1 << 1); 
	PIT->CHANNEL[0].LDVAL = ;
	PIT->CHANNEL[0].TCTRL = 0x03;
	NVIC_EnableIRQ(PIT0_IRQn);
	
}

/*void init_RTC(){
	
}

int main(){

	hardware_init();
	Accelerometer_Initialize();
	init_timer();
	 
	
	unsigned int j = 0; 
	while(1){
		
	unsigned int i = 0; 
	unsigned int sum = 0; 
		
	while(i != 50) {
			
		 if(PIT->CHANNEL[0].TFLG == 1) {
			 sum += Accelerometer_GetState(&state->state.x);	 
			 i++;
			 PIT ->CHANNEL[0].TFLG = 1; 	 
		} 
		 
	}
	
	proc_data[j] = sum/50; 
	time_stamp[j] = get_timestamp(); 
	
	if(j!=180) j++; 
	else j = 0; 
}
	
int PIT1_IRQHandler(void)
{
	PIT->CHANNEL[1].TFLG=1;
	return 
	
}
**/

#include <Board_Accelerometer.h> 
#include <Board_Magnetometer.h>
#include <fsl_debug_console.h> 
#include <board.h> 
#include <Driver_I2C.h>
#include "utils.h"

#define blueLED 21
#define greenLED 26
#define redLED 22
#define SPEEDLIMIT 50
#define MARGIN 5
#define num_speed 3

ACCELEROMETER_STATE state;
MAGNETOMETER_STATE mstate;

int data[50];
int proc_data[180]; 
char time_stamp[180]; 
//const sys_clock = g_defaultClockConfigRun; 		
		

void init_timer(){
	
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK;
	PIT->MCR &= ~(1 << 1); 
	PIT->CHANNEL[0].LDVAL = 2000;  // 120 *10^6 / (60*10^3); 
	PIT->CHANNEL[0].TCTRL = 0x03;
	NVIC_EnableIRQ(PIT0_IRQn);
	
}

/*void init_RTC(){
	
}*/

int main(){

	hardware_init();
	Accelerometer_Initialize();
	init_timer();
	LED_Initialize(); 
	LEDGreen_On(); 
	 
	unsigned int speed = 0;  
	unsigned int j = 0; 
	while(1){
		
	unsigned int i = 0; 
	unsigned int sum = 0; 
		
	while(i != 50) {
			
		 if(PIT->CHANNEL[0].TFLG == 1) {
			// ptrI2C -> Control	(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST); 
			//ACCELEROMETER_STATE pstate;
			Accelerometer_GetState(&state);
			sum += state.x;
			i++;
			PIT ->CHANNEL[0].TFLG = 1;
		} 
		 
	}
	
	proc_data[j] = sum/50; 
	
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
	
	//handle array size for processed data and timestamps
	if(j!=180) j++; 
	else j = 0; 
	

	}

	}
	
}

void write_txt() {
	
		
	
}





