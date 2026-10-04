#include "MiNodeSwitch.h"



MiNodeSwitch::MiNodeSwitch() :
pin(NULL)
{
  this->baseId = MINODE_ID_MODULE_SWITCH;
}


MiNodeSwitch::~MiNodeSwitch()
{
#if !MICROBIT_CODAL
  if(pin) {
    delete pin;
  }
#endif
}

void MiNodeSwitch::attach(ConnName connName)
{
  if(this->cn != MN_NC) {
    return;
  }

  MiNodeComponent::initConnector(connName);
  eventOn();
}


void MiNodeSwitch::eventOn()
{
#if MICROBIT_CODAL
  pin = minode::getMicroBitPin(this->cn);
  minode::pinSetPullNone(pin);
  minode::pinOnRise(pin, this, &MiNodeSwitch::onOpenEvent);
  minode::pinOnFall(pin, this, &MiNodeSwitch::onCloseEvent);
#else
  PinName pinName = MiNodeConn::calcP0Name(this->cn);
  if(pin) {
    delete pin;
  }
  pin = new InterruptIn(pinName);

  pin->mode(PullNone);
  pin->rise(this, &MiNodeSwitch::onOpen);
  pin->fall(this, &MiNodeSwitch::onClose);
#endif
}

#if MICROBIT_CODAL
void MiNodeSwitch::onOpenEvent(MicroBitEvent evt)
{
  (void)evt;
  MicroBitEvent event(this->baseId + this->id, MINODE_SWITCH_EVT_OPEN);
}

void MiNodeSwitch::onCloseEvent(MicroBitEvent evt)
{
  (void)evt;
  MicroBitEvent event(this->baseId + this->id, MINODE_SWITCH_EVT_CLOSE);
}
#else
void MiNodeSwitch::onOpen()
{
  MicroBitEvent event(this->baseId + this->id, MINODE_SWITCH_EVT_OPEN);
}

void MiNodeSwitch::onClose()
{
  MicroBitEvent event(this->baseId + this->id, MINODE_SWITCH_EVT_CLOSE);
}
#endif

int MiNodeSwitch::isOpened()
{
#if MICROBIT_CODAL
  return minode::pinGetDigitalValue(pin);
#else
  int status = 0;
  if(pin) {
    status = pin->read();
  }
  return status;
#endif
}
