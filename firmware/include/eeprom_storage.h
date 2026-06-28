/*
 * eeprom_storage.h
 * Direct register-level EEPROM read/write driver for the ATmega32 and
 * higher-level helpers for storing/recovering the configuration value.
 */

#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <avr/io.h>

/* Low-level raw EEPROM access (matches the assignment's example signatures) */
void EEPROM_Write(unsigned int address, unsigned char data);
unsigned char EEPROM_Read(unsigned int address);

/* High-level configuration storage helpers */
void EEPROM_Storage_Init(void);
void EEPROM_SaveValue(unsigned char value);
unsigned char EEPROM_LoadValue(void);
void EEPROM_ClearValue(void);

#endif /* EEPROM_STORAGE_H */
