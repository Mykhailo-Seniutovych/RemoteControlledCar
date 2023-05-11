#include "driver/dc-driver-pwm.h"

DcDriverPwm::DcDriverPwm(
    volatile uint32_t *npn1CCR,
    volatile uint32_t *npn2CCR,
    volatile uint32_t ARR
) : npn1CCR(npn1CCR), npn2CCR(npn2CCR), ARR(ARR) {}