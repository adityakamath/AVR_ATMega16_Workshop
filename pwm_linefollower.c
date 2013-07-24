#include<avr/io.h>
#define F_CPU 8000000UL
#include<util/delay.h>


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


void ocr_drive(int a,int b)
{
	OCR1A=a;
	OCR1B=b;
}

int main(void)
{
	DDRB=0xFF;
	DDRD=0xFF;
	PORTB=0xFF;
	PORTD=0xFF;
	PORTA=0x00;

	//DDRD=0xFF;
	//PORTD=0x00;
	

	ICR1=10000;
	TCCR1A=(1<<COM1A1|1<<COM1B1|0<<COM1A0|0<<COM1B0|0<<FOC1A|0<<FOC1B|0<<WGM11|0<<WGM10); //check datasheet (can be written in hex/bin)
	TCCR1B=(1<<WGM13|1<<CS11|0<<CS12|0<<CS10|0<<WGM12|0<<ICNC1|0<<ICES1);    //check datasheet (can be written in hex/bin)


	for(;;)
	{
		Drive_motor(1,1);
		if((bit_is_set(PINA,0))&&(bit_is_set(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,4)))
		{
			ocr_drive(10000,10000);
		}
		if((bit_is_set(PINA,0))&&(bit_is_set(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_set(PINA,4)))
		{
			ocr_drive(10000,7500);
		}
		if((bit_is_set(PINA,0))&&(bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,4)))
		{
			ocr_drive(7500,10000);
		}
		if((bit_is_set(PINA,0))&&(bit_is_set(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_clear(PINA,4)))
		{
			ocr_drive(10000,5000);
		}
		if((bit_is_clear(PINA,0))&&(bit_is_clear(PINA,1))&&(bit_is_clear(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,4)))
		{
			ocr_drive(5000,10000);
		}
		if((bit_is_set(PINA,0))&&(bit_is_set(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_clear(PINA,3))&&(bit_is_clear(PINA,4)))
		{
			ocr_drive(10000,2500);
		}
		if((bit_is_clear(PINA,0))&&(bit_is_clear(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,4)))
		{
			ocr_drive(2500,10000);
		}
		if((bit_is_set(PINA,0))&&(bit_is_set(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_clear(PINA,4)))
		{
			ocr_drive(10000,0);
		}
		if((bit_is_clear(PINA,0))&&(bit_is_set(PINA,1))&&(bit_is_set(PINA,2))&&(bit_is_set(PINA,3))&&(bit_is_set(PINA,4)))
		{
			ocr_drive(0,10000);
		}
	}

}

	
