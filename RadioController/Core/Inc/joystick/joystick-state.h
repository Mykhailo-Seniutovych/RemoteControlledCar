#ifndef __JOYSTICK_JOYSTICK_STATE_H
#define __JOYSTICK_JOYSTICK_STATE_H

enum class JoystickState {
    None = 1 << 0,
    Forward = 1 << 1,
    Backward = 1 << 2,
    Right = 1 << 3,
    Left = 1 << 4,
    BtnPressed = 1 << 5,
};

inline JoystickState operator|(JoystickState state1, JoystickState state2) {
    return static_cast<JoystickState>(static_cast<int>(state1) | static_cast<int>(state2));
}
inline JoystickState operator&(JoystickState state1, JoystickState state2) {
    return static_cast<JoystickState>(static_cast<int>(state1) & static_cast<int>(state2));
}
inline bool isNone(JoystickState state) {
    return (state & JoystickState::None) == JoystickState::None;
}
inline bool isForward(JoystickState state) {
    return (state & JoystickState::Forward) == JoystickState::Forward;
}
inline bool isBackward(JoystickState state) {
    return (state & JoystickState::Backward) == JoystickState::Backward;
}
inline bool isRight(JoystickState state) {
    return (state & JoystickState::Right) == JoystickState::Right;
}
inline bool isLeft(JoystickState state) {
    return (state & JoystickState::Left) == JoystickState::Left;
}
inline bool isBtnPressed(JoystickState state) {
    return (state & JoystickState::BtnPressed) == JoystickState::BtnPressed;
}

#endif