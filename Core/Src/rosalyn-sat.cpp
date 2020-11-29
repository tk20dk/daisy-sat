#include <rosalyn-sat.h>


TSpi Spi( hspi1 );
TRosalynSat RosalynSat;
TSystem System;

void TRosalynSat::Setup()
{
  SbusSerial.Setup();

  if( Radio.Init( System.Config.Modulation[ 2 ], System.Config.TxPower, System.Config.Channel ))
  {
    Radio.Receive();
  }
}

void TRosalynSat::Loop()
{
  HmiLoop();

  static uint32_t LastTick;
  auto const Tick = HAL_GetTick();
  if(( Tick > LastTick ) && ( Tick % 14 ) == 0 )
  {
    LastTick = Tick;
    Sbus.EncodeSerial( SbusSerial );
  }

  if( RadioFlag )
  {
    RadioFlag = false;
    Radio.Interrupt();
  }

  if( SerialFlag )
  {
    SerialFlag = false;
    Sbus.DecodeSerial( SbusSerial );
    HmiError( 5 );
  }
}

void TRosalynSat::RadioEvent( TRadioEvent const Event )
{
  uint8_t Buffer[ 256 ];

  if( Event == TRadioEvent::RxDone )
  {
    auto const Snr = Radio.GetSnr();
    auto const Rssi = Radio.GetRssi();
    auto const LenRx = Radio.ReadPacket( Buffer, sizeof( Buffer ));

    HmiStatus( 100 );
    UartPrintf( "Rssi:%4d Snr:%3d.%u Len:%u Length\n", Rssi, Snr / 10, abs(Snr) % 10, LenRx );

    Sbus.DecodeLoRa( Snr, Rssi, Buffer, LenRx );

    auto const LenTx = Sbus.EncodeLoRa( Buffer, sizeof( Buffer ));
    Radio.Transmit( Buffer, LenTx );
  }

  if( Event == TRadioEvent::TxDone )
  {
    HmiStatus( 100 );
    Radio.Receive();
  }

  if( Event == TRadioEvent::Timeout )
  {
    HmiError( 1000 );
  }

  if( Event == TRadioEvent::CrcError )
  {
    auto const Snr = Radio.GetSnr();
    auto const Rssi = Radio.GetRssi();
    auto const LenRx = Radio.ReadPacket( Buffer, sizeof( Buffer ));

    HmiError( 1000 );
    UartPrintf( "Rssi:%4d Snr:%3d.%u Len:%u CRC Error\n", Rssi, Snr / 10, abs(Snr) % 10, LenRx );
  }

  if( Event == TRadioEvent::NoCrc )
  {
    auto const Snr = Radio.GetSnr();
    auto const Rssi = Radio.GetRssi();
    auto const LenRx = Radio.ReadPacket( Buffer, sizeof( Buffer ));

    HmiError( 1000 );
    UartPrintf( "Rssi:%4d Snr:%3d.%u Len:%u No CRC\n", Rssi, Snr / 10, abs(Snr) % 10, LenRx );
  }
}

void TRosalynSat::HAL_GPIO_EXTI_Callback( uint16_t const GPIO_Pin )
{
  switch( GPIO_Pin )
  {
    case RADIO_DIO1_Pin:
    {
      RadioFlag = true;
    }
    break;

    default:
    {
      HmiError();
    }
    break;
  }
}

void TRosalynSat::HmiLoop()
{
  if( TimeoutHmiError && ( HAL_GetTick() >= TimeoutHmiError ))
  {
    TimeoutHmiError = 0;
    ResetPin( HMI_ERROR_GPIO_Port, HMI_ERROR_Pin );
  }

  if( TimeoutHmiStatus && ( HAL_GetTick() >= TimeoutHmiStatus ))
  {
    TimeoutHmiStatus = 0;
    ResetPin( HMI_STATUS_GPIO_Port, HMI_STATUS_Pin );
  }
}

void TRosalynSat::HmiError( uint32_t const Interval )
{
  TimeoutHmiError = HAL_GetTick() + Interval;
  SetPin( HMI_ERROR_GPIO_Port, HMI_ERROR_Pin );
}

void TRosalynSat::HmiStatus( uint32_t const Interval )
{
  TimeoutHmiStatus = HAL_GetTick() + Interval;
  SetPin( HMI_STATUS_GPIO_Port, HMI_STATUS_Pin );
}

void TRosalynSat::USART_IRQHandler()
{
  SbusSerial.USART_IRQHandler();
}

TRosalynSat::TRosalynSat() :
  TimerFlag( false ),
  RadioFlag( false ),
  SerialFlag( false ),
  Sbus(),
  Radio(
    433050000u,
	RADIO_NSS_GPIO_Port,
	RADIO_NSS_Pin,
	RADIO_NRST_GPIO_Port,
	RADIO_NRST_Pin,
	RADIO_BUSY_GPIO_Port,
	RADIO_BUSY_Pin,
	RADIO_RXEN_GPIO_Port,
	RADIO_RXEN_Pin,
	RADIO_TXEN_GPIO_Port,
	RADIO_TXEN_Pin,
    std::bind( &TRosalynSat::RadioEvent, this, std::placeholders::_1 )),
  NvData(),
  TimeoutHmiError( 0 ),
  TimeoutHmiStatus( 0 ),
  SbusSerial( USART1, SerialFlag )
{
}

extern "C" void RosalynSatLoop()
{
  RosalynSat.Loop();
}

extern "C" void RosalynSatSetup()
{
  RosalynSat.Setup();
}

extern "C" void USART1_IRQHandler()
{
  RosalynSat.USART_IRQHandler();
}

extern "C" void HAL_GPIO_EXTI_Callback( uint16_t const GPIO_Pin )
{
  RosalynSat.HAL_GPIO_EXTI_Callback( GPIO_Pin );
}
