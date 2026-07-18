# ATmega32 EEPROM Data Logging and Configuration Storage System

Demonstrates internal EEPROM read/write on the ATmega32 by letting a
user increment/decrement a value in RAM, save it to EEPROM, read it
back, clear it, and recover it after reset or a power cycle.

## Repository structure

```
ATmega32_EEPROM_Data_Storage/
├── README.md
├── firmware/
│   ├── src/        button.c, display.c, eeprom_storage.c, indicator.c, lcd.c, main.c, uart.c
│   ├── include/    matching headers + config.h
│   └── hex/        firmware.hex / firmware.elf
├── kicad/
│   ├── schematic/  ATmega32_EEPROM_Storage.kicad_sch (+ project files), rendered PDF
│   ├── pcb/        ATmega32_EEPROM_Storage.kicad_pcb (+ project files), rendered PDF
│   └── gerber/     Gerber + drill (.drl) output
├── simulide/
│   ├── circuit/     EEPROM_Storage.sim1
│   └── screenshots/ test-run screenshots
├── documentation/
│   ├── block_diagram.png
│   ├── flowchart.png
│   ├── pin_mapping_table.md
│   ├── eeprom_address_table.md
│   ├── test_results.md
│   ├── erc_report.txt
│   └── drc_report.txt
└── media/
    └── demonstration_video.mp4
```

The schematic and PCB are kept in separate `kicad/schematic/` and
`kicad/pcb/` folders, each with its own copy of the KiCad project file
so either one opens directly in KiCad.

## Firmware

Source lives in `firmware/`, written in C for the ATmega32 at 8 MHz
(adjust `F_CPU` in `firmware/include/config.h` if a different clock is
used).

Build with avr-gcc:

```
cd firmware
make
```

(or open the `firmware/` folder as an MPLAB for VS Code project, using
the same source/include paths.)

### Behavior

| Action            | Result |
|--------------------|--------|
| Power ON / OFF   | Last value loaded from EEPROM and shown as CURRENT |
| INC button (PA0)   | CURRENT increases (clamped at 255) |
| DEC button (PA1)   | CURRENT decreases (clamped at 0) |
| SAVE button (PA2)  | CURRENT written to EEPROM; SAVE_LED + buzzer confirm |
| READ button (PA3)  | SAVED re-read from EEPROM and displayed |
| CLEAR button (PA4) | EEPROM and CURRENT reset to default (0); both LEDs + buzzer confirm |

Display output is a 16x2 HD44780 LCD (RS=PD4, RW=PD5, EN=PD6,
D4-D7=PC4-PC7 via the 16-pin header J1), showing:

```
CURRENT: 025
SAVED:   020
```

`DISPLAY_MODE` in `firmware/include/config.h` can be switched to
`DISPLAY_MODE_USART` to print the same values over USART (9600 baud,
8N1, PD1/TXD) for bench testing without an LCD attached.

### EEPROM layout

See [documentation/eeprom_address_table.md](documentation/eeprom_address_table.md).

### Pin mapping

See [documentation/pin_mapping_table.md](documentation/pin_mapping_table.md).

## SimulIDE simulation

`simulide/circuit/EEPROM_Storage.sim1` mirrors the KiCad PCB wiring
(4-bit LCD interface, RS/RW/EN on PD4-PD6, data lines on PC4-PC7) so
the same firmware behaves identically in simulation and on hardware.
Test results and screenshots for all required test cases are in
[documentation/test_results.md](documentation/test_results.md).

## KiCad design

Schematic and PCB pass ERC and DRC with no violations (see
`documentation/erc_report.txt` and `documentation/drc_report.txt`).
Gerber and drill files are in `kicad/gerber/`.
