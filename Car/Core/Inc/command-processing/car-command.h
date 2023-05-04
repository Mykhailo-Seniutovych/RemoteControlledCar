#ifndef INC_COMMAND_PROCESSING_CAR_COMMAND_H_
#define INC_COMMAND_PROCESSING_CAR_COMMAND_H_

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
    TurnLeftBackward = 110,

    TiltUp = 120,
    TiltDown = 130,
    PanRight = 140,
    PanLeft = 150
};


#endif
