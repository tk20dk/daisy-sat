#include "sbus-serial.h"


void TSbusSerial::Receive( uint8_t *const Data, uint32_t const MaxLen )
{
  for( auto Index = 0u; Index < MaxLen; Index++ )
  {
    Data[ Index ] = Receive();
  }
}

void TSbusSerial::Transmit( uint8_t const* const Data, uint32_t const Length )
{
  for( auto Index = 0u; Index < Length; Index++ )
  {
    Transmit( Data[ Index ] );
  }
}

uint8_t TSbusSerial::Receive()
{
  if( RxLen == 0 )
  {
    if( CountRxFUE < UCHAR_MAX )
    {
      CountRxFUE++;
    }
    return 0;
  }

  auto const Data = RxData[ RxTail ];
  if( ++RxTail == RxFifoSize )
  {
    RxTail = 0;
  }

  LL_USART_DisableIT_RXNE( USARTx );
  RxLen--;
  LL_USART_EnableIT_RXNE( USARTx );

  return Data;
}

void TSbusSerial::Transmit( uint8_t const Data )
{
  if( TxLen >= TxFifoSize )
  {
    if( CountTxFOE < UCHAR_MAX )
    {
      CountTxFOE++;
    }
    return;
  }

  TxData[ TxHead ] = Data;
  if( ++TxHead == TxFifoSize )
  {
    TxHead = 0;
  }

  LL_USART_DisableIT_TXE( USARTx );
  TxLen++;
  LL_USART_EnableIT_TXE( USARTx );
}

void TSbusSerial::TxEmpty()
{
  if( TxLen == 0 )
  {
	LL_USART_DisableIT_TXE( USARTx );
  }
  else
  {
    LL_USART_TransmitData8( USARTx, TxData[ TxTail ] );
    if( ++TxTail == TxFifoSize )
    {
      TxTail = 0;
    }
    TxLen--;
  }
}

void TSbusSerial::RxNotEmpty()
{
  auto const Data = LL_USART_ReceiveData8( USARTx );

  if( RxLen >= RxFifoSize )
  {
    RxLen = 0;
    RxHead = 0;
    RxTail = 0;
    FlagRxTimeout = false;

    if( CountRxFOE < UCHAR_MAX )
    {
      CountRxFOE++;
    }
    return;
  }

  if( RxLen == SbusFrameSOF )
  {
    if(( Data == SbusSOF ) && ( FlagRxTimeout == true ))
    {
      FlagRxTimeout = false;
    }
    else
    {
      return;
    }
  }

  if( RxLen == SbusFrameEOF)
  {
    if(( Data != SbusEOF ) || ( FlagRxTimeout == true ))
    {
      RxLen = 0;
      RxHead = 0;
      RxTail = 0;
      FlagRxTimeout = false;
      return;
    }
  }

  RxData[ RxHead ] = Data;
  if( ++RxHead == RxFifoSize )
  {
    RxHead = 0;
  }
  RxLen++;

  if( RxLen >= SbusFrameSize )
  {
    SerialFlag = true;
    FlagRxTimeout = false;
  }
}

void TSbusSerial::RxError()
{
  RxLen = 0;
  RxHead = 0;
  RxTail = 0;
  FlagRxTimeout = false;
}

void TSbusSerial::RxTimeout()
{
  FlagRxTimeout = true;
}

void TSbusSerial::USART_IRQHandler()
{
  if( LL_USART_IsActiveFlag_RXNE( USARTx ) && LL_USART_IsEnabledIT_RXNE( USARTx ))
  {
    RxNotEmpty();
  }

  if(LL_USART_IsEnabledIT_TXE( USARTx ) && LL_USART_IsActiveFlag_TXE( USARTx ))
  {
    TxEmpty();
  }

  if( LL_USART_IsActiveFlag_PE( USARTx ))
  {
    LL_USART_ClearFlag_PE( USARTx );
    RxError();

    if( CountPE < UCHAR_MAX )
    {
      CountPE++;
    }
  }

  if( LL_USART_IsActiveFlag_FE( USARTx ))
  {
    LL_USART_ClearFlag_FE( USARTx );
    RxError();

    if( CountFE < UCHAR_MAX )
    {
      CountFE++;
    }
  }

  if( LL_USART_IsActiveFlag_NE( USARTx ))
  {
    LL_USART_ClearFlag_NE( USARTx );
    RxError();

    if( CountNE < UCHAR_MAX )
    {
      CountNE++;
    }
  }

  if( LL_USART_IsActiveFlag_ORE( USARTx ))
  {
    LL_USART_ClearFlag_ORE( USARTx );
    RxError();

    if( CountORE < UCHAR_MAX )
    {
      CountORE++;
    }
  }

  if( LL_USART_IsActiveFlag_RTO( USARTx ))
  {
    LL_USART_ClearFlag_RTO( USARTx );
    RxTimeout();
  }
}

void TSbusSerial::Setup()
{
  LL_USART_SetRxTimeout( USARTx, 400 ); // 4ms at 100000 Bit/s
  LL_USART_EnableRxTimeout( USARTx );
  LL_USART_EnableIT_RTO( USARTx );

  LL_USART_EnableIT_PE( USARTx );
  LL_USART_EnableIT_RXNE( USARTx );
  LL_USART_EnableIT_ERROR( USARTx );
}

TSbusSerial::TSbusSerial( USART_TypeDef *const USARTx, bool &SerialFlag ) :
  FailSafe( false ),
  SerialFlag( SerialFlag ),
  FlagRxTimeout( false ),
  RxData(),
  TxData(),
  CountPE( 0 ),
  CountFE( 0 ),
  CountNE( 0 ),
  CountORE( 0 ),
  CountRxFUE( 0 ),
  CountRxFOE( 0 ),
  CountTxFOE( 0 ),
  RxLen( 0 ),
  TxLen( 0 ),
  RxTail( 0 ),
  TxTail( 0 ),
  RxHead( 0 ),
  TxHead( 0 ),
  USARTx( USARTx )
{
}
