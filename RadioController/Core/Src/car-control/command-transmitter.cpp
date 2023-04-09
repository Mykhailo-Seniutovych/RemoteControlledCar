#include "car-control/command-transmitter.h"
#include "nrf24/nrf24.h"

void CommandTransmitter::initializeTransmission() {
    nrf24_Init();
    nrf24_EnterTxMode();
}

void CommandTransmitter::transmitCommand(JoystickState rightState, JoystickState leftState) {
    uint8_t cmd = 0;
    if (isBtnPressed(rightState) && isForward(rightState)) {
        cmd = static_cast<uint8_t>(CarCommand::MoveForwardFast);
    } else if (isForward(rightState)) {
        cmd = static_cast<uint8_t>(CarCommand::MoveForwardSlow);
    }

    else if (isBtnPressed(rightState) && isBackward(rightState)) {
        cmd = static_cast<uint8_t>(CarCommand::MoveBackwardFast);
    } else if (isBackward(rightState)) {
        cmd = static_cast<uint8_t>(CarCommand::MoveBackwardSlow);
    }

    else if (isBtnPressed(rightState) && isRight(rightState)) {
        cmd = static_cast<uint8_t>(CarCommand::TurnRightFast);
    } else if (isRight(rightState)) {
        cmd = static_cast<uint8_t>(CarCommand::TurnRightSlow);
    }

    else if (isBtnPressed(rightState) && isLeft(rightState)) {
        cmd = static_cast<uint8_t>(CarCommand::TurnLeftFast);
    } else if (isLeft(rightState)) {
        cmd = static_cast<uint8_t>(CarCommand::TurnLeftSlow);
    }

    else if (isForward(leftState)) {
        cmd = static_cast<uint8_t>(CarCommand::TiltDown);
    } else if (isBackward(leftState)) {
        cmd = static_cast<uint8_t>(CarCommand::TiltUp);
    } else if (isRight(leftState)) {
        cmd = static_cast<uint8_t>(CarCommand::PanRight);
    } else if (isLeft(leftState)) {
        cmd = static_cast<uint8_t>(CarCommand::PanLeft);
    }

    // bool test = false;
    // if (cmd == 0) {
    //     test = true;
    // }
    if (cmd > 0) {
        nrf24_TransmitData(&cmd, 1);
    }
}