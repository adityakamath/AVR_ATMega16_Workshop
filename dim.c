#include<avr/io.h>
#include<util/delay.h>


int main(){
DDRD=0x03;
for(;;){
for(int x=1;x<=100;x++){
int y=100-x;
PORTD=0x03;
_delay_ms(x);
PORTD=0x00;
_delay_ms(y);
}
for(int z=1;z<=100;z++){
int a=100-z;
PORTD=0x00;
_delay_ms(z);
PORTD=0x03;
_delay_ms(a);
}
}
}

