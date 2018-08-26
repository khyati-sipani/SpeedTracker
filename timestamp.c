#include <Board_Accelerometer.h> 
#include <Board_Magnetometer.h>
#include <fsl_debug_console.h> 
#include <board.h> 
#include "RTC.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "math.h"

#define blueLED 21
#define greenLED 26
#define redLED 22
#define SPEEDLIMIT 50
#define MARGIN 5
#define num_speed 3

ACCELEROMETER_STATE state;
MAGNETOMETER_STATE mstate;

int data[50];
int proc_data[240];

typedef struct{
	uint8_t timestr[50];
} time;

time time_stamp[240];

//Helper Methods
void init_timer(){
	
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK;
	PIT->MCR &= ~(1 << 1);
	PIT->CHANNEL[0].LDVAL = 240000;
	PIT->CHANNEL[0].TCTRL = 0x03;
	NVIC_EnableIRQ(PIT0_IRQn);
	
}

/**Extracts bits from integer**/
uint8_t extractInt(uint8_t orig8BitWord, unsigned from, unsigned to) 
{
  unsigned mask = ( (1<<(to-from+1))-1) << from;
  return (orig8BitWord & mask) >> from;
}

/**Converts integer into and integer array with digits**/
uint8_t * toArray(int number){
	/* count number of digits */
	int c = 0; /* digit position */
	uint8_t n = number;

	while (n != 0)
	{
    n /= 10;
    c++;
	}

	uint8_t numberArray[c];

	c = 0;    
	n = number;

/* extract each digit */
	while (n != 0)
	{
    numberArray[c] = n % 10;
    n /= 10;
    c++;
	}
}

/**Concatenates two integer arrays**/
uint8_t * concat(uint8_t *s1, uint8_t *s2)
{
    uint8_t *result = malloc(4*(sizeof(uint8_t)));//+1 for the zero-terminator
    memcpy(result, s1, 2);
    memcpy(result + 2, s2, 2);
		//free(s1);
		//free(s2);
    return result;
}

/**Get Timestamp, creates time struct, and puts it into timestamp array**/
time get_timestamp(){
uint8_t timestamp[16];
read_RTC(timestamp);
uint8_t seconds = timestamp[0];
uint8_t tenseconds = extractInt(seconds, 4, 6);
seconds = extractInt(seconds, 0, 3);
seconds = 10*(tenseconds) + seconds;
uint8_t minutes = timestamp[1];
uint8_t tenminutes = extractInt(minutes, 4, 6);
minutes = extractInt(minutes, 0, 3);
minutes = 10*(tenminutes) + minutes;
int hours = timestamp[2];
int tenhours = extractInt(hours, 4, 5);
hours = extractInt(hours, 0, 3);
hours = 10*(tenhours) + hours;
int date = timestamp[4];
int tendate = extractInt(date, 4, 5);
date = extractInt(date, 0, 3);
date = tendate*10 + date;
int month = timestamp[5];
int tenmonth = extractInt(date, 4, 4);
month = extractInt(month, 0, 3);
month = tenmonth*10 + month;
int year = timestamp[6];
int tenyear = extractInt(date, 4, 7);
year = extractInt(date, 0, 3);
date = tenyear*10 + year;


uint8_t * secondsarray = toArray(seconds);
uint8_t * minutesarray = toArray(minutes);
uint8_t * result = concat(secondsarray, minutesarray);
debug_printf("%5d", result);

//time * timest = malloc(sizeof(uint_8*13));
//timest.timestr = (assign final array to this)
//return timest;
}

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
			Accelerometer_GetState(&state);
			 sum += state.x;
			 i++;
			 PIT ->CHANNEL[0].TFLG = 1;
			debug_printf("%5d %5d %5d\r\n", state.x, state.y, state.z);
		}
	}
	
	proc_data[j] = sum/50;
	

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

	
	
	timestamp[j] = get_timestamp(); 
			
	//handle array size for processed data and timestamps
	if(j!=180) j++;
	else j = 0;
}
}


}