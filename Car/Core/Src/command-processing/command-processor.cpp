#include "command-processing/command-processor.h"
#include "led/color.h"
#define MAX_IGNORED_NONE_COMMANDS 15

const Color WhiteColor = Color(255, 255, 255);
const Color RedColor = Color(255, 0, 0);
const Color GreenColor = Color(0, 255, 0);
const Color BlueColor = Color(0, 0, 255);
const Color YellowColor = Color(255, 255, 0);
const Color TurquoiseColor = Color(48, 213, 199);

CommandProcessor::CommandProcessor(CameraMount *cameraMount, CommandReader *commandReader, Led *led)
    : cameraMount_(cameraMount), commandReader_(commandReader), led_(led) {}

void CommandProcessor::processNextCommand() {
    CarCommand cmd = commandReader_->getNextCommand();

    if (cmd != CarCommand::None) {
        lastCommand_ = cmd;
        isMountMoving_ = true;
        ignoredNoneCmdCount_ = 0;
    }

    if (isMountMoving_ && cmd == CarCommand::None && ignoredNoneCmdCount_ < MAX_IGNORED_NONE_COMMANDS) {
        ++ignoredNoneCmdCount_;
        cmd = lastCommand_;
    } else if (cmd == CarCommand::None) {
        ignoredNoneCmdCount_ = 0;
        lastCommand_ = CarCommand::None;
        isMountMoving_ = false;
        led_->turnOff();
    }

    if (cmd == CarCommand::MoveForward) {
        cameraMount_->rotateDown(1);
        led_->turnColorOn(TurquoiseColor);
    } else if (cmd == CarCommand::MoveBackward) {
        cameraMount_->rotateUp(1);
        led_->turnColorOn(TurquoiseColor);
    } else if (cmd == CarCommand::TurnRight) {
        cameraMount_->rotateRight(1);
        led_->turnColorOn(YellowColor);

    } else if (cmd == CarCommand::TurnLeft) {
        cameraMount_->rotateLeft(1);
        led_->turnColorOn(YellowColor);
    }
}
