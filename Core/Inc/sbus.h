#ifndef SBUS_H__
#define SBUS_H__

#include "sbus-serial.h"


struct TSbus
{
  static uint8_t const SbusSOF           = 0x0f;
  static uint8_t const SbusEOF           = 0x00;
  static uint8_t const SbusFlagNull      = 0x00;
  static uint8_t const SbusFlagCh17      = 0x01;
  static uint8_t const SbusFlagCh18      = 0x02;
  static uint8_t const SbusFlagLostFrame = 0x04;
  static uint8_t const SbusFlagFailSafe  = 0x08;
  static uint8_t const SbusFrameSize     = 25;

  TSbus();
  TSbus( uint8_t const* const Buffer, uint32_t const Length );

  bool Encode( uint8_t* const, uint32_t const MaxLen );
  bool Decode( uint8_t const* const Buffer, uint32_t const Length );

  void DecodeLoRa( int32_t const Snr, uint32_t const Rssi, uint8_t const* const Buffer, uint32_t const Length );
  uint32_t EncodeLoRa( uint8_t *const Buffer, uint32_t const MaxLen );
  void EncodeSerial( TSbusSerial &SbusSerial );
  void DecodeSerial( TSbusSerial &SbusSerial );

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

#endif // SBUS_H__
