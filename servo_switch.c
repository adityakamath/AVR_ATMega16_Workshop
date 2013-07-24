#include<avr/io.h>
#define F_CPU 8000000UL
#include<util/delay.h>

void Servo_drive(int a,int b)
{
	OCR1A=a;
	OCR1B=b;
}
int main()
{
	DDRD=0b00100000;
	PORTD=0b00001100; //switches pulled up
	
	ICR1=10000;
	TCCR1A=(1<<COM1A1|1<<COM1B1|0<<COM1A0|0<<COM1B0|0<<FOC1A|0<<FOC1B|0<<WGM11|0<<WGM10); //check datasheet(can be written in hex/bin)
	TCCR1B=(1<<WGM13|1<<CS11|0<<CS12|0<<CS10|0<<WGM12|0<<ICNC1|0<<ICES1);    //check datasheet(can be written in hex/bin)
	
	while(1)
	{	
		int j;
		if(bit_is_clear(PIND,2)&&bit_is_set(PIND,3))
		{
			for(int i=0;i<=18;i++)
			{
				j=i*55;
				Servo_drive(250+j,250+j);
				_delay_ms(50);
			}
			
		}
		if(bit_is_clear(PIND,3)&&bit_is_set(PIND,2))
		{
			for(int i=0;i<=18;i++)
			{
				j=i*55;
				Servo_drive(1250-j,1250-j);
				_delay_ms(50);
			}
		}
	}
}
