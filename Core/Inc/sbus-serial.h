#ifndef SBUS_SERIAL_H__
#define SBUS_SERIAL_H__

#include "main.h"
#include <climits>


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
  TSbusSerial( USART_TypeDef *const USARTx, bool &SerialFlag );
  void Setup();

  uint8_t Receive();
  void Receive( uint8_t *const Data, uint32_t const MaxLen );

  void Transmit( uint8_t const Data );
  void Transmit( uint8_t const *const Data, uint32_t const Length );

  void TxEmpty();
  void RxNotEmpty();
  void RxError();
  void RxTimeout();
  void USART_IRQHandler();

private:
  bool volatile FailSafe;
  bool &SerialFlag;
  uint8_t RxData[ RxFifoSize ];
  uint8_t TxData[ TxFifoSize ];
  uint8_t CountPE;
  uint8_t CountFE;
  uint8_t CountNE;
  uint8_t CountORE;
  uint8_t CountRxFUE;
  uint8_t CountRxFOE;
  uint8_t CountTxFOE;
  uint32_t RxLen;
  uint32_t TxLen;
  uint32_t RxTail;
  uint32_t TxTail;
  uint32_t RxHead;
  uint32_t TxHead;
  USART_TypeDef *const USARTx;
};

#endif // SBUS_SERIAL_H__
