#include "MiNodeCompat.h"

#if MICROBIT_CODAL

namespace minode {

MicroBitPin *getMicroBitPin(ConnName name)
{
  return minodeGetPin(name);
}

MicroBitPin *getMicroBitPin(AnalogConnName name)
{
  return minodeGetPin(name);
}

MicroBitPin *getSecondPin(ConnName name)
{
  return minodeGetSecondPin(name);
}

void pinSetDigitalValue(MicroBitPin *pin, int value)
{
  if (pin) {
    pin->setDigitalValue(value);
  }
}

void pinSetPullNone(MicroBitPin *pin)
{
  if (pin) {
    pin->setPull(codal::PullMode::None);
  }
}

int pinGetDigitalValue(MicroBitPin *pin)
{
  return pin ? pin->getDigitalValue() : 0;
}

int pinGetAnalogValue(MicroBitPin *pin)
{
  return pin ? pin->getAnalogValue() : 0;
}

void delayUs(volatile int &delayCounter)
{
  for (volatile int i = 0; i < delayCounter; ++i) {
    __asm volatile("nop");
  }
}

}

#endif
