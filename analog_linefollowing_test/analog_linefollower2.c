#include<avr/io.h>
#define F_CPU 8000000UL
#include<util/delay.h>

int ADC_read(int channel)
{
	ADMUX = (ADMUX&0xE0) | channel; //Select next channel
	ADCSRA |= (1<<ADSC);			//Start conversion by setting ADSC bit
	while( ADCSRA & _BV(ADSC) );	//Wait for conversion to end
	return ADCH;					//return conversion result
}


int main(void)
{
	   	 				
	DDRA = 0x00;
	DDRB=0xFF;
	DDRD=0xFF;
	PORTB=0x90;
	
	ICR1=10000;	TCCR1A=(1<<COM1A1|1<<COM1B1|0<<COM1A0|0<<COM1B0|0<<FOC1A|0<<FOC1B|0<<WGM11|0<<WGM10); //check datasheet (can be written in hex/bin)
	TCCR1B=(1<<WGM13|1<<CS11|0<<CS12|0<<CS10|0<<WGM12|0<<ICNC1|0<<ICES1);    //check datasheet (can be written in hex/bin)
	

	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0)|(1<<ADEN);
	ADMUX |= (1<<REFS0)|(1<<ADLAR);
	

	char q,p;
	for(;;)
	{
		
			q = ADC_read(1);
			p = ADC_read(3);
			OCR1A=p*0x28; //value changed for high speed motors 0x0E
			OCR1B=q*0x28; //value changed for high speed motors 0x0E
	}
}
