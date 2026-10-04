#ifndef MINODE_PIR_H
#define MINODE_PIR_H

#include "mbed.h"
#include "MicroBitConfig.h"
#include "MicroBitComponent.h"
#include "MicroBitEvent.h"
#include "MiNodeComponent.h"

#if MICROBIT_CODAL
#include "MiNodeCompat.h"
#endif

#define MINODE_PIR_EVT_TRIG                  1
#define MINODE_PIR_EVT_CLOSE                 2

class MiNodePIR : public MiNodeComponent
{
public:

  MiNodePIR();

#if MICROBIT_CODAL
  void onTriggerEvent(MicroBitEvent evt);
#else
  void onTrigger();
#endif
  int readPir();
  int isTriged();

  void attach(ConnName connName);

  ~MiNodePIR();

private:
#if MICROBIT_CODAL
  MicroBitPin* pin;
#else
  InterruptIn* pin;
#endif
  
};

#endif
