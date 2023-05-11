#ifndef __CAR_CONTROL_STATE_TRANSMITTER_H
#define __CAR_CONTROL_STATE_TRANSMITTER_H

#include "joystick/joystick-state.h"
#include "car-control/car-command.h"
#include "nrf24/nrf24.h"

class CommandTransmitter {
  public:
    void initializeTransmission();
    void transmitCommand(const JoystickState *leftState, const JoystickState *rightState);
    CarCommand convertIntoCommand(const JoystickState *leftState, const JoystickState *rightState) const;

  private:
    bool lastTransmissionSuccess_ = false;
    CarCommand lastCmd_ = CarCommand::None;
};

#endif