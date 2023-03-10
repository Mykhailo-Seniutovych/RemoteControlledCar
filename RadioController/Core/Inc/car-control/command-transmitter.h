#ifndef __CAR_CONTROL_COMMAND_TRANSMITTER_H
#define __CAR_CONTROL_COMMAND_TRANSMITTER_H

#include "car-control/car-command.h"
#include "joystick/joystick-state.h"
#include "nrf24/nrf24.h"

class CommandTransmitter {
  public:
    void initializeTransmission();
    void transmitCommand(JoystickState state);
};

#endif