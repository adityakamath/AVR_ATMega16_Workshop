#define RS 0

#define E  2 

#include<avr/io.h>
#include<util/delay.h>

void lcd_init();
void enable();
void lcd_cmd(unsigned char c);

void lcd_char(unsigned char c);
void lcd_string(unsigned char *str);
void lcd_num(unsigned int n);

void lcd_gotoxy1(unsigned char);
void lcd_gotoxy2(unsigned char);
void lcd_str(unsigned char*);


void lcd_init()
{
 
	DDRA=0xff;								//making port A as output 

	lcd_cmd(0x01);							//cmd for clear screen
	lcd_cmd(0x02);							//cmd for return to home 
	lcd_cmd(0x28);							//cmd to operate lcd 4bit mode 
	lcd_cmd(0x0f);							//cmd for cursor blinking  
	lcd_cmd(0x80);							//cmd for start the cursor on first line

}


void enable()						//to latch the data
{
	 PORTA|=(1<<E);
	 _delay_ms(1);
	 PORTA&=~(1<<E);				
	 _delay_ms(1);
}

void lcd_cmd(unsigned char c)
{
 PORTA&=~(1<<RS);										//for lcd to take as command
 PORTA=(c&0xf0);
 
 enable();

 PORTA=((c&0x0f)<<4);
 enable();
}

void lcd_char(unsigned char c)
{

	 PORTA|=(1<<RS);							//for lcd to take as data
	 
	PORTA=(0x01)|(c&0xf0);
	enable();

	PORTA=(0x01)|((c&0x0f)<<4);
	enable();
  
}


void lcd_string(unsigned char *str)
{
 PORTA|=(1<<RS);									//for lcd to take as data
 int i=0;
 while(*str!='\0')
 {
 if(i<16)
   lcd_cmd(0x80+i);
 else lcd_cmd(0xc0+(i%16));
  
  PORTA=(0x03)|(*str&0xf0);
 
  enable();

  PORTA=(0x03)|((*str&0x0f)<<4);
  enable();
  str++;
  i++;
 }
}


void lcd_gotoxy1(unsigned char postion)				//go to particular postion on row1
{

if(postion<16)
lcd_cmd(0x80+postion);

else
{
lcd_string("postion 0-15");
_delay_ms(1000);
lcd_cmd(0x01);

}

}

void lcd_gotoxy2(unsigned char postion)		//go to particular postion on row2
{

if(postion<16)
lcd_cmd(0xc0+postion);

else
{
lcd_string("postion 0-15");
_delay_ms(1000);
lcd_cmd(0x01);

}

}


void lcd_num(unsigned int n)				//to display number on lcd
{
  int a[6],i=0;
  if(n==0)
  {
	lcd_char('0');
  }
  else
  {
		  while(n>0)
		  { 
		   a[i]=n%10;
		   n/=10;
		   i++;
		  }

		  for (i=i-1;i>=0;i--)
		  { 
			PORTA=(0x03)|((48+a[i])&0xf0);
		 
		   enable();

		  PORTA=(0x03)|(((48+a[i])&0x0f)<<4);
		  enable();
		  }
  
  }

}

























void lcd_str(unsigned char *data)
{
while(*data!='\0')
{
lcd_char(*data);
data++;
_delay_ms(20);
}

}

