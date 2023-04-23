#include "command-processing/command-receiver.h"
#include "nrf24/nrf24.h"

CarCommand CommandReceiver::getNextCommand() {
    CarCommand cmd = CarCommand::None;
    uint8_t data[1] = {};
    if (nrf24_IsDataAvailable()) {
        nrf24_ReadData(data, 1);
        cmd = static_cast<CarCommand>(data[0]);
    }
    return cmd;
}

void CommandReceiver::initializeReceiver() {
    nrf24_Init();
    nrf24_EnterRxMode();
    HAL_Delay(1000);
}
