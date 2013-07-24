#include<avr/io.h>
#include<util/delay.h>
#define F_CPU 8000000UL

int main(){
DDRB=0x03;
for(;;){
PORTB=0x03;
_delay_ms(500);
PORTB=0x00;
_delay_ms(500);
}
}
