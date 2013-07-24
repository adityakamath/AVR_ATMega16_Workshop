#include<avr/io.h>
#include<util/delay.h>
void main(){
	DDRA=0x08;
	DDRC=0xff;
	DDRD=0x00;
	DDRE=0x00;
	PORTD=0xff;
	PORTE=0xff;
while(1){
	if(!(PIND&(1<<6)))
		PORTC=0x00;
	if(!(PIND&(1<<7)))	
		PORTC=0xff;
	if(!(PINE&(1<<6)))
		PORTA=0xff;
	if(!(PINE&(1<<7)))
		PORTA=0x00;
}
}
	

