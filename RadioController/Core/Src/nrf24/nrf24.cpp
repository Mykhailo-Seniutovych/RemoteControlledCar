#include "nrf24/nrf24.h"
#include <stdint.h>
#include <stm32f1xx_hal.h>

static uint8_t TxRxAddress[] = {0xE3, 0xF0, 0xF0, 0xE8, 0xE8};
#define TX_RX_ADDR_SIZE 5

static void nrf24_EnableCE();
static void nrf24_DisableCE();
static void nrf24_EnableCSN();
static void nrf24_DisableCSN();
static void nrf24_CleanStatus();
static void nrf24_EnableAckPayload();
static void nrf24_FlushTxCmd();
static void nrf24_FlushRxCmd();
static void nrf24_ReceivePayloadCmd(uint8_t *data, uint8_t size);
static void nrf24_WritePayloadCmd(const uint8_t *data, uint8_t size);

void nrf24_Init() {
    uint8_t configData = 0b00001100; // Reset NRF_CONFIG and enable 16-bit CRC.
    nrf24_WriteRegister(CONFIG_REG, &configData, 1);

    uint8_t retriesData = 0b01011111; // 5 delay, 15 retries
    nrf24_WriteRegister(SETUP_RETR_REG, &retriesData, 1);

    uint8_t rfSetup = 0b00010111; // Speed - 1 Mbps, output power - 0dBm
    nrf24_WriteRegister(RF_SETUP_REG, &rfSetup, 1);

    uint8_t channel = 19;
    nrf24_WriteRegister(RF_CH_REG, &channel, 1);

    uint8_t featureSetup = 0b00000110;
    nrf24_WriteRegister(FEATURE_REG, &featureSetup, 1); // Enable dynamic payload length, and enable payload with ACK

    uint8_t dynpdSetup = 0b00000011;
    nrf24_WriteRegister(DYNPD_REG, &dynpdSetup, 1);

    nrf24_FlushTxCmd();
    nrf24_FlushRxCmd();
    nrf24_CleanStatus();

    nrf24_EnableAckPayload();
}

static void nrf24_EnableAckPayload() {
    uint8_t featureSetup = 0b00000110;
    nrf24_WriteRegister(FEATURE_REG, &featureSetup, 1); // Enable dynamic payload length, and enable payload with ACK

    uint8_t dynpdSetup = 0b00000011;
    nrf24_WriteRegister(DYNPD_REG, &dynpdSetup, 1);
}

static void nrf24_FlushTxCmd() {
    nrf24_EnableCSN();
    uint8_t command = FLUSH_TX_CMD;
    HAL_SPI_Transmit(SPI, &command, 1, 100);
    HAL_Delay(1);
    nrf24_DisableCSN();
}
static void nrf24_FlushRxCmd() {
    nrf24_EnableCSN();
    uint8_t command = FLUSH_RX_CMD;
    HAL_SPI_Transmit(SPI, &command, 1, 100);
    HAL_Delay(1);
    nrf24_DisableCSN();
}

void nrf24_DisableAck() {
    uint8_t noRetransmission = 0b00000000;
    nrf24_WriteRegister(SETUP_RETR_REG, &noRetransmission, 1);

    uint8_t noAck = 0b00000000;
    nrf24_WriteRegister(EN_AA_REG, &noAck, 1);
}

void nrf24_EnterTxMode() {
    nrf24_EnableCE();

    uint8_t config = 0;
    nrf24_ReadRegister(CONFIG_REG, &config, 1);
    config = (config | 0b00000010) & (~0b00000001); // set PWR_UP bit high, and PRIM_RX low
    nrf24_WriteRegister(CONFIG_REG, &config, 1);

    nrf24_WriteRegister(TX_ADDR_REG, TxRxAddress, TX_RX_ADDR_SIZE);
    nrf24_WriteRegister(RX_ADDR_P0_REG, TxRxAddress, TX_RX_ADDR_SIZE);
    uint8_t payloadSize = 5;
    nrf24_WriteRegister(RX_PW_P0_REG, &payloadSize, 1);

    nrf24_DisableCE();
}

void nrf24_EnterRxMode() {
    nrf24_WriteRegister(RX_ADDR_P0_REG, TxRxAddress, TX_RX_ADDR_SIZE);
    uint8_t payloadSize = 5;
    nrf24_WriteRegister(RX_PW_P0_REG, &payloadSize, 1);

    uint8_t config = 0;
    nrf24_ReadRegister(CONFIG_REG, &config, 1);
    config = (config | 0b00000010) | (0b00000001); // set PWR_UP bit high, and PRIM_RX high
    nrf24_WriteRegister(CONFIG_REG, &config, 1);

    nrf24_CleanStatus();

    nrf24_EnableCE();
}

bool nrf24_TransmitData(const uint8_t *data, uint8_t size) {
    nrf24_CleanStatus();

    nrf24_WritePayloadCmd(data, size);

    // Pulse CE to start the transmission
    nrf24_EnableCE();
    HAL_Delay(10);
    nrf24_DisableCE();
    nrf24_FlushTxCmd();

    // Check success
    HAL_Delay(50);
    uint8_t status = 0;
    nrf24_ReadRegister(STATUS_REG, &status, 1);
    bool isSuccessfulTrans = status & 0b00100000;

    nrf24_CleanStatus();

    return isSuccessfulTrans;
}

static void nrf24_WritePayloadCmd(const uint8_t *data, uint8_t size) {
    nrf24_EnableCSN();
    uint8_t command = W_TX_PAYLOAD_CMD;
    HAL_SPI_Transmit(SPI, &command, 1, 100);
    HAL_Delay(1);
    HAL_SPI_Transmit(SPI, (uint8_t *)data, size, 1000);
    nrf24_DisableCSN();

    nrf24_FlushRxCmd();
}

void nrf24_WaitAndReceiveData(uint8_t *buffer, uint8_t size) {
    nrf24_CleanStatus();
    nrf24_EnableCE();

    uint8_t status = 0;
    while (true) {
        nrf24_ReadRegister(FIFO_STATUS_REG, &status, 1);
        uint8_t isRxEmpty = status & 0b00000001;
        if (!isRxEmpty) {
            break;
        }
    }

    nrf24_ReceivePayloadCmd(buffer, size);
    nrf24_DisableCE();
}

bool nrf24_IsDataAvailable() {
    nrf24_EnableCE();
    uint8_t status = 0;
    nrf24_ReadRegister(FIFO_STATUS_REG, &status, 1);
    uint8_t isRxEmpty = status & 0b00000001;
    return !isRxEmpty;
}

void nrf24_ReadData(uint8_t *buffer, uint8_t size) {

    nrf24_ReceivePayloadCmd(buffer, size);
    nrf24_FlushRxCmd();

    nrf24_DisableCE();
    //nrf24_CleanStatus();
}

static void nrf24_ReceivePayloadCmd(uint8_t *buffer, uint8_t size) {
    nrf24_EnableCSN();

    uint8_t command = R_RX_PAYLOAD_CMD;
    HAL_SPI_Transmit(SPI, &command, 1, 100);
    HAL_Delay(1);

    HAL_SPI_Receive(SPI, (uint8_t *)buffer, size, 1000);

    nrf24_DisableCSN();
}

static void nrf24_CleanStatus() {
    uint8_t cleanStatusReg = 0b01111111;
    nrf24_WriteRegister(STATUS_REG, &cleanStatusReg, 1);
}

void nrf24_ReadRegister(uint8_t registerAddress, uint8_t *buffer, uint8_t size) {
    uint8_t command = R_REGISTER_CMD | registerAddress;

    nrf24_EnableCSN();

    HAL_SPI_Transmit(SPI, &command, 1, 100);
    HAL_SPI_Receive(SPI, buffer, size, 100);

    nrf24_DisableCSN();
}

void nrf24_WriteRegister(uint8_t registerAddress, const uint8_t *data, uint8_t size) {
    uint8_t command = W_REGISTER_CMD | registerAddress;

    nrf24_EnableCSN();

    HAL_SPI_Transmit(SPI, &command, 1, 100);
    HAL_SPI_Transmit(SPI, (uint8_t *)data, size, 100);

    nrf24_DisableCSN();
}

static void nrf24_EnableCE() {
    HAL_GPIO_WritePin(CE_PORT, CE_PIN, GPIO_PIN_SET);
}
static void nrf24_DisableCE() {
    HAL_GPIO_WritePin(CE_PORT, CE_PIN, GPIO_PIN_RESET);
}

static void nrf24_EnableCSN() {
    HAL_GPIO_WritePin(CSN_PORT, CSN_PIN, GPIO_PIN_RESET);
}

static void nrf24_DisableCSN() {
    HAL_GPIO_WritePin(CSN_PORT, CSN_PIN, GPIO_PIN_SET);
}