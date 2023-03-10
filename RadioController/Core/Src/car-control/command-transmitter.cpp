#include "car-control/command-transmitter.h"
#include "nrf24/nrf24.h"

void CommandTransmitter::initializeTransmission() {
    nrf24_Init();
    nrf24_EnterTxMode();
}

void CommandTransmitter::transmitCommand(JoystickState state) {
    bool isSuccess = false;
    if (state == JoystickState::Forward) {
        uint8_t cmd = CarCommand::MoveForward;
        isSuccess = nrf24_TransmitData(&cmd, 1);
    } else if (state == JoystickState::Backward) {
        uint8_t cmd = CarCommand::MoveBackward;
        isSuccess = nrf24_TransmitData(&cmd, 1);
    } else if (state == JoystickState::Right) {
        uint8_t cmd = CarCommand::TurnRight;
        isSuccess = nrf24_TransmitData(&cmd, 1);
    } else if (state == JoystickState::Left) {
        uint8_t cmd = CarCommand::TurnLeft;
        isSuccess = nrf24_TransmitData(&cmd, 1);
    }
}