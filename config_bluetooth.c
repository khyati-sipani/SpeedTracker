//#include "mbed.h"
#include <board.h>
//#include "main.h" 
#include "utils.h"
#include "fsl_debug_console.h"


/*unsigned int d_received[5]; 
unsigned int ts_received[5]; 


//int d_received[sizeof(data)]; 
//int ts_received[sizeof(time_stamp)]; 


/* void uart_putchar(char ch) {


	//S1 reg provides inputs to MCU 
		while(!(UART0_S1 & UART_S1_TDRE_MASK)); 
		UART0_D = (uint8_t)ch; 
	
}

uint8_t uart_getdata() {
	
	while (!(UART0_S1 & UART_S1_RDRF_MASK)); 
	return UART0_D; 
}


void put(char *ptr_str) {
	
	while(*ptr_str) { 
		uart_putchar(*ptr_str++); 
	}

} 

void transfer_data (int d[], int ts[]) {
	
	//sends over proc_data and time_stamp iteratively
	for(int i = 0; i < sizeof(d); i++) {		
	
	//transmit proc_data	
	while(!(UART0_S1 & UART_S1_TDRE_MASK)); 
	UART0_D = (uint8_t)d[i]; 		
	debug_printf("%d\r\n", (int)UART0_D);
		
	//receive and store proc_data	
	while (!(UART0_S1  & UART_S1_RDRF_MASK));
	UART0_D = (int)d[i]; 
	d_received[i] = (int)UART0_D; 	
	
	//transmit time_stamp
	while(!(UART0_S1 & UART_S1_TDRE_MASK)); 
	UART0_D = (uint8_t)ts[i]; 		
		
	//receive and store time_stamp	
	while (!(UART0_S1  & UART_S1_RDRF_MASK));
	UART0_D = ts[i]; 
	ts_received[i] = (int)UART0_D; 	
		
	}	
	

	//UART0_S1_TC = 1; 	
}



void put_data(int *data[], char *time_stamp[]) {

	while(data && time_stamp) {
		uart_putdata(*data++, *time_stamp++); 
	}
}


 initialize UART to interface with bluetooth 
void init_uart(void) {

	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; //enable portb clock
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;  //enable UART clock
	
	//configuring port register
	
	PORTB_PCR16 |= 	PORT_PCR_MUX(3); 
	PORTB_PCR17 |=  PORT_PCR_MUX(3); 
	
	UART0_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK); //disables Tx and Rx
	
	UART0_C1 = 0; //default setting of the regs
	
	//configure baud rate
	uint16_t u_baudrate = ((21000*1000)/(9600*16)); //calculate baud rate CHECK IF THESE VALUES ARE RIGHT!!
	uint8_t t = UART0_BDH & ~(UART_BDH_SBR(0x1F)); //save the val of UART0
	
	//set high, low baud rates
	UART0_BDH = t | (((u_baudrate & 0x1F00) >> 8)); 
	UART0_BDL = (uint8_t)(u_baudrate & UART_BDL_SBR_MASK); 
	
	//enable Tx, Rx
	UART0_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK); 

}


int main (void) {
	
	
//	init_uart();  
	
	int t1[5]; 
	int t2[5]; 

	for(int i = 0; i < 5; i++) {
	
			t1[i] = 2;
			t2[i] = 3; 
			debug_printf("%d %d\r\n", t1[i], t2[i]);
		
	}
	
	
	//transfer_data(t1, t2); 
	
	for(int i=0; i<5; i++) 
	{	debug_printf("%d %d\r\n", d_received[i], ts_received[i]); }
	
	
	//transfer_data(proc_data, time_stamp); 
} */  


unsigned int d_received[5] = {1, 2, 3, 4, 5}; 
char ts_received[5] = {'a', 'b', 'c', 'd', 'e'}; 
int i = 0; 


/* initialize UART0 to interface with bluetooth */
void init_uart0(void) {
	
	 
	  //uint8_t temp4;

    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;  //clock    
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;  //port b
		
    PORTB_PCR16 |= PORT_PCR_MUX(3);
    PORTB_PCR17 |= PORT_PCR_MUX(3);
	
		UART0_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );  //Disable Tx and Rx
		
		UART0_C1 = 0; 
	
		//configure baud rate
	  uint16_t ubd;                  
    uint8_t temp0;
		ubd = (uint16_t)((21000*1000)/(9600 * 16));  
    temp0 = UART0_BDH & ~(UART_BDH_SBR(0x1F));
		UART0_BDH = temp0 | (((ubd & 0x1F00) >> 8));
    UART0_BDL = (uint8_t)(ubd & UART_BDL_SBR_MASK);
		
		//enale TX and RX
    UART0_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK ); 
}

/*initialize UART4 to interface with bluetooth*/ 
void init_uart4() {
	
		SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;      //enable clock
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;      //enable port E
		

	
		UART4_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK); //disables Tx and Rx 
		UART4_C1 = 0; 
		
		//configure baud rate
		uint16_t ubd;                   
	  uint8_t temp4;
		temp4 = UART4_BDH & ~(UART_BDH_SBR(0x1F));
	
		UART4_BDH = temp4 | (((ubd & 0x1F00) >> 8));
    UART4_BDL = (uint8_t)(ubd & UART_BDL_SBR_MASK);
	
		//emable TX and RX
		UART4_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK ); 
}

//Print a character using UART0
void uart_putc (char ch)
{
	/* Wait until space is available in the FIFO */
	while(!(UART0_S1 & UART_S1_TDRE_MASK));
	/* Send the character */
	UART0_D = (uint8_t)ch;
	//debug_printf("%c", UART0_D); 
}

//print characters from arrays using UART0 
void uart_putc2 ()
{
	/* Wait until space is available in the FIFO */
	for(int i=0; i<5; i++) {
	while(!(UART0_S1 & UART_S1_TDRE_MASK));
	/* Send the character */
	UART4_D = (uint8_t) ts_received[i]; }
	//debug_printf("%c", UART0_D); 
}

//Print a string using UART
void uart_puts(char *ptr_str)
{
    while(*ptr_str){
			/* Replace newlines with \r\n carriage return */
			if(*ptr_str == '\n') { uart_putc('\r'); }
      uart_putc(*ptr_str++);
		}
}

/* Clear terminal screen using VT100 commands */
/* http://braun-home.net/michael/info/misc/VT100_commands.htm */
void uart_clear_screen(void) {
	uart_putc(0x1B);		/* Escape character */
 	uart_puts("[2J");		/* Clear screen */
 	uart_putc(0x1B);		/* Escape character */
 	uart_puts("[0;0H");		/* Move cursor to 0,0 */
}

void uart0_transmit() {

	for(int i = 0; i<sizeof(d_received); i++) {
	while(!(UART0_S1 & UART_S1_RDRF_MASK));
	UART0_D = d_received[i];  	
	}	
	
}

void uart4_transmit() {
	
	for(int i = 0; i<sizeof(d_received); i++) {
	while(!(UART4_S1 & UART_S1_TDRE_MASK));
	UART4_D = (uint8_t) ts_received[i]; 	
	//uart0_receive(); 
	}	
}



int main(){
	
	init_uart0(); 
  init_uart4();
	
//	for(int i=0; i<5; i++) d_received[i] = 2; 
	//debug_printf("%d", d_received[i]); }
		/* Setup UART (Note: Sets clock to 1 MHz) */
	//uart4_transmit(); 
	uart_putc2(); 
	// init_uart4(); 
    // have some delay() here using for loop.
 //	uart_clear_screen();
//	uart0_transmit(); 
//	uart4_transmit(); 
// uart_puts("\n Simple test to use UART \n"); // print a string to UART.
 //	uart_putc('a'); // print a character using UART.
    return 0;
}


/*
void receive_IRQHandler() {

	//transmit data3
	while(!(UART0_S1 & UART_S1_TDRE_MASK)); 
	UART4_D = (uint8_t)proc_data[i]; 		
	debug_printf("%d\r\n", (int)UART0_D);

}


void receive_data (int d[]) {
	
	while(i<sizeof(proc_data)) {
	while (!(UART0_S1  & UART_S1_RDRF_MASK));
	UART4_D = (int)d[i]; 
	d_received[i] = (int)UART0_D; 
	i++; 
	}

} */ 

