#ifndef INC_DRIVER_DC_DRIVER_PWM_H_
#define INC_DRIVER_DC_DRIVER_PWM_H_
#include "stdint.h"

// NPN, and PNP are abbreviations for different transistors types, used in the DC driver module.
struct DcDriverPwm {
    volatile uint32_t *npn1CCR;
    volatile uint32_t *npn2CCR;
    volatile uint32_t ARR;

    DcDriverPwm(
        volatile uint32_t *npn1CCR,
        volatile uint32_t *npn2CCR,
        volatile uint32_t ARR
    );
};

#endif