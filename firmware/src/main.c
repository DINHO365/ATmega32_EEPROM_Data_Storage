/*
 * main.c
 * Project 16: EEPROM-Based Data Logging and Configuration Storage System
 *
 * Flow:
 *   Startup  -> load saved value from EEPROM, show CURRENT/SAVED
 *   Standby  -> poll the 5 buttons
 *   INC/DEC  -> adjust current_value in RAM (clamped to VALUE_MIN/VALUE_MAX)
 *   SAVE     -> write current_value to EEPROM, confirm with LED + buzzer
 *   READ     -> re-read EEPROM and show it as the saved value
 *   CLEAR    -> reset EEPROM (and current_value) to VALUE_DEFAULT
 *   Reset/Power-cycle -> EEPROM retains the last saved value, reloaded on
 *                        the next startup
 */

#include "config.h"
#include "eeprom_storage.h"
#include "button.h"
#include "indicator.h"
#include "display.h"

int main(void)
{
    unsigned char current_value;
    unsigned char saved_value;

    Indicator_Init();
    Button_Init();
    Display_Init();
    EEPROM_Storage_Init();

    /* Startup: last saved value is loaded from EEPROM */
    saved_value = EEPROM_LoadValue();
    current_value = saved_value;

    Display_ShowMessage("EEPROM STORAGE", "SYSTEM READY");
    Display_ShowValues(current_value, saved_value);

    for (;;) {
        Button_t pressed = Button_Scan();

        switch (pressed) {
            case BUTTON_INC:
                if (current_value < VALUE_MAX) {
                    current_value++;
                    Display_ShowValues(current_value, saved_value);
                } else {
                    Indicator_Error(); /* already at maximum */
                }
                break;

            case BUTTON_DEC:
                if (current_value > VALUE_MIN) {
                    current_value--;
                    Display_ShowValues(current_value, saved_value);
                } else {
                    Indicator_Error(); /* already at minimum */
                }
                break;

            case BUTTON_SAVE:
                EEPROM_SaveValue(current_value);
                saved_value = current_value;
                Indicator_SaveSuccess();
                Display_ShowValues(current_value, saved_value);
                break;

            case BUTTON_READ:
                saved_value = EEPROM_LoadValue();
                Display_ShowValues(current_value, saved_value);
                break;

            case BUTTON_CLEAR:
                EEPROM_ClearValue();
                current_value = VALUE_DEFAULT;
                saved_value = VALUE_DEFAULT;
                Indicator_ClearConfirm();
                Display_ShowValues(current_value, saved_value);
                break;

            case BUTTON_NONE:
            default:
                break;
        }
    }
}
