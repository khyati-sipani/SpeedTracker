//libraries for accelerometer
#include <Board_Accelerometer.h> 
#include <Board_Magnetometer.h>
#include <fsl_debug_console.h> 
#include <board.h> 
#include <Driver_I2C.h>
#include "utils.h"
//libraries for RTC
#include "RTC.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include "math.h"

// Definitions 
#define SPEEDLIMIT 8
#define MARGIN 1
#define num_speed 2
ACCELEROMETER_STATE pState;
MAGNETOMETER_STATE mstate;
char str[50];
int data[50];
float proc_data[180];
char *proc_ts[180];
/*******************************************************************************************
REAL TIME CLOCK INITIALIZATION
********************************************************************************************/
  
typedef struct{
 uint8_t timestr[50];
} time;
time time_stamp[240];
/**Extracts bits from integer**/
uint8_t extractInt(uint8_t orig8BitWord, unsigned from, unsigned to)
{
  unsigned mask = ( (1<<(to-from+1))-1) << from;
  return (orig8BitWord & mask) >> from;
}
/**Get Timestamp, creates time struct, and puts it into timestamp array**/
void get_timestamp(){
 
uint8_t timestamp[16];
read_RTC(timestamp);
int seconds = timestamp[0];
int tenseconds = extractInt(seconds, 4, 6);
seconds = extractInt(seconds, 0, 3);
seconds = 10*(tenseconds) + seconds;
int minutes = timestamp[1];
int tenminutes = extractInt(minutes, 4, 6);
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
int tenmonth = extractInt(month, 4, 4);
month = extractInt(month, 0, 3);
month = tenmonth*10 + month;
int year = timestamp[6];
int tenyear = extractInt(year, 4, 7);
year = extractInt(year, 0, 3);
year = tenyear*10 + year;

char space = ' ';
//char * space_ptr = &space;
sprintf(str, "\n%02d/%02d/%02d %c %02d:%02d:%02d \n", month, date, year, space, hours, minutes, seconds);
//str[sizeof(str)-1] = '\0'; 
//debug_printf("%s\r\n", str);

}

/*******************************************************************************************
BLUETOOTH INITIALIZATIONS
********************************************************************************************/


/*******************************************************************************************
ALGORITHM IN MAIN METHOD
********************************************************************************************/
void init_dataTimer(){
 
 SIM->SCGC6 = SIM_SCGC6_PIT_MASK;
 PIT->MCR &= ~(1 << 1);
 PIT->CHANNEL[0].TCTRL = 0x001;
 PIT->CHANNEL[0].LDVAL = 0x1D4C0; //0x2710 120 *10^6 / (60*10^3); 240000 -- 
 
}

int main(){
 //Accelerometer initializatons
 hardware_init();
 Accelerometer_Initialize();
 
 //Polling Initializations
 init_dataTimer();
 LED_Initialize();
 LEDGreen_On();
 
 //Initializize bluetooth
 //init_uart(); 
 //NVIC_EnableIRQ(UART4_IRQn);
 
 unsigned int j = 0;
 unsigned int speed = 0;
 float prevVelocity = 0;
 float unit_convert = 0.01*9.80665; //sets acceleration units to dm/s^2
 //unsigned int not_moving = 0; 	

 //Open file to edit
 //	FILE *f = fopen("C:\\Users\\Aasta\\Desktop\\Test\\test_project.txt", "a");	
		
 while(1){
 
 unsigned int i = 0;
 float sum = 0;
 
 	 
 
 while(i < 500) {
   if(PIT->CHANNEL[0].TFLG == 1) {
    Accelerometer_GetState(&pState);
   //ptrI2C -> Control (ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST); 
   // debug_printf("%5d %5d %5d\r\n", pState.x, pState.y, pState.z); 
	 
   sum += ((float)pState.x)*unit_convert + 3;
		 //debug_printf("%5d\r\n", (int)x);
   i++;
   //prevState = pState.x; 
   PIT -> CHANNEL[0].TFLG = 1;
    
  }
 }
  
 // if(not_moving < 3) 
  //else proc_data[j] = 0;  
  
  //if(proc_data[j] == prevVelocity) not_moving++; 
  
  proc_data[j] = sum/500 + prevVelocity; 
  prevVelocity = proc_data[j];
	
 debug_printf("%5d %5d\r\n", sum/500, proc_data[j]); 
 
 get_timestamp();
 proc_ts[j] = str;
 
 if(proc_data[j] >= SPEEDLIMIT+MARGIN) speed++;
 else speed = 0;
 
 //check speeding
 if(speed > num_speed) {
 /**flash red**/
 if(proc_data[j] >= 2*(SPEEDLIMIT+MARGIN)) {
   LEDGreen_Off();
    LEDRed_Toggle();
   LEDGreen_On();
   speed++; }
 
 /**flash blue**/
 else if(proc_data[j] >= 1.5*(SPEEDLIMIT+MARGIN)) {
   LEDGreen_Off();
   LEDBlue_Toggle();
   LEDGreen_On();
   speed++; }
 
 /**flash yellow**/
 else if(proc_data[j] >= (SPEEDLIMIT+MARGIN)) {
   LEDRed_Toggle();
   speed++; }
 //debug_printf("%c\r\n", str); 
 
 
 //handle array size for processed data and timestamps
  }
// debug_printf("%s %5d %5d %5d\r\n", proc_ts[j], (int) sum, speed, (int) proc_data[j]);
 
// fprintf(f, "%s %5.4f %5d %5.4f\r\n", proc_ts[j], sum, speed, proc_data[j]); 
	
 if(j<180) j++;
 else j = 0;
  
 }
 
 //fclose(f); 
}