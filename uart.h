#define UART_BAUD_RATE 9600L
#define UART_BAUD_CALC(UART_BAUD_RATE,F_CPU) ((F_CPU)/((UART_BAUD_RATE)*16L)-1)

/****************************************************************************
  Function definitions
****************************************************************************/
unsigned char ser_getc (void);	
void uart_putc(unsigned char);
void uart_puts (char *);
void uart_init (void);
