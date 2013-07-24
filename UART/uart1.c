#include<avr/io.h>
#include<util/delay.h>
#include"uart.h"
void main(){
	u1init();
	char x;
	while(1){
		u1txnum(12345);
		u1tx(' ');
		_delay_ms(500);
	}
}
