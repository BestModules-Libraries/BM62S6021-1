/*****************************************************************
  File:         BM62S621-1.h
  Author:       BEST MODULES CORP.
  Description:  Define classes and required variables
  Version:      V1.0.1   -- 2025-07-01
******************************************************************/
#ifndef _BM62S6021_1_H
#define _BM62S6021_1_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#define UART_RECIVE_LEN_MAX 8
#define CHECK_OK 1
#define CHECK_ERR 0

class BM62S6021_1
{
public:
  BM62S6021_1(uint8_t staPin, HardwareSerial *theSerial = &Serial);
  BM62S6021_1(uint8_t staPin, uint8_t rxPin, uint8_t txPin);
  void begin();
  void reset();
  uint8_t setID(uint8_t id, uint8_t new_id);
  uint8_t readLux(uint8_t id);
  void readHistoricalLux(uint8_t id, uint32_t HistoricalLux_cnt_arr[]);
  bool clearHistoricallux(uint8_t id);
  bool calibrateLux(uint8_t id, uint32_t lux);
  bool resetCalibrateLux(uint8_t id);

private:
  /** Provide bit field access to the configuration register */
  bool rev_buf(uint8_t len);
  void send_buf(uint8_t *par);
  void Modbus_CRCcaculate(uint8_t *ptr, int len, uint8_t *crc_H, uint8_t *crc_L);

  uint8_t _tx_buf[10] = {0};
  uint8_t _rx_buf[10] = {0};

  /*COMMAND*/
  const uint8_t _CMD_READ = 0x03;
  const uint8_t _CMD_WRITE = 0x06;
  const uint8_t _CMD_CALI = 0x08;
  const uint8_t _CMD_BOARDCAST = 0xff;

  /*REGISITER*/
  const uint8_t _REG_ID = 0x01;
  const uint8_t _REG_LUX = 0x02;
  const uint8_t _REG_HIS_LUX = 0x03;
  const uint8_t _REG_CLEAR_DATA = 0x04;
  const uint8_t _REG_RESET = 0x05;

  uint8_t _rxPin;
  uint8_t _txPin;
  uint8_t _staPin;
  HardwareSerial *_Serial;
  SoftwareSerial *softSerial;
  uint16_t Baudrate;
  uint8_t crc_H, crc_L;
};
#endif
