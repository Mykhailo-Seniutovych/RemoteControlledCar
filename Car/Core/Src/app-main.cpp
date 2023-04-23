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
#include "nrf24/nrf24.h"
#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

#define MOUNT_HOR_CCR htim4.Instance->CCR1
#define MOUNT_VER_CCR htim4.Instance->CCR2
#define MOUNT_ARR htim4.Instance->ARR
#define MOUNT_MIN_PWM 0.025f
#define MOUNT_MAX_PWM 0.125f

#define DRV1_PNP1_PORT GPIOB
#define DRV1_PNP1_PIN GPIO_PIN_12
#define DRV1_PNP2_PORT GPIOB
#define DRV1_PNP2_PIN GPIO_PIN_13
#define DRV1_NPN1_CCR htim1.Instance->CCR1
#define DRV1_NPN2_CCR htim1.Instance->CCR2
#define DRV1_ARR htim1.Instance->ARR

#define DRV2_PNP1_PORT GPIOB
#define DRV2_PNP1_PIN GPIO_PIN_14
#define DRV2_PNP2_PORT GPIOB
#define DRV2_PNP2_PIN GPIO_PIN_15
#define DRV2_NPN1_CCR htim1.Instance->CCR3
#define DRV2_NPN2_CCR htim1.Instance->CCR4
#define DRV2_ARR htim1.Instance->ARR

#define LED_RED_CCR htim2.Instance->CCR1
#define LED_GREEN_CCR htim2.Instance->CCR2
#define LED_BLUE_CCR htim2.Instance->CCR3

static void initialize();
static void stop();
static void moveForward();
static void moveBackward();

int appMain() {
initialize();

    auto leftDriverPwm = DcDriverPwm(&DRV1_NPN1_CCR, &DRV1_NPN2_CCR, &DRV1_ARR);
    auto leftDriver = DcDriver(DRV1_PNP1_PORT, DRV1_PNP1_PIN, DRV1_PNP2_PORT, DRV1_PNP2_PIN, &leftDriverPwm);

    auto rightDriverPwm = DcDriverPwm(&DRV2_NPN1_CCR, &DRV2_NPN2_CCR, &DRV2_ARR);
    auto rightDriver = DcDriver(DRV2_PNP1_PORT, DRV2_PNP1_PIN, DRV2_PNP2_PORT, DRV2_PNP2_PIN, &rightDriverPwm);

    auto carController = CarController(&leftDriver, &rightDriver);

    auto mountPwm = MountPwm(&MOUNT_HOR_CCR, &MOUNT_VER_CCR, &MOUNT_ARR);
    auto cameraMount = CameraMount(&mountPwm, MOUNT_MIN_PWM, MOUNT_MAX_PWM);

    CommandReader commandReader;

    LedPwm ledPwm(&LED_RED_CCR, &LED_GREEN_CCR, &LED_BLUE_CCR);
    Led led(&ledPwm);

    auto commandProcessor = CommandProcessor(&carController, &cameraMount, &commandReader, &led);
    while (true) {
       commandProcessor.processNextCommand();
    }
    return 0;
}

void initialize() {
    nrf24_Init();
    nrf24_EnterRxMode();
    HAL_Delay(1000);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
}
