#include "display.h"
#include "config.h"

#if DISPLAY_MODE == DISPLAY_MODE_LCD
#include "lcd.h"
#else
#include "uart.h"
#endif

void Display_Init(void)
{
#if DISPLAY_MODE == DISPLAY_MODE_LCD
    LCD_Init();
#else
    USART_Init();
    USART_SendString("\r\n--- ATmega32 EEPROM Storage System ---\r\n");
#endif
}

void Display_ShowValues(unsigned char current_value, unsigned char saved_value)
{
#if DISPLAY_MODE == DISPLAY_MODE_LCD
    LCD_SetCursor(0, 0);
    LCD_String("CURRENT: ");
    {
        char digits[8];
        digits[0] = (char)('0' + (current_value / 100));
        digits[1] = (char)('0' + ((current_value / 10) % 10));
        digits[2] = (char)('0' + (current_value % 10));
        digits[3] = ' ';
        digits[4] = ' ';
        digits[5] = ' ';
        digits[6] = ' ';
        digits[7] = '\0';
        LCD_String(digits);
    }

    LCD_SetCursor(1, 0);
    LCD_String("SAVED:   ");
    {
        char digits[8];
        digits[0] = (char)('0' + (saved_value / 100));
        digits[1] = (char)('0' + ((saved_value / 10) % 10));
        digits[2] = (char)('0' + (saved_value % 10));
        digits[3] = ' ';
        digits[4] = ' ';
        digits[5] = ' ';
        digits[6] = ' ';
        digits[7] = '\0';
        LCD_String(digits);
    }
#else
    USART_SendString("CURRENT: ");
    USART_SendNumber3(current_value);
    USART_SendString("\r\nSAVED:   ");
    USART_SendNumber3(saved_value);
    USART_SendString("\r\n");
#endif
}

void Display_ShowMessage(const char *line1, const char *line2)
{
#if DISPLAY_MODE == DISPLAY_MODE_LCD
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_String(line1);
    if (line2 != 0) {
        LCD_SetCursor(1, 0);
        LCD_String(line2);
    }
#else
    USART_SendString(line1);
    USART_SendString("\r\n");
    if (line2 != 0) {
        USART_SendString(line2);
        USART_SendString("\r\n");
    }
#endif
}
