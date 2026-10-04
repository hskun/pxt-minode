#ifndef MINODE_COMPAT_H
#define MINODE_COMPAT_H

#include "pxt.h"
#include "MiNodeConn.h"

#if MICROBIT_CODAL
#include "MicroBitSystemTimer.h"

#define MINODE_SYSTEM_TIMER_ADD_COMPONENT(component) \
  (component)->status |= DEVICE_COMPONENT_STATUS_SYSTEM_TICK
#define MINODE_SYSTEM_TIMER_REMOVE_COMPONENT(component) \
  (component)->status &= ~DEVICE_COMPONENT_STATUS_SYSTEM_TICK

namespace minode {

#define systemTick periodicCallback

MicroBitPin *getMicroBitPin(ConnName name);
MicroBitPin *getMicroBitPin(AnalogConnName name);
MicroBitPin *getSecondPin(ConnName name);

void pinSetDigitalValue(MicroBitPin *pin, int value);
void pinSetPullNone(MicroBitPin *pin);
int pinGetDigitalValue(MicroBitPin *pin);
int pinGetAnalogValue(MicroBitPin *pin);

template<typename T>
void pinOnRise(MicroBitPin *pin, T *owner, void (T::*method)(MicroBitEvent))
{
  if (pin) {
    pin->eventOn(MICROBIT_PIN_EVENT_ON_EDGE);
    uBit.messageBus.listen(pin->id, MICROBIT_PIN_EVT_RISE, owner, method);
  }
}

template<typename T>
void pinOnFall(MicroBitPin *pin, T *owner, void (T::*method)(MicroBitEvent))
{
  if (pin) {
    pin->eventOn(MICROBIT_PIN_EVENT_ON_EDGE);
    uBit.messageBus.listen(pin->id, MICROBIT_PIN_EVT_FALL, owner, method);
  }
}

void delayUs(volatile int &delayCounter);

}

#endif

#endif
