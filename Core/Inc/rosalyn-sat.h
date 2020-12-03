#ifndef ROSALYN_SAT_H__
#define ROSALYN_SAT_H__

#include "sbus.h"
#include "nvdata.h"
#include "system.h"
#include "sx1268.h"
#include "aes-crypto.h"


class TRosalynSat
{
  friend void RosalynSatLoop0();
  friend void RosalynSatSetup0();
  friend void SysTick_Handler0();
  friend void USART1_IRQHandler0();
  friend void EXTI0_1_IRQHandler0();

public:
  TRosalynSat();

private:
  void Loop();
  void Setup();
  void HmiLoop();
  void HmiError( uint32_t const Interval = 0 );
  void HmiStatus( uint32_t const Interval = 0 );
  void RadioEvent( TRadioEvent const Event );
  void SysTick_Handler();
  void USART_IRQHandler();
  void EXTI0_1_IRQHandler();

private:
  TNvData NvData;
  bool TimerFlag;
  bool RadioFlag;
  bool SerialFlag;
  TSx1268 Radio;
  uint32_t TimeoutHmiError;
  uint32_t TimeoutHmiStatus;
  TSbusData SbusDataUpstream;
  TSbusData SbusDataDownstream;
  TAesCrypto AesCrypto;
  TSbusSerial SbusSerial;
};

#endif // ROSALYN_SAT_H__
