#include<avr/io.h>
#include<util/delay.h>
void main(){
	DDRC=0xff;
	DDRA|=(1<<3);
	while(1){
		PORTC=0xff;
		PORTA=0x08;
		_delay_ms(500);
		PORTC=0x00;
		PORTA=0x00;
		_delay_ms(500);
	}
}
