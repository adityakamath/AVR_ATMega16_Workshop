#include<avr/io.h>
#include<util/delay.h>
void inittimer0(){
	TCCR0|=((1<<0)|(1<<1)|(1<<2)); //divide by 1024
	TCNT0=0;
}
void main(){
	inittimer0();
		
	int count;	
	DDRC=0xff;	
	while(1){
	
	while(!(TIFR&(1<<0))); //wait till overflow
		TIFR|=(1<<0); //in polling method, flag needs to be cleared. this done by writing 1 to the flag.		
		count++;
		if(count==61){
			PORTC^=0xff;
			count=0;
		}
	}
}
	
