#ifndef MANAGEBUTTON_H
#define MANAGEBUTTON_H

#include "DeviceData.h"
#include "Button.h"
#include <string>


class ManageButton : public Button
{
private:
    DeviceData buttonData;

public:
    ManageButton(int pin, std::string name);
    virtual ~ManageButton();
    bool checkButton();
    DeviceData getButtonData();
};

#endif