/*
 * config.h
 * Project 16: EEPROM-Based Data Logging and Configuration Storage System
 * Target: ATmega32, F_CPU = 8,000,000 Hz (internal/external clock, adjust if different)
 */

#ifndef CONFIG_H
#define CONFIG_H

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>

/* ---------------------------------------------------------------------
 * Display backend selection
 * The project's KiCad schematic/PCB wires a 16-pin HD44780 LCD header
 * (J1) to PD4/PD5/PD6 (RS/RW/EN) and PC4-PC7 (D4-D7); PD0/PD1 (RXD/TXD)
 * are left unconnected on that board, so DISPLAY_MODE_LCD is the
 * correct default for the actual hardware. DISPLAY_MODE_USART remains
 * available for bench testing on a breadboard with a serial terminal.
 * --------------------------------------------------------------------- */
#define DISPLAY_MODE_USART 0
#define DISPLAY_MODE_LCD   1

#ifndef DISPLAY_MODE
#define DISPLAY_MODE DISPLAY_MODE_LCD
#endif

/* ---------------------------------------------------------------------
 * Push buttons - PORTA, active LOW (internal pull-ups enabled)
 * --------------------------------------------------------------------- */
#define BTN_PORT   PORTA
#define BTN_DDR    DDRA
#define BTN_PIN    PINA

#define BTN_INC    PA0   /* Increment button */
#define BTN_DEC    PA1   /* Decrement button */
#define BTN_SAVE   PA2   /* Save button      */
#define BTN_READ   PA3   /* Read button       */
#define BTN_CLEAR  PA4   /* Clear button      */

/* ---------------------------------------------------------------------
 * LEDs and buzzer - PORTB
 * --------------------------------------------------------------------- */
#define IND_PORT   PORTB
#define IND_DDR    DDRB

#define SAVE_LED   PB0   /* Save / status LED */
#define ERROR_LED  PB1   /* Error LED         */
#define BUZZER     PB2   /* Buzzer            */

/* ---------------------------------------------------------------------
 * EEPROM address map
 * --------------------------------------------------------------------- */
#define EEPROM_ADDR_MAGIC    0x00   /* marks EEPROM as initialized              */
#define EEPROM_ADDR_VALUE    0x01   /* explicitly saved value (SAVE button)     */
#define EEPROM_ADDR_CURRENT  0x02   /* auto-saved value (every INC/DEC press)   */
#define EEPROM_MAGIC_BYTE    0xAA   /* sentinel written after first save        */

/* ---------------------------------------------------------------------
 * Value limits
 * --------------------------------------------------------------------- */
#define VALUE_MIN     0
#define VALUE_MAX     255
#define VALUE_DEFAULT 0

/* ---------------------------------------------------------------------
 * Timing
 * --------------------------------------------------------------------- */
#define DEBOUNCE_DELAY_MS  25
#define BEEP_DELAY_MS      250
#define LED_PULSE_MS       400

#endif /* CONFIG_H */
