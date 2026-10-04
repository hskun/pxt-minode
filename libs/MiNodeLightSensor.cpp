#include "MiNodeLightSensor.h"

#if !MICROBIT_CODAL
#define MINODE_SYSTEM_TIMER_ADD_COMPONENT(component) system_timer_add_component(component)
#define MINODE_SYSTEM_TIMER_REMOVE_COMPONENT(component) system_timer_remove_component(component)
#endif

MiNodeLight::MiNodeLight() :
pin(NULL),sigma(0),pre_status(MICROBIT_LIGHT_LEVEL_NULL)
{
  this->baseId = MINODE_ID_MODULE_LIGHT;
  MINODE_SYSTEM_TIMER_ADD_COMPONENT(this);
}

MiNodeLight::~MiNodeLight()
{
#if !MICROBIT_CODAL
  if(pin) {
    delete pin;
  }
#endif
  MINODE_SYSTEM_TIMER_REMOVE_COMPONENT(this);
}


void MiNodeLight::attach(AnalogConnName connName)
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

unsigned short MiNodeLight::getLight()
{
#if MICROBIT_CODAL
  return minode::pinGetAnalogValue(pin);
#else
  return pin->read_u16();
#endif
}

unsigned short MiNodeLight::getLightLevel(void)
{
  unsigned short currnt_light = 0;
  for (int i = 0; i < MINODE_LIGHT_SAMPLE_NUM; ++i)
  {
    currnt_light += getLight();
  }

  currnt_light /= MINODE_LIGHT_SAMPLE_NUM;

  if ((currnt_light > MICROBIT_LIGHT_MIN) && (currnt_light < MICROBIT_LIGHT_LEVEL_A+1))
  {
    return MICROBIT_LIGHT_LEVEL_1;
  }
  else if ((currnt_light > MICROBIT_LIGHT_LEVEL_A) && (currnt_light < MICROBIT_LIGHT_LEVEL_B+1))
  {
    return MICROBIT_LIGHT_LEVEL_2;
  }
  else if ((currnt_light > MICROBIT_LIGHT_LEVEL_B) && (currnt_light < MICROBIT_LIGHT_LEVEL_C+1))
  {
    return MICROBIT_LIGHT_LEVEL_3;
  }
  else if ((currnt_light > MICROBIT_LIGHT_LEVEL_C) && (currnt_light < MICROBIT_LIGHT_LEVEL_D+1))
  {
    return MICROBIT_LIGHT_LEVEL_4;
  }
  else if ((currnt_light > MICROBIT_LIGHT_LEVEL_D) && (currnt_light < MICROBIT_LIGHT_MAX+1))
  {
    return MICROBIT_LIGHT_LEVEL_5;
  }
  else
  {
    return MICROBIT_LIGHT_LEVEL_ERROR;
  }
}

void MiNodeLight::systemTick()
{
  sigma++;

  if(sigma == 100)
  {
    sigma = 0;

    status = getLightLevel();
    if(pre_status == MICROBIT_LIGHT_LEVEL_NULL)
    {
      pre_status = status;
    }

    if (status != pre_status)
    {
      pre_status = status;
      MicroBitEvent(this->baseId + this->id, MINODE_LIGHT_EVT_LEVEL_CHANGE);
    }
  }
}
