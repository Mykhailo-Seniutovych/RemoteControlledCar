#include "app-main.h"
#include "nrf24/nrf24.h"
#include "stm32f1xx_hal.h"

#define LED_GREEN_PORT GPIOB
#define LED_GREEN_PIN GPIO_PIN_12

#define LED_RED_PORT GPIOB
#define LED_RED_PIN GPIO_PIN_14

#define LED_BLUE_PORT GPIOA
#define LED_BLUE_PIN GPIO_PIN_10

#define LED_YELLOW_PORT GPIOA
#define LED_YELLOW_PIN GPIO_PIN_12

#define MOVE_BACK_PORT GPIOC
#define MOVE_BACK_PIN GPIO_PIN_14

#define MOVE_FORWARD_PORT GPIOC
#define MOVE_FORWARD_PIN GPIO_PIN_15

extern SPI_HandleTypeDef hspi1;

enum CarCommand {
    None = 0,
    MoveForward = 10,
    MoveBackward = 20,
    TurnRight = 30,
    TurnLeft = 40
};

static void receiveCommandsOld();
static void receiveCommands();
static void turnOnAndOffLed(GPIO_TypeDef *port, uint16_t pin);
static void driveForward();
static void driveBackward();
static void turnOffMotion();

int appMain() {
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
            HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOVE_FORWARD_PORT, MOVE_FORWARD_PIN, GPIO_PIN_SET);
            isRunning = true;
            counter = 0;

        } else if (cmd == CarCommand::MoveBackward) {
            if (currentCmd != CarCommand::MoveBackward) {
                turnOffMotion();
                currentCmd = CarCommand::MoveBackward;
            }
            HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOVE_BACK_PORT, MOVE_BACK_PIN, GPIO_PIN_SET);
            isRunning = true;
            counter = 0;
        } else if (cmd == CarCommand::TurnRight) {
            if (currentCmd != CarCommand::TurnRight) {
                turnOffMotion();
                currentCmd = CarCommand::TurnRight;
            }
            HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOVE_FORWARD_PORT, MOVE_FORWARD_PIN, GPIO_PIN_SET);
            isRunning = true;
            counter = 0;
        } else if (cmd == CarCommand::TurnLeft) {
            if (currentCmd != CarCommand::TurnLeft) {
                turnOffMotion();
                currentCmd = CarCommand::TurnLeft;
            }
            HAL_GPIO_WritePin(LED_YELLOW_PORT, LED_YELLOW_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOVE_BACK_PORT, MOVE_BACK_PIN, GPIO_PIN_SET);
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
    HAL_GPIO_WritePin(MOVE_FORWARD_PORT, MOVE_FORWARD_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOVE_BACK_PORT, MOVE_BACK_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_YELLOW_PORT, LED_YELLOW_PIN, GPIO_PIN_SET);
    isRunning = false;
}

static void receiveCommandsOld() {
    uint8_t data[1] = {};
    nrf24_WaitAndReceiveData(data, 1);

    CarCommand cmd = static_cast<CarCommand>(data[0]);
    if (cmd == CarCommand::MoveForward) {
        turnOnAndOffLed(LED_GREEN_PORT, LED_GREEN_PIN);
    } else if (cmd == CarCommand::MoveBackward) {
        turnOnAndOffLed(LED_RED_PORT, LED_RED_PIN);
    } else if (cmd == CarCommand::TurnRight) {
        turnOnAndOffLed(LED_BLUE_PORT, LED_BLUE_PIN);
    } else if (cmd == CarCommand::TurnLeft) {
        turnOnAndOffLed(LED_YELLOW_PORT, LED_YELLOW_PIN);
    }
}

static void turnOnAndOffLed(GPIO_TypeDef *port, uint16_t pin) {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}