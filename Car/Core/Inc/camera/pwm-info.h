#ifndef INC_CAMERA_PMW_INFO_H_
#define INC_CAMERA_PMW_INFO_H_

#include <stm32f1xx_hal.h>

struct PwmInfo {
    volatile uint32_t *horCCR;
    volatile uint32_t *vertCCR;
    volatile uint32_t *ARR;

    PwmInfo(
        volatile uint32_t *horCCR,
        volatile uint32_t *horARR,
        volatile uint32_t *ARR
    );
};

#endif