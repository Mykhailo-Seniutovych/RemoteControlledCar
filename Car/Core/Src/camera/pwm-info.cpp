#include "camera/pwm-info.h"

PwmInfo::PwmInfo(
    volatile uint32_t *horCCR,
    volatile uint32_t *vertCCR,
    volatile uint32_t *ARR
) : horCCR(horCCR), vertCCR(vertCCR), ARR(ARR) {}