#include<avr/io.h>

int main(){
DDRB=0x03;
DDRD=0x00;
PORTD=0xFF;
for(;;){
if (bit_is_clear(PIND,3)){
PORTB=0x01;
}
if(bit_is_clear(PIND,2)){
PORTB=0x02;
}
}
}

