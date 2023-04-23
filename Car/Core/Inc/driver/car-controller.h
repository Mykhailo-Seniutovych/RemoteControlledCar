#ifndef INC_DRIVER_CAR_CONTROLLER_H_
#define INC_DRIVER_CAR_CONTROLLER_H_
#include "driver/dc-driver.h"
#include "driver/movement-direction.h"
#include "driver/speed.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"

// Small delay to avoid changing direction instantly
static uint16_t MOVEMENT_CHANGE_DELAY = 500;

// NPN, and PNP are abbreviations for different transistors types, used in the DC driver module.
class CarController {
  public:
    CarController(
        DcDriver *leftDriver,
        DcDriver *rightDriver
    );

    void driveForwardFast();
    void driveForwardSlow();

    void driveBackwardFast();
    void driveBackwardSlow();

    void turnRightFast();
    void turnRightSlow();

    void turnLeftFast();
    void turnLeftSlow();

    void stop();

  private:
    DcDriver *leftDriver_;
    DcDriver *rightDriver_;

    Speed currentSpeed_ = Speed::None;
    MovementDirection currentDirection_ = MovementDirection::None;

    bool isMovementChange(MovementDirection newDirection, Speed newSpeed);

    inline void delayIfMoving() {
        if (currentDirection_ != MovementDirection::None) {
            HAL_Delay(MOVEMENT_CHANGE_DELAY);
        }
    }
};

#endif