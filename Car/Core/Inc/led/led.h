#ifndef INC_LED_LED_H_
#define INC_LED_LED_H_

#include "led/led-pwm.h"
#include "led/color.h"

class Led {
  public:
    Led(LedPwm *pwm);

    void turnColorOn(const Color &color);
    void turnOff();

  private:
    LedPwm *pwm_;
};

#endif