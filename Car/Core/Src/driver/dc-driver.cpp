#include "driver/dc-driver.h"
#include "stm32f1xx_hal.h"

DcDriver::DcDriver(
    GPIO_TypeDef *pnp1Port,
    uint16_t pnp1Pin,
    GPIO_TypeDef *pnp2Port,
    uint16_t pnp2Pin,
    DcDriverPwm *pwm
) : pnp1Port_(pnp1Port),
    pnp1Pin_(pnp1Pin),
    pnp2Port_(pnp2Port),
    pnp2Pin_(pnp2Pin),
    pwm_(pwm) {
}

void DcDriver::stop() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(pnp1Port_, pnp1Pin_, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pnp2Port_, pnp2Pin_, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = pnp1Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(pnp1Port_, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = pnp2Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(pnp2Port_, &GPIO_InitStruct);

    *pwm_->npn1CCR = 0;
    *pwm_->npn2CCR = 0;
}

void DcDriver::moveForward(Speed speed) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(pnp1Port_, pnp1Pin_, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = pnp1Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(pnp1Port_, &GPIO_InitStruct);
    *pwm_->npn2CCR = 0;

    GPIO_InitStruct.Pin = pnp2Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(pnp2Port_, &GPIO_InitStruct);
    HAL_GPIO_WritePin(pnp2Port_, pnp2Pin_, GPIO_PIN_RESET);

    // Even when the speed is slow, we still need to provide enough stall current for the motor to start first.
    // If we set the needed ARR from the beginning, most likely the motor will not start.
    *pwm_->npn1CCR = *pwm_->ARR;
    if (speed == Speed::Slow) {
        HAL_Delay(100);
        *pwm_->npn1CCR = (*pwm_->ARR) * 0.35;
    }
}

void DcDriver::moveBackward(Speed speed) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(pnp2Port_, pnp2Pin_, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = pnp2Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(pnp2Port_, &GPIO_InitStruct);
    *pwm_->npn1CCR = 0;

    GPIO_InitStruct.Pin = pnp1Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(pnp1Port_, &GPIO_InitStruct);
    HAL_GPIO_WritePin(pnp1Port_, pnp1Pin_, GPIO_PIN_RESET);

    *pwm_->npn2CCR = *pwm_->ARR;
    if (speed == Speed::Slow) {
        HAL_Delay(100);
        *pwm_->npn2CCR = (*pwm_->ARR) * 0.35;
    }
}
