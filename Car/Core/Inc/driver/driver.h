#ifndef INC_DRIVER_DRIVER_H_
#define INC_DRIVER_DRIVER_H_
#include "driver/driver-pwm.h"
#include "driver/movement-direction.h"
#include "driver/speed.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"

// NPN, and PNP are abbreviations for different transistors types, used in the DC driver module.
class Driver {
  public:
    Driver(
        GPIO_TypeDef *pnp1Port,
        uint16_t pnp1Pin,
        GPIO_TypeDef *pnp2Port,
        uint16_t pnp2Pin,
        DriverPwm *pwm
    );

    void moveForward(Speed speed);
    void moveBackward(Speed speed);
    void stop();

  private:
    MovementDirection currentDirection_ = MovementDirection::None;
    Speed currentSpeed_ = Speed::None;

    GPIO_TypeDef *pnp1Port_;
    uint16_t pnp1Pin_;

    GPIO_TypeDef *pnp2Port_;
    uint16_t pnp2Pin_;

    DriverPwm *pwm_;

    bool isMovementChange(MovementDirection newDirection, Speed newSpeed);
};

#endif