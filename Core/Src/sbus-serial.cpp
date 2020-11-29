#include "sbus-serial.h"


uint8_t TSbusSerial::Receive()
{
  if( RxLen == 0 )
  {
    while( true )
    {
    }
    return 0;
  }

  auto const Data = RxData[ RxTail ];
  if( ++RxTail == RxFifoSize )
  {
    RxTail = 0;
  }

  // Disable IRQ
  RxLen--;

  return Data;
}

void TSbusSerial::Receive( uint8_t *const Data, uint32_t const MaxLen )
{
  for( auto Index = 0u; Index < MaxLen; Index++ )
  {
    Data[ Index ] = Receive();
  }
}

void TSbusSerial::Transmit( uint8_t const Data )
{
  if( TxLen >= TxFifoSize )
  {
    while( true )
    {
    }
  }

  TxData[ TxHead ] = Data;
  if( ++TxHead == TxFifoSize )
  {
    TxHead = 0;
  }

  // Disable IRQ
  if( ++TxLen == 1 )
  {
    HAL_UART_Transmit_IT( &USARTx, &TxData[ TxTail ], 1 );
  }
}

void TSbusSerial::Transmit( uint8_t const* const Data, uint32_t const Length )
{
  for( auto Index = 0u; Index < Length; Index++ )
  {
    Transmit( Data[ Index ] );
  }
}

void TSbusSerial::HAL_UART_ErrorCallback()
{
  if( USARTx.ErrorCode & HAL_UART_ERROR_PE )
  {
    RxLen = 0;
  }
  if( USARTx.ErrorCode & HAL_UART_ERROR_NE )
  {
    RxLen = 0;
  }
  if( USARTx.ErrorCode & HAL_UART_ERROR_FE )
  {
    RxLen = 0;
  }
  if( USARTx.ErrorCode & HAL_UART_ERROR_ORE )
  {
    RxLen = 0;
  }
  if( USARTx.ErrorCode & HAL_UART_ERROR_RTO )
  {
    RxLen = 0;
  }

//  HAL_UART_Receive_IT( &USARTx, &RxData[ RxHead ], 1 );
}

void TSbusSerial::HAL_UART_RxCpltCallback()
{
  auto const Data = RxData[ RxHead ];

  if(( RxLen == SbusFrameSOF ) && ( Data != SbusSOF ))
  {
    HAL_UART_Receive_IT( &USARTx, &RxData[ RxHead ], 1 );
    return;
  }

  if(( RxLen == SbusFrameEOF ) && ( Data != SbusEOF ))
  {
    RxLen = 0;
    RxHead = 0;
    RxTail = 0;
    HAL_UART_Receive_IT( &USARTx, &RxData[ RxHead ], 1 );
    return;
  }

  if( RxLen >= RxFifoSize )
  {
    while( true )
    {
    }
  }

  RxData[ RxHead ] = Data;
  if( ++RxHead == RxFifoSize )
  {
    RxHead = 0;
  }
  RxLen++;

  HAL_UART_Receive_IT( &USARTx, &RxData[ RxHead ], 1 );

  if( RxLen == SbusFrameSize )
  {
    FailSafe = false;
    SerialFlag = true;
  }
}

void TSbusSerial::HAL_UART_TxCpltCallback()
{
  if( ++TxTail == TxFifoSize )
  {
    TxTail = 0;
  }
  --TxLen;

  if( TxLen )
  {
    HAL_UART_Transmit_IT( &USARTx, &TxData[ TxTail ], 1 );
  }
}


TSbusSerial::TSbusSerial( UART_HandleTypeDef &USARTx, bool &SerialFlag ) :
  FailSafe( false ),
  SerialFlag( SerialFlag ),
  RxData(),
  TxData(),
  RxLen( 0 ),
  TxLen( 0 ),
  RxTail( 0 ),
  TxTail( 0 ),
  RxHead( 0 ),
  TxHead( 0 ),
  USARTx( USARTx )
{
}

void TSbusSerial::Setup()
{
//  HAL_UART_ReceiverTimeout_Config( &USARTx, 400 );
//  HAL_UART_EnableReceiverTimeout( &USARTx );

  HAL_UART_Receive_IT( &USARTx, &RxData[ RxHead ], 1 );
}
