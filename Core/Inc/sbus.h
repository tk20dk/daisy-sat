#ifndef SBUS_H__
#define SBUS_H__

#include <limits>
#include "main.h"


class TStatCount
{
public:
  TStatCount() :
    Count( 0 )
  {
  }

  TStatCount& operator++()
  {
    if( Count < std::numeric_limits< uint8_t >::max() )
    {
      ++Count;
    }
    return *this;
  }

  TStatCount operator++( int )
  {
    TStatCount const Tmp( *this );
    operator++();
    return Tmp;
  }

  TStatCount& operator=( uint8_t const Rhs )
  {
    Count = Rhs;
    return *this;
  }

  operator uint8_t() const
  {
    return Count;
  }

public:
  uint8_t Count;
};

union TSbusFrame
{
  static uint32_t const SbusFrameSize = 25;
  uint8_t Buffer[ SbusFrameSize ];
  struct
  {
    uint8_t Sof;
    uint32_t Ch1:11;
    uint32_t Ch2:11;
    uint32_t Ch3:11;
    uint32_t Ch4:11;
    uint32_t Ch5:11;
    uint32_t Ch6:11;
    uint32_t Ch7:11;
    uint32_t Ch8:11;
    uint32_t Ch9:11;
    uint32_t Ch10:11;
    uint32_t Ch11:11;
    uint32_t Ch12:11;
    uint32_t Ch13:11;
    uint32_t Ch14:11;
    uint32_t Ch15:11;
    uint32_t Ch16:11;
    uint8_t Flag;
    uint8_t Eof;
  } __attribute__((__packed__));
};

struct TSbusData
{
  static uint32_t const SbusSOF           = 0x0f;
  static uint32_t const SbusEOF           = 0x00;
  static uint32_t const SbusFlagNull      = 0x00;
  static uint32_t const SbusFlagCh17      = 0x01;
  static uint32_t const SbusFlagCh18      = 0x02;
  static uint32_t const SbusFlagLostFrame = 0x04;
  static uint32_t const SbusFlagFailSafe  = 0x08;

  void Encode( uint8_t *const Buffer ) const
  {
    Encode( *reinterpret_cast< TSbusFrame* >( Buffer ));
  }

  void Decode( uint8_t const* const Buffer )
  {
    Decode( *reinterpret_cast< TSbusFrame const* >( Buffer ));
  }

  void Encode( TSbusFrame &Data ) const
  {
    Data.Sof = SbusSOF;
	Data.Eof = SbusEOF;

	Data.Ch1 = Ch1;
	Data.Ch2 = Ch2;
	Data.Ch3 = Ch3;
	Data.Ch4 = Ch4;
	Data.Ch5 = Ch5;
	Data.Ch6 = Ch6;
	Data.Ch7 = Ch7;
	Data.Ch8 = Ch8;
	Data.Ch9 = Ch9;
	Data.Ch10 = Ch10;
	Data.Ch11 = Ch11;
	Data.Ch12 = Ch12;
	Data.Ch13 = Ch13;
	Data.Ch14 = Ch14;
	Data.Ch15 = Ch15;
	Data.Ch16 = Ch16;

	Data.Flag = SbusFlagNull;
	Data.Flag |= Ch17 ? SbusFlagCh17 : SbusFlagNull;
	Data.Flag |= Ch18 ? SbusFlagCh18 : SbusFlagNull;
	Data.Flag |= FailSafe ? SbusFlagLostFrame : SbusFlagNull;
	Data.Flag |= LostFrame ? SbusFlagFailSafe : SbusFlagNull;
  }

  void Decode( TSbusFrame const &Data )
  {
    Ch1 = Data.Ch1;
	Ch2 = Data.Ch2;
	Ch3 = Data.Ch3;
	Ch4 = Data.Ch4;
	Ch5 = Data.Ch5;
	Ch6 = Data.Ch6;
	Ch7 = Data.Ch7;
	Ch8 = Data.Ch8;
	Ch9 = Data.Ch9;
	Ch10 = Data.Ch10;
	Ch11 = Data.Ch11;
	Ch12 = Data.Ch12;
	Ch13 = Data.Ch13;
	Ch14 = Data.Ch14;
	Ch15 = Data.Ch15;
	Ch16 = Data.Ch16;
	Ch17 = ( Data.Flag & SbusFlagCh17 ) == SbusFlagCh17;
	Ch18 = ( Data.Flag & SbusFlagCh18 ) == SbusFlagCh18;
	FailSafe = ( Data.Flag & SbusFlagLostFrame ) == SbusFlagLostFrame;
	LostFrame = ( Data.Flag & SbusFlagFailSafe ) == SbusFlagFailSafe;
  }

  bool Ch17;
  bool Ch18;
  bool FailSafe;
  bool LostFrame;
  uint16_t Ch1;
  uint16_t Ch2;
  uint16_t Ch3;
  uint16_t Ch4;
  uint16_t Ch5;
  uint16_t Ch6;
  uint16_t Ch7;
  uint16_t Ch8;
  uint16_t Ch9;
  uint16_t Ch10;
  uint16_t Ch11;
  uint16_t Ch12;
  uint16_t Ch13;
  uint16_t Ch14;
  uint16_t Ch15;
  uint16_t Ch16;
};

class TSbusSerial
{
  static uint32_t const SbusSOF       = 0x0f;
  static uint32_t const SbusEOF       = 0x00;
  static uint32_t const SbusStateNull = 0;
  static uint32_t const SbusStateSOF  = 1;
  static uint32_t const SbusStateEOF  = 25;
  static uint32_t const SbusFrameSize = 25;

public:
  TSbusSerial( USART_TypeDef *const USARTx, bool &SerialFlag );

  void Setup();
  void Receive( TSbusData &Data ) const;
  void Transmit( TSbusData const &Data );

  void RxError();
  void TxEmpty();
  void RxNotEmpty();
  void USART_IRQHandler();

private:
  bool RxTimeout;
  bool &SerialFlag;
  uint32_t RxState;
  uint32_t TxState;
  TSbusFrame RxFrame;
  TSbusFrame TxFrame;
  TStatCount StatPE;
  TStatCount StatFE;
  TStatCount StatNE;
  TStatCount StatORE;
  USART_TypeDef *const USARTx;
};

#endif // SBUS_H__
