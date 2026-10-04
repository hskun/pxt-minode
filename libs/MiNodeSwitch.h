#ifndef MINODE_SWITCH_H
#define MINODE_SWITCH_H

#include "mbed.h"
#include "MicroBitConfig.h"
#include "MicroBitComponent.h"
#include "MicroBitEvent.h"
#include "MiNodeComponent.h"

#if MICROBIT_CODAL
#include "MiNodeCompat.h"
#endif


enum SwitchEvent
{
  //% block="open" enumval=1
  MINODE_SWITCH_EVT_OPEN = 1,
  //% block="close" enumval=2
  MINODE_SWITCH_EVT_CLOSE = 2
};


class MiNodeSwitch : public MiNodeComponent
{
public:
  MiNodeSwitch();
  ~MiNodeSwitch();

  void attach(ConnName connName);

  int isOpened();


private:
#if MICROBIT_CODAL
  void onOpenEvent(MicroBitEvent evt);
  void onCloseEvent(MicroBitEvent evt);
#else
  void onOpen();
  void onClose();
#endif
  void eventOn();

#if MICROBIT_CODAL
  MicroBitPin* pin;
#else
  InterruptIn* pin;
#endif
};

#endif
