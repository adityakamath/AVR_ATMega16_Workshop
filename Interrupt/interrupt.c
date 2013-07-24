#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
void initint(){
	EICRB|=((1<<ISC71)|(1<<ISC61));
	EIMSK|=((1<<INT7)|(1<<INT6));
	sei();
}
ISR(INT6_vect){
	PORTC=0xAA;
}
	
ISR(INT7_vect){
	PORTC=0x55;
}
void main(){
	initint();
	DDRC=0xff;
	DDRE=0x00;
	PORTE=0xff;
	while(1){
	}
}


