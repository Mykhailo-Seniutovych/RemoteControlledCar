
#include "driver/car-controller.h"
#include "driver/speed.h"
#include "stm32f1xx_hal.h"


float FAST_SPEED_COEFFICIENT = 1.0f;
float SLOW_SPEED_COEFFICIENT = 0.4f;
float TURN_SPEED_COEFFICIENT = 0.7f;

CarController::CarController(
    DcDriver *leftDriver,
    DcDriver *rightDriver
) : leftDriver_(leftDriver), rightDriver_(rightDriver) {}

void CarController::driveForwardFast() {
    if (!isMovementChange(MovementDirection::Forward, FAST_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->moveForward(FAST_SPEED_COEFFICIENT);
    rightDriver_->moveForward(FAST_SPEED_COEFFICIENT);

    currentDirection_ = MovementDirection::Forward;
    currentSpeedCoefficient_ = FAST_SPEED_COEFFICIENT;
}
void CarController::driveForwardSlow() {
    if (!isMovementChange(MovementDirection::Forward, SLOW_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->moveForward(SLOW_SPEED_COEFFICIENT);
    rightDriver_->moveForward(SLOW_SPEED_COEFFICIENT);

    currentDirection_ = MovementDirection::Forward;
    currentSpeedCoefficient_ = SLOW_SPEED_COEFFICIENT;
}

void CarController::driveBackwardFast() {
    if (!isMovementChange(MovementDirection::Backward, FAST_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->moveBackward(FAST_SPEED_COEFFICIENT);
    rightDriver_->moveBackward(FAST_SPEED_COEFFICIENT);

    currentDirection_ = MovementDirection::Backward;
    currentSpeedCoefficient_ = FAST_SPEED_COEFFICIENT;
}
void CarController::driveBackwardSlow() {
    if (!isMovementChange(MovementDirection::Backward, SLOW_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->moveBackward(SLOW_SPEED_COEFFICIENT);
    rightDriver_->moveBackward(SLOW_SPEED_COEFFICIENT);

    currentDirection_ = MovementDirection::Backward;
    currentSpeedCoefficient_ = SLOW_SPEED_COEFFICIENT;
}

void CarController::turnRightFast() {
    if (!isMovementChange(MovementDirection::Right, FAST_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->moveForward(FAST_SPEED_COEFFICIENT);
    rightDriver_->moveBackward(FAST_SPEED_COEFFICIENT);

    currentDirection_ = MovementDirection::Right;
    currentSpeedCoefficient_ = FAST_SPEED_COEFFICIENT;
}
void CarController::turnRightSlow() {
    if (!isMovementChange(MovementDirection::Right, TURN_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->moveForward(TURN_SPEED_COEFFICIENT);
    rightDriver_->stop();

    currentDirection_ = MovementDirection::Right;
    currentSpeedCoefficient_ = TURN_SPEED_COEFFICIENT;
}

void CarController::turnLeftFast() {
    if (!isMovementChange(MovementDirection::Left, FAST_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->moveBackward(FAST_SPEED_COEFFICIENT);
    rightDriver_->moveForward(FAST_SPEED_COEFFICIENT);

    currentDirection_ = MovementDirection::Left;
    currentSpeedCoefficient_ = FAST_SPEED_COEFFICIENT;
}
void CarController::turnLeftSlow() {
    if (!isMovementChange(MovementDirection::Left, TURN_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->stop();
    rightDriver_->moveForward(TURN_SPEED_COEFFICIENT);

    currentDirection_ = MovementDirection::Left;
    currentSpeedCoefficient_ = TURN_SPEED_COEFFICIENT;
}

void CarController::stop() {
    if (!isMovementChange(MovementDirection::None, 0))
        return;

    leftDriver_->stop();
    rightDriver_->stop();

    currentDirection_ = MovementDirection::None;
    currentSpeedCoefficient_ = 0;
}

bool CarController::isMovementChange(MovementDirection newDirection, float newSpeedCoefficient) {
    return currentDirection_ != newDirection || currentSpeedCoefficient_ != newSpeedCoefficient;
}