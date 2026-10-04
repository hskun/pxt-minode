#include "MiNodeDHT11.h"

#if MICROBIT_CODAL
namespace {
void waitForMs(int milliseconds)
{
  codal::system_timer_wait_ms(milliseconds);
}
}
#else
#define MINODE_SYSTEM_TIMER_ADD_COMPONENT(component) system_timer_add_component(component)
#define MINODE_SYSTEM_TIMER_REMOVE_COMPONENT(component) system_timer_remove_component(component)
#define waitForMs(milliseconds) wait_ms(milliseconds)
#endif

MiNodeDHT::MiNodeDHT() :
pin(NULL),currentTem(-99)
{
  this->baseId = MINODE_ID_MODULE_DHT11;
  MINODE_SYSTEM_TIMER_ADD_COMPONENT(this);
}

MiNodeDHT::~MiNodeDHT()
{
#if !MICROBIT_CODAL
  if(pin) {
    delete pin;
  }
#endif
  MINODE_SYSTEM_TIMER_REMOVE_COMPONENT(this);
}

void MiNodeDHT::attach(ConnName connName)
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
  pin = new DigitalInOut(pinName);
#endif
}

void MiNodeDHT::dhtSet(int level)
{
#if MICROBIT_CODAL
  minode::pinSetDigitalValue(pin, level);
#else
  pin->output();
  pin->mode(PullNone);
  pin->write(level);
#endif
}

int MiNodeDHT::dhtGet()
{
#if MICROBIT_CODAL
  return minode::pinGetDigitalValue(pin);
#else
  pin->input();
  return pin->read();
#endif
}

int MiNodeDHT::whileGet(int v)
{
  time_out = 0;
  while((v==dhtGet()) && (time_out < TIME_TH))
  {
    time_out ++;
  }
  if(time_out == TIME_TH)
    return 1;
  else
    return 0;
}

void MiNodeDHT::dhtStart()
{
    dhtSet(1);
    delay60US();
    dhtSet(0);
    waitForMs(25);
    dhtSet(1);
}

int MiNodeDHT::dhtReadAck()
{
    if(whileGet(1) == 1)
      return 1;
    if(whileGet(0) == 1)
      return 1;
    if(whileGet(1) == 1)
      return 1;

    return 0;
}

void MiNodeDHT::dhtReadOneBit()
{
  whileGet(0);
  delay60US();
  bt <<= 1;
  if(1==dhtGet())
  {
    bt |= 1;
    whileGet(1);
  }
  else
    bt |= 0;
}

void MiNodeDHT::dhtReadOneByte()
{
  bt=0;
  dhtReadOneBit();
  dhtReadOneBit();
  dhtReadOneBit();
  dhtReadOneBit();
  dhtReadOneBit();
  dhtReadOneBit();
  dhtReadOneBit();
  dhtReadOneBit();
}

void MiNodeDHT::systemTick()
{
  int temp=0;
  count++;

 if (count == 200)
  {
    dhtGetHt();
    if (currentTem == -99)
    {
      currentTem = Temperature;
    }

    if((Temperature - currentTem == 1) || (currentTem - Temperature == 1))
    {
      currentTem = temp;
      MicroBitEvent(this->baseId + this->id, MINODE_DHT_EVT_CHANGE);
    }
    count = 0;
  }

}

int MiNodeDHT::dhtGetHt()
{
    int CHECKSUM=0;
    int R_H=0;
    int R_L=0;
    int T_H=0;
    int T_L=0;

    dhtStart();
    if(dhtReadAck() == 1)
      return 0;

    dhtReadOneByte();
    R_H = bt;
    dhtReadOneByte();
    R_L = bt;
    dhtReadOneByte();
    T_H = bt;
    dhtReadOneByte();
    T_L = bt;
    dhtReadOneByte();
    CHECKSUM = bt;

    if(CHECKSUM == R_H+R_L+T_H+T_L)
    {
        Humidity = R_H;
        Temperature = T_H;
        return 0;
    }
    else
        return 1;
}

int MiNodeDHT::getTemperature()
{
  if (currentTem == -99)
  {
    dhtGetHt();
    currentTem = Temperature;
  }
  return Temperature;
}

int MiNodeDHT::getFahrenheitTemperature()
{
  if (currentTem == -99)
  {
    dhtGetHt();
    currentTem = Temperature;
  }
  return Temperature*9/5+32;
}

int MiNodeDHT::getHumidity()
{
  if (currentTem == -99)
  {
    dhtGetHt();
    currentTem = Temperature;
  }
  return Humidity;
}
