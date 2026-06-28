/*
 * lcd.h
 * 16x2 HD44780 LCD driver, 4-bit mode.
 * Wiring (per the project's KiCad schematic/PCB, J1 16-pin LCD header):
 *   RS = PD4, RW = PD5, EN = PD6, D4..D7 = PC4..PC7.
 * RW is driven by the MCU (not tied to GND) and is held low by the
 * driver since the LCD is only ever written to, never read from.
 */

#ifndef LCD_H
#define LCD_H

void LCD_Init(void);
void LCD_Command(unsigned char cmd);
void LCD_Char(char c);
void LCD_String(const char *str);
void LCD_SetCursor(unsigned char row, unsigned char col); /* row: 0 or 1 */
void LCD_Clear(void);

#endif /* LCD_H */
