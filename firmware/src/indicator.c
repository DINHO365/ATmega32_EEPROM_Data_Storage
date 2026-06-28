#include "indicator.h"
#include "config.h"
#include <util/delay.h>

void Indicator_Init(void)
{
    IND_DDR |= (1 << SAVE_LED) | (1 << ERROR_LED) | (1 << BUZZER);
    IND_PORT &= (unsigned char)~((1 << SAVE_LED) | (1 << ERROR_LED) | (1 << BUZZER));
}

void Indicator_SaveSuccess(void)
{
    IND_PORT |= (1 << SAVE_LED);
    IND_PORT |= (1 << BUZZER);
    _delay_ms(BEEP_DELAY_MS);
    IND_PORT &= (unsigned char)~(1 << BUZZER);
    _delay_ms(LED_PULSE_MS - BEEP_DELAY_MS);
    IND_PORT &= (unsigned char)~(1 << SAVE_LED);
}

void Indicator_Error(void)
{
    for (unsigned char i = 0; i < 2; i++) {
        IND_PORT |= (1 << ERROR_LED) | (1 << BUZZER);
        _delay_ms(BEEP_DELAY_MS);
        IND_PORT &= (unsigned char)~((1 << ERROR_LED) | (1 << BUZZER));
        _delay_ms(BEEP_DELAY_MS);
    }
}

void Indicator_ClearConfirm(void)
{
    IND_PORT |= (1 << SAVE_LED) | (1 << ERROR_LED) | (1 << BUZZER);
    _delay_ms(LED_PULSE_MS);
    IND_PORT &= (unsigned char)~((1 << SAVE_LED) | (1 << ERROR_LED) | (1 << BUZZER));
}

void Indicator_AllOff(void)
{
    IND_PORT &= (unsigned char)~((1 << SAVE_LED) | (1 << ERROR_LED) | (1 << BUZZER));
}
