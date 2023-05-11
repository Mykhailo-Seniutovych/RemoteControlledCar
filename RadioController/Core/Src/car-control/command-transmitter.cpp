#include "car-control/command-transmitter.h"
#include "nrf24/nrf24.h"
#include <cstdlib>

#define MOVEMENT_MIN_BOUNDARY 200
#define MOVE_STRAIGHT_MIN_VERT_HOR_DIFF 1200

#define TURN_FORWARD_MIN_BOUNDARY -50

void send_command(int command, void *message);

void CommandTransmitter::initializeTransmission() {
    nrf24_Init();
    nrf24_EnterTxMode();
}

void CommandTransmitter::transmitCommand(const JoystickState *leftState, const JoystickState *rightState) {
    auto cmd = convertIntoCommand(leftState, rightState);

    if (cmd == CarCommand::None &&
        lastCmd_ != CarCommand::None &&
        (lastCmd_ != CarCommand::Stop || !lastTransmissionSuccess_)) {
        cmd = CarCommand::Stop;
    }

    if (cmd != CarCommand::None) {
        uint8_t cmdNumber = static_cast<uint8_t>(cmd);
        lastTransmissionSuccess_ = nrf24_TransmitData(&cmdNumber, 1);
    }

    lastCmd_ = cmd;
}

CarCommand CommandTransmitter::convertIntoCommand(const JoystickState *leftState, const JoystickState *rightState) const {
    auto absLeftVert = abs(leftState->vertValue);
    auto absLeftHor = abs(leftState->horValue);
    auto isLeftMovement = absLeftHor > MOVEMENT_MIN_BOUNDARY ||
                          absLeftVert > MOVEMENT_MIN_BOUNDARY;
    if (isLeftMovement) {
        if (absLeftVert > absLeftHor) {
            return leftState->vertValue < 0 ? CarCommand::TiltUp : CarCommand::TiltDown;
        } else {
            return leftState->horValue < 0 ? CarCommand::PanLeft : CarCommand::PanRight;
        }
    }

    auto absRightVert = abs(rightState->vertValue);
    auto absRightHor = abs(rightState->horValue);
    auto isRightMovement = absRightHor > MOVEMENT_MIN_BOUNDARY ||
                           absRightVert > MOVEMENT_MIN_BOUNDARY;

    if (isRightMovement) {
        auto isBtnPressed = rightState->isBtnPressed;
        if (isBtnPressed) {
            if (absRightVert > absRightHor) {
                return rightState->vertValue > 0 ? CarCommand::MoveForwardFast : CarCommand::MoveBackwardFast;
            } else {
                return rightState->horValue > 0 ? CarCommand::SpinRight : CarCommand::SpinLeft;
            }
        }

        if (!isBtnPressed) {
            auto isOnlyVertMovement =
                absRightHor < MOVEMENT_MIN_BOUNDARY ||
                (absRightVert > MOVEMENT_MIN_BOUNDARY && absRightVert - absRightHor > MOVE_STRAIGHT_MIN_VERT_HOR_DIFF);

            if (isOnlyVertMovement) {
                return rightState->vertValue > 0 ? CarCommand::MoveForwardSlow : CarCommand::MoveBackwardSlow;
            }

            auto isSomeHorizontalMovement = !isOnlyVertMovement;
            if (isSomeHorizontalMovement) {
                if (rightState->horValue > 0) {
                    return rightState->vertValue > TURN_FORWARD_MIN_BOUNDARY ? CarCommand::TurnRightForward : CarCommand::TurnRightBackward;
                } else {
                    return rightState->vertValue > TURN_FORWARD_MIN_BOUNDARY ? CarCommand::TurnLeftForward : CarCommand::TurnLeftBackward;
                }
            }
        }
    }

    return CarCommand::None;
}
