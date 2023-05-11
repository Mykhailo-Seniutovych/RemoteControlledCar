#ifndef __JOYSTICK_JOYSTICK_READER_H
#define __JOYSTICK_JOYSTICK_READER_H
#include "joystick/joystick-state.h"
#include "stm32f1xx_hal.h"

class JoystickReader {
  public:
    JoystickReader(uint32_t vertChannel, uint32_t horChannel, ADC_HandleTypeDef *adc, GPIO_TypeDef *btnPort, uint16_t btnPin);
    JoystickState readState();

  private:
    uint32_t vertChannel_;
    uint32_t horChannel_;
    ADC_HandleTypeDef *adc_;
    GPIO_TypeDef *btnPort_;
    uint16_t btnPin_;
    ADC_ChannelConfTypeDef adcChannelConfig_;

    bool isBtnPressed();
    uint16_t readAdcValue(uint32_t channel);
};

#endif
