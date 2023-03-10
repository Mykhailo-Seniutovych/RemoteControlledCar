#ifndef INC_NRF24_NRF24_H_
#define INC_NRF24_NRF24_H_

#include <stdbool.h>
#include <stdint.h>
#include <stm32f1xx_hal.h>

// GPIO
#define CSN_PORT GPIOA
#define CSN_PIN GPIO_PIN_8
#define CE_PORT GPIOA
#define CE_PIN GPIO_PIN_9

// SPI
extern SPI_HandleTypeDef hspi2;
#define SPI &hspi2

// Registers
#define CONFIG_REG 0x00
#define RF_CH_REG 0x05
#define RX_ADDR_P0_REG 0x0A
#define TX_ADDR_REG 0x10
#define STATUS_REG 0x07
#define EN_AA_REG 0x01
#define SETUP_RETR_REG 0x04
#define RF_SETUP_REG 0x06
#define FIFO_STATUS_REG 0x17
#define RX_PW_P0_REG 0x11
// new
#define SETUP_RETR_REG 0x04
#define FEATURE_REG 0x1D
#define DYNPD_REG 0x1C

// Commands
#define R_REGISTER_CMD 0b00000000
#define W_REGISTER_CMD 0b00100000
#define R_RX_PAYLOAD_CMD 0b01100001
#define W_TX_PAYLOAD_CMD 0b10100000
#define FLUSH_TX_CMD 0b11100001
#define FLUSH_RX_CMD 0b11100010

void nrf24_ReadRegister(uint8_t registerAddress, uint8_t *buffer, uint8_t size);
void nrf24_WriteRegister(uint8_t registerAddress, const uint8_t *data, uint8_t size);
void nrf24_Init();
void nrf24_EnterTxMode();
void nrf24_EnterRxMode();
bool nrf24_TransmitData(const uint8_t *data, uint8_t size);
void nrf24_WaitAndReceiveData(uint8_t *buffer, uint8_t size);
bool nrf24_IsDataAvailable();
void nrf24_ReadData(uint8_t *buffer, uint8_t size);

void nrf24_DisableAck();

#endif