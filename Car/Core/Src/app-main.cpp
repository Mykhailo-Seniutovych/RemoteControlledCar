#include "app-main.h"
#include "camera/camera-mount.h"
#include "camera/mount-pwm.h"
#include "command-processing/command-processor.h"
#include "led/led-pwm.h"
#include "led/led.h"
#include "led/color.h"
#include "nrf24/nrf24.h"
#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

#define MOUNT_HOR_CCR htim1.Instance->CCR1
#define MOUNT_VER_CCR htim1.Instance->CCR2
#define MOUNT_ARR htim1.Instance->ARR

#define LED_RED_CCR htim2.Instance->CCR1
#define LED_GREEN_CCR htim2.Instance->CCR2
#define LED_BLUE_CCR htim2.Instance->CCR3

#define MIN_PWM 0.025f
#define MAX_PWM 0.125f

static void initialize();

int appMain() {
    HAL_Delay(1000);
    initialize();
    HAL_Delay(1000);

    MountPwm mountPwm(&MOUNT_HOR_CCR, &MOUNT_VER_CCR, &MOUNT_ARR);

    CameraMount cameraMount(&mountPwm, MIN_PWM, MAX_PWM);
    CommandReader commandReader;

    LedPwm ledPwm(&LED_RED_CCR, &LED_GREEN_CCR, &LED_BLUE_CCR);
    Led led(&ledPwm);
    CommandProcessor commandProcessor(&cameraMount, &commandReader, &led);

    while (1) {
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

    nrf24_Init();
    nrf24_EnterRxMode();
}

//////////////////// OLD CODE SHOULD BE DELETED

static void receiveCommandsOld();
static void receiveCommands();
static void turnOnAndOffLed(GPIO_TypeDef *port, uint16_t pin);
static void driveForward();
static void driveBackward();
static void turnOffMotion();

int appMainOld() {
    HAL_Delay(1000);
    nrf24_Init();
    nrf24_EnterRxMode();
    while (true) {
        receiveCommands();
    }
    return 0;
}

bool isRunning = false;
int counter = 0;
CarCommand currentCmd = CarCommand::None;

static void receiveCommands() {
    uint8_t data[1] = {};
    if (nrf24_IsDataAvailable()) {
        nrf24_ReadData(data, 1);

        CarCommand cmd = static_cast<CarCommand>(data[0]);
        if (cmd == CarCommand::MoveForward) {
            if (currentCmd != CarCommand::MoveForward) {
                turnOffMotion();
                currentCmd = CarCommand::MoveForward;
            }
            isRunning = true;
            counter = 0;

        } else if (cmd == CarCommand::MoveBackward) {
            if (currentCmd != CarCommand::MoveBackward) {
                turnOffMotion();
                currentCmd = CarCommand::MoveBackward;
            }

            isRunning = true;
            counter = 0;
        } else if (cmd == CarCommand::TurnRight) {
            if (currentCmd != CarCommand::TurnRight) {
                turnOffMotion();
                currentCmd = CarCommand::TurnRight;
            }

            isRunning = true;
            counter = 0;
        } else if (cmd == CarCommand::TurnLeft) {
            if (currentCmd != CarCommand::TurnLeft) {
                turnOffMotion();
                currentCmd = CarCommand::TurnLeft;
            }

            isRunning = true;
            counter = 0;
        }
    } else if (isRunning) {
        ++counter;
    }

    if (counter > 50) {
        turnOffMotion();
    }
    HAL_Delay(5);
}

static void turnOffMotion() {
    isRunning = false;
}
