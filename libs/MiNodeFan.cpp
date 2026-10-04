#include "MiNodeFan.h"


MiNodeFan::MiNodeFan() :
pin(NULL)
{
  this->baseId = MINODE_ID_MODULE_FAN;
}

MiNodeFan::~MiNodeFan()
{
#if !MICROBIT_CODAL
  if(pin) {
    delete pin;
  }
#endif
}

void MiNodeFan::attach(ConnName connName)
{
  if(this->cn != MN_NC) {
    return;
  }

  MiNodeComponent::initConnector(connName);

#if MICROBIT_CODAL
  pin = minode::getMicroBitPin(this->cn);
#else
  PinName pinName = MiNodeConn::calcP0Name(this->cn);
  if(pin) {
    delete pin;
  }
  pin = new DigitalOut(pinName);
#endif
}

void MiNodeFan::fanOpen()
{
#if MICROBIT_CODAL
  minode::pinSetDigitalValue(pin, 1);
#else
	pin->write(1);
#endif
}

void MiNodeFan::fanClose()
{
#if MICROBIT_CODAL
  minode::pinSetDigitalValue(pin, 0);
#else
	pin->write(0);
#endif
}
