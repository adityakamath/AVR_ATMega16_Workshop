#define F_CPU 8000000UL  /* 8 MHz CPU clock */
#include <avr/io.h>			//header file used to relate code to AVR device-specific IO definitions
#include <util/delay.h>		//header file used for generating software delay
#include "uart.h"			// header for UART communication Protocal

// PD4 (OCR1B)--> Right 
// PD5 (OCR1A)--> Left

#define black 0
#define white 1

#define stop	0
#define fwd		1
#define rev		2
#define Iterm_max 6
#define Iterm_min 0.09
#define base_speed 230

	uint8_t c = 1;
	char linefollowing_bw;

int s1_min=255, s2_min= 255, s3_min = 255;
int s1_max = 0, s2_max = 0, s3_max = 0;
float kp=.295; 
float kd=0;
float ki=0;
float Pterm, Iterm, Dterm;

	int error=0, prev_error=0, rate=0, pwm_duty, integ=0;
	int s1=0, s2=0, s3=0, s1_d, s2_d, lft, rgt;


void Drive_Motor(unsigned char LEFT,unsigned char RGHT)
{
	if(RGHT==0)//if right == 0 then right motor will stop
	{
	PORTB&=~_BV(5);
	PORTB&=~_BV(4);
	}
	if(RGHT==1)//if right == 1 then right motor will go forward
	{
	PORTB&=~_BV(5);
	PORTB|=_BV(4);
	}
	if(RGHT==2)//if right == 2 then right motor will go backward
	{

     PORTB|=_BV(5);
	 PORTB&=~_BV(4);
	}
	
	if(LEFT==0)//if left == 0 then left motor will stop
	{
	PORTB&=~_BV(6);
	PORTB&=~_BV(7);
	}
	if(LEFT==1)//if left == 1 then left motor will go forward
	{
	PORTB&=~_BV(6);
	PORTB|=_BV(7);
	}
	if(LEFT==2)//if left == 2 then left motor will go backward
	{
	 PORTB|=_BV(6);
	 PORTB&=~_BV(7);
	}
}


void init(void)
{
	DDRB = 0xff;   	 				// Make Port B as Output:
	DDRD = 0xfa;     				// Make Port D as Output
	DDRA = 0x00;
	PORTD = 0xff;    				// a all pins on port D as high 
	TCCR1A=0xa1;       				//a to compare output mode & pwm 8 bit
	TCCR1B=0x01;         			// a to oscillator clk source no prescaling 
}

void init_ADC (void)
{
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
	ADMUX |= (1<<REFS0);
	ADMUX |= (1<<ADLAR);
	ADCSRA |= (1<<ADEN);
}


int ADC_read(int channel)
{
	ADMUX = (ADMUX&0xE0) | channel; //Select next channel
	ADCSRA |= (1<<ADSC);			//Start conversion by setting ADSC bit
	while( ADCSRA & _BV(ADSC) );	//Wait for conversion to end
	return ADCH;					//return conversion result
}

char sensor_calibrate ()
{
	while (c)
	{
	if(bit_is_clear(PIND,2))
	{
	linefollowing_bw = black;
	c = 0;
	}
	else if(bit_is_clear(PIND,3))
	{
	linefollowing_bw = white;
	c = 0;
	}
	}
	
	int adc_value;
	OCR1B=160;
	OCR1A=160;

	c = 1;
	while (c)
	{

		if (!linefollowing_bw)
		{
		
			Drive_Motor(1,2);
		
			for(int i=0;i<250;i++)
			{
				_delay_ms(2);
				adc_value = ADC_read(1);
				s1_min = (adc_value < s1_min) ? adc_value:s1_min;
				adc_value = ADC_read(2);
				s2_min = (adc_value < s2_min) ? adc_value:s2_min;
				adc_value = ADC_read(3);
				s3_min = (adc_value < s3_min) ? adc_value:s3_min;
			}

			Drive_Motor(2,1);
			_delay_ms(50);

			for(int i=0;i<350;i++)
			{
				_delay_ms(2);
				adc_value = ADC_read(1);
				s1_max = (adc_value > s1_max) ? adc_value:s1_max;
				adc_value = ADC_read(2);
				s2_max = (adc_value > s2_max) ? adc_value:s2_max;
				adc_value = ADC_read(3);
				s3_max = (adc_value > s3_max) ? adc_value:s3_max;
			}

			Drive_Motor(1,2);
			while (ADC_read(2) < (s2_max-20));
			Drive_Motor(0,0);
			s1_max-=s1_min;
			s2_max-=s2_min;
			s3_max-=s3_min;
			c = 0;
		}

		if (linefollowing_bw)
		{
		
			Drive_Motor(2,1);
		
			for(int i=0;i<90;i++)
			{
				_delay_ms(10);
				adc_value = ADC_read(1);
				s1_max = (adc_value > s1_max) ? adc_value:s1_max;
				adc_value = ADC_read(2);
				s2_max = (adc_value > s2_max) ? adc_value:s2_max;
				adc_value = ADC_read(3);
				s3_max = (adc_value > s3_max) ? adc_value:s3_max;
			}

			Drive_Motor(1,2);
			_delay_ms(300);
		
			for(int i=0;i<90;i++)
			{
				_delay_ms(10);
				adc_value = ADC_read(1);
				s1_min = (adc_value < s1_min) ? adc_value:s1_min;
				adc_value = ADC_read(2);
				s2_min = (adc_value < s2_min) ? adc_value:s2_min;
				adc_value = ADC_read(3);
				s3_min = (adc_value < s3_min) ? adc_value:s3_min;
			}

			Drive_Motor(2,1);
			while (ADC_read(2) < (s2_min+20));
			Drive_Motor(0,0);
			s1_max-=s1_min;
			s2_max-=s2_min;
			s3_max-=s3_min;
			c = 0;
		}


	}
return linefollowing_bw;
}


void black_line_follow ()
{
		lft=fwd;
		rgt=fwd;
		s1=ADC_read(1);
		s2=ADC_read(2);
		s3=ADC_read(3);
		s1=(s1-s1_min);
		s2=(s2-s2_min);
		s3=(s3-s3_min);

		error = -((s1_max-s1) - (s3_max-s3));
		rate = error - prev_error;

		Pterm = error*kp;
		Iterm += ki*error;
		Dterm = rate*kd;
		
		if (error>0)
			PORTB = (1<<0) | (0<<1);
		else if (error<0)
			PORTB = (1<<1) | (0<<0);

		if (Iterm > Iterm_max)		Iterm = Iterm_min/2;
		if (Iterm < Iterm_min)		Iterm = Iterm_min;

		pwm_duty = Pterm - (2*Dterm) + Iterm;
//		pwm_duty=error/kp+rate/kd+integ;
		
		if(error > prev_error)
		{
		s1_d=base_speed+pwm_duty-4*Dterm;
		s2_d=base_speed-pwm_duty-4*Dterm;
		}
		else
		{
		s1_d=base_speed+pwm_duty;
		s2_d=base_speed-pwm_duty;
		}

		if (s1_d>255)
		{
			s2_d=s2_d-(s1_d-255);
			s1_d=255;
		}
		if (s2_d>255)
		{	
			s1_d=s1_d-(s2_d-255);
			s2_d=255;
		}
		if (s1_d<0)
		{
			s1_d=0;
			
		}
		if (s2_d<0)
		{
			s2_d=0;
		}
		if (s1<45 && s2<45 && s3<45)	
		{
			s1_d=180;
			s2_d=180;
			lft = rev;
			rgt = rev;
			Drive_Motor(lft,rgt);
			_delay_ms(150);
		}

		OCR1B=s1_d;
		OCR1A=s2_d;

// PD4 (OCR1B)--> Right 
// PD5 (OCR1A)--> Left

		Drive_Motor(lft,rgt);
		prev_error = error;
}


void white_line_follow()
{
}

int main()
{
	uart_init();
	init();
	init_ADC();
	sensor_calibrate();
		
	while(1)
	{	
		if (linefollowing_bw == black)
		{
		black_line_follow();
		}
		else
		if (linefollowing_bw == white)
		{
		white_line_follow();
		}

	}

	return 0;
}

