# ATmega32 EEPROM Data Logging and Configuration Storage System

Project 16 (PBL): demonstrates internal EEPROM read/write on the
ATmega32 by letting a user increment/decrement a value in RAM, save it
to EEPROM, read it back, clear it, and recover it after reset or a
power cycle.

## Repository structure

Matches the assignment's suggested layout:

```
ATmega32_EEPROM_Data_Storage/
├── README.md
├── firmware/
│   ├── src/        button.c, display.c, eeprom_storage.c, indicator.c, lcd.c, main.c, uart.c
│   ├── include/    matching headers + config.h
│   └── hex/        build output (.hex) goes here
├── kicad/
│   ├── schematic/  ATmega32_EEPROM_Storage.kicad_sch (+ .kicad_pro/.kicad_prl), rendered PDF, report.txt
│   ├── pcb/         ATmega32_EEPROM_Storage.kicad_pcb (+ .kicad_pro), rendered PDF, report.txt
│   └── gerber/      Gerber + drill (.drl) output
├── simulide/
│   ├── circuit/     EEPROM_Storage.sim1
│   └── screenshots/ EEPROM_Storage.png + 8 test-run screenshots
├── documentation/
│   ├── block_diagram.png
│   ├── flowchart.png
│   ├── pin_mapping_table.md
│   ├── eeprom_address_table.md
│   ├── test_results.md
│   ├── erc_report.txt          (0 violations)
│   └── drc_report.txt          (0 violations, 0 unconnected nets)
└── media/
    └── demonstration_video_link.txt
```

The schematic and PCB are kept in separate `kicad/schematic/` and
`kicad/pcb/` folders per the assignment brief; each folder has its own
copy of the `.kicad_pro` project file so it still opens directly in
KiCad. `report.txt` (duplicated in both) is the schematic→PCB netlist
sync log used to verify the pin mapping below.

## Status

| Deliverable        | Status |
|---------------------|--------|
| Firmware            | Done — see `firmware/`, wired to match the real schematic pinout, builds cleanly (1.4 KB flash / 69 B RAM) |
| KiCad schematic     | Done — see `kicad/schematic/` (0 ERC violations) |
| KiCad PCB           | Done — see `kicad/pcb/` (0 DRC violations, 0 unconnected nets) |
| Gerbers / drill     | Done — see `kicad/gerber/` |
| Block diagram / flowchart | Done — `documentation/block_diagram.png`, `documentation/flowchart.png` |
| SimulIDE simulation | Done — see `simulide/`, run end-to-end, all 8 functional test cases passed |
| Documentation       | Pin map, EEPROM map, ERC/DRC reports — see `documentation/` |

## Firmware overview

Source lives in `firmware/`, written in C for avr-gcc/MPLAB targeting
`atmega32` at 8 MHz (adjust `F_CPU` in `firmware/include/config.h` if a
different clock is used).

```
firmware/
├── include/   config.h, eeprom_storage.h, button.h, indicator.h, display.h, uart.h, lcd.h
├── src/       matching .c files + main.c
├── hex/       build output (.hex) goes here
└── Makefile   avr-gcc build (optional, MPLAB project can be added alongside)
```

Build with avr-gcc (or open the `firmware/` folder as an MPLAB for VS
Code project and add the same source/include paths):

```
cd firmware
make
```

This has been built and verified successfully with the standalone
**AVR 8-bit GNU Toolchain** (avr-gcc 15.1.0, installed at
`C:\avr8-gnu-toolchain-win32_x86_64\bin`, which ships ATmega32 headers
natively): all 7 source files compile with zero warnings under
`-Wall -Wextra`, and the linked image uses ~1.4 KB flash / ~69 B RAM,
well within the ATmega32's 32 KB/2 KB. `make` wasn't installed, so
each compile/link/objcopy step was run directly with the same flags
the Makefile uses.

If instead your `avr-gcc` comes bundled with **MPLAB XC8** rather than
a standalone avr-gcc install, it ships without classic-AVR device
headers/CRT/lib — you'll hit `iom32.h: No such file` or `cannot find
crtatmega32.o`. Fix by pointing at Microchip's installed `ATmega_DFP`
pack (typically under
`%USERPROFILE%\.mchp_packs\Microchip\ATmega_DFP\<version>`):

```
make DFP_DIR="C:\Users\<you>\.mchp_packs\Microchip\ATmega_DFP\3.6.299"
```

(or pass the equivalent `-I<DFP>\include` / `-B<DFP>\gcc\dev\atmega32\avr5`
flags manually if invoking avr-gcc directly without `make`).

### Behavior

| Action            | Result |
|--------------------|--------|
| Power ON / Reset   | Last saved value loaded from EEPROM and shown as CURRENT/SAVED |
| INC button (PA0)   | CURRENT increases (clamped at 255) |
| DEC button (PA1)   | CURRENT decreases (clamped at 0) |
| SAVE button (PA2)  | CURRENT written to EEPROM; SAVE_LED + buzzer confirm |
| READ button (PA3)  | SAVED re-read from EEPROM and displayed |
| CLEAR button (PA4) | EEPROM and CURRENT reset to default (0); both LEDs + buzzer confirm |

Display output defaults to the 16x2 HD44780 LCD wired on the actual
PCB (RS=PD4, RW=PD5, EN=PD6, D4-D7=PC4-PC7 via the 16-pin header J1),
showing:

```
CURRENT: 025
SAVED:   020
```

For breadboard/bench testing without the LCD, set `DISPLAY_MODE` to
`DISPLAY_MODE_USART` in `firmware/include/config.h` to print the same
values over USART (9600 baud, 8N1, PD1/TXD) instead — note PD0/PD1 are
not connected to anything on the actual board/schematic.

### EEPROM layout

See [documentation/eeprom_address_table.md](documentation/eeprom_address_table.md).

### Pin mapping

See [documentation/pin_mapping_table.md](documentation/pin_mapping_table.md).

## SimulIDE simulation

`simulide/circuit/EEPROM_Storage.sim1` originally wired the LCD in
**8-bit mode** (D0-D7 all tied to PC0-PC7, RW hard-grounded), which
didn't match the finalized KiCad PCB's **4-bit mode** wiring (only
D4-D7 on PC4-PC7, RW driven by PD5). Since the assignment requires
testing the same circuit in SimulIDE and hardware, the `.sim1` file
was edited to match the PCB: the D0-D3 connections were removed and
RW was rewired from GND to `mega32-1-PORTD5`.

The circuit was then run end-to-end with `firmware/hex/firmware.hex`
loaded into the `mega32-1` MCU. Two more issues were found and fixed
during that run:
- The LCD component itself was rendering mirrored (rows swapped,
  characters reversed) — fixed via SimulIDE's Mirror H/V on the
  component (cosmetic wire-routing crossover left over from the flip
  has no electrical effect and was left as-is).
- First boot showed `CURRENT: 000GE` — a real firmware bug: the
  startup banner `"EEPROM STORAGE"` (14 chars) wasn't fully overwritten
  by the shorter `"CURRENT: 000"` (12 chars), leaving `GE` from
  `"STORAGE"` in columns 13-14. Fixed in `firmware/src/display.c` by
  padding LCD output to the full 16 columns; rebuilt and reflashed.

All 8 functional test cases (startup, increment, decrement, save,
read, clear, reset recovery, power-cycle recovery) passed. Full
results and screenshots are in
[documentation/test_results.md](documentation/test_results.md) and
`simulide/screenshots/`.

## Remaining tasks

The demonstration video is currently being recorded — once done, add
its link to `media/demonstration_video_link.txt`. Three test cases
(Clear, Reset recovery, Power-cycle recovery) were verbally confirmed
but not screenshotted — grab those if every row needs a screenshot.
