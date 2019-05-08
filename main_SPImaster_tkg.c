
// Author : Tor K. Gjerde

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

/**************************** PROTOTYPE FUNCTIONS **********************/ 
void SPI_init_master(void); 
void SPI_MasterTransmit (char Data); 
/***********************************************************************/

int main(void)
{ 
	//-------------INITs--------------
	SPI_init_master(); 
	
   //------------ EVENT LOOP ------------------- 
    while (1) 
    {
		uint8_t i = 0; 
		while (i < 255 ){
			 // transmits one byte worth of data over to slave every 2 seconds
			 // Lights up LEDs on Slave  output  
		SPI_MasterTransmit(i);
		_delay_ms(2000); 
		i++; 
		}
    }
}

/*************************** FUNCTIONS ****************************************************/ 

void SPI_init_master(void){
	
	DDRB |= (1<<DDB3)|(1<<DDB5);  // set MOSI and SCK output  MOSI = PB3 and SCK = PB5 
	
	SPCR |= (1 << SPE); // Enable SPI 
	SPCR |= (1 << MSTR); // Set the device as MASTER 
	SPCR |= (1 << DORD); // LSB of data word is written first 
	SPCR |= (1<< SPR0) | (1 << SPR1); // SCK frequency is set to 16 000 000/128 = 125KHz (yes primitive speed indeed)
	
	SREG |= (1 << 7); // Global interrupt enable
	
	// SPDR = data-register for both SLAVE and MASTER -- husk det - load data to be sent here!;) 
	
	// when a serial transfer is complete the SPIF flag is set. (bit 7 in SPSR - status-register)
	// setting SPIE (interrupt enable) causes the SPI interrupt to be executed if SPIF in register SPSR is set. -
	// -ANA global interrupt enable bit in SREG is set. 
}

void SPI_MasterTransmit (char Data){
	// Start transmission
	SPDR = Data;
	// Wait for transmission to complete
	while(!(SPSR & (1 << SPIF))){}	
}
