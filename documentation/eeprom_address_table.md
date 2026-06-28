# EEPROM Address Table

| Address (hex) | Size    | Content                              | Written By                          | Read By |
|----------------|---------|----------------------------------------|--------------------------------------|----------|
| 0x00           | 1 byte  | Magic/init byte (`0xAA` once initialized) | `EEPROM_Storage_Init`, `EEPROM_SaveValue`, `EEPROM_ClearValue` | `EEPROM_Storage_Init` (first-boot detection) |
| 0x01           | 1 byte  | Saved configuration value (0-255)     | `EEPROM_SaveValue`, `EEPROM_ClearValue` | `EEPROM_LoadValue` (on startup and on Read button) |

## Why a magic byte?

Unprogrammed/erased EEPROM cells read back as `0xFF`. Without a sentinel,
the very first power-up would load `255` as if it were a valid saved
value. `EEPROM_Storage_Init()` checks address `0x00` for `0xAA`; if it is
not present, it writes the default value (`0`) to address `0x01` and
stamps the magic byte, guaranteeing a well-defined first-run state.

## Extension task reservation

If the optional "store multiple records" extension is implemented, the
record array can start at address `0x02` (e.g. 5 records x 1 byte =
addresses `0x02`-`0x06`), with a separate index byte for the
currently-selected record. This is not required for the base assignment
and is not implemented in the current firmware.
