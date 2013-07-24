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
	DDRD=0xFF;
	//PORTD=0x00;
	

	ICR1=10000;
	TCCR1A=(1<<COM1A1|1<<COM1B1|0<<COM1A0|0<<COM1B0|0<<FOC1A|0<<FOC1B|0<<WGM11|0<<WGM10); //check datasheet (can be written in hex/bin)
	TCCR1B=(1<<WGM13|1<<CS11|0<<CS12|0<<CS10|0<<WGM12|0<<ICNC1|0<<ICES1);    //check datasheet (can be written in hex/bin)

	//int a=300;

	int a=3;  //0-9
	for(;;)
	{
	int j;
	for(int i=0;i<=(180/(a+1));i++)
	{
		j=250+i*(5.5*(a+1));
		Servo_drive(j,j);
		_delay_ms(50);
	}
	for(int i=0;i<=(180/(a+1));i++)
	{
		j=1250-i*(5.5*(a+1));
		Servo_drive(j,j);
		_delay_ms(50);
	}
	}
}
