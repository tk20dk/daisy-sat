#ifndef ROSALYN_SAT_H__
#define ROSALYN_SAT_H__

#include <sbus.h>
#include "nvdata.h"
#include "system.h"
#include "sx1268.h"
#include "sbus.h"


class TRosalynSat
{
public:
  TRosalynSat();

  void Loop();
  void Setup();
  void SysTick_Handler();
  void USART_IRQHandler();
  void EXTI0_1_IRQHandler();

private:
  void HmiLoop();
  void HmiError( uint32_t const Interval = 0 );
  void HmiStatus( uint32_t const Interval = 0 );
  void RadioEvent( TRadioEvent const Event );

private:
  bool TimerFlag;
  bool RadioFlag;
  bool SerialFlag;
  TSx1268 Radio;
  TNvData NvData;
  uint32_t TimeoutHmiError;
  uint32_t TimeoutHmiStatus;
  TSbusSerial SbusSerial;
  TSbusData SbusDataUpstream;
  TSbusData SbusDataDownstream;
};

#endif // ROSALYN_SAT_H__
