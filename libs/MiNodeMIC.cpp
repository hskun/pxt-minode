#include "MiNodeMIC.h"

#if MICROBIT_CODAL
#define MINODE_MIC_TIMER_ID 83
#define MINODE_MIC_TIMER_VALUE 0
#else
#define MINODE_SYSTEM_TIMER_ADD_COMPONENT(component) system_timer_add_component(component)
#define MINODE_SYSTEM_TIMER_REMOVE_COMPONENT(component) system_timer_remove_component(component)
#endif


MiNodeMIC::MiNodeMIC() :
pin(NULL)
{
  this->baseId = MINODE_ID_MODULE_LIGHT;
  MINODE_SYSTEM_TIMER_ADD_COMPONENT(this);
}

MiNodeMIC::~MiNodeMIC()
{
#if MICROBIT_CODAL
  detachTimer();
#else
  if(pin) {
    delete pin;
  }
  timer.detach();
#endif
  MINODE_SYSTEM_TIMER_REMOVE_COMPONENT(this);
}

void MiNodeMIC::checking(void)
{
  static int adHolder[8] = {0,0,0,0,0,0,0,0};
  static int ad_count = 0;
  int temp = 0;
  int i=0;
  int current_ad = 0;
  static int tri_flag = 0;
  static int time_count = 0;

#if MICROBIT_CODAL
  current_ad = minode::pinGetAnalogValue(pin);
#else
  current_ad = pin->read_u16();
#endif
  if (current_ad > 528)
  {
    adHolder[ad_count%8] = current_ad - 528;
    ad_count++;
  }
  else
  {
    adHolder[ad_count%8] = 528 - current_ad;
    ad_count++;
  }

  if (tri_flag == 1)
  {
    time_count++;
    if (time_count > 1000)
    {
      tri_flag = 0;
      time_count = 0;
    }
  }

  if(tri_flag == 0)
  {
    for(i=0; i<8; i++)
    {
      temp += adHolder[i];
    }
    temp /= 8;

    if(temp > 150)
    {
      MicroBitEvent evt(this->baseId + this->id,MINODE_MIC_EVT_NOISE);
      tri_flag = 1;
      temp = 0;
    }
  }
}

#if MICROBIT_CODAL
void MiNodeMIC::onTimerEvent(MicroBitEvent evt)
{
  (void)evt;
  checking();
}
#endif

void MiNodeMIC::attach(AnalogConnName connName)
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

#if MICROBIT_CODAL
  uBit.messageBus.listen(MINODE_MIC_TIMER_ID, MINODE_MIC_TIMER_VALUE, this, &MiNodeMIC::onTimerEvent);
  codal::system_timer_event_every_us(500, MINODE_MIC_TIMER_ID, MINODE_MIC_TIMER_VALUE);
#else
  timer.attach_us(this, &MiNodeMIC::checking, 500);
#endif
}

void MiNodeMIC::detachTimer()
{
#if MICROBIT_CODAL
  codal::system_timer_cancel_event(MINODE_MIC_TIMER_ID, MINODE_MIC_TIMER_VALUE);
#else
  timer.detach();
#endif
}
