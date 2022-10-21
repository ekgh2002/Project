#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <string>
#include <vector>
#include "MembersEntity.h"
#include "Memberinfo.h"
#include "ComDev.h"
#include "View.h"
#include "menustate.h"
#include "LCD.h"
#include "Led.h"
#include <cstring>
#include <wiringPi.h>
#include <time.h>
#include <stdio.h>


class View
{

private:
    LCD *lcd;
    Led *light1;
    Led *light2;
    Led *light3;
    Led *light4;
    Led *light5;
    MembersEntity *membersEntity;
    int membersManagerState;    
    int clocktime;
    int clocktime_sec;
    int clocktime_min;
    int clocktime_hour;
    int clockstate;
    struct tm timeDate;
    char buff[30];
    time_t curTime;

public:
    View();
    virtual ~View();
    void setstate(int state);
    void print_view();
    void Clock();
    void updateTime(struct tm *timeData);

};

#endif