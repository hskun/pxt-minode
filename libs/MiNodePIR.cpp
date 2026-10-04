#include "MiNodePIR.h"


MiNodePIR::MiNodePIR() :
pin(NULL)
{
  this->baseId = MINODE_ID_MODULE_PIR;
}

MiNodePIR::~MiNodePIR()
{
#if !MICROBIT_CODAL
  if(pin) {
    delete pin;
  }
#endif
}

void MiNodePIR::attach(ConnName connName)
{
  if(this->cn != MN_NC) {
    return;
  }

  MiNodeComponent::initConnector(connName);

#if MICROBIT_CODAL
  pin = minode::getMicroBitPin(this->cn);
  minode::pinSetPullNone(pin);
  minode::pinOnRise(pin, this, &MiNodePIR::onTriggerEvent);
#else
  PinName pinName = MiNodeConn::calcP0Name(this->cn);
  if(pin) {
    delete pin;
  }
  pin = new InterruptIn(pinName);
  pin->mode(PullNone);
  pin->rise(this, &MiNodePIR::onTrigger);
#endif
}

int MiNodePIR::isTriged()
{
  int temp=0;
  for (int i = 0; i < 10; ++i)
  {
    if (readPir() != 0)
    {
      temp += 1;
    }
  }
  if(temp > 8)
    return 1;
  else
    return 0;
}

#if MICROBIT_CODAL
void MiNodePIR::onTriggerEvent(MicroBitEvent evt)
{
  (void)evt;
  MicroBitEvent event(this->baseId + this->id,MINODE_PIR_EVT_TRIG);
}
#else
void MiNodePIR::onTrigger()
{
  MicroBitEvent event(this->baseId + this->id,MINODE_PIR_EVT_TRIG);
}
#endif

int MiNodePIR::readPir()
{
#if MICROBIT_CODAL
  return minode::pinGetDigitalValue(pin);
#else
  return pin->read();
#endif
}
