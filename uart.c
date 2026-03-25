#include <avr/io.h>
#include <avr/pgmspace.h>
#include "uart.h"
#define F_CPU 8000000UL

void UART_init()
{
    #ifndef BAUD_RATE
    #define BAUD_RATE 9600
    #endif

	// set rate
	UBRR0H = (unsigned char) (((F_CPU/(BAUD_RATE*16UL))) - 1) >> 8;
	UBRR0L = (unsigned char) ((F_CPU/(BAUD_RATE*16UL))) - 1;

	// Enable reciever and transmitter
	UCSR0B |= (1 << RXEN0)|(1 << TXEN0);
}

void UART_putc(const unsigned char data)
{
	// wait for empty transmit buffer
	while(!(UCSR0A & (1<<UDRE0)));

	// send data to output register
	UDR0 = data;
}

void UART_puts(const char* charString)
{
	// iterate through string
	while(*charString > 0)
		// print character
		UART_putc(*charString++);
}

void UART_puthex8(uint8_t val)
{
    // extract upper and lower nibbles from input value
    uint8_t upperNibble = (val & 0xF0) >> 4;
    uint8_t lowerNibble = val & 0x0F;

    // convert nibble to its ASCII hex equivalent
    upperNibble += upperNibble > 9 ? 'A' - 10 : '0';
    lowerNibble += lowerNibble > 9 ? 'A' - 10 : '0';

    // print the characters
    UART_putc(upperNibble);
    UART_putc(lowerNibble);
}

unsigned char UART_getc(void)
{
	// wait for data to be received
	while(!(UCSR0A & (1 << RXC0)));

	// get data to output register
	return UDR0;
}

void UART_puts_p(const char* ps)
{
    register char c;

    while ((c = pgm_read_byte(ps++)))
        UART_putc(c);
}

void UART_puthex16(uint16_t val)
{
    // extract upper and lower bytes from the 16-bit integer
    uint8_t upperByte = (val >> 8) & 0xFF;
    uint8_t lowerByte = val & 0xFF;

    // print the upper and lower bytes as hex
    UART_puthex8(upperByte);
    UART_puthex8(lowerByte);
}

void UART_puthex32(uint32_t val)
{
    // extract the four bytes from the 32-bit integer
    uint8_t byte1 = (val >> 24) & 0xFF;  // most significant byte
    uint8_t byte2 = (val >> 16) & 0xFF;
    uint8_t byte3 = (val >> 8) & 0xFF;
    uint8_t byte4 = val & 0xFF;          // least significant byte

    // print the four bytes as hex
    UART_puthex8(byte1);
    UART_puthex8(byte2);
    UART_puthex8(byte3);
    UART_puthex8(byte4);
}

void UART_putbit(uint8_t bit)
{
    // Send '0' or '1' as a character ('0' = 48, '1' = 49 in ASCII)
    if (bit == 0) 
        UART_putc('0');
    else if (bit == 1)
        UART_putc('1');
    else
        UART_putc('E'); // Error handling for invalid bit
}