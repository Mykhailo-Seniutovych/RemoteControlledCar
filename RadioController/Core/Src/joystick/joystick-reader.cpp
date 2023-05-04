#include "joystick/joystick-reader.h"
#include "stm32f1xx_hal.h"

#define ADC_RESOLUTION 4096
#define MOVEMENT_MIN_BOUNDARY 1024
#define MOVEMENT_MAX_BOUNDARY 3072

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

    // uint16_t vertScore = getScoreFromAdcValue(vertAdcValue);
    // uint16_t horScore = getScoreFromAdcValue(horAdcValue);

    auto isCentralPosition =
        horAdcValue > MOVEMENT_MIN_BOUNDARY && horAdcValue < MOVEMENT_MAX_BOUNDARY &&
        vertAdcValue > MOVEMENT_MIN_BOUNDARY && vertAdcValue < MOVEMENT_MAX_BOUNDARY;

    if (isCentralPosition) {
        return btnPressed ? JoystickState::BtnPressed : JoystickState::None;
    }

    auto isVerticalMovement = horAdcValue > 1747 && horAdcValue < 2347;
    JoystickState state;
    if (isVerticalMovement) {
        state = getVertMovementFromAdcValue(vertAdcValue);
    } else {
        state = getHorMovementFromAdcValue(horAdcValue, vertAdcValue);
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

JoystickState JoystickReader::getHorMovementFromAdcValue(uint16_t horAdcValue, uint16_t vertAdcValue) {
    auto state = horAdcValue > ADC_RESOLUTION / 2 ? JoystickState::Right : JoystickState::Left;
    // for horizontal movement, we need to know vertical state as well, so the car can drive forward or backward when making a turn
    auto verticalState = vertAdcValue > 1947 ? JoystickState::Forward : JoystickState::Backward;
    state = state | verticalState;
    return state;
}