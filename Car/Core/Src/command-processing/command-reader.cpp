#include "command-processing/command-reader.h"
#include "nrf24/nrf24.h"

CarCommand CommandReader::getNextCommand() {
    CarCommand cmd = CarCommand::None;
    uint8_t data[1] = {};
    if (nrf24_IsDataAvailable()) {
        nrf24_ReadData(data, 1);
        cmd = static_cast<CarCommand>(data[0]);
    }
    return cmd;
}