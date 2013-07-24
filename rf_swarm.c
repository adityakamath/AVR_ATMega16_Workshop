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

void linefollow()
{
if((bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_set(PINA,0))&&(bit_is_set(PINA,4))) 
				{
					Drive_motor(1,1);
				}
				if((bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_clear(PINA,0))&&(bit_is_clear(PINA,4))) 
				{
					Drive_motor(1,1);
				}
				if((bit_is_set(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,0))&&(bit_is_set(PINA,4))) 
				{
					Drive_motor(1,1);
				}
				if((bit_is_set(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_set(PINA,0)))
				{
					Drive_motor(0,1);
				}
				if((bit_is_set(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_clear(PINA,4))&&(bit_is_set(PINA,0)))
				{
					Drive_motor(0,1);
				}
				if((bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,4)))
				{
					Drive_motor(1,0);
				}
				if((bit_is_clear(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_clear(PINA,0))&&(bit_is_set(PINA,4)))
				{
					Drive_motor(1,0);
				}
				if((bit_is_set(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_clear(PINA,0))&&(bit_is_clear(PINA,4)))
				{
					Drive_motor(0,0);
				}
				if((bit_is_clear(PINA,0))&&(bit_is_set(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,4)))
				{
					Drive_motor(1,0);
				}
				/*if((bit_is_set(PINA,0))&&(bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_clear(PINA,4)))
				{
					Drive_motor(2,1);
				} //Try in arena** */
				if((bit_is_set(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,0))&&(bit_is_clear(PINA,4)))
				{
					Drive_motor(1,0);
				}
				/*if((bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_clear(PINA,0))&&(bit_is_set(PINA,4)))
				{
					Drive_motor(1,2);
				}// Try in arena** */
				if((bit_is_set(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,0))&&(bit_is_set(PINA,4)))
				{
					Drive_motor(2,2); // reverse if out of loop. Finds loop again
				} //no differential rotation AT ALL (1,2)/(2,1)
}
int main()
{
	char ReceivedByte;
	DDRB=0xFF;
	DDRD=0xFF;
	DDRA=0x00;
	PORTB=0xFC;
	PORTD=0xFF;
	PORTA=0x00;

	UCSRB|=(1<<RXEN)|(1<<TXEN); //turn on rx and tx (can also be done using hex instead of 1<<RXEN)
	//1<<RXEN --> 1 shifted to RXEN.
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1); // asynchronous | 8bit character sizes.

	UBRRL=BAUD_PRESCALE; //load lower 8bits of baud rate into UBRR low register
	UBRRH=(BAUD_PRESCALE>>8); //load upper 8 bits of baud rate into UBRR high register

	for(;;)
		{
			while((UCSRA&(1<<RXC))==0) {}
			ReceivedByte=UDR;
			while(ReceivedByte=='1')
			{ 	
				if(bit_is_clear(PINA,6))
				{
					linefollow();
					
				}
				else
				{
					Drive_motor(0,0);
					//while((UCSRA&(1<<UDRE))==0){}
					ReceivedByte='2';
					UDR=ReceivedByte;   //ReceivedByte;
					break;
				}
			}
		}
}
						
