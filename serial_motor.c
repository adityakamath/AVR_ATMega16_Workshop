#include<avr/io.h>
#define USART_BAUDRATE 9600
#define F_CPU 8000000UL
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)


void Drive_motor(unsigned char left, unsigned char right)
{
	if(left==0) //stops left motor
	{
		PORTB&=~_BV(4);
		PORTB&=~_BV(5);
	}
	
	if(right==0) //stops right motor
	{
		PORTB&=~_BV(6);
		PORTB&=~_BV(7);
	}
	
	if(left==1) //left motor - forward
	{
		PORTB|=_BV(4);
		PORTB&=~_BV(5);
	}

	if(right==1) //right motor - forward
	{
		PORTB|=_BV(7);
		PORTB&=~_BV(6);
	}

	if(left==2) //left motor - backward
	{
		PORTB|=_BV(5);
		PORTB&=~_BV(4);
	}

	if(right==2) //right motor - backward
	{
		PORTB|=_BV(6);
		PORTB&=~_BV(7);
	}
} 

int main()
{
	char ReceivedByte;
	DDRB=0xFF;
	DDRD=0xFF;
	PORTB=0xFF;
	PORTD=0xFF;
	PORTA=0x00;

	UCSRB|=(1<<RXEN)|(1<<TXEN); //turn on rx and tx (can also be done using hex instead of 1<<RXEN)
	//1<<RXEN --> 1 shifted to RXEN.
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1); // asynchronous | 8bit character sizes.

	UBRRL=BAUD_PRESCALE; //load lower 8bits of baud rate into UBRR low register
	UBRRH=(BAUD_PRESCALE>>8); //load upper 8 bits of baud rate into UBRR high register

	for(;;)
	{
		while((UCSRA&(1<<RXC))==0) {} //runs loop till data is received

		/* by default UCSRA receiving bit is 0. And 1<<RXC (receiving status register) is 1.
		so loop continues till data is received which makes UCSRA receiving register 1. 
		breaks out of loop*/

		ReceivedByte=UDR; // data transferred to RX data register

		if(ReceivedByte=='w') 
		{
			Drive_motor(1,1);
		}
		else if(ReceivedByte=='s') 
		{
			Drive_motor(2,2);
		}
		else if(ReceivedByte=='a') 
		{
			Drive_motor(0,1);
		}
		else if(ReceivedByte=='d') 
		{
			Drive_motor(1,0);
		}
		else
		{
			Drive_motor(0,0);
		}
		/* while((UCSRA&(1<<UDRE))==0){} //do nothing till UDR ready for more data


		UDR=ReceivedByte; //transfers data to TX UDR */
	}
}
