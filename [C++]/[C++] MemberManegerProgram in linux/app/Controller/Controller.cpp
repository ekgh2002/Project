#include "Controller.h"
#include <iostream>
Controller::Controller(MembersManageService *membersManageService)
{
    this->membersManageService = membersManageService;
    membersEntity = new MembersEntity();
}

Controller::~Controller()
{

}

void Controller::updateEvent(DeviceData data)
{
    if (data.devName == "cardReader")
    {
        int cardNumber[5];
        for(int i = 0; i < 5; i++)
        cardNumber[i] = data.devData[i];

        membersManageService->checkCardNumber(cardNumber);
    }
    if (data.devName == "ModeButton")
    {
        membersManageService->updateStateEvent(data.devName);
    }
    if (data.devName == "ModeButton2")
    {
        membersManageService->updateStateEvent(data.devName);
    }
}