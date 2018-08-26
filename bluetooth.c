//#include "mbed.h"
#include <board.h>
//#include "main.h" 
#include "utils.h"
#include "fsl_debug_console.h"


unsigned int d_received[5]; 
unsigned int ts_received[5]; 
int i = 0; 


/* initialize UART to interface with bluetooth */
void init_uart(void) {

	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; //enable portb clock
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;  //enable UART clock
	
	//configuring port register
	
	PORTB_PCR16 |= 	PORT_PCR_MUX(3); 
	PORTB_PCR17 |=  PORT_PCR_MUX(3); 
	
	UART4_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK); //disables Tx and Rx
	
	UART4_C1 = 0; //default setting of the regs
	
	//configure baud rate
	uint16_t u_baudrate = ((21000*1000)/(9600*16)); //calculate baud rate CHECK IF THESE VALUES ARE RIGHT!!
	uint8_t t = UART4_BDH & ~(UART_BDH_SBR(0x1F)); //save the val of UART0
	
	//set high, low baud rates
	UART4_BDH = t | (((u_baudrate & 0x1F00) >> 8)); 
	UART4_BDL = (uint8_t)(u_baudrate & UART_BDL_SBR_MASK); 
	
	//enable Tx, Rx
	UART4_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK); 
	
	//enable interrupts
	UART4_C7816_ISO_7816E = 1; 
	UART4_IE7816_RXTE = 1; 

}

void receive_IRQHandler() {

	//transmit data
	while(!(UART0_S1 & UART_S1_TDRE_MASK)); 
	UART4_D = (uint8_t)proc_d[i]; 		
	debug_printf("%d\r\n", (int)UART0_D);

}


void receive_data (int d[]) {
	
	while(i<sizeof(d)) {
	while (!(UART0_S1  & UART_S1_RDRF_MASK));
	UART4_D = (int)d[i]; 
	d_received[i] = (int)UART0_D; 
	i++; 
	}

}





