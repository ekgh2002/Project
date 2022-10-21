#include "Listener.h"
#include <wiringPi.h>
#include <iostream>

Listener::Listener(Button *modeButton, Button *powerButton, Button *panpowerbutton, Button *onoffbt, Button *clockbutton, Controller *control, ClockCheck *clock, DHT11 *dht11, UltraSonic *ultraSonic)
{
    this->modeButton = modeButton;
    this->powerButton = powerButton;
    this->panpowerbutton = panpowerbutton;
    this->clockbutton = clockbutton;
    this->onoffbt = onoffbt;
    this->controller = control;
    this->clockCheck = clock;
    this->dht11 = dht11;
    this->ultraSonic = ultraSonic;
}

Listener::~Listener()
{
}

void Listener::checkEvent()
{
    if (modeButton->getState() == RELEASE_ACTIVE)
    {
        controller->updateEvent("modeButton");
    }

    if (powerButton->getState() == RELEASE_ACTIVE)
    {
        controller->updateEvent("powerButton");
    }

    if (panpowerbutton->getState() == RELEASE_ACTIVE)
    {
        controller->updateEvent("panpowerbutton");
        // printf("hi");
    }

    if (onoffbt->getState() == RELEASE_ACTIVE)
    {
        controller->updateEvent("onoffbt");
        // printf("hi");
    }
    if (clockbutton->getState() == RELEASE_ACTIVE)
    {
        controller->updateEvent("clockbutton");
    }
    if (clockCheck->isUpdate())
    {
        controller->updateEvent("clockUpdate");
    }
    static unsigned int prevTempHumidTime = 0; 
    if(millis()- prevTempHumidTime > 2000)
    {
        prevTempHumidTime = millis();
        DHT_Data dhtData = dht11->readData();
        if (!dhtData.error)  //  0이 아닐때
        {
            controller->updateTempHumid(dhtData);
        }
    }
    static unsigned int prevUltraSonicTime = 0; 
    if(millis()- prevUltraSonicTime > 1000)
    {
        prevUltraSonicTime = millis();
        int distance = ultraSonic->readDistance();
        controller->updateDistance(distance);
    }
      
}