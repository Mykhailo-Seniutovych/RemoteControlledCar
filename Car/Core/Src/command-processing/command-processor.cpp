#include "command-processing/command-processor.h"
#include "led/color.h"

#define MAX_IGNORED_NONE_COMMANDS 50

const Color WhitishColor = Color(150, 255, 255);
const Color RedColor = Color(255, 0, 0);
const Color GreenColor = Color(0, 255, 0);
const Color AmberColor = Color(255, 191, 0);

CommandProcessor::CommandProcessor(CarController *carController, CameraMount *cameraMount, Led *led)
    : carController_(carController), cameraMount_(cameraMount), led_(led) {}

void CommandProcessor::processCommand(CarCommand cmd) {
    if (cmd != CarCommand::None) {
        lastCommand_ = cmd;
        ignoredNoneCmdCount_ = 0;
    }

    if (cmd == CarCommand::Stop) {
        stopMovement();
    }

    if (cmd == CarCommand::None && isMoving_ && ignoredNoneCmdCount_ < MAX_IGNORED_NONE_COMMANDS) {
        ++ignoredNoneCmdCount_;
        cmd = lastCommand_;
    } else if (cmd == CarCommand::None) {
        stopMovement();
    }

    if (cmd == CarCommand::MoveForwardFast) {
        carController_->driveForwardFast();
        led_->turnColorOn(GreenColor);
        isMoving_ = true;
    } else if (cmd == CarCommand::MoveForwardSlow) {
        carController_->driveForwardSlow();
        led_->turnColorOn(GreenColor);
        isMoving_ = true;
    }

    else if (cmd == CarCommand::MoveBackwardFast) {
        carController_->driveBackwardFast();
        led_->turnColorOn(WhitishColor);
        isMoving_ = true;
    } else if (cmd == CarCommand::MoveBackwardSlow) {
        carController_->driveBackwardSlow();
        led_->turnColorOn(WhitishColor);
        isMoving_ = true;
    }

    else if (cmd == CarCommand::SpinRight) {
        carController_->spinRight();
        led_->turnColorOn(AmberColor);
        isMoving_ = true;
    } else if (cmd == CarCommand::SpinLeft) {
        carController_->spinLeft();
        led_->turnColorOn(AmberColor);
        isMoving_ = true;
    }

    else if (cmd == CarCommand::TurnRightForward) {
        carController_->turnRightForward();
        led_->turnColorOn(AmberColor);
        isMoving_ = true;
    } else if (cmd == CarCommand::TurnRightBackward) {
        carController_->turnRightBackward();
        led_->turnColorOn(AmberColor);
        isMoving_ = true;
    }

    else if (cmd == CarCommand::TurnLeftForward) {
        carController_->turnLeftForward();
        led_->turnColorOn(AmberColor);
        isMoving_ = true;
    } else if (cmd == CarCommand::TurnLeftBackward) {
        carController_->turnLeftBackward();
        led_->turnColorOn(AmberColor);
        isMoving_ = true;
    }

    else if (cmd == CarCommand::TiltDown) {
        cameraMount_->rotateDown(1);
        isMoving_ = true;
    } else if (cmd == CarCommand::TiltUp) {
        cameraMount_->rotateUp(1);
        isMoving_ = true;
    } else if (cmd == CarCommand::PanRight) {
        cameraMount_->rotateRight(1);
        isMoving_ = true;
    } else if (cmd == CarCommand::PanLeft) {
        cameraMount_->rotateLeft(1);
        isMoving_ = true;
    }

    HAL_Delay(10);
}

void CommandProcessor::stopMovement() {
    ignoredNoneCmdCount_ = 0;
    isMoving_ = false;
    led_->turnOff();
    carController_->stop();
}
