#include<avr/io.h>
#include<avr/eeprom.h>

void Drive_motor(unsigned char left, unsigned char right)
{
	for(;;)
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
	DDRB=0xFF;
	DDRD=0xFF;
	PORTB=0xFC;
	PORTD=0xFF;
	PORTA=0x00;

	if(bit_is_clear(PIND,2)&&bit_is_set(PIND,3))
	{
		eeprom_write_byte(0x10,0x00);
	}
	if(bit_is_set(PIND,2)&&bit_is_clear(PIND,3))
	{
		eeprom_write_byte(0x10,0x01);
	}

	unsigned char x=eeprom_read(0x10);

	if(x==0x00)
	{
		PORTB&=~_BV(0);
		PORTB&=~_BV(1);
		if((bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_set(PINA,0))&&(bit_is_set(PINA,5))) 
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
	}
	else if(x==0x01)
	{
		for(int x=1;x<=100;x++)
		{
			int y=100-x;
			PORTB=0x02;
			_delay_ms(x);
			PORTB=0x01;
			_delay_ms(y);
		}
		for(int z=1;z<=100;z++)
		{
			int a=100-z;
			PORTB=0x01;
			_delay_ms(z);
			PORTB=0x02;
			_delay_ms(a);
		}
	}
}
}


