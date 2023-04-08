#include "driver/driver.h"
#include "stm32f1xx_hal.h"

Driver::Driver(
    GPIO_TypeDef *pnp1Port,
    uint8_t pnp1Pin,
    GPIO_TypeDef *pnp2Port,
    uint8_t pnp2Pin,
    DriverPwm *pwm
) : pnp1Port_(pnp1Port),
    pnp1Pin_(pnp1Pin),
    pnp2Port_(pnp2Port),
    pnp2Pin_(pnp2Pin),
    pwm_(pwm) {}

void Driver::stop() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(pnp1Port_, pnp1Pin_, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pnp2Port_, pnp2Pin_, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = pnp1Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(pnp1Port_, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = pnp2Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(pnp2Port_, &GPIO_InitStruct);

    *pwm_->npn1CCR = 0;
    *pwm_->npn2CCR = 0;

    currentDirection_ = MovementDirection::None;
    currentSpeed_ = Speed::None;
}

void Driver::moveForward(Speed speed) {
    if (!isMovementChange(MovementDirection::Forward, speed))
        return;

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(pnp1Port_, pnp1Pin_, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = pnp1Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(pnp1Port_, &GPIO_InitStruct);
    *pwm_->npn2CCR = 0;

    GPIO_InitStruct.Pin = pnp2Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(pnp2Port_, &GPIO_InitStruct);
    HAL_GPIO_WritePin(pnp2Port_, pnp2Pin_, GPIO_PIN_RESET);

    // This delay is needed because when the dc motor changes direction instantly, for some reason it can run slower
    HAL_Delay(500);

    // Even when the speed is slow, we still need to provide enough stall current for the motor to start first.
    // If we set the needed ARR from the beginning, most likely the motor will not start.
    *pwm_->npn1CCR = *pwm_->ARR;
    if (speed == Speed::Slow) {
        HAL_Delay(100);
        *pwm_->npn1CCR = (*pwm_->ARR) * 0.5;
    }

    currentDirection_ = MovementDirection::Forward;
    currentSpeed_ = speed;
}

void Driver::moveBackward(Speed speed) {
    if (!isMovementChange(MovementDirection::Backward, speed))
        return;

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(pnp2Port_, pnp2Pin_, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = pnp2Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(pnp2Port_, &GPIO_InitStruct);
    *pwm_->npn1CCR = 0;

    GPIO_InitStruct.Pin = pnp1Pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(pnp1Port_, &GPIO_InitStruct);
    HAL_GPIO_WritePin(pnp1Port_, pnp1Pin_, GPIO_PIN_RESET);

    HAL_Delay(500);
    *pwm_->npn2CCR = *pwm_->ARR;
    if (speed == Speed::Slow) {
        HAL_Delay(100);
        *pwm_->npn2CCR = (*pwm_->ARR) * 0.5;
    }

    currentDirection_ = MovementDirection::Backward;
    currentSpeed_ = speed;
}

bool Driver::isMovementChange(MovementDirection newDirection, Speed newSpeed) {
    return currentDirection_ != newDirection || currentSpeed_ != newSpeed;
}
