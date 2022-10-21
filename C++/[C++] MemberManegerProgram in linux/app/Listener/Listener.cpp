#include "Listener.h"
#include <string.h>

Listener::Listener(Controller *controller)
{
   rfid = new CardReader(new SPI(10, 3000000));
   modeButton = new ManageButton(27, "ModeButton");
   modeButton2 = new ManageButton(28, "ModeButton2");
   this->controller = controller;
   this->tcpserver = tcpserver;
}

Listener::~Listener()
{

}

void Listener::checkEvent()
{
    if (checkRfid())
    {
        controller->updateEvent(rfid->getCardNumber());
    }   
    if (modeButton->checkButton())
    {
        controller->updateEvent(modeButton->getButtonData());
    }
    if (modeButton2->checkButton())
    {
        controller->updateEvent(modeButton2->getButtonData());
    }
}

bool Listener::checkRfid()
{
    static unsigned int prevCheckTime = 0;

    if (millis() - prevCheckTime < 1000)
    {
        return false;
    }
    prevCheckTime = millis();

    if (rfid->isCard()) return true;

    return false;
}
