#ifndef INC_CAMERA_MOUNT_PWM_H_
#define INC_CAMERA_MOUNT_PWM_H_

#include <stdint.h>

struct MountPwm {
    volatile uint32_t *horCCR;
    volatile uint32_t *vertCCR;
    volatile uint32_t *ARR;

    MountPwm(
        volatile uint32_t *horCCR,
        volatile uint32_t *horARR,
        volatile uint32_t *ARR
    );
};

#endif