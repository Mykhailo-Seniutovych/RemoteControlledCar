#include <led/led-pwm.h>

LedPwm::LedPwm(
    volatile uint32_t *redCCR,
    volatile uint32_t *greenCCR,
    volatile uint32_t *blueCCR
) : redCCR(redCCR), greenCCR(greenCCR), blueCCR(blueCCR) {}
