//scanning 3 sensors and finding min and max values of each

#include<avr/io.h>
#include<util/delay.h>
#define USART_BAUDRATE 9600
#define F_CPU 8000000UL
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)


void Drive_motor(unsigned char left, unsigned char right)
{
	if(left==0) //stops left motor
	{
		PORTB&=~_BV(4);
		PORTB&=~_BV(5);
	}
	
	if(right==0) //stops right motor
	{
		PORTB&=~_BV(6);
		PORTB&=~_BV(7);
	}
	
	if(left==1) //left motor - forward
	{
		PORTB|=_BV(4);
		PORTB&=~_BV(5);
	}

	if(right==1) //right motor - forward
	{
		PORTB|=_BV(7);
		PORTB&=~_BV(6);
	}

	if(left==2) //left motor - backward
	{
		PORTB|=_BV(5);
		PORTB&=~_BV(4);
	}

	if(right==2) //right motor - backward
	{
		PORTB|=_BV(6);
		PORTB&=~_BV(7);
	}
} 

int main(void)
{
	DDRB=0xFF;
	DDRD=0xFF;
	PORTB=0xFF;
	PORTD=0xFF;
	PORTA=0x00;
	
	DDRA&=~_BV(0);
	PORTA|=_BV(0);
	

	
	UCSRB|=(1<<RXEN)|(1<<TXEN); //turn on rx and tx (can also be done using hex instead of 1<<RXEN)
//	1<<RXEN --> 1 shifted to RXEN.
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1); // asynchronous | 8bit character sizes.

	UBRRL=BAUD_PRESCALE; //load lower 8bits of baud rate into UBRR low register
	UBRRH=(BAUD_PRESCALE>>8); //load upper 8 bits of baud rate into UBRR high register
	

	ICR1=10000;
	TCCR1A=(1<<COM1A1|1<<COM1B1|0<<COM1A0|0<<COM1B0|0<<FOC1A|0<<FOC1B|0<<WGM11|0<<WGM10); //check datasheet 
//(can be written in hex/bin)
	TCCR1B=(1<<WGM13|1<<CS11|0<<CS12|0<<CS10|0<<WGM12|0<<ICNC1|0<<ICES1);    //check datasheet (can be written 
//in hex/bin)

	ADMUX=0x61; //registers for ADC
	ADCSRA=0x8F;
	

	
	int a[250][3];
	int g,r,c,y=ADMUX;
	Drive_motor(1,2);
	for(int j=0;j<3;j++)
	{
	for(int i=0;i<250;i++)
	{
		ADCSRA|=(1<<ADSC); //conversion started for every loop
		while((ADCSRA&(1<<ADIF))==0) {}		
		a[i][j]=ADCH;
		_delay_ms(50);
	}

	for ( r=0; r <=248; r++) //sorting
   	{
     		for ( g=r+1;g<=249;g++)
        	{
            		if ( a[r][j]  < a[g][j] )
               		{
                   		c=a[r][j];                // these 3 statements swap values
                   		a[r][j] = a[g][j];          // in the 2 cells being compared  
                   		a[g][j] = c;
                	}
       		 }   
  	 } 
	y=y+1;
	ADMUX=y;
	}
Drive_motor(0,0);

for(int k=0;k<3;k++) //min
{
	while((UCSRA&(1<<UDRE))==0){}
	UDR=a[0][k];
}
for(int l=0;l<3;l++) //max
{
	while((UCSRA&(1<<UDRE))==0){}
	UDR=a[249][l];
}

}

		
	
	
