#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <cstdlib>
#include <cstdarg>
#include "main.h"
#include "sx1268-def.h"


void UartPrintf( char const *const Format, ... );

inline void SetPin( GPIO_TypeDef *const Gpio, uint32_t const PinMask )
{
  LL_GPIO_SetOutputPin( Gpio, PinMask );
}
inline void ResetPin( GPIO_TypeDef *const Gpio, uint32_t const PinMask )
{
  LL_GPIO_ResetOutputPin( Gpio, PinMask );
}
inline bool ReadPin( GPIO_TypeDef *const Gpio, uint32_t const PinMask )
{
  return LL_GPIO_IsInputPinSet( Gpio, PinMask ) != 0;
}

uint8_t const FLAG_CONFIG = 0x12;

class TSystem
{
  struct TConfig
  {
    TConfig() :
      Flag( FLAG_CONFIG ),
      Mode( 0 ),
      UnitId( 0 ),
      TxPower( -3 ),  // -3dBm to 22dBm
      Channel( 38 ),
	  Modulation
      {
        { LORA_BW_125, LORA_CR_4_5, LORA_SF12 }, // Low
        { LORA_BW_250, LORA_CR_4_5, LORA_SF8  }, // Medium
        { LORA_BW_500, LORA_CR_4_5, LORA_SF5  }  // High
      }
    {
    }

    uint8_t Flag;
    uint8_t Mode;
    uint8_t UnitId;
    int8_t  TxPower;
    uint8_t Channel;
    Modulation_t Modulation[ 3 ];
  };

  void UpdateConfig( void );

public:
  TConfig Config;
};
extern TSystem System;

#endif // SYSTEM_H_
