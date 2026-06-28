/*
 * display.h
 * Backend-agnostic display interface. Routes to USART or LCD depending
 * on DISPLAY_MODE in config.h, so main.c does not need to care which
 * physical output device is connected.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

void Display_Init(void);
void Display_ShowValues(unsigned char current_value, unsigned char saved_value);
void Display_ShowMessage(const char *line1, const char *line2);

#endif /* DISPLAY_H */
