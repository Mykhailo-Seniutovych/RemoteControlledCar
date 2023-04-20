
#include "driver/car-controller.h"
#include "driver/speed.h"
#include "stm32f1xx_hal.h"

// Small delay to avoid changing direction instantly
static uint16_t MOVEMENT_CHANGE_DELAY = 500;

CarController::CarController(
    DcDriver *leftDriver,
    DcDriver *rightDriver
) : leftDriver_(leftDriver), rightDriver_(rightDriver) {}

void CarController::driveForwardFast() {
    if (!isMovementChange(MovementDirection::Forward, Speed::Fast))
        return;

    HAL_Delay(MOVEMENT_CHANGE_DELAY);

    leftDriver_->moveForward(Speed::Fast);
    rightDriver_->moveForward(Speed::Fast);

    currentDirection_ = MovementDirection::Forward;
    currentSpeed_ = Speed::Fast;
}

void CarController::driveForwardSlow() {
    if (!isMovementChange(MovementDirection::Forward, Speed::Slow))
        return;

    HAL_Delay(MOVEMENT_CHANGE_DELAY);

    leftDriver_->moveForward(Speed::Slow);
    rightDriver_->moveForward(Speed::Slow);

    currentDirection_ = MovementDirection::Forward;
    currentSpeed_ = Speed::Slow;
}

void CarController::driveBackwardFast() {
    if (!isMovementChange(MovementDirection::Backward, Speed::Fast))
        return;

    HAL_Delay(MOVEMENT_CHANGE_DELAY);

    leftDriver_->moveBackward(Speed::Fast);
    rightDriver_->moveBackward(Speed::Fast);

    currentDirection_ = MovementDirection::Backward;
    currentSpeed_ = Speed::Fast;
}
void CarController::driveBackwardSlow() {
    if (!isMovementChange(MovementDirection::Backward, Speed::Slow))
        return;

    HAL_Delay(MOVEMENT_CHANGE_DELAY);

    leftDriver_->moveBackward(Speed::Slow);
    rightDriver_->moveBackward(Speed::Slow);

    currentDirection_ = MovementDirection::Backward;
    currentSpeed_ = Speed::Slow;
}

void CarController::turnRightFast() {
    if (!isMovementChange(MovementDirection::Right, Speed::Fast))
        return;

    HAL_Delay(MOVEMENT_CHANGE_DELAY);

    leftDriver_->moveForward(Speed::Fast);
    rightDriver_->moveBackward(Speed::Fast);

    currentDirection_ = MovementDirection::Right;
    currentSpeed_ = Speed::Fast;
}
void CarController::turnRightSlow() {
    if (!isMovementChange(MovementDirection::Right, Speed::Slow))
        return;

    HAL_Delay(MOVEMENT_CHANGE_DELAY);

    leftDriver_->moveForward(Speed::Slow);
    rightDriver_->moveBackward(Speed::Slow);

    currentDirection_ = MovementDirection::Right;
    currentSpeed_ = Speed::Slow;
}

void CarController::turnLeftFast() {
    if (!isMovementChange(MovementDirection::Left, Speed::Fast))
        return;

    HAL_Delay(MOVEMENT_CHANGE_DELAY);

    leftDriver_->moveBackward(Speed::Fast);
    rightDriver_->moveForward(Speed::Fast);

    currentDirection_ = MovementDirection::Left;
    currentSpeed_ = Speed::Fast;
}
void CarController::turnLeftSlow() {
    if (!isMovementChange(MovementDirection::Left, Speed::Slow))
        return;

    HAL_Delay(MOVEMENT_CHANGE_DELAY);

    leftDriver_->moveBackward(Speed::Slow);
    rightDriver_->moveForward(Speed::Slow);

    currentDirection_ = MovementDirection::Left;
    currentSpeed_ = Speed::Slow;
}

void CarController::stop() {
    if (!isMovementChange(MovementDirection::None, Speed::None))
        return;

    HAL_Delay(MOVEMENT_CHANGE_DELAY);

    leftDriver_->stop();
    rightDriver_->stop();

    currentDirection_ = MovementDirection::None;
    currentSpeed_ = Speed::None;
}

bool CarController::isMovementChange(MovementDirection newDirection, Speed newSpeed) {
    return currentDirection_ != newDirection || currentSpeed_ != newSpeed;
}