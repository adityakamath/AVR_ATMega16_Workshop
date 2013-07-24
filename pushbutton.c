#include<avr/io.h>
#include<util/delay.h>
#define F_CPU 8000000UL

int main(){
DDRB=0x03; //or 0b00000011
DDRD=0x00;
PORTD=0xFF;
for(;;){
if (bit_is_clear(PIND,3)){
PORTB|=_BV(0); //port b previous state OR 00000001 (or PORTB=0x01)

}
if(bit_is_clear(PIND,2)){
PORTB|=_BV(1); 
}
if(bit_is_set(PIND,3)){
PORTB&=~_BV(0); //port b previous state AND 11111110 (or PORTB=0x00)
}
if(bit_is_set(PIND,2)){
PORTB&=~_BV(1);
}
}
}
