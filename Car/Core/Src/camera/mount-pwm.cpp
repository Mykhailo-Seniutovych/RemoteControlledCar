#include "camera/mount-pwm.h"
#include <stdint.h>

MountPwm::MountPwm(
    volatile uint32_t *horCCR,
    volatile uint32_t *vertCCR,
    volatile uint32_t *ARR
) : horCCR(horCCR), vertCCR(vertCCR), ARR(ARR) {}