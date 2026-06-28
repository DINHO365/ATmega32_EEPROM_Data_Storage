# EEPROM Test Results

Tested in SimulIDE using `simulide/circuit/EEPROM_Storage.sim1` with
`firmware/hex/firmware.hex` loaded into the `mega32-1` MCU component.

| Test Case               | Action                          | Expected Result                          | Observed Result | Screenshot |
|---------------------------|----------------------------------|---------------------------------------------|--------------------|-------------|
| Startup (first power-up) | Power ON with blank EEPROM       | CURRENT: 000, SAVED: 000                    | PASS — LCD showed `CURRENT: 000` / `SAVED: 000` | [01_startup.png](../simulide/screenshots/01_startup.png) |
| Increment                | Press INC button x N             | CURRENT increases by 1 per press, stops at 255 | PASS — CURRENT increased correctly with each press; clamps at 255 without wraparound | [02_increment.png](../simulide/screenshots/02_increment.png) |
| Decrement                | Press DEC button x N             | CURRENT decreases by 1 per press, stops at 0   | PASS — CURRENT decreased correctly with each press; clamps at 0 without wraparound | [03_decrement.png](../simulide/screenshots/03_decrement.png) |
| Save                     | Press SAVE button                | SAVE_LED + buzzer pulse, SAVED == CURRENT   | PASS — SAVED updated to match CURRENT | [04_save.png](../simulide/screenshots/04_save.png) |
| Read                     | Press READ button                | SAVED value re-read from EEPROM and displayed | PASS — SAVED correctly re-read from EEPROM independent of any unsaved CURRENT changes | [05_read.png](../simulide/screenshots/05_read.png) |
| Clear                    | Press CLEAR button               | Both LEDs + buzzer pulse, CURRENT and SAVED reset to 000 | PASS — both values reset to 000 | — |
| Reset recovery           | Press RST button                 | SAVED value from EEPROM reloaded as CURRENT | PASS — CURRENT correctly reloaded from the last saved EEPROM value, discarding any unsaved changes | — |
| Power-cycle recovery     | Stop simulation, then Play again | Last saved value recovered                  | PASS — last saved value recovered after the power cycle | — |
