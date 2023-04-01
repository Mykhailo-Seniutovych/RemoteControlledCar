#ifndef INC_LED_LED_PWM_H_
#define INC_LED_LED_PWM_H_

#include <stdint.h>

struct LedPwm {
    volatile uint32_t *redCCR;
    volatile uint32_t *greenCCR;
    volatile uint32_t *blueCCR;

    LedPwm(
        volatile uint32_t *redCCR,
        volatile uint32_t *greenCCR,
        volatile uint32_t *blueCCR
    );
};

#endif