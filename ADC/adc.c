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
	DDRA&=~_BV(0);
	PORTA|=_BV(0);
	DDRB=0x03;
	PORTB=0x03;

	
	UCSRB|=(1<<RXEN)|(1<<TXEN); //turn on rx and tx (can also be done using hex instead of 1<<RXEN)
//	1<<RXEN --> 1 shifted to RXEN.
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1); // asynchronous | 8bit character sizes.

	UBRRL=BAUD_PRESCALE; //load lower 8bits of baud rate into UBRR low register
	UBRRH=(BAUD_PRESCALE>>8); //load upper 8 bits of baud rate into UBRR high register
	

	ICR1=10000;
	TCCR1A=(1<<COM1A1|1<<COM1B1|0<<COM1A0|0<<COM1B0|0<<FOC1A|0<<FOC1B|0<<WGM11|0<<WGM10); //check datasheet //(can be written in hex/bin)
	TCCR1B=(1<<WGM13|1<<CS11|0<<CS12|0<<CS10|0<<WGM12|0<<ICNC1|0<<ICES1);    //check datasheet (can be written //in hex/bin)

	ADMUX=0x61; //registers for ADC
	ADCSRA=0x8F;

	for(;;)
	{
		ADCSRA|=(1<<ADSC); //conversion started for every loop
		
		while((ADCSRA&(1<<ADIF))==0) {}
		while((UCSRA&(1<<UDRE))==0){} //from serial code
			UDR=ADCH;
			_delay_ms(1000);
	}
}


