/*
 * lcd.c
 * Pin mapping taken from the project's actual KiCad schematic/PCB
 * (16-pin LCD header J1, ATmega32 net labels):
 *   RS = PD4, RW = PD5, EN = PD6, D4-D7 = PC4-PC7
 * RW is wired to a GPIO (not tied to GND), so the driver must hold it
 * low to keep the LCD permanently in write mode.
 */

#include "lcd.h"
#include "config.h"
#include <util/delay.h>

#define LCD_CTRL_PORT  PORTD
#define LCD_CTRL_DDR   DDRD
#define LCD_RS         PD4
#define LCD_RW         PD5
#define LCD_EN         PD6

#define LCD_DATA_PORT  PORTC
#define LCD_DATA_DDR   DDRC
#define LCD_D4         PC4
#define LCD_D5         PC5
#define LCD_D6         PC6
#define LCD_D7         PC7

static void lcd_pulse_enable(void)
{
    LCD_CTRL_PORT |= (1 << LCD_EN);
    _delay_us(1);
    LCD_CTRL_PORT &= (unsigned char)~(1 << LCD_EN);
    _delay_us(100);
}

static void lcd_write_nibble(unsigned char nibble)
{
    LCD_DATA_PORT &= (unsigned char)~((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7));
    if (nibble & 0x01) LCD_DATA_PORT |= (1 << LCD_D4);
    if (nibble & 0x02) LCD_DATA_PORT |= (1 << LCD_D5);
    if (nibble & 0x04) LCD_DATA_PORT |= (1 << LCD_D6);
    if (nibble & 0x08) LCD_DATA_PORT |= (1 << LCD_D7);
    lcd_pulse_enable();
}

static void lcd_write_byte(unsigned char value, unsigned char is_data)
{
    if (is_data) {
        LCD_CTRL_PORT |= (1 << LCD_RS);
    } else {
        LCD_CTRL_PORT &= (unsigned char)~(1 << LCD_RS);
    }
    lcd_write_nibble((unsigned char)(value >> 4));
    lcd_write_nibble((unsigned char)(value & 0x0F));
}

void LCD_Command(unsigned char cmd)
{
    lcd_write_byte(cmd, 0);
    if (cmd == 0x01 || cmd == 0x02) {
        _delay_ms(2); /* clear / home need extra time */
    }
}

void LCD_Char(char c)
{
    lcd_write_byte((unsigned char)c, 1);
}

void LCD_String(const char *str)
{
    while (*str) {
        LCD_Char(*str);
        str++;
    }
}

void LCD_SetCursor(unsigned char row, unsigned char col)
{
    unsigned char addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Command(addr);
}

void LCD_Clear(void)
{
    LCD_Command(0x01);
}

void LCD_Init(void)
{
    LCD_CTRL_DDR |= (1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN);
    LCD_DATA_DDR |= (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);

    LCD_CTRL_PORT &= (unsigned char)~((1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN));
    LCD_DATA_PORT &= (unsigned char)~((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7));

    _delay_ms(40); /* power-on settle */

    /* HD44780 4-bit init sequence */
    lcd_write_nibble(0x03);
    _delay_ms(5);
    lcd_write_nibble(0x03);
    _delay_us(150);
    lcd_write_nibble(0x03);
    lcd_write_nibble(0x02); /* switch to 4-bit mode */

    LCD_Command(0x28); /* 4-bit, 2 line, 5x8 font */
    LCD_Command(0x0C); /* display ON, cursor OFF */
    LCD_Command(0x06); /* auto-increment cursor */
    LCD_Command(0x01); /* clear display */
}
