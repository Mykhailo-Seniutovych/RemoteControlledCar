#ifndef INC_LED_COLOR_H_
#define INC_LED_COLOR_H_

#include <stdint.h>

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    Color(
        uint8_t red,
        uint8_t green,
        uint8_t blue
    );
};

#endif