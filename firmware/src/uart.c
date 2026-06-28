#include "uart.h"
#include "config.h"

void USART_Init(void)
{
    unsigned int ubrr = (F_CPU / (16UL * UART_BAUD)) - 1;

    UBRRH = (unsigned char)(ubrr >> 8);
    UBRRL = (unsigned char)ubrr;

    UCSRB = (1 << TXEN) | (1 << RXEN);          /* enable TX and RX */
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0); /* 8 data bits, 1 stop, no parity */
}

void USART_Transmit(unsigned char data)
{
    while (!(UCSRA & (1 << UDRE)))
        ;
    UDR = data;
}

void USART_SendString(const char *str)
{
    while (*str) {
        USART_Transmit((unsigned char)*str);
        str++;
    }
}

void USART_SendNumber3(unsigned char value)
{
    char digits[4];
    digits[0] = (char)('0' + (value / 100));
    digits[1] = (char)('0' + ((value / 10) % 10));
    digits[2] = (char)('0' + (value % 10));
    digits[3] = '\0';
    USART_SendString(digits);
}
