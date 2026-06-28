/*
 * button.c
 * Buttons are wired active-low with internal pull-ups (PORTA bits set to 1).
 * Debounce strategy: on a falling edge, wait DEBOUNCE_DELAY_MS, re-check the
 * pin is still low, then wait for release before reporting another press
 * (simple "single-shot per press" debounce as requested by the assignment).
 */

#include "button.h"
#include "config.h"
#include <util/delay.h>

static const unsigned char button_pins[5] = {
    BTN_INC, BTN_DEC, BTN_SAVE, BTN_READ, BTN_CLEAR
};
static const Button_t button_ids[5] = {
    BUTTON_INC, BUTTON_DEC, BUTTON_SAVE, BUTTON_READ, BUTTON_CLEAR
};

/* Tracks whether each button was already down, so a held button only
 * fires once until it is released. */
static unsigned char button_was_down[5];

void Button_Init(void)
{
    BTN_DDR &= (unsigned char)~((1 << BTN_INC) | (1 << BTN_DEC) | (1 << BTN_SAVE) |
                                 (1 << BTN_READ) | (1 << BTN_CLEAR));
    BTN_PORT |= (1 << BTN_INC) | (1 << BTN_DEC) | (1 << BTN_SAVE) |
                (1 << BTN_READ) | (1 << BTN_CLEAR);

    for (unsigned char i = 0; i < 5; i++) {
        button_was_down[i] = 0;
    }
}

Button_t Button_Scan(void)
{
    for (unsigned char i = 0; i < 5; i++) {
        unsigned char pin = button_pins[i];
        unsigned char is_down = (BTN_PIN & (1 << pin)) == 0;

        if (is_down && !button_was_down[i]) {
            _delay_ms(DEBOUNCE_DELAY_MS);
            if ((BTN_PIN & (1 << pin)) == 0) {
                button_was_down[i] = 1;
                return button_ids[i];
            }
        } else if (!is_down) {
            button_was_down[i] = 0;
        }
    }
    return BUTTON_NONE;
}
