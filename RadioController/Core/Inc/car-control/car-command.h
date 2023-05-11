#ifndef __CAR_CONTROL_CAR_COMMAND_H
#define __CAR_CONTROL_CAR_COMMAND_H

enum class CarCommand {
    None = 0,
    Stop = 10,

    MoveForwardFast = 20,
    MoveForwardSlow = 30,

    MoveBackwardFast = 40,
    MoveBackwardSlow = 50,

    SpinRight = 60,
    SpinLeft = 70,

    TurnRightForward = 80,
    TurnRightBackward = 90,

    TurnLeftForward = 100,
    TurnLeftBackward = 119, // for some reason NRF has troubles to send 110, so I used 119 instead, which is sent without problems

    TiltUp = 120,
    TiltDown = 130,
    PanRight = 140,
    PanLeft = 150
};


#endif