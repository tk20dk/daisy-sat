#include "rosalyn-sat.h"


TDriverSpi Spi;
TRosalynSat RosalynSat;

uint32_t volatile TickSys;
extern "C" uint32_t HAL_GetTick()
{
  return TickSys;
}

void TRosalynSat::Setup()
{
//  AesCrypto.TestCFB();

  // Enable SysTick IRQ
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

  Spi.Setup();
  SbusSerial.Setup();

  if( Radio.Setup( NvData.Modulation[ 2 ], NvData.TxPower, NvData.Channel ))
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
    SbusSerial.Transmit( SbusDataDownstream );
  }

  if( RadioFlag )
  {
    RadioFlag = false;
    Radio.Interrupt();
  }

  if( SerialFlag )
  {
    SerialFlag = false;
    SbusDataUpstream = SbusSerial.Receive();
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

    HmiStatus( 10 );
    UartPrintf( "Rssi:%4d Snr:%3d.%u Len:%u Length\n", Rssi, Snr / 10, abs(Snr) % 10, LenRx );

    if( LenRx == 25 )
    {
      int32_t LenOut;
      TSbusFrame SbusFrameRx;

      AesCrypto.DecryptCFB( Buffer, LenRx, SbusFrameRx.Buffer, LenOut );
      TSbusData SbusData( SbusFrameRx );

      auto const SbusFrameTx = SbusData.Encode();
      AesCrypto.EncryptCFB( SbusFrameTx.Buffer, LenRx, Buffer, LenOut );

      Radio.Transmit( Buffer, LenRx );
    }
    else
    {
      Radio.Receive();
    }
  }

  if( Event == TRadioEvent::TxDone )
  {
    HmiStatus( 10 );
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

    Radio.Receive();
  }

  if( Event == TRadioEvent::NoCrc )
  {
    auto const Snr = Radio.GetSnr();
    auto const Rssi = Radio.GetRssi();
    auto const LenRx = Radio.ReadPacket( Buffer, sizeof( Buffer ));

    HmiError( 1000 );
    UartPrintf( "Rssi:%4d Snr:%3d.%u Len:%u No CRC\n", Rssi, Snr / 10, abs(Snr) % 10, LenRx );

    Radio.Receive();
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

void TRosalynSat::SysTick_Handler()
{
  TickSys++;
}

void TRosalynSat::USART_IRQHandler()
{
  SbusSerial.USART_IRQHandler();
}

void TRosalynSat::EXTI0_1_IRQHandler()
{
  // Radio DIO1 interrupt
  if( LL_EXTI_IsActiveFlag_0_31( LL_EXTI_LINE_0 ) != RESET )
  {
    LL_EXTI_ClearFlag_0_31( LL_EXTI_LINE_0 );
    RadioFlag = true;
  }
}

TRosalynSat::TRosalynSat() :
  NvData(),
  TimerFlag( false ),
  RadioFlag( false ),
  SerialFlag( false ),
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
  TimeoutHmiError( 0 ),
  TimeoutHmiStatus( 0 ),
  SbusDataUpstream(),
  SbusDataDownstream(),
  AesCrypto( NvData.AesIV, NvData.AesKey ),
  SbusSerial( USART1, SerialFlag )
{
}

inline void RosalynSatLoop0()
{
  RosalynSat.Loop();
}
extern "C" void RosalynSatLoop()
{
  RosalynSatLoop0();
}

inline void RosalynSatSetup0()
{
  RosalynSat.Setup();
}
extern "C" void RosalynSatSetup()
{
  RosalynSatSetup0();
}

inline void SysTick_Handler0()
{
  RosalynSat.SysTick_Handler();
}
extern "C" void SysTick_Handler()
{
  SysTick_Handler0();
}

inline void USART1_IRQHandler0()
{
  RosalynSat.USART_IRQHandler();
}
extern "C" void USART1_IRQHandler()
{
  USART1_IRQHandler0();
}

inline void EXTI0_1_IRQHandler0()
{
  RosalynSat.EXTI0_1_IRQHandler();
}
extern "C" void EXTI0_1_IRQHandler()
{
  EXTI0_1_IRQHandler0();
}
