/*
 * button.h
 * Debounced, active-low button reading for the five control buttons.
 */

#ifndef BUTTON_H
#define BUTTON_H

typedef enum {
    BUTTON_NONE = 0,
    BUTTON_INC,
    BUTTON_DEC,
    BUTTON_SAVE,
    BUTTON_READ,
    BUTTON_CLEAR
} Button_t;

void Button_Init(void);

/* Returns the button that was just pressed (debounced, single-shot per
 * press), or BUTTON_NONE if nothing new was pressed. Non-blocking. */
Button_t Button_Scan(void);

#endif /* BUTTON_H */
