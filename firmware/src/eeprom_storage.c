/*
 * eeprom_storage.c
 *
 * EEPROM_Write/EEPROM_Read manipulate the ATmega32 EEARL/EEARH/EEDR/EECR
 * registers directly instead of relying on <avr/eeprom.h>, so the
 * write/read sequence required by the datasheet is explicit:
 *   write: set address+data -> set EEMWE -> set EEWE within 4 cycles
 *   read : set address -> set EERE -> read EEDR
 */

#include "eeprom_storage.h"
#include "config.h"

void EEPROM_Write(unsigned int address, unsigned char data)
{
    /* Wait for any previous write to complete */
    while (EECR & (1 << EEWE))
        ;

    EEARH = (unsigned char)(address >> 8);
    EEARL = (unsigned char)(address & 0xFF);
    EEDR = data;

    EECR |= (1 << EEMWE);   /* Master write enable */
    EECR |= (1 << EEWE);    /* Start the write (must follow EEMWE within 4 clock cycles) */
}

unsigned char EEPROM_Read(unsigned int address)
{
    while (EECR & (1 << EEWE))
        ;

    EEARH = (unsigned char)(address >> 8);
    EEARL = (unsigned char)(address & 0xFF);
    EECR |= (1 << EERE);    /* Start the read */
    return EEDR;
}

void EEPROM_Storage_Init(void)
{
    if (EEPROM_Read(EEPROM_ADDR_MAGIC) != EEPROM_MAGIC_BYTE) {
        EEPROM_Write(EEPROM_ADDR_VALUE, VALUE_DEFAULT);
        EEPROM_Write(EEPROM_ADDR_MAGIC, EEPROM_MAGIC_BYTE);
    }
}

void EEPROM_SaveValue(unsigned char value)
{
    EEPROM_Write(EEPROM_ADDR_VALUE, value);
    EEPROM_Write(EEPROM_ADDR_MAGIC, EEPROM_MAGIC_BYTE);
}

unsigned char EEPROM_LoadValue(void)
{
    return EEPROM_Read(EEPROM_ADDR_VALUE);
}

void EEPROM_ClearValue(void)
{
    EEPROM_Write(EEPROM_ADDR_VALUE, VALUE_DEFAULT);
    EEPROM_Write(EEPROM_ADDR_MAGIC, EEPROM_MAGIC_BYTE);
}
