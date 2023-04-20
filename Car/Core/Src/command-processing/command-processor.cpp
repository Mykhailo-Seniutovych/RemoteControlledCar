#include "command-processing/command-processor.h"
#include "led/color.h"
#define MAX_IGNORED_NONE_COMMANDS 30

const Color WhiteColor = Color(255, 255, 255);
const Color RedColor = Color(255, 0, 0);
const Color GreenColor = Color(0, 255, 0);
const Color BlueColor = Color(0, 0, 255);
const Color YellowColor = Color(255, 255, 0);
const Color TurquoiseColor = Color(48, 213, 199);

CommandProcessor::CommandProcessor(CarController *carController, CameraMount *cameraMount, CommandReader *commandReader, Led *led)
    : carController_(carController), cameraMount_(cameraMount), commandReader_(commandReader), led_(led) {}

void CommandProcessor::processNextCommand() {
    CarCommand cmd = commandReader_->getNextCommand();

    if (cmd != CarCommand::None) {
        lastCommand_ = cmd;
        isMoving_ = true;
        ignoredNoneCmdCount_ = 0;
    }

    if (isMoving_ && cmd == CarCommand::None && ignoredNoneCmdCount_ < MAX_IGNORED_NONE_COMMANDS) {
        ++ignoredNoneCmdCount_;
        cmd = lastCommand_;
    } else if (cmd == CarCommand::None) {
        ignoredNoneCmdCount_ = 0;
        lastCommand_ = CarCommand::None;
        isMoving_ = false;
        led_->turnOff();
        carController_->stop();
    }

    if (cmd == CarCommand::MoveForwardFast) {
        carController_->driveForwardFast();
        led_->turnColorOn(GreenColor);
    } else if (cmd == CarCommand::MoveForwardSlow) {
        carController_->driveForwardSlow();
        led_->turnColorOn(GreenColor);
    }

    else if (cmd == CarCommand::MoveBackwardFast) {
        carController_->driveBackwardFast();
        led_->turnColorOn(WhiteColor);
    } else if (cmd == CarCommand::MoveBackwardSlow) {
        carController_->driveBackwardSlow();
        led_->turnColorOn(WhiteColor);
    }

    else if (cmd == CarCommand::TurnRightFast) {
        carController_->turnRightFast();
        led_->turnColorOn(YellowColor);
    } else if (cmd == CarCommand::TurnRightSlow) {
        carController_->turnRightSlow();
        led_->turnColorOn(YellowColor);
    }

    else if (cmd == CarCommand::TurnLeftFast) {
        carController_->turnLeftFast();
        led_->turnColorOn(YellowColor);
    } else if (cmd == CarCommand::TurnLeftSlow) {
        carController_->turnLeftSlow();
        led_->turnColorOn(YellowColor);
    }

    else if (cmd == CarCommand::TiltDown) {
        cameraMount_->rotateDown(1);
    } else if (cmd == CarCommand::TiltUp) {
        cameraMount_->rotateUp(1);
    } else if (cmd == CarCommand::PanRight) {
        cameraMount_->rotateRight(1);
    } else if (cmd == CarCommand::PanLeft) {
        cameraMount_->rotateLeft(1);
    }
}
