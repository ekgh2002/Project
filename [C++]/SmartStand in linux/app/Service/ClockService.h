#ifndef CLOCKSERVICE_H
#define CLOCKSERVICE_H
#include <time.h>
#include "ClockView.h"
#include "clockstate.h"
#pragma once

class ClockService
{

private:
    time_t curTime;
    ClockView *clockView;
    int clockstate;

public:
    ClockService(ClockView *view);
    virtual ~ClockService();
    void updateEvent();
    void clockselect(std::string strState3);
};

#endif