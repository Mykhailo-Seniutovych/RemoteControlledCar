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

extern SPI_HandleTypeDef hspi1;

enum CarCommand {
    MoveForward = 10,
    MoveBackward = 20,
    TurnRight = 30,
    TurnLeft = 40
};

static void receiveCommands();
static void turnOnLed(GPIO_TypeDef *port, uint16_t pin);

int appMain() {
    nrf24_Init();
    nrf24_EnterRxMode();
    while (true) {
        receiveCommands();
    }
    return 0;
}

static void receiveCommands() {
    uint8_t data[1] = {};
    nrf24_WaitAndReceiveData(data, 1);

    CarCommand cmd = static_cast<CarCommand>(data[0]);
    if (cmd == CarCommand::MoveForward) {
        turnOnLed(LED_GREEN_PORT, LED_GREEN_PIN);
    } else if (cmd == CarCommand::MoveBackward) {
        turnOnLed(LED_RED_PORT, LED_RED_PIN);
    } else if (cmd == CarCommand::TurnRight) {
        turnOnLed(LED_BLUE_PORT, LED_BLUE_PIN);
    } else if (cmd == CarCommand::TurnLeft) {
        turnOnLed(LED_YELLOW_PORT, LED_YELLOW_PIN);
    }
}

static void turnOnLed(GPIO_TypeDef *port, uint16_t pin) {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}