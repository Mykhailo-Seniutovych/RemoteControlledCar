#include "joystick/joystick-reader.h"
#include "stm32f1xx_hal.h"

#define ADC_RESOLUTION 4096
#define MOVEMENT_MIN_BOUNDARY 1024
#define MOVEMENT_MAX_BOUNDARY 3072

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

    uint16_t vertScore = getScoreFromAdcValue(vertAdcValue);
    uint16_t horScore = getScoreFromAdcValue(horAdcValue);

    if (vertScore == 0 && horScore == 0) {
        return btnPressed ? JoystickState::BtnPressed : JoystickState::None;
    }

    JoystickState state;
    if (vertScore >= horScore) {
        state = getVertMovementFromAdcValue(vertAdcValue);
    } else {
        state = getHorMovementFromAdcValue(horAdcValue);
    }

    if (btnPressed) {
        state = state | JoystickState::BtnPressed;
    }

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

int16_t JoystickReader::getScoreFromAdcValue(uint16_t adcValue) {
    if (adcValue < MOVEMENT_MIN_BOUNDARY) {
        return MOVEMENT_MIN_BOUNDARY - adcValue;
    }
    if (adcValue > MOVEMENT_MAX_BOUNDARY) {
        return adcValue - MOVEMENT_MAX_BOUNDARY;
    }
    return 0;
}

JoystickState JoystickReader::getVertMovementFromAdcValue(uint16_t adcValue) {
    return adcValue > ADC_RESOLUTION / 2 ? JoystickState::Forward : JoystickState::Backward;
}
JoystickState JoystickReader::getHorMovementFromAdcValue(uint16_t adcValue) {
    return adcValue > ADC_RESOLUTION / 2 ? JoystickState::Right : JoystickState::Left;
}