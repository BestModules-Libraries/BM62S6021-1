/*****************************************************************
  File:          BM62S6021-1.cpp
  Author:        BEST MODULES CORP.
  Description:   Use UART communication to obtain ambient light intensity
  Version:       V1.0.1   -- 2025-07-01
******************************************************************/
#include "BM62S6021-1.h"

uint8_t status;

/**********************************************************
  Description:    Constructor
  Parameters:     *theSerial : point of Serial
                  staPin : control pin of rs485
  Return          None
  Others:         None
**********************************************************/
BM62S6021_1::BM62S6021_1(uint8_t staPin, HardwareSerial *theSerial)
{
  _Serial = theSerial;
  _staPin = staPin;
  softSerial = NULL;
}
/*************************************************
  Description:    Initialize a member variable
  Parameters:     sta_pin : control pin of rs485
                  rxPin：RX PIN of SoftwareSerial
                  txPin：TX PIN of SoftwareSerial
  Return          None
  Others:         None
*************************************************/
BM62S6021_1::BM62S6021_1(uint8_t staPin, uint8_t rxPin, uint8_t txPin)
{
  _Serial = NULL;
  _staPin = staPin;
  _rxPin = rxPin;
  _txPin = txPin;
  softSerial = new SoftwareSerial(_rxPin, _txPin);
}
/**********************************************************
  Description: Initialize communication rate
  Parameters:  void
  Return:      void
  Others:      None
**********************************************************/
void BM62S6021_1::begin()
{
  if (softSerial != NULL)
  {
    softSerial->begin(9600);
    softSerial->setTimeout(20); // set softwareUart boudrate
    pinMode(_staPin, OUTPUT);
  }
  else
  {
    _Serial->begin(9600);
    _Serial->setTimeout(20); // set hardwareUart timeout
    pinMode(_staPin, OUTPUT);
  }
}
/**********************************************************
  Description:  Reset ID become 0x01
  Parameters:   void
  Return:       void
  Others:       None
**********************************************************/
void BM62S6021_1::reset()
{
  _tx_buf[0] = _CMD_BOARDCAST;
  _tx_buf[1] = _CMD_WRITE;
  _tx_buf[2] = 0x00;
  _tx_buf[3] = _REG_ID;
  _tx_buf[4] = 0x00;
  _tx_buf[5] = 0x01;
  send_buf(_tx_buf);
  rev_buf(8);
}
/**********************************************************
  Description: Set the module ID
  Parameters:  id : current ID of module
               new_id : new ID of module 1 - 254
  Return:      Communication status  1:Success 0:Fail
  Others:      None
**********************************************************/
uint8_t BM62S6021_1::setID(uint8_t id, uint8_t new_id)
{
  _tx_buf[0] = id;
  _tx_buf[1] = _CMD_WRITE;
  _tx_buf[2] = 0x00;
  _tx_buf[3] = _REG_ID;
  _tx_buf[4] = 0x00;
  _tx_buf[5] = new_id;
  send_buf(_tx_buf);
  status = rev_buf(8);
  if (!status)
  {
    return CHECK_ERR;
  }
  else if (_rx_buf[0] != new_id)
  {
    return CHECK_ERR;
  }
  else
  {
    return CHECK_OK;
  }
}
/**********************************************************
  Description: Get light intensity
  Parameters:  id : current ID of module
  Return:      Light intensity, unit:LUX
  Others:      None
**********************************************************/
uint8_t BM62S6021_1::readLux(uint8_t id)
{
  uint16_t lux_L, lux_H, lux;

  _tx_buf[0] = id;
  _tx_buf[1] = _CMD_READ;
  _tx_buf[2] = 0x00;
  _tx_buf[3] = _REG_LUX;
  _tx_buf[4] = 0x00;
  _tx_buf[5] = 0x00;

  send_buf(_tx_buf);
  status = rev_buf(8);

  lux_L = (((uint16_t)_rx_buf[4]) << 8) + ((uint16_t)_rx_buf[5]);
  lux_H = ((uint16_t)_rx_buf[3]);
  lux = (((uint32_t)lux_H) << 16) + ((uint32_t)lux_L);

  if (!status)
  {
    return CHECK_ERR;
  }
  else if (_rx_buf[0] != id)
  {
    return CHECK_ERR;
  }
  else
  {
    return lux;
  }
}
/**********************************************************
  Description:  Get historical light intensity data
  Parameters:   id: current ID of module
                HistoricalLux_cnt_arr[]: Store historical records of ambient light intensity
  Return:       void
  Others:       None
**********************************************************/
void BM62S6021_1::readHistoricalLux(uint8_t id, uint32_t HistoricalLux_cnt_arr[])
{
  bool once = false;
  uint8_t HistoricalLux_cnt = 167, number = 0;

  _tx_buf[0] = id;
  _tx_buf[1] = _CMD_READ;
  _tx_buf[2] = 0x00;
  _tx_buf[3] = _REG_HIS_LUX;
  _tx_buf[4] = 0x00;
  _tx_buf[5] = 0x00;
  send_buf(_tx_buf);

  while (HistoricalLux_cnt > 0)
  {
    status = rev_buf(8);
    if (!once)
    {
      once = true;
      HistoricalLux_cnt = _rx_buf[2];
    }
    HistoricalLux_cnt_arr[number] = (((uint32_t)_rx_buf[3]) << 16) + (((uint32_t)_rx_buf[4]) << 8) + _rx_buf[5];

    number++;
    HistoricalLux_cnt--;

    // for (uint8_t i = 0; i < number; i++)
    // {
    //   Serial.print("lux");
    //   Serial.print(i);
    //   Serial.print(" = ");
    //   Serial.println(HistoricalLux_cnt_arr[i]);
    // }
  }
}
/**********************************************************
  Description:  Clear historical light intensity data
  Parameters:   id : current ID of module
  Return:       true or false
  Others:       None
**********************************************************/
bool BM62S6021_1::clearHistoricallux(uint8_t id)
{
  _tx_buf[0] = id;
  _tx_buf[1] = _CMD_WRITE;
  _tx_buf[2] = 0x00;
  _tx_buf[3] = _REG_CLEAR_DATA;
  _tx_buf[4] = 0x00;
  _tx_buf[5] = 0x00;
  send_buf(_tx_buf);
  status = rev_buf(8);
  if (!status)
  {
    return false;
  }
  else if (_rx_buf[0] != id)
  {
    return false;
  }
  else
  {
    return true;
  }
}
/**********************************************************
  Description:  Calibrate the light intensity
  Parameters:   id: current ID of module
                lux: real lux
  Return:       true: Calibration successful
                false: Calibration failed
  Others:       None
**********************************************************/
bool BM62S6021_1::calibrateLux(uint8_t id, uint32_t lux)
{
  static uint8_t lux_mid, lux_high, lux_low;

  lux_high = lux >> 16;
  lux_mid = (lux & 0x0ff00) >> 8;
  lux_low = lux & 0x00ff;

  _tx_buf[0] = id;
  _tx_buf[1] = _CMD_CALI;
  _tx_buf[2] = 0x03;
  _tx_buf[3] = lux_high;
  _tx_buf[4] = lux_mid;
  _tx_buf[5] = lux_low;

  send_buf(_tx_buf);
  status = rev_buf(8);
  if (!status)
  {
    return false;
  }
  else if (_rx_buf[0] != id)
  {
    return false;
  }
  else
  {
    return true;
  }
}
/**********************************************************
  Description:  Clear calibration data
  Parameters:   id : current ID of module
  Return:       true or false
  Others:       None
**********************************************************/
bool BM62S6021_1::resetCalibrateLux(uint8_t id)
{
  _tx_buf[0] = id;
  _tx_buf[1] = _CMD_WRITE;
  _tx_buf[2] = 0x00;
  _tx_buf[3] = _REG_RESET;
  _tx_buf[4] = 0x00;
  _tx_buf[5] = 0x00;
  send_buf(_tx_buf);
  status = rev_buf(8);
  if (!status)
  {
    return false;
  }
  else if (_rx_buf[0] != id)
  {
    return false;
  }
  else
  {
    return true;
  }
}
/**********************************************************
  Description:  Send MODBUS RTU Protocol inquiry frame
  Parameters:   *par : array first address
  Return:       void
  Others:       None
**********************************************************/
void BM62S6021_1::send_buf(uint8_t *par)
{
  uint8_t index;

  Modbus_CRCcaculate(_tx_buf, UART_RECIVE_LEN_MAX - 2, &_tx_buf[UART_RECIVE_LEN_MAX - 1], &_tx_buf[UART_RECIVE_LEN_MAX - 2]);
  digitalWrite(_staPin, HIGH);
  if (softSerial != NULL)
  {
    for (index = 0; index < UART_RECIVE_LEN_MAX; index++)
    {
      softSerial->write(par[index]);
    }
    softSerial->flush();
  }
  else
  {
    for (index = 0; index < UART_RECIVE_LEN_MAX; index++)
    {
      _Serial->write(par[index]);
    }
    _Serial->flush();
  }
  digitalWrite(_staPin, LOW);
}
/**********************************************************
  Description:  Receive MODBUS RTU Protocol response frame
  Parameters:   len: receive buffer lengh
  Return:       true or false
  Others:       None
**********************************************************/
bool BM62S6021_1::rev_buf(uint8_t len)
{
  uint8_t rev_len;
  uint8_t rev_crc_High, rev_crc_Low;

  rev_len = len;

  digitalWrite(_staPin, LOW);

  if (softSerial != NULL)
  {
    softSerial->readBytes(_rx_buf, rev_len);
  }
  else
  {
    _Serial->readBytes(_rx_buf, rev_len);
  }

  Modbus_CRCcaculate(_rx_buf, rev_len - 2, &rev_crc_High, &rev_crc_Low);

  if ((_rx_buf[rev_len - 1] == rev_crc_High) && (_rx_buf[rev_len - 2] == rev_crc_Low))
  {
    return true;
  }
  else
  {
    return false;
  }
}
/**********************************************************
  Description:  MODBUS checksum calculation
  Parameters:  *ptr: array first address
                len:num of array members
                crc_H: check code 16-bit high byte address
                crc_L: check code 16-bit low byte address
  Return:       void
  Others:       None
**********************************************************/
void BM62S6021_1::Modbus_CRCcaculate(uint8_t *ptr, int len, uint8_t *crc_H, uint8_t *crc_L)
{
  unsigned char i;
  unsigned short crc = 0xFFFF;

  while (len--)
  {
    crc ^= *ptr++;
    for (i = 0; i < 8; ++i)
    {
      if (crc & 1)
        crc = (crc >> 1) ^ 0xA001;
      else
        crc = (crc >> 1);
    }
  }
  *crc_L = crc & 0x00ff;
  *crc_H = (crc >> 8) & 0x00ff;
}
