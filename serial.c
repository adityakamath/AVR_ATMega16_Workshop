#include<avr/io.h>
#define USART_BAUDRATE 9600
#define F_CPU 8000000UL
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

int main()
{
	char ReceivedByte;

	UCSRB|=(1<<RXEN)|(1<<TXEN); //turn on rx and tx (can also be done using hex instead of 1<<RXEN)
	//1<<RXEN --> 1 shifted to RXEN.
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1); // asynchronous | 8bit character sizes.

	UBRRL=BAUD_PRESCALE; //load lower 8bits of baud rate into UBRR low register
	UBRRH=(BAUD_PRESCALE>>8); //load upper 8 bits of baud rate into UBRR high register

	for(;;)
	{
		DDRB=0xff;
		while((UCSRA&(1<<RXC))==0) {} //runs loop till data is received

		/* by default UCSRA receiving bit is 0. And 1<<RXC (receiving status register) is 1.
		so loop continues till data is received which makes UCSRA receiving register 1. 
		breaks out of loop*/

		ReceivedByte=UDR; // data transferred to RX data register
		PORTB=0x00;

		//**To switch on/off LED**
		if(ReceivedByte=='a')
		{
			PORTB=0x03;
		}
		if(ReceivedByte=='s')
		{
			PORTB=0x00;
		}
			

		//**To echo input**
		while((UCSRA&(1<<UDRE))==0){} //do nothing till UDR ready for more data


		UDR=ReceivedByte+1; //transfers data to TX UDR
	}
}
