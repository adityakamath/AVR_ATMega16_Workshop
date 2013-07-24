#include<avr/io.h>
#include<util/delay.h>
#define F_CPU 8000000UL

int main(void)
{
	DDRB=0xFF;
	DDRD=0xFF;
	PORTB=0x00;
	PORTD=0xFF;

	for(;;)
	{
		//forward
		PORTB|=_BV(4); //Forward: 4:high 5:low
		PORTB|=_BV(7); //Forward: 7:high 6:low
		_delay_ms(1000);
		PORTB=0x00;
		_delay_ms(2000);

	}
}
