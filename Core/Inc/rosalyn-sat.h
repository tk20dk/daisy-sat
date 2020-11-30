#ifndef ROSALYN_SAT_H__
#define ROSALYN_SAT_H__

#include "nvdata.h"
#include "system.h"
#include "sx1268.h"
#include "sbus.h"
#include "sbus-serial.h"


class TRosalynSat
{
public:
  TRosalynSat();

  void Loop();
  void Setup();
  void SysTick_Handler();
  void USART_IRQHandler();
  void HAL_GPIO_EXTI_Callback( uint16_t const GPIO_Pin );

private:
  void HmiLoop();
  void HmiError( uint32_t const Interval = 0 );
  void HmiStatus( uint32_t const Interval = 0 );
  void RadioEvent( TRadioEvent const Event );

private:
  bool TimerFlag;
  bool RadioFlag;
  bool SerialFlag;
  TSbus Sbus;
  TSx1268 Radio;
  TNvData NvData;
  uint32_t TimeoutHmiError;
  uint32_t TimeoutHmiStatus;
  TSbusSerial SbusSerial;
};

#endif // ROSALYN_SAT_H__
