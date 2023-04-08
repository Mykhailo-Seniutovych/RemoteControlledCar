#include "app-main.h"
#include "camera/camera-mount.h"
#include "camera/mount-pwm.h"
#include "command-processing/command-processor.h"
#include "driver/driver-pwm.h"
#include "driver/driver.h"
#include "led/color.h"
#include "led/led-pwm.h"
#include "led/led.h"
#include "nrf24/nrf24.h"
#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

#define MOUNT_HOR_CCR htim1.Instance->CCR1
#define MOUNT_VER_CCR htim1.Instance->CCR2
#define MOUNT_ARR htim1.Instance->ARR
#define MOUNT_MIN_PWM 0.025f
#define MOUNT_MAX_PWM 0.125f

#define DRV1_PNP1_PORT GPIOB
#define DRV1_PNP1_PIN GPIO_PIN_1
#define DRV1_PNP2_PORT GPIOB
#define DRV1_PNP2_PIN GPIO_PIN_0
#define DRIVER1_NPN1_CCR htim4.Instance->CCR1
#define DRIVER1_NPN2_CCR htim4.Instance->CCR2
#define DRIVER1_ARR htim4.Instance->ARR

#define LED_RED_CCR htim2.Instance->CCR1
#define LED_GREEN_CCR htim2.Instance->CCR2
#define LED_BLUE_CCR htim2.Instance->CCR3

static void initialize();

int appMain() {
    HAL_Delay(1000);
    initialize();
    HAL_Delay(1000);

    auto driverPwm = DriverPwm(&DRIVER1_NPN1_CCR, &DRIVER1_NPN2_CCR, &DRIVER1_ARR);
    auto driver = Driver(DRV1_PNP1_PORT, DRV1_PNP1_PIN, DRV1_PNP2_PORT, DRV1_PNP2_PIN, &driverPwm);

    auto mountPwm = MountPwm(&MOUNT_HOR_CCR, &MOUNT_VER_CCR, &MOUNT_ARR);
    auto cameraMount = CameraMount(&mountPwm, MOUNT_MIN_PWM, MOUNT_MAX_PWM);

    CommandReader commandReader;

    LedPwm ledPwm(&LED_RED_CCR, &LED_GREEN_CCR, &LED_BLUE_CCR);
    Led led(&ledPwm);

    auto commandProcessor =  CommandProcessor(&driver, &cameraMount, &commandReader, &led);
    while (true) {
        commandProcessor.processNextCommand();
        HAL_Delay(10);
    }
    return 0;
}

void initialize() {
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);

    nrf24_Init();
    nrf24_EnterRxMode();
}
