#include "led/led.h"
#include "led/color.h"

Led::Led(LedPwm *pwm) : pwm_(pwm) {}

void Led::turnColorOn(const Color &color) {
    *(pwm_->redCCR) = color.red;
    *(pwm_->greenCCR) = color.green;
    *(pwm_->blueCCR) = color.blue;
}

void Led::turnOff() {
    *(pwm_->redCCR) = 0;
    *(pwm_->greenCCR) = 0;
    *(pwm_->blueCCR) = 0;
}