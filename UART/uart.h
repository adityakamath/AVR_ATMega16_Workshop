void u1init(){
	UBRR1H=0x00;
	UBRR1L=0x67;
	UCSR1B|=(1<<TXEN1)|(1<<RXEN1);
}
void u1tx(char x){
	UDR1=x;
	while(!(UCSR1A&(1<<UDRE1)));
}
char u1rx(){
	while(!(UCSR1A&(1<<RXC1)));
	return UDR1;
}
void u1txstr(char *str){
	while(*str!='\0'){
		u1tx(*str);
		str++;
	}
}
char buffer[20];
void u1txnum(int no){
	sprintf(buffer, "%d", no);
	u1txstr(buffer);
}

