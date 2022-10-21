#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Monitor.h"
#include "DeviceData.h"
#include "MembersManageService.h"
#include "MembersEntity.h"
#include <stdio.h>
#include <cstring>
#include <iostream>

class Controller
{
private:
    MembersManageService *membersManageService;
    MembersEntity *membersEntity;

public:
    Controller(MembersManageService *membersManageService);
    virtual ~Controller();
    void updateEvent(DeviceData data);

};

#endif