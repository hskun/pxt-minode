#ifndef MINODE_VRES_H
#define MINODE_VRES_H

#include "mbed.h"

#include "MicroBitConfig.h"
#include "MicroBitComponent.h"
#include "MicroBitEvent.h"
#include "MiNodeComponent.h"
#include "MiNodeConn.h"
#include "MicroBitDisplay.h"
#include "MicroBitSystemTimer.h"

#if MICROBIT_CODAL
#include "MiNodeCompat.h"
#endif

#define MINODE_ROTARY_EVT_CHANGE                  1

class MiNodeRotary : public MiNodeComponent
{
public:

  MiNodeRotary();

  float getVolt();
  int getADValue();
  int getPercentage();

  void attach(AnalogConnName connName);

  virtual void systemTick();
  ~MiNodeRotary();

private:
#if MICROBIT_CODAL
  MicroBitPin* pin;
#else
  AnalogIn* pin;
#endif
  int currentAD;
  int count;
  
};

#endif
