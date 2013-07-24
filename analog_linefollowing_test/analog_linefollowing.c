#define F_CPU 8000000UL  
#include <avr/io.h>
#include <util/delay.h>

int l1=255, l2= 255, l3 = 255;
int u1 = 0, u2 = 0, u3 = 0;

void Drive_Motor(unsigned char LEFT,unsigned char RGHT)
{
	
	if(RGHT==0)
	{
	
		PORTB&=~_BV(5);
	
		PORTB&=~_BV(4);
	
	}
	
	if(RGHT==1)
	{
	
		PORTB&=~_BV(5);
	
		PORTB|=_BV(4);
	
	}
	
	if(RGHT==2)	
	{

     
		PORTB|=_BV(5);
	 
		PORTB&=~_BV(4);
	
	}
	
	
	if(LEFT==0)	
	{
	
		PORTB&=~_BV(6);
	
		PORTB&=~_BV(7);
	
	}
	
	if(LEFT==1)	
	{
	
		PORTB&=~_BV(6);
	
		PORTB|=_BV(7);
	
	}	
	
	if(LEFT==2)
	
	{
	 
		PORTB|=_BV(6);
	 
		PORTB&=~_BV(7);
	
	}

}


void init(void)
{
	
	DDRB = 0xff;   	 					
	DDRD = 0xfa;     				 	DDRA = 0x00;
	
	PORTD = 0xff;    				 
	
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0)| (1<<ADEN);
	ADMUX |= (1<<REFS0) | (1<<ADLAR);	
	TCCR1A=0xa1;       				
	
	TCCR1B=0x01;         			}

void send(int b)
{
	
	UDR = b;
	while ((UCSRA & (1 << UDRE)) == 0) {};
	_delay_ms(10);
	
}

int adc_start(int channel)
{
	ADMUX = (ADMUX&0xE0) | channel; //Select next channel
	ADCSRA |= (1<<ADSC);			//Start conversion by setting ADSC bit
	while( ADCSRA & _BV(ADSC) );	//Wait for conversion to end
	return ADCH;					//return conversion result
}

void init_sensors()
{
	int a;
	OCR1B=160;
	OCR1A=160;
	Drive_Motor(1,2);
	for(int i=0;i<90;i++)
	{
		_delay_ms(10);
		a=adc_start(1);
		l1=(a<l1)?a:l1;
		a=adc_start(2);
		l2=(a<l2)?a:l2;
		a=adc_start(3);
		l3=(a<l3)?a:l3;
	}
	Drive_Motor(2,1);
	_delay_ms(300);
	for(int i=0;i<90;i++)
	{
		
		_delay_ms(10);
		a=adc_start(1);
		u1=(a>u1)?a:u1;
		a=adc_start(2);
		u2=(a>u2)?a:u2;
		a=adc_start(3);
		u3=(a>u3)?a:u3;
	}
	
	Drive_Motor(1,2);
	while(adc_start(2)<(u2-20));
	Drive_Motor(0,0);
	u1-=l1;
	u2-=l2;
	u3-=l3;

}

int main()
{
	
	init();
	float kp=1.5; 
	float kd=2.5;
	
	
	int error=0, prev_error=0, rate=0, pwm_duty, integ=0;
	
	int v1=0, v2=0, v3=0, s1, s2,a,b;
	
	while(bit_is_set(PIND,3));
	init_sensors();
	

		
	while(1)
	
	{	
		a=1;
		b=1;
		v1=adc_start(1);
		v2=adc_start(2);
		v3=adc_start(3);
		
		v1=(v1-l1);
		v2=(v2-l2);
		v3=(v3-l3);

		error=-((u1-v1)-(u3-v3));
		rate=error-prev_error;
		prev_error=error;
		integ+=0.001*error;
		
		if (error>0)
			PORTB = (1<<0) | (0<<1);
		else if (error<0)
			PORTB = (1<<1) | (0<<0);
		pwm_duty=error/kp+rate/kd+integ;
		
		s1=240+pwm_duty;
		s2=240-pwm_duty;

		if (s1>255)
		{
			s2=s2-(s1-255);
			s1=255;
		}
		if (s2>255)
		{	
			s1=s1-(s2-255);
			s2=255;
		}
		if (s1<0)
		{
			s1=0;
			
		}
		if (s2<0)
		{
			s2=0;
		}
		if (v1<45 && v2<45 && v3<45)	
		{
			s1=180;
			s2=180;
			a=2;
			Drive_Motor(0,0);
			break;
		}
		OCR1B=s1;
		OCR1A=s2;
		Drive_Motor(a,b);

	}
	return 0;
}

