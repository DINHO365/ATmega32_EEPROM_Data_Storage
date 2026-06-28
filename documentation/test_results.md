# EEPROM Test Results

Tested in SimulIDE using `simulide/circuit/EEPROM_Storage.sim1` with
`firmware/hex/firmware.hex` loaded into the `mega32-1` MCU component.
All test cases below passed.

| Test Case               | Action                          | Expected Result                          | Observed Result | Screenshot |
|---------------------------|----------------------------------|---------------------------------------------|--------------------|-------------|
| Circuit power-up (debug) | Power source initially off       | n/a — caught the 5V source defaulting to 0V/not running | Identified and corrected before testing | [00_circuit_unpowered.png](../simulide/screenshots/00_circuit_unpowered.png) |
| LCD orientation (debug)  | First run after powering on      | LCD readable                                | Found inverted (mirrored text + swapped rows) — fixed via Mirror H/V in SimulIDE | [01_lcd_inverted_before_fix.png](../simulide/screenshots/01_lcd_inverted_before_fix.png) |
| LCD readable after flip (debug) | Re-ran after flip          | Text orientation correct                    | Correct orientation confirmed; wire routing left cosmetically crossing the screen (no electrical effect) | [02_lcd_fixed_wire_routing_cosmetic.png](../simulide/screenshots/02_lcd_fixed_wire_routing_cosmetic.png) |
| Startup (first power-up) | Power ON with blank EEPROM       | CURRENT: 000, SAVED: 000                    | PASS — LCD showed `CURRENT: 000` / `SAVED: 000` cleanly after the display-padding fix | [03_startup_clean_boot.png](../simulide/screenshots/03_startup_clean_boot.png) |
| Increment                | Press INC button x N             | CURRENT increases by 1 per press, stops at 255 | PASS (increment) — CURRENT rose 000→070 over repeated clicks. Clamp at 255 verified by code review (`main.c`: increments only `if (current_value < VALUE_MAX)`, else triggers error indicator) rather than exhausted via 255 manual clicks | [04_increment_test.png](../simulide/screenshots/04_increment_test.png) |
| Decrement                | Press DEC button x N             | CURRENT decreases by 1 per press, stops at 0   | PASS — CURRENT correctly decreased to 000 and stayed at 000 on further presses (no wraparound to 255). ERROR_LED/buzzer flash at the floor was not visually confirmed (very brief, ~160 ms) | [05_decrement_test.png](../simulide/screenshots/05_decrement_test.png) |
| Save                     | Press SAVE button                | SAVE_LED + buzzer pulse, SAVED == CURRENT   | PASS — CURRENT: 011, SAVED: 011 after pressing SAVE | [06_save_test.png](../simulide/screenshots/06_save_test.png) |
| Read                     | Press READ button                | SAVED value re-read from EEPROM and displayed | PASS — with CURRENT incremented further to 015 (unsaved), pressing READ left SAVED at 011, confirming it re-reads EEPROM rather than mirroring RAM | [07_read_test.png](../simulide/screenshots/07_read_test.png) |
| Clear                    | Press CLEAR button               | Both LEDs + buzzer pulse, CURRENT and SAVED reset to 000 | PASS — both CURRENT and SAVED reset to 000 | not captured |
| Reset recovery           | Press RST button                 | SAVED value from EEPROM reloaded as CURRENT | PASS — saved 005, then incremented unsaved to 010, pressed RST: both CURRENT and SAVED came back as 005 (unsaved 010 discarded) | not captured |
| Power-cycle recovery     | Stop simulation, then Play again | Last saved value recovered                  | PASS — after Stop/Play, both CURRENT and SAVED reloaded as 005 (EEPROM persisted via the MCU component's `saveEepr="true"` setting) | not captured |

## Bug found and fixed during testing

First boot displayed `CURRENT: 000GE` instead of `CURRENT: 000`. Root
cause: the startup message `"EEPROM STORAGE"` (14 characters) was
written to row 0, then immediately overwritten by `"CURRENT: 000"`
(only 12 characters) without clearing the rest of the line — columns
13-14 still held leftover characters `G` and `E` from `"STORAGE"`.
Fixed in `firmware/src/display.c` by padding `Display_ShowValues`'s
LCD output to the full 16 columns on both rows. Rebuilt and reflashed
into SimulIDE; confirmed clean on the next boot.

## Outstanding

Clear, Reset recovery, and Power-cycle recovery were confirmed verbally
during testing but not screenshotted. Capture those three before final
submission if a screenshot is required for every row.
