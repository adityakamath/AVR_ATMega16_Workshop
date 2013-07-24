#include<avr/io.h>
#include<avr/eeprom.h>
#include<util/delay.h>
#define F_CPU 8000000UL

unsigned char eeprombyte; //address of this used. no cast pointer error occurs.

void eep()
{
	if(bit_is_clear(PIND,2))
	{
		eeprom_write_byte(&eeprombyte,0xff);
	}
	else
	{
		eeprom_write_byte(&eeprombyte,0x00);
	}
}

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
	for(;;)
	{

	DDRD=0x00;
	PORTD=0xff;
	DDRB=0xff;
	PORTB=0xfc;
	PORTA&=~_BV(6);
	PORTA&=~_BV(7);


	if(bit_is_clear(PIND,3))
	{
		eep();
	}
	else
	{
		unsigned char x=eeprom_read_byte(&eeprombyte);
		if(x==0xff)
		{
			//line follower
			PORTB&=~_BV(0);
			PORTB&=~_BV(1);
			if((bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_set(PINA,0))&&(bit_is_set(PINA,5))) 
		{
			Drive_motor(1,1);
		}
			if((bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_clear(PINA,0))&&(bit_is_clear(PINA,5))) 
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
		if((bit_is_set(PINA,0))&&(bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_clear(PINA,4)))
		{
			Drive_motor(2,1);
		} //Try in arena**
		if((bit_is_set(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,0))&&(bit_is_clear(PINA,4)))
		{
			Drive_motor(1,0);
		}
		if((bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_clear(PINA,0))&&(bit_is_set(PINA,4)))
		{
			Drive_motor(1,2);
		}// Try in arena**
		if((bit_is_set(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,0))&&(bit_is_set(PINA,4)))
		{
			Drive_motor(2,2); // reverse if out of loop. Finds loop again
		} //no differential rotation AT ALL (1,2)/(2,1)
		if(bit_is_set(PINA,6))
		{
			Drive_motor(0,0);
		}
		if(bit_is_set(PINA,7))
		{
			Drive_motor(0,0);
		}
			
		}
		else
		{
		    //blink
			PORTB|=_BV(0); PORTB|=_BV(1);
			_delay_ms(500);
			PORTB&=~_BV(0); PORTB&=~_BV(1);
			_delay_ms(500);
		}
	}
	}
}

	
