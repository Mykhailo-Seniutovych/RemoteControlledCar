#ifndef __JOYSTICK_JOYSTICK_STATE_H
#define __JOYSTICK_JOYSTICK_STATE_H

#include <stdint.h>

struct JoystickState {
    bool isBtnPressed;
    int16_t vertValue;
    int16_t horValue;
};

#endif