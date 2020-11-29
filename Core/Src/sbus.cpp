#include "sbus.h"


struct TSbusFrame
{
  static uint8_t const SOF           = 0x0f;
  static uint8_t const EOF           = 0x00;
  static uint8_t const FlagNull      = 0x00;
  static uint8_t const FlagCh17      = 0x01;
  static uint8_t const FlagCh18      = 0x02;
  static uint8_t const FlagLostFrame = 0x04;
  static uint8_t const FlagFailSafe  = 0x08;

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

TSbus::TSbus() :
  Ch17( false ),
  Ch18( false ),
  FailSafe( false ),
  LostFrame( false ),
  Ch1( 1024 ),
  Ch2( 1024 ),
  Ch3( 1024 ),
  Ch4( 1024 ),
  Ch5( 1024 ),
  Ch6( 1024 ),
  Ch7( 1024 ),
  Ch8( 1024 ),
  Ch9( 1024 ),
  Ch10( 1024 ),
  Ch11( 1024 ),
  Ch12( 1024 ),
  Ch13( 1024 ),
  Ch14( 1024 ),
  Ch15( 1024 ),
  Ch16( 1024 )
{
}

TSbus::TSbus( uint8_t const* const Buffer, uint32_t const Length )
{
  Decode( Buffer, Length );
}

bool TSbus::Encode( uint8_t* const Buffer, uint32_t const MaxLen )
{
  if( MaxLen < sizeof( TSbusFrame ))
  {
    return false;
  }

  auto &SbusFrame = *reinterpret_cast< TSbusFrame* >( Buffer );
  SbusFrame.Ch1 = Ch1;
  SbusFrame.Ch2 = Ch2;
  SbusFrame.Ch3 = Ch3;
  SbusFrame.Ch4 = Ch4;
  SbusFrame.Ch5 = Ch5;
  SbusFrame.Ch6 = Ch6;
  SbusFrame.Ch7 = Ch7;
  SbusFrame.Ch8 = Ch8;
  SbusFrame.Ch9 = Ch9;
  SbusFrame.Ch10 = Ch10;
  SbusFrame.Ch11 = Ch11;
  SbusFrame.Ch12 = Ch12;
  SbusFrame.Ch13 = Ch13;
  SbusFrame.Ch14 = Ch14;
  SbusFrame.Ch15 = Ch15;
  SbusFrame.Ch16 = Ch16;
  SbusFrame.Sof = SbusSOF;
  SbusFrame.Eof = SbusEOF;
  SbusFrame.Flag = 0;
  SbusFrame.Flag |= Ch17 ? SbusFlagCh17 : SbusFlagNull;
  SbusFrame.Flag |= Ch18 ? SbusFlagCh18 : SbusFlagNull;
  SbusFrame.Flag |= FailSafe ? SbusFlagFailSafe : SbusFlagNull;
  SbusFrame.Flag |= LostFrame ? SbusFlagLostFrame : SbusFlagNull;

  return true;
}

bool TSbus::Decode( uint8_t const* const Buffer, uint32_t const Length )
{
  if( Length != sizeof( TSbusFrame ))
  {
    return false;
  }

  auto const &SbusFrame = *reinterpret_cast< TSbusFrame const* >( Buffer );
  if(( SbusFrame.Sof != SbusSOF ) || ( SbusFrame.Eof != SbusEOF ))
  {
    return false;
  }

  Ch1 = SbusFrame.Ch1;
  Ch2 = SbusFrame.Ch2;
  Ch3 = SbusFrame.Ch3;
  Ch4 = SbusFrame.Ch4;
  Ch5 = SbusFrame.Ch5;
  Ch6 = SbusFrame.Ch6;
  Ch7 = SbusFrame.Ch7;
  Ch8 = SbusFrame.Ch8;
  Ch9 = SbusFrame.Ch9;
  Ch10 = SbusFrame.Ch10;
  Ch11 = SbusFrame.Ch11;
  Ch12 = SbusFrame.Ch12;
  Ch13 = SbusFrame.Ch13;
  Ch14 = SbusFrame.Ch14;
  Ch15 = SbusFrame.Ch15;
  Ch16 = SbusFrame.Ch16;
  Ch17 = ( SbusFrame.Flag & SbusFlagCh17 ) == SbusFlagCh17;
  Ch18 = ( SbusFrame.Flag & SbusFlagCh18 ) == SbusFlagCh18;
  FailSafe = ( SbusFrame.Flag & SbusFlagFailSafe ) == SbusFlagFailSafe;
  LostFrame = ( SbusFrame.Flag & SbusFlagLostFrame ) == SbusFlagLostFrame;

  return true;
}

void TSbus::DecodeLoRa( int32_t const Snr, uint32_t const Rssi, uint8_t const* const Buffer, uint32_t const Length )
{
  Decode( Buffer, Length );
}

uint32_t TSbus::EncodeLoRa( uint8_t *const Buffer, uint32_t const MaxLen )
{
  Encode( Buffer, SbusFrameSize );
  return SbusFrameSize;
}

void TSbus::EncodeSerial( TSbusSerial &SbusSerial )
{
  uint8_t Buffer[ SbusFrameSize ];
  Encode( Buffer, SbusFrameSize );
  SbusSerial.Transmit( Buffer, SbusFrameSize );
}
void TSbus::DecodeSerial( TSbusSerial &SbusSerial )
{
  uint8_t Buffer[ SbusFrameSize ];
  SbusSerial.Receive( Buffer, SbusFrameSize );
  Decode( Buffer, SbusFrameSize );
}
