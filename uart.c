#include <avr/io.h>				// header for pin discription
#define USART_BAUDRATE 9600		//Set Baudrate
#define F_CPU 8000000UL		// 8 MHz CPU clock
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)	// UBRR value
#include "uart.h"				// header for UART communication Protocal


volatile unsigned char s_buff;			// Serial Buffer

//Serial Initialize
void uart_init (void) 
{
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register 
	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UCSRB |= (1 << RXEN) | (1 << TXEN);   // Turn on the transmission and reception circuitry 
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Use 8-bit character sizes, NO parity
	
	uart_puts("\n\r");								// Send string \n=new line \r=carriage return \f=clear screen
	uart_puts("Start!\n\n\r");						// Send string Start! + \n=new line + \n=new line + \f=clear screen
}

// Send a char
void uart_putc(unsigned char c)
{
    while(!(UCSRA & (1 << UDRE)));   	// wait before UDR is ready
    UDR = c;    						// Send char 
}

// Send a string
void uart_puts (char *s)
{
    while (*s)	//While *s is not NULL
    {
        uart_putc(*s);
        s++;
    }
}

unsigned char ser_getc (void)
{
	while ((UCSRA & (1 << RXC)) == 0); 	// wait before UDR is ready
    s_buff = UDR;    					// receive char 
return s_buff;							// return Serial data to calling function
}
