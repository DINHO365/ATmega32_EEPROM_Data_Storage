MCU       = atmega32
F_CPU     = 8000000UL
CC        = avr-gcc
OBJCOPY   = avr-objcopy
SIZE      = avr-size

SRC_DIR   = src
INC_DIR   = include
HEX_DIR   = hex

# Optional: set to a Microchip ATmega_DFP pack path if avr-gcc is
# missing classic-AVR device headers/CRT/lib for the ATmega32.
DFP_DIR   ?=
DFP_INC   = $(DFP_DIR)/include
DFP_LIB   = $(DFP_DIR)/gcc/dev/$(MCU)/avr5

CFLAGS    = -Wall -Wextra -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I$(INC_DIR) -std=gnu11
LDFLAGS   = -mmcu=$(MCU)
ifneq ($(DFP_DIR),)
CFLAGS   += -I$(DFP_INC)
LDFLAGS  += -B$(DFP_LIB)
endif

SOURCES   = $(wildcard $(SRC_DIR)/*.c)
OBJECTS   = $(SOURCES:.c=.o)
TARGET    = $(HEX_DIR)/firmware

all: $(TARGET).hex

$(TARGET).elf: $(OBJECTS)
	mkdir -p $(HEX_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	$(SIZE) $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET).elf $(TARGET).hex

.PHONY: all clean
