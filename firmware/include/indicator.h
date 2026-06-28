/*
 * indicator.h
 * Save/status LED, error LED, and buzzer control.
 */

#ifndef INDICATOR_H
#define INDICATOR_H

void Indicator_Init(void);
void Indicator_SaveSuccess(void);   /* SAVE_LED pulse + short beep */
void Indicator_Error(void);         /* ERROR_LED pulse + double beep */
void Indicator_ClearConfirm(void);  /* both LEDs pulse + long beep */
void Indicator_AllOff(void);

#endif /* INDICATOR_H */
