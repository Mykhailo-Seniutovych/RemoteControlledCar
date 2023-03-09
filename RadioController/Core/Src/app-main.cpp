#include "app-main.h"
#include "joystick/joystick-reader.h"
#include "stm32f1xx_hal.h"

#define LED_PORT GPIOC
#define LED_PIN GPIO_PIN_13

#define RIGHT_JOYSTICK_BTN_PORT GPIOC
#define RIGHT_JOYSTICK_BTN_PIN GPIO_PIN_15

extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi2;

int appMain() {
    HAL_ADCEx_Calibration_Start(&hadc1);

    JoystickReader joystick(ADC_CHANNEL_0, ADC_CHANNEL_1, &hadc1, RIGHT_JOYSTICK_BTN_PORT, RIGHT_JOYSTICK_BTN_PIN);
    while (true) {
        JoystickState state = joystick.readState();
    }
    return 0;
}