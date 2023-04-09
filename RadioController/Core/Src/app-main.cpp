#include "app-main.h"
#include "car-control/command-transmitter.h"
#include "joystick/joystick-reader.h"
#include "nrf24/nrf24.h"
#include "stm32f1xx_hal.h"

#define LED_PORT GPIOC
#define LED_PIN GPIO_PIN_13

#define RIGHT_JOYSTICK_VERTICAL_ADC ADC_CHANNEL_0
#define RIGHT_JOYSTICK_HORIZONTAL_ADC ADC_CHANNEL_1
#define RIGHT_JOYSTICK_BTN_PORT GPIOC
#define RIGHT_JOYSTICK_BTN_PIN GPIO_PIN_15

#define LEFT_JOYSTICK_VERTICAL_ADC ADC_CHANNEL_2
#define LEFT_JOYSTICK_HORIZONTAL_ADC ADC_CHANNEL_3
#define LEFT_JOYSTICK_BTN_PORT GPIOB
#define LEFT_JOYSTICK_BTN_PIN GPIO_PIN_9

extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi2;

int appMain() {
    HAL_ADCEx_Calibration_Start(&hadc1);

    auto rightJoystick = JoystickReader(
        RIGHT_JOYSTICK_VERTICAL_ADC,
        RIGHT_JOYSTICK_HORIZONTAL_ADC,
        &hadc1,
        RIGHT_JOYSTICK_BTN_PORT,
        RIGHT_JOYSTICK_BTN_PIN
    );
    auto leftJoystick = JoystickReader(
        LEFT_JOYSTICK_VERTICAL_ADC,
        LEFT_JOYSTICK_HORIZONTAL_ADC,
        &hadc1,
        LEFT_JOYSTICK_BTN_PORT,
        LEFT_JOYSTICK_BTN_PIN
    );
    CommandTransmitter cmdTransmitter;
    cmdTransmitter.initializeTransmission();
    while (true) {
        auto rightState = rightJoystick.readState();
        auto leftState = leftJoystick.readState();
        cmdTransmitter.transmitCommand(rightState, leftState);
    }
    return 0;
}