#ifndef SBUS_SERIAL_H__
#define SBUS_SERIAL_H__

#include "main.h"


class TSbusSerial
{
  static uint8_t const SbusSOF = 0x0f;
  static uint8_t const SbusEOF = 0x00;
  static uint32_t const RxFifoSize = 64;
  static uint32_t const TxFifoSize = 64;
  static uint32_t const SbusFrameGap = 3;
  static uint32_t const SbusFrameSOF = 0;
  static uint32_t const SbusFrameEOF = 24;
  static uint32_t const SbusFrameSize = 25;

public:
  TSbusSerial( UART_HandleTypeDef  &USARTx, bool &SerialFlag );
  void Setup();

  uint8_t Receive();
  void Receive( uint8_t *const Data, uint32_t const MaxLen );

  void Transmit( uint8_t const Data );
  void Transmit( uint8_t const *const Data, uint32_t const Length );

  void HAL_UART_ErrorCallback();
  void HAL_UART_RxCpltCallback();
  void HAL_UART_TxCpltCallback();

private:
  bool volatile FailSafe;
  bool &SerialFlag;
  uint8_t RxData[ RxFifoSize ];
  uint8_t TxData[ TxFifoSize ];
  uint32_t volatile RxLen;
  uint32_t volatile TxLen;
  uint32_t volatile RxTail;
  uint32_t volatile TxTail;
  uint32_t volatile RxHead;
  uint32_t volatile TxHead;
  UART_HandleTypeDef &USARTx;
};

#endif // SBUS_SERIAL_H__
