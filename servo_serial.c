#include<avr/io.h>
#include<util/delay.h>
#define USART_BAUDRATE 9600
#define F_CPU 8000000UL
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

void Servo_drive(int a,int b)
{
	OCR1A=a;
	OCR1B=b;
}

int main()
{
	DDRD=0xFF;
	//PORTD=0x00;

	
	UCSRB|=(1<<RXEN)|(1<<TXEN); //turn on rx and tx (can also be done using hex instead of 1<<RXEN)
//	1<<RXEN --> 1 shifted to RXEN.
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1); // asynchronous | 8bit character sizes.

	UBRRL=BAUD_PRESCALE; //load lower 8bits of baud rate into UBRR low register
	UBRRH=(BAUD_PRESCALE>>8); //load upper 8 bits of baud rate into UBRR high register
	

	ICR1=10000;
	TCCR1A=(1<<COM1A1|1<<COM1B1|0<<COM1A0|0<<COM1B0|0<<FOC1A|0<<FOC1B|0<<WGM11|0<<WGM10); //check datasheet 
//(can be written in hex/bin)
	TCCR1B=(1<<WGM13|1<<CS11|0<<CS12|0<<CS10|0<<WGM12|0<<ICNC1|0<<ICES1);    //check datasheet (can be written 
//in hex/bin)

	//int a=300;
	
	while(1)
	{
	while((UCSRA&(1<<RXC))==0){}
	char b=UDR; 
	int j;
	if(b=='1')
	{
	for(int i=0;i<=(18);i++)
	{
		j=250+i*(55);
		Servo_drive(j,j);
		_delay_ms(50);
	}
	}
	else if(b=='2')
	{
	for(int i=0;i<=(18);i++)
	{
		j=1250-i*(55);
		Servo_drive(j,j);
		_delay_ms(50);
	}
	}
	}
}










/*#include<avr/io.h>
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
}*/

