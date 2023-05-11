#include "app-main.h"
#include "camera/camera-mount.h"
#include "camera/mount-pwm.h"
#include "command-processing/command-processor.h"
#include "driver/car-controller.h"
#include "driver/dc-driver-pwm.h"
#include "driver/dc-driver.h"
#include "led/color.h"
#include "led/led-pwm.h"
#include "led/led.h"
#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

#define MOUNT_HOR_CCR htim4.Instance->CCR1
#define MOUNT_VER_CCR htim4.Instance->CCR2
#define MOUNT_ARR htim4.Instance->ARR
#define MOUNT_MIN_PWM 0.025f
#define MOUNT_MAX_PWM 0.125f

#define DRV_RIGHT_PNP1_PORT GPIOB
#define DRV_RIGHT_PNP1_PIN GPIO_PIN_12
#define DRV_RIGHT_PNP2_PORT GPIOB
#define DRV_RIGHT_PNP2_PIN GPIO_PIN_13
#define DRV_RIGHT_NPN1_CCR htim3.Instance->CCR2
#define DRV_RIGHT_NPN2_CCR htim1.Instance->CCR2

#define DRV_LEFT_PNP1_PORT GPIOB
#define DRV_LEFT_PNP1_PIN GPIO_PIN_14
#define DRV_LEFT_PNP2_PORT GPIOB
#define DRV_LEFT_PNP2_PIN GPIO_PIN_15
#define DRV_LEFT_NPN1_CCR htim1.Instance->CCR3
#define DRV_LEFT_NPN2_CCR htim1.Instance->CCR4

#define DRV_ARR 255

#define LED_RED_CCR htim2.Instance->CCR1
#define LED_GREEN_CCR htim2.Instance->CCR2
#define LED_BLUE_CCR htim2.Instance->CCR3

static void initializePWM();

int appMain() {
    auto commandReceiver = CommandReceiver();

    commandReceiver.initializeReceiver();
    initializePWM();

    auto leftDriverPwm = DcDriverPwm(&DRV_LEFT_NPN1_CCR, &DRV_LEFT_NPN2_CCR, DRV_ARR);
    auto leftDriver = DcDriver(DRV_LEFT_PNP1_PORT, DRV_LEFT_PNP1_PIN, DRV_LEFT_PNP2_PORT, DRV_LEFT_PNP2_PIN, &leftDriverPwm);

    auto rightDriverPwm = DcDriverPwm(&DRV_RIGHT_NPN1_CCR, &DRV_RIGHT_NPN2_CCR, DRV_ARR);
    auto rightDriver = DcDriver(DRV_RIGHT_PNP1_PORT, DRV_RIGHT_PNP1_PIN, DRV_RIGHT_PNP2_PORT, DRV_RIGHT_PNP2_PIN, &rightDriverPwm);

    auto carController = CarController(&leftDriver, &rightDriver);

    auto mountPwm = MountPwm(&MOUNT_HOR_CCR, &MOUNT_VER_CCR, &MOUNT_ARR);

    auto cameraMount = CameraMount(&mountPwm, MOUNT_MIN_PWM, MOUNT_MAX_PWM);

    LedPwm ledPwm(&LED_RED_CCR, &LED_GREEN_CCR, &LED_BLUE_CCR);
    Led led(&ledPwm);

    auto commandProcessor = CommandProcessor(&carController, &cameraMount, &led);

    while (true) {
        auto cmd = commandReceiver.getNextCommand();
        commandProcessor.processCommand(cmd);
    }
    return 0;
}

void initializePWM() {
    htim1.Instance->ARR = DRV_ARR;
    htim3.Instance->ARR = DRV_ARR;
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
}
