#include<avr/io.h>
#include<util/delay.h>
void inittimer(){
	TCCR2|=((1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6));
}
void main(){
	inittimer();
	DDRB|=(1<<7);	
	int i;	
	while(1){	
		for(i=0;i<=255;i++){
			OCR2=i;
			_delay_ms(1);
		}
		for(i=255;i>=0;i--){
			OCR2=i;
			_delay_ms(1);
		}
	}
}
