#include "command-processing/command-processor.h"
#define MAX_IGNORED_NONE_COMMANDS 20

CommandProcessor::CommandProcessor(CameraMount *cameraMount, CommandReader *commandReader)
    : cameraMount_(cameraMount), commandReader_(commandReader) {}

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
    }

    if (cmd == CarCommand::MoveForward) {
        cameraMount_->rotateDown(1);
    } else if (cmd == CarCommand::MoveBackward) {
        cameraMount_->rotateUp(1);
    } else if (cmd == CarCommand::TurnRight) {
        cameraMount_->rotateRight(1);
    } else if (cmd == CarCommand::TurnLeft) {
        cameraMount_->rotateLeft(1);
    }
}
