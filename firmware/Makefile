MCU       = atmega32
F_CPU     = 8000000UL
CC        = avr-gcc
OBJCOPY   = avr-objcopy
SIZE      = avr-size

SRC_DIR   = src
INC_DIR   = include
HEX_DIR   = hex

# If your avr-gcc came bundled with MPLAB XC8 (e.g. under
# "...\Microchip\xc8\vX.Y\avr\bin"), it ships WITHOUT classic-AVR
# device headers/CRT/lib for parts like the ATmega32 — those live in
# Microchip's "ATmega_DFP" Device Family Pack instead. Point DFP_DIR
# at the installed pack version if you hit "iom32.h: No such file" or
# "cannot find crtatmega32.o" errors. A plain WinAVR/avr-gcc install
# (or Linux/macOS package) already includes these and DFP_DIR can be
# left empty.
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
