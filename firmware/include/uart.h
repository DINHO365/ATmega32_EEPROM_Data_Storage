/*
 * uart.h
 * Simple USART driver (PD0/RXD, PD1/TXD) for sending CURRENT/SAVED values
 * to a serial terminal at 9600 baud, 8N1.
 */

#ifndef UART_H
#define UART_H

#define UART_BAUD 9600UL

void USART_Init(void);
void USART_Transmit(unsigned char data);
void USART_SendString(const char *str);
void USART_SendNumber3(unsigned char value); /* zero-padded 3-digit, e.g. 025 */

#endif /* UART_H */
