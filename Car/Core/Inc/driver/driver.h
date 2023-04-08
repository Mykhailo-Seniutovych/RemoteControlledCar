#ifndef INC_DRIVER_DRIVER_H_
#define INC_DRIVER_DRIVER_H_
#include "driver/driver-pwm.h"
#include "driver/speed.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"

// NPN, and PNP are abbreviations for different transistors types, used in the DC driver module.
class Driver {
  public:
    Driver(
        GPIO_TypeDef *pnp1Port,
        uint8_t pnp1Pin,
        GPIO_TypeDef *pnp2Port,
        uint8_t pnp2Pin,
        DriverPwm *pwm
    );

    void moveForward(Speed speed);
    void moveBackward(Speed speed);
    void stop();

  private:
    GPIO_TypeDef *pnp1Port_;
    uint8_t pnp1Pin_;

    GPIO_TypeDef *pnp2Port_;
    uint8_t pnp2Pin_;

    DriverPwm *pwm_;
};

#endif