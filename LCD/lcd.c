#include<avr/io.h>
#include<util/delay.h>
#include"lcd.h"
void main(){
lcd_init();
	lcd_char('a');
	lcd_gotoxy1(5);
	lcd_str("Aditya");
	lcd_gotoxy2(4);
	lcd_num(3);
	for(int i=0;i<10;i++){
		lcd_cmd(0x18);
		_delay_ms(500);
	}
}

