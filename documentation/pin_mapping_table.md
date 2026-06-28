# ATmega32 Pin Mapping Table

| ATmega32 Pin / Resource | Function       | Connected Device       | Firmware Macro |
|--------------------------|-----------------|--------------------------|------------------|
| Internal EEPROM          | Non-volatile storage | Stores saved value | `EEPROM_ADDR_VALUE`, `EEPROM_ADDR_MAGIC` |
| PA0                       | Digital input  | Increment button         | `BTN_INC` |
| PA1                       | Digital input  | Decrement button         | `BTN_DEC` |
| PA2                       | Digital input  | Save button               | `BTN_SAVE` |
| PA3                       | Digital input  | Read button               | `BTN_READ` |
| PA4                       | Digital input  | Clear button              | `BTN_CLEAR` |
| PB0                       | Digital output | Save / status LED        | `SAVE_LED` |
| PB1                       | Digital output | Error LED                 | `ERROR_LED` |
| PB2                       | Digital output | Buzzer (via R4 + NPN driver transistor Q1) | `BUZZER` |
| PD4                       | Digital output | LCD RS (J1 pin 4)         | `LCD_RS` |
| PD5                       | Digital output | LCD RW (J1 pin 5, held low) | `LCD_RW` |
| PD6                       | Digital output | LCD EN (J1 pin 6)         | `LCD_EN` |
| PC4-PC7                   | Digital output | LCD D4-D7 (J1 pins 11-14) | `LCD_D4..LCD_D7` |
| PD0 / RXD, PD1 / TXD      | unused on this board | not wired to anything (no serial header on the actual PCB) | n/a |

J1 is a standard 16-pin HD44780 header: pin 1=GND, pin 2=+5V, pin
3=VEE (contrast, via potentiometer RV1), pins 7-10=GND (D0-D3, unused
in 4-bit mode), pin 15=LED+ (via resistor R5), pin 16=LED- (GND).

DDR configuration used by the firmware:

```
DDRA = 0x00   // PORTA: all 5 buttons as inputs (internal pull-ups enabled)
DDRB = 0x07   // PB0 (SAVE_LED), PB1 (ERROR_LED), PB2 (BUZZER) as outputs
DDRC |= 0xF0  // PC4-PC7: LCD D4-D7 as outputs
DDRD |= 0x70  // PD4-PD6: LCD RS/RW/EN as outputs
```

Display backend is selected at compile time in `firmware/include/config.h`
via `DISPLAY_MODE` (`DISPLAY_MODE_USART` or `DISPLAY_MODE_LCD`).
