#ifndef __CAR_CONTROL_CAR_COMMAND_H
#define __CAR_CONTROL_CAR_COMMAND_H

enum class CarCommand {
    None = 0,
    Stop = 10,

    MoveForwardFast = 20,
    MoveForwardSlow = 30,

    MoveBackwardFast = 40,
    MoveBackwardSlow = 50,

    TurnRightFast = 60,
    TurnRightSlow = 70,

    TurnLeftFast = 80,
    TurnLeftSlow = 90,

    TiltUp = 100,
    TiltDown = 110,
    PanRight = 120,
    PanLeft = 130
};

#endif