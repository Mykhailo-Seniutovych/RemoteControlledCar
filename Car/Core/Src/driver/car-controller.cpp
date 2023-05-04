
#include "driver/car-controller.h"
#include "driver/speed.h"
#include "stm32f1xx_hal.h"

float FAST_SPEED_COEFFICIENT = 1.0f;
float SLOW_SPEED_COEFFICIENT = 0.4f;
float TURN_SPEED_COEFFICIENT = 0.8f;

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

void CarController::spinRight() {
    if (!isMovementChange(MovementDirection::SpinRight, FAST_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->moveForward(FAST_SPEED_COEFFICIENT);
    rightDriver_->moveBackward(FAST_SPEED_COEFFICIENT);

    currentDirection_ = MovementDirection::SpinRight;
    currentSpeedCoefficient_ = FAST_SPEED_COEFFICIENT;
}
void CarController::spinLeft() {
    if (!isMovementChange(MovementDirection::SpinLeft, FAST_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->moveBackward(FAST_SPEED_COEFFICIENT);
    rightDriver_->moveForward(FAST_SPEED_COEFFICIENT);

    currentDirection_ = MovementDirection::SpinLeft;
    currentSpeedCoefficient_ = FAST_SPEED_COEFFICIENT;
}

void CarController::turnRightForward() {
    if (!isMovementChange(MovementDirection::RightForward, TURN_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->moveForward(TURN_SPEED_COEFFICIENT);
    rightDriver_->stop();

    currentDirection_ = MovementDirection::RightForward;
    currentSpeedCoefficient_ = TURN_SPEED_COEFFICIENT;
}
void CarController::turnRightBackward() {
    if (!isMovementChange(MovementDirection::RightBackward, TURN_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->moveBackward(TURN_SPEED_COEFFICIENT);
    rightDriver_->stop();

    currentDirection_ = MovementDirection::RightBackward;
    currentSpeedCoefficient_ = TURN_SPEED_COEFFICIENT;
}

void CarController::turnLeftForward() {
    if (!isMovementChange(MovementDirection::LeftForward, TURN_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->stop();
    rightDriver_->moveForward(TURN_SPEED_COEFFICIENT);

    currentDirection_ = MovementDirection::LeftForward;
    currentSpeedCoefficient_ = TURN_SPEED_COEFFICIENT;
}
void CarController::turnLeftBackward() {
    if (!isMovementChange(MovementDirection::LeftBackward, TURN_SPEED_COEFFICIENT))
        return;

    delayIfMoving();

    leftDriver_->stop();
    rightDriver_->moveBackward(TURN_SPEED_COEFFICIENT);

    currentDirection_ = MovementDirection::LeftForward;
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