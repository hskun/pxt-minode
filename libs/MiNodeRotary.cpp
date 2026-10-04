#include "MiNodeRotary.h"

#if !MICROBIT_CODAL
#define MINODE_SYSTEM_TIMER_ADD_COMPONENT(component) system_timer_add_component(component)
#define MINODE_SYSTEM_TIMER_REMOVE_COMPONENT(component) system_timer_remove_component(component)
#endif

MiNodeRotary::MiNodeRotary() :
pin(NULL),currentAD(-1),count(0)
{
  this->baseId = MINODE_ID_MODULE_SWITCH;
  MINODE_SYSTEM_TIMER_ADD_COMPONENT(this);
}

MiNodeRotary::~MiNodeRotary()
{
#if !MICROBIT_CODAL
  if(pin) {
    delete pin;
  }
#endif
  MINODE_SYSTEM_TIMER_REMOVE_COMPONENT(this);
}

void MiNodeRotary::attach(AnalogConnName connName)
{
  if(this->cn != MN_NC) {
    return;
  }

  MiNodeComponent::initAConnector(connName);

#if MICROBIT_CODAL
  pin = minode::getMicroBitPin(this->cna);
#else
  PinName pinName = MiNodeConn::calcP0Name(this->cna);
  if(pin) {
    delete pin;
  }
  pin = new AnalogIn(pinName);
#endif
}

void MiNodeRotary::systemTick()
{
  count++;

  if (count == 40)
  {
    if (currentAD == -1)
    {
      currentAD = getADValue();
    }
    else
    {
      if ((getADValue() - currentAD > 31) || (getADValue() - currentAD < -31))
      {
        MicroBitEvent evt(this->baseId + this->id,MINODE_ROTARY_EVT_CHANGE);
        currentAD = getADValue();
      }
    }
    count = 0;
  }
}

int MiNodeRotary::getPercentage()
{
  int temp;
  temp = getADValue();

  return temp*100/1023;
}

float MiNodeRotary::getVolt()
{
  float result;
  result = 3.3*(getADValue()/1023.0);

  return result;
}

int MiNodeRotary::getADValue()
{
  int temp=0;
  for (int i = 0; i < 3; ++i)
  {
#if MICROBIT_CODAL
    temp += minode::pinGetAnalogValue(pin);
#else
    temp += pin->read_u16();
#endif
  }
  temp /= 3;
  return temp;
}
