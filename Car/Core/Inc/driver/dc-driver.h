#ifndef INC_DRIVER_DC_DRIVER_H_
#define INC_DRIVER_DC_DRIVER_H_
#include "driver/dc-driver-pwm.h"
#include "driver/movement-direction.h"
#include "driver/speed.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"

// NPN, and PNP are abbreviations for different transistors types, used in the DC driver module.

class DcDriver {
  public:
    DcDriver(
        GPIO_TypeDef *pnp1Port,
        uint16_t pnp1Pin,
        GPIO_TypeDef *pnp2Port,
        uint16_t pnp2Pin,
        DcDriverPwm *pwm
    );

    void moveForward(float speedCoefficient);
    void moveBackward(float speedCoefficient);
    void stop();

  private:
    GPIO_TypeDef *pnp1Port_;
    uint16_t pnp1Pin_;

    GPIO_TypeDef *pnp2Port_;
    uint16_t pnp2Pin_;

    DcDriverPwm *pwm_;

};

#endif