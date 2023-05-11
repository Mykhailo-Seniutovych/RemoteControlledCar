#include "joystick/joystick-reader.h"
#include "stm32f1xx_hal.h"

#define ADC_RESOLUTION 4096


#define HOR_JOYSTICK_

JoystickReader::JoystickReader(
    uint32_t vertChannel,
    uint32_t horChannel,
    ADC_HandleTypeDef *adc,
    GPIO_TypeDef *btnPort,
    uint16_t btnPin
) : vertChannel_(vertChannel), horChannel_(horChannel), adc_(adc), btnPort_(btnPort), btnPin_(btnPin) {
    adcChannelConfig_.Rank = ADC_REGULAR_RANK_1;
    adcChannelConfig_.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
}

JoystickState JoystickReader::readState() {
    auto btnPressed = isBtnPressed();
    uint16_t vertAdcValue = readAdcValue(vertChannel_);
    uint16_t horAdcValue = readAdcValue(horChannel_);

    uint16_t middleAdcValue = ADC_RESOLUTION / 2;

    auto state = JoystickState();
    state.vertValue = vertAdcValue - middleAdcValue;
    state.horValue = horAdcValue - middleAdcValue;
    state.isBtnPressed = btnPressed;

    return state;
}

bool JoystickReader::isBtnPressed() {
    return !HAL_GPIO_ReadPin(btnPort_, btnPin_);
}

uint16_t JoystickReader::readAdcValue(uint32_t channel) {
    adcChannelConfig_.Channel = channel;
    HAL_ADC_ConfigChannel(adc_, &adcChannelConfig_);
    HAL_ADC_Start(adc_);
    HAL_ADC_PollForConversion(adc_, 100);
    return HAL_ADC_GetValue(adc_);
}

