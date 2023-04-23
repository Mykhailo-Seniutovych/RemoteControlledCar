#include "car-control/command-transmitter.h"
#include "nrf24/nrf24.h"

void CommandTransmitter::initializeTransmission() {
    nrf24_Init();
    nrf24_EnterTxMode();
}

void CommandTransmitter::transmitCommand(JoystickState rightState, JoystickState leftState) {
    CarCommand cmd = CarCommand::None;

    if (isBtnPressed(rightState) && isForward(rightState)) {
        cmd = CarCommand::MoveForwardFast;
    } else if (isForward(rightState)) {
        cmd = CarCommand::MoveForwardSlow;
    }

    else if (isBtnPressed(rightState) && isBackward(rightState)) {
        cmd = CarCommand::MoveBackwardFast;
    } else if (isBackward(rightState)) {
        cmd = CarCommand::MoveBackwardSlow;
    }

    else if (isBtnPressed(rightState) && isRight(rightState)) {
        cmd = CarCommand::TurnRightFast;
    } else if (isRight(rightState)) {
        cmd = CarCommand::TurnRightSlow;
    }

    else if (isBtnPressed(rightState) && isLeft(rightState)) {
        cmd = CarCommand::TurnLeftFast;
    } else if (isLeft(rightState)) {
        cmd = CarCommand::TurnLeftSlow;
    }

    else if (isForward(leftState)) {
        cmd = CarCommand::TiltDown;
    } else if (isBackward(leftState)) {
        cmd = CarCommand::TiltUp;
    } else if (isRight(leftState)) {
        cmd = CarCommand::PanRight;
    } else if (isLeft(leftState)) {
        cmd = CarCommand::PanLeft;
    }

    if (cmd == CarCommand::None &&
        lastCmd_ != CarCommand::None &&
        (lastCmd_ != CarCommand::Stop || !lastCmdSuccess_)) {
        cmd = CarCommand::Stop;
    }

    if (cmd != CarCommand::None) {
        uint8_t cmdNumber = static_cast<uint8_t>(cmd);
        lastCmdSuccess_ = nrf24_TransmitData(&cmdNumber, 1);
    }

    lastCmd_ = cmd;
}