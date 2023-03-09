#ifndef __JOYSTICK_JOYSTICK_READER_H
#define __JOYSTICK_JOYSTICK_READER_H
#include "joystick/joystick-state.h"
#include "stm32f1xx_hal.h"

class JoystickReader {
  public:
    JoystickReader(uint32_t xChannel, uint32_t yChannel, ADC_HandleTypeDef *adc, GPIO_TypeDef *btnPort, uint16_t btnPin);
    JoystickState readState();

  private:
    uint32_t xChannel_;
    uint32_t yChannel_;
    ADC_HandleTypeDef *adc_;
    GPIO_TypeDef *btnPort_;
    uint16_t btnPin_;
    ADC_ChannelConfTypeDef adcChannelConfig_;

    bool isBtnPressed();
    uint16_t readAdcValue(uint32_t channel);
    int16_t getScoreFromAdcValue(uint16_t adcValue);
    JoystickState getXMovementFromAdcValue(uint16_t adcValue);
    JoystickState getYMovementFromAdcValue(uint16_t adcValue);
};

#endif
