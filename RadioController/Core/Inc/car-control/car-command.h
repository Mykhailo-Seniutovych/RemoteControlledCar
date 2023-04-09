#ifndef __CAR_CONTROL_CAR_COMMAND_H
#define __CAR_CONTROL_CAR_COMMAND_H

enum class CarCommand {
    MoveForwardFast = 10,
    MoveForwardSlow = 20,

    MoveBackwardFast = 30,
    MoveBackwardSlow = 40,

    TurnRightFast = 50,
    TurnRightSlow = 60,

    TurnLeftFast = 70,
    TurnLeftSlow = 80,

    TiltUp = 90,
    TiltDown = 100,
    PanRight = 110,
    PanLeft = 120
};

#endif